#include "networking.h"

// Signal handler for SIGINT
void sigint_handler(int sig) {
  //printf("Server exit");
  remove(".server_ip");
  exit(0);
}

int subserver_logic(int client_socket) {
  char buffer[BUFFER_SIZE];
  //Listen for string
  int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
  if (bytes_read < 0) {
    //printf("read() error: %s\n", strerror(errno));
    return 0;
  }
  if (bytes_read == 0) {
    //printf("Subserver client disconnected\n");
    close(client_socket);
    return 0;
  }
  buffer[bytes_read] = '\0';
  //printf("%d Subserver received from client: %s\n", getpid(), buffer);
  //Use rotX
  rotX(buffer, 13);
  //printf("%d Subserver sending back ROT13: %s\n", getpid(), buffer);
  //Send rotX to client
  int bytes_written = write(client_socket, buffer, strlen(buffer));
  if (bytes_written < 0) {
    //printf("write() error: %s\n", strerror(errno));
    return 0;
  }
  //printf("%d Subserver sent rotX successfully\n", getpid());
  return 1;
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_handler);
  char hostname[256];
  gethostname(hostname, sizeof(hostname));
  FILE *fp = fopen(".server_ip", "w");
  if (fp) {
    fprintf(fp, "%s\n", hostname);
    fclose(fp);
    //printf("Server running %s\n", hostname);
  }
  int listen_socket = server_setup();
  while (1) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
    //printf("Waiting for client\n");
    int client_socket = server_tcp_handshake(listen_socket);
    //Fork subserver
    pid_t pid = fork();
    if (pid < 0) {
      //printf("fork() error: %s\n", strerror(errno));
      close(client_socket);
      continue;
    }
    if (pid == 0) {
      close(listen_socket);
      while (subserver_logic(client_socket)) {
      }
      close(client_socket);
      //printf("%d Connection closed\n", getpid());
      exit(0);
    }
    else {
      close(client_socket);
      //printf("Forked child process %d for client\n", pid);
    }
  }
  return 0;
}
