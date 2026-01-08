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
    if (strstr(buffer, "Game starting")) {
      printf("%s", buffer);
      break;
    }
    printf("\r%s", buffer);
    fflush(stdout);
  }
}

void game(int server_socket) {
  char buffer[BUFFER_SIZE];
  while (1) {
    //TRIVIA FUNCTION REPLACE LATER
    int bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
      printf("Server disconnected\n");
      close(server_socket);
      exit(0);
    }
    buffer[bytes_read] = '\0';
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
  close(server_socket);
  return 0;
}
