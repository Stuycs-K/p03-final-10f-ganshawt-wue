#include "networking.h"
#include "questioncreation.h"

typedef struct {
  int client_id;
  int score;
  int socket;
  char username[32];
} PlayerScore;

void sigint_handler(int sig) {
  remove(".server_ip");
  exit(0);
}

void send_client(int socket, const char *msg) {
  write(socket, msg, strlen(msg));
}

void send_question(int client_socket, const char *question, char **answers) {
  char buffer[BUFFER_SIZE];
  snprintf(buffer, sizeof(buffer), "Q: %s\n", question);
  send_client(client_socket, buffer);
  for (int i = 0; i < 4; i++) {
    snprintf(buffer, sizeof(buffer), "%d. %s\n", i + 1, answers[i]);
    send_client(client_socket, buffer);
  }
  send_client(client_socket, "END\n");
}

int subserver_logic(int client_socket, int client_id, char *username) {
  char * qtypes[12] = {"name", "gen", "height (dm)", "weight (hg)", "type", "hp", "attack", "defense", "special attack", "special defense", "speed", "base experience"};
  char buffer[BUFFER_SIZE];
  int score = 0;
  snprintf(buffer, sizeof(buffer), "USERNAME:%s\n", username);
  send_client(client_socket, buffer);
  for (int round = 1; round <= 10; round++) {
    char **answers = calloc(4, 50);
    for (int i = 0; i < 4; i++) {
      answers[i] = calloc(50, 1);
    }

    int rand_qtype = rand() % 12;
    if(rand_qtype == 0){rand_qtype = 1;}

    int rand_dexnum = rand() % 1025;
    if(rand_dexnum <= 0){rand_dexnum = 1;}

    char pkmn_name[20];
    getName(rand_dexnum, pkmn_name);
    // printf("What is %s's %s?\n", pkmn_name, qtypes[rand_qtype]);

    int correct_pos = questioncreation(rand_dexnum, rand_qtype, answers);
    char question[256];
    snprintf(question, sizeof(question), "Question %d/10: What is %s's %s?", round, pkmn_name, qtypes[rand_qtype]);
    printf("question: %s\n",question);
    send_question(client_socket, question, answers);
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0) {
      for (int i = 0; i < 4; i++)  {
        free(answers[i]);
      }
      return score;
    }
    buffer[bytes_read] = '\0';
  //  printf("%d: %s\n",bytes_read,buffer);
    int client_answer;
    sscanf(buffer, "%d", &client_answer);
    client_answer -= 1;
    if (client_answer == correct_pos) {
    //  printf("correct!\n");
      score++;

      send_client(client_socket, "RESULT:CORRECT\n");
    }  else  {
      snprintf(buffer, sizeof(buffer), "RESULT:WRONG%d", correct_pos + 1);
    //  printf("%s\n", buffer);
    	send_client(client_socket, buffer);
    }

    printf("score: %d\n", score);
    for (int i = 0; i < 4; i++)  {
      //printf("freeing answers\n");
    //  printf("INVALID: %s\n", answers[i]);
      free(answers[i]);
    }
  //  free(answers);
  }
  snprintf(buffer, sizeof(buffer), "Your final score was %d\n", score);
  send_client(client_socket, buffer);
  return score;
}

void send_lobby_status(int *sockets, int count) {
  char msg[64];
  snprintf(msg, sizeof(msg), "Waiting for full lobby (%d/4)\n", count);
  for (int i = 0; i < count; i++) {
    write(sockets[i], msg, strlen(msg));
  }
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_handler);
  char hostname[256];
  gethostname(hostname, sizeof(hostname));
  FILE *fp = fopen(".server_ip", "w");
  if (fp) {
    fprintf(fp, "%s\n", hostname);
    fclose(fp);
  }
  int listen_socket = server_setup();
  int client_sockets[4];
  while (1) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
    printf("Waiting for 4 clients...\n");
    for (int i = 0; i < 4; i++) {
      client_sockets[i] = server_tcp_handshake(listen_socket);
      printf("Client %d/4 connected\n", i + 1);
      send_lobby_status(client_sockets, i + 1);
    }
    srand(time(NULL));
    printf("Lobby full!\n");
    char request_username[] = "Insert a username: ";
    for (int i = 0; i < 4; i++) {
      write(client_sockets[i], request_username, strlen(request_username));
    }
    PlayerScore players[4];
    char usernames[4][32];
    for (int i = 0; i < 4; i++) {
      char buffer[BUFFER_SIZE];
      int bytes_read = read(client_sockets[i], buffer, sizeof(buffer) - 1);
      if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(usernames[i], buffer, 31);
        usernames[i][31] = '\0';
      }  else  {
        snprintf(usernames[i], 32, "Player%d", i + 1);
      }
      players[i].client_id = i + 1;
      players[i].socket = client_sockets[i];
      players[i].score = 0;
    }
    for (int i = 0; i < 4; i++)  {
      int duplicate = 0;
      for (int j = 0; j < i; j++) {
        if (strcmp(usernames[i], usernames[j]) == 0) {
          duplicate = 1;
          break;
        }
      }
      if (duplicate)  {
      	snprintf(players[i].username, 31, "%s_%d", usernames[i], i + 1);
      }  else  {
      	strncpy(players[i].username, usernames[i], sizeof(players[i].username) - 1);
      }
      printf("Player %d username: %s\n", i + 1, players[i].username);
    }
    char start_msg[] = "START";
    for (int i = 0; i < 4; i++) {
      write(client_sockets[i], start_msg, strlen(start_msg));
    }
    pid_t child_pids[4];
    for (int i = 0; i < 4; i++) {
      pid_t pid = fork();
      if (pid < 0) {
        close(client_sockets[i]);
        continue;
      }
      if (pid == 0) {
        close(listen_socket);
        for (int j = 0; j < 4; j++) {
          if (j != i)  {
            close(client_sockets[j]);
          }
        }
        int final_score = subserver_logic(client_sockets[i], i, players[i].username);
        char scorefile[32];
        snprintf(scorefile, sizeof(scorefile), ".score_%d", getpid());
        FILE *sf = fopen(scorefile, "w");
        if (sf) {
          fprintf(sf, "%d", final_score);
          fclose(sf);
        }
        close(client_sockets[i]);
        exit(0);
      }  else  {
      	child_pids[i] = pid;
      }
    }
    for (int i = 0; i < 4; i++) {
      int status;
      waitpid(child_pids[i], &status, 0);
      char scorefile[32];
      snprintf(scorefile, sizeof(scorefile), ".score_%d", child_pids[i]);
      FILE *sf = fopen(scorefile, "r");
      if (sf) {
        fscanf(sf, "%d", &players[i].score);
        fclose(sf);
        remove(scorefile);
      }
    }
    for (int i = 0; i < 3; i++) {
      for (int j = i + 1; j < 4; j++) {
        if (players[j].score > players[i].score) {
          PlayerScore temp = players[i];
          players[i] = players[j];
          players[j] = temp;
        }
      }
    }
    printf("\nFinal Scores\n");
    for (int i = 0; i < 4; i++) {
      printf("%d. %s: %d/10\n", i + 1, players[i].username, players[i].score);
    }
    printf("\n");
    char leaderboard[BUFFER_SIZE];
    int offset = 0;
    offset += snprintf(leaderboard + offset, sizeof(leaderboard) - offset, "LEADERBOARD\n");
    for (int i = 0; i < 4; i++) {
      offset += snprintf(leaderboard + offset, sizeof(leaderboard) - offset, "%d. %s: %d/10\n", i + 1, players[i].username, players[i].score);
    }
    snprintf(leaderboard + offset, sizeof(leaderboard) - offset, "END\n");
    for (int i = 0; i < 4; i++) {
      printf("THIS IS THE THINGY FOR %d", i);
      write(client_sockets[i], leaderboard, strlen(leaderboard));
    }
    sleep(3);
    for (int i = 0; i < 4; i++) {
      close(client_sockets[i]);
    }
  }
  return 0;
}
