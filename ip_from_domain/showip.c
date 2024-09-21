#include <arpa/inet.h>   // Provides definitions for internet operations
#include <netdb.h>       // Provides definitions for network database operations
#include <stdio.h>       // Standard I/O functions
#include <string.h>      // String manipulation functions

int main(int ac, char **av) {
  struct addrinfo hints;         // Structure to specify criteria for getaddrinfo()
  struct addrinfo *res;          // Pointer to hold the result of getaddrinfo()
  struct addrinfo *r;            // Pointer to iterate through the linked list of results
  int status;                    // Variable to hold the return status of getaddrinfo()
  char buffer[INET6_ADDRSTRLEN]; // Buffer to store the converted IP address string

  // Check for correct number of command line arguments (hostname)
  if (ac != 2) {
    fprintf(stderr, "usage: /a.out hostname\n"); // Error message for incorrect usage
    return (1); // Exit with error code
  }

  // Initialize the hints structure to zero
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     // Allow both IPv4 and IPv6 addresses
  hints.ai_socktype = SOCK_STREAM; // We are looking for TCP sockets

  // Get the associated IP address(es) based on the hostname provided
  status = getaddrinfo(av[1], 0, &hints, &res);
  if (status != 0) { // Check for errors in getaddrinfo
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)); // Print error message
    return (2); // Exit with error code
  }

  printf("IP addresses for %s:\n", av[1]); // Print the hostname for which we are finding IPs

  r = res; // Start with the first result
  while (r != NULL) { // Iterate through the results
    void *addr;                    // Pointer to hold the IP address

    // Check if the address family is IPv4
    if (r->ai_family == AF_INET) { // IPv4
      // Cast ai_addr to sockaddr_in to access the IPv4 address
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)r->ai_addr;
      // Convert the binary IP address to a human-readable string format
      inet_ntop(r->ai_family, &(ipv4->sin_addr), buffer, sizeof buffer);
      printf("IPv4: %s\n", buffer); // Print the IPv4 address
    } else { // Otherwise, it must be IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)r->ai_addr; // Cast to sockaddr_in6
      inet_ntop(r->ai_family, &(ipv6->sin6_addr), buffer, sizeof buffer); // Convert to string
      printf("IPv6: %s\n", buffer); // Print the IPv6 address
    }
    r = r->ai_next; // Move to the next address in the linked list
  }

  freeaddrinfo(res); // Free the memory allocated for the address information
  return (0); // Exit successfully
}
