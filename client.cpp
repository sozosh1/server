#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>

int main() {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("socket");
    return -1;
  }
  sockaddr_in address = {};
  address.sin_port = htons(1231);
  address.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

  if (connect(client_fd, reinterpret_cast<sockaddr*>(&address),
              sizeof(address)) < 0) {
    perror("connect");
    close(client_fd);
    return -1;
  }

  while (1) {
    char text[256];
    std::cin.getline(text, sizeof(text));
    int size = send(client_fd, text, strlen(text), 0);
    if (size < 0) {
      perror("send");
      break;
    }

    char data[256];
    size = recv(client_fd, data, sizeof(data), 0);
    if (size < 0) {
      perror("recv");
      break;
    }

    std::cout << "Recieved: " << size << " Text:" << data << std::endl;
  }

  if (close(client_fd) == -1) {
    perror("close");
    return -1;
  }

  return 1;
}