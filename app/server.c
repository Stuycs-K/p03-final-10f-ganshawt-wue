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
  snprintf(buffer, sizeof(buffer), "Question: %s\n", question);
  send_client(client_socket, buffer);
  for (int i = 0; i < 4; i++) {
    snprintf(buffer, sizeof(buffer), "%d. %s\n", i + 1, answers[i]);
    send_client(client_socket, buffer);
  }
  send_client(client_socket, "END\n");
}

int subserver_logic(int client_socket, int client_id, char *username) {
  char buffer[BUFFER_SIZE];
  int score = 0
  snprintf(buffer, sizeof(buffer), "USERNAME:%s\n", username);
  send_client(client_socket, buffer);
  for (int round = 1; round <= 10; round++) {
    char *answers[4];
    for (int i = 0; i < 4; i++) {
      answers[i] = calloc(50, 1);
    }
    int correct_pos = questioncreation(answers);
    char question[256];
    snprintf(question, sizeof(question), "Question %d/10", round);
    send_question(client_socket, question, answers);
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
  }
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
    printf("Lobby full! Starting game...\n");
    char start_msg[] = "Game starting!\n";
    for (int i = 0; i < 4; i++) {
      write(client_sockets[i], start_msg, strlen(start_msg));
    }
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
        while (subserver_logic(client_sockets[i]));
        close(client_sockets[i]);
        exit(0);
      }
    } 
    for (int i = 0; i < 4; i++) {
      close(client_sockets[i]);
    }
  }
  return 0;
}
