// article:
// https://dev.to/sanjayrv/a-beginners-guide-to-socket-programming-in-c-5an5

// sockets and networking in C:
// https://www.codequoi.com/en/sockets-and-network-programming-in-c/

// basic server that listents to localhost 8081
// we you can test it by building the file
// gcc name.c -o name && ./name
// and from other terminal nc localhost 8081
// nc -> netcat

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <sys/types.h>

// we need to prepare socket connection on our server and client
// form the tcp/ip connection
// making the server accepting the requests
// sending the requests from the client to server and then waiting fo the
// response

// we need to form the domain
// domain will have protocol family and address family ( IP and ARP )
// IP routing the packets between devices while ARP is mapping the IP addresses
// to physical addresses

// PF -> Protocol Family
// AF -> Address Family

// SOCK_STREAM ensures stable connection between devices and reliabley
// transmitting the data
//
// SOCK_DGRAM makes transmitting data faster between two connected devices over
// TCP but data package loss is possible

// type will be SOCK_STREAM

#define PORT "8081"
#define BACKLOG 10

int main(void) {
  struct addrinfo hints;
  struct addrinfo *res;
  // socket file descriptor
  int socket_fd;
  // client file descriptor
  int client_fd;

  int status;

  // sockaddr_storage is a structure that is not associated to
  // a particular family. This allows us to receive either
  // an IPv4 or an IPv6 address
  struct sockaddr_storage client_addr;
  socklen_t addr_size;

  // Prepare the address and port for the server socket

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6 -> supporting both
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags = AI_PASSIVE;     // Automaticlly fills IP address

  status = getaddrinfo(NULL, PORT, &hints, &res);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
  }

  // the file descriptor of the socket is used for
  // sending data through it

  // creating socket, binding it and listening with it
  socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  status = bind(socket_fd, res->ai_addr, res->ai_addrlen);

  if (status != 0) {
    fprintf(stderr, "bind: %s\n", strerror(errno));
    return (2);
  }

  listen(socket_fd, BACKLOG);

  // Accept incoming connection
  addr_size = sizeof(client_addr);

  // client side file descriptor
  client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);

  if (client_fd == -1) {
    fprintf(stderr, "accept %s\n", strerror(errno));
  }

  printf("New connection! Socket fd: %d, client fd: %d\n", socket_fd,
         client_fd);

  return 0;
}
