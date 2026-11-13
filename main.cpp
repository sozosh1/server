#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

int main() {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("socket");
    return 1;
  }

  sockaddr_in address{};

  address.sin_family = AF_INET;
  address.sin_port = htons(8080);

  if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) != 1) {
    std::cerr << "Invalid adress";
    return 1;
  }

  if (connect(client_fd, reinterpret_cast<sockaddr*>(&address),
              sizeof(address)) != 0) {
    perror("connect");
    close(client_fd);
    return 1;
  }
  char buffer1[1024];
  ssize_t rec1 = recv(client_fd, buffer1, 1024, 0);
  if (rec1 == -1) {
    perror("recv");
    close(client_fd);
    return 1;

  } else if (rec1 == 0) {
    std::cout << "Server closed the connection" << std::endl;
    close(client_fd);
    return 0;
  }

  if (rec1 > 0) std::cout.write(buffer1, rec1) << std::endl;

  const char message[] = "Hello, server!";
  ssize_t sent = send(client_fd, message, sizeof(message) - 1, 0);
  if (sent == -1) {
    perror("send");
    close(client_fd);
    return 1;
  }

  std::cout << "sent bytes: " << sent << std::endl;

  char buffer[1024];

  ssize_t rec = recv(client_fd, buffer, 1024, 0);

  if (rec == -1) {
    perror("recv");
    close(client_fd);
    return 1;

  } else if (rec == 0) {
    std::cout << "Server closed the connection" << std::endl;
    close(client_fd);
    return 0;
  }

  std::cout << "rec bytes: " << rec << std::endl;

  if (rec > 0) std::cout.write(buffer, rec) << std::endl;
  close(client_fd);
}