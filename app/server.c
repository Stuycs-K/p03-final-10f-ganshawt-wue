#include "networking.h"

void sigint_handler(int sig) {
  remove(".server_ip");
  exit(0);
}

int subserver_logic(int client_socket) {
  char buffer[BUFFER_SIZE];
  int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
  if (bytes_read <= 0) {
    close(client_socket);
    return 0;
  }
  buffer[bytes_read] = '\0';
  int bytes_written = write(client_socket, buffer, strlen(buffer));
  if (bytes_written < 0) {
    return 0;
  }
  return 1;
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
