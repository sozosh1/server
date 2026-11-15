#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket");
    return -1;
  }
  sockaddr_in address = {};
  address.sin_port = htons(1231);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) !=
      0) {
    perror("bind");
    close(server_fd);
    return -1;
  }

  if (listen(server_fd, 5) != 0) {
    perror("listen");
    close(server_fd);
    return -1;
  }

  int client_fd = accept(server_fd, nullptr, nullptr);

  while (1) {
    char buffer[255] = {};
    int size = recv(client_fd, buffer, sizeof(buffer), 0);
    if (size < 0) {
      perror("recv");
      break;
    }

    std::cout << "Recieved: " << size << std::endl;
    int sent = send(client_fd, buffer, size, 0);
    if (sent < 0) {
      perror("send");
      break;
    }

    std::cout << "Sent: " << sent << std::endl;
  }

  if (close(client_fd) != 0) {
    perror("close");
    return -1;
  }
  if (close(server_fd) != 0) {
    perror("close");
    return -1;
  }

  return 1;
}