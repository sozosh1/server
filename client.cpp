#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

int main() {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in address = {};

  address.sin_port = htons(8080);
  address.sin_family = AF_INET;
  int in = inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

  if (in == -1) {
    std::cerr << "connet failed";
    close(client_fd);
    return 1;
  }

  if (connect(client_fd, reinterpret_cast<sockaddr*>(&address),
              sizeof(address)) == -1) {
    perror("connect");
    close(client_fd);
    return 1;
  }
  const char message[] = "Hello, server!";
  ssize_t sent = send(client_fd, message, sizeof(message) - 1, 0);

  if (sent == -1) {
    perror("send");
    close(client_fd);
    return 1;
  }

  std::cout << "Sent bytes: " << sent << std::endl;

  close(client_fd);
  return 0;
}