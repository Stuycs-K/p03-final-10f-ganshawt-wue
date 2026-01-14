#include "networking.h"

void sigint_handler(int sig) {
  exit(0);
}

void lobby(int server_socket){
  char buffer[BUFFER_SIZE];
  while (1) {
    int bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
      printf("Server disconnected.\n");
      exit(0);
    }
    buffer[bytes_read] = '\0';
    if (strstr(buffer, "Insert a username: ")) {
      printf("\nEnter your username: ");
      char username[32];
      if (fgets(username, sizeof(username), stdin) == NULL) {
        strcpy(username, "Guest");
      }
      username[strcspn(username, "\n")] = '\0';
      write(server_socket, username, strlen(username));
      continue;
    }
    if (strstr(buffer, "START")) {
      printf("\nGame starting!\n");
      break;
    }
    printf("\r%s", buffer);
    fflush(stdout);
  }
}

void receive_question(int server_socket, char *question, char answers[4][50]) {
  char buffer[BUFFER_SIZE];
  while (1) {
    int bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
      printf("Server disconnected\n");
      exit(0);
    }
   buffer[bytes_read] = '\0';
    char *line_start = buffer;
    char *line_end;
    while ((line_end = strchr(line_start, '\n')) != NULL) {
      *line_end = '\0';
      if (strncmp(line_start, "Q: ", 3) == 0) {
        strcpy(question, line_start + 3);
      }  else if (line_start[0] >= '1' && line_start[0] <= '4') {
      	int idx = line_start[0] - '1';
      	strcpy(answers[idx], line_start + 3);
      }  else if (strcmp(line_start, "END") == 0) {
      	return;
      }
      line_start = line_end + 1;
    }
  }
}

void game(int server_socket) {
  char buffer[BUFFER_SIZE];
  int bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    if (strncmp(buffer, "USERNAME:", 9) == 0) {
      buffer[strcspn(buffer, "\n")] = '\0';
      printf("Playing as: %s\n\n", buffer + 9);
    }
  }
  for (int round = 1; round <= 10; round++) {
    char question[256];
    char answers[4][50];
    receive_question(server_socket, question, answers);
    printf("%s\n", question);
    for (int i = 0; i < 4; i++) {
      printf("%d. %s\n", i + 1, answers[i]);
    }

    printf("Your answer (1-4): ");
    sleep(0.5);
    char * temp = fgets(buffer, sizeof(buffer), stdin);
    if(temp == NULL){
      close(server_socket);
      exit(0);
    }
    while(strlen(buffer) > 2 || buffer[0] < 49 || buffer[0] > 52)
    {
      printf("Your answer (1-4): ");
      char * temp = fgets(buffer, sizeof(buffer), stdin);
      if(temp == NULL){
        close(server_socket);
        exit(0);
      }

    buffer[strcspn(buffer, "\n")] = '\0';
    write(server_socket, buffer, strlen(buffer));
    bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
      printf("Server disconnected\n");
      exit(0);
    }
    buffer[bytes_read] = '\0';
<<<<<<< HEAD
    //printf("%s\n", buffer);
=======
>>>>>>> 7440822115250f78de24d4611ecaa0802f84cce2

    printf("\x001B[0;0H");
    printf("\x001B[2J");
    if (strncmp(buffer, "RESULT:CORRECT", 14) == 0) {
      printf("Correct!\n\n");
    }  else if  (strncmp(buffer, "RESULT:WRONG:", 12) == 0)  {
      int correct_ans;
      sscanf(buffer + 12, "%d", &correct_ans);
      printf("Wrong! Correct answer was %d\n\n", correct_ans);
    }
  }
<<<<<<< HEAD
=======

  bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    if (strstr(buffer, "Your final score was"))  {
    	printf("%s", buffer);
    }

    if (strstr(buffer, "LEADERBOARD"))  {
      char *line_start = strstr(buffer, "LEADERBOARD");
      printf("\x001B[0;0H");
      printf("\x001B[2J");
      printf("\n=== FINAL LEADERBOARD ===\n");
      line_start = strchr(line_start, '\n');
      if (line_start) line_start++;
       while (line_start && *line_start) {
         char *line_end = strchr(line_start, '\n');
         if (!line_end) break;
         *line_end = '\0';
         if (strcmp(line_start, "END") == 0) {
           printf("\n");
           return;
         }
         printf("%s\n", line_start);
         line_start = line_end + 1;
       }
       return;
     }
   }
}

>>>>>>> 7440822115250f78de24d4611ecaa0802f84cce2
  printf("Waiting for all players to finish...\n");

  while (1) {
    bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
      return;
    }
    buffer[bytes_read] = '\0';
    char *line_start = buffer;
    char *line_end;
    int in_leaderboard = 0;
    while ((line_end = strchr(line_start, '\n')) != NULL) {
      *line_end = '\0';
      if (strcmp(line_start, "LEADERBOARD") == 0) {
        printf("\n=== FINAL LEADERBOARD ===\n");
        in_leaderboard = 1;
      }  else if  (strcmp(line_start, "END") == 0)  {
        printf("\n");
        return;
      }  else if  (in_leaderboard)  {
        printf("%s\n", line_start);
      }
      line_start = line_end + 1;
    }
  }
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_handler);
  char IP[256] = "127.0.0.1";
  if (argc > 1) {
    strncpy(IP, argv[1], sizeof(IP) - 1);
  } else {
    FILE *fp = fopen(".server_ip", "r");
    if (fp) {
      if (fscanf(fp, "%255s", IP) != 1) {
        strcpy(IP, "127.0.0.1");
      }
      fclose(fp);
    }
  }
  int server_socket = client_tcp_handshake(IP);
  printf("Connected to server\n");
  lobby(server_socket);
  game(server_socket);
  printf("Game over. Ctrl-C to exit\n");
  while (1)	 {
  	sleep(1);
  }
  close(server_socket);
  return 0;
}
