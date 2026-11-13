#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in addres = {};
  addres.sin_port = htons(8080);
  addres.sin_family = AF_INET;
  addres.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (bind(server_fd, reinterpret_cast<sockaddr*>(&addres), sizeof(addres)) ==
      -1) {
    perror("bind");
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, 5) == -1) {
    perror("listen");
    close(server_fd);
    return 1;
  }

  int client_fd = accept(server_fd, nullptr, nullptr);

  if (client_fd == -1) {
    perror("accept");
    close(server_fd);
    return 1;
  }

  char buffer[1024];

  ssize_t received = recv(client_fd, buffer, 1024, 0);

  std::cout.write(buffer, received) << std::endl;
  close(client_fd);
  close(server_fd);
  return 0;
}