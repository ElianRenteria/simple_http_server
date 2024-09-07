#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For sockaddr_in structure
#include <arpa/inet.h>   // For inet_addr() to manipulate IP addresses
#include <unistd.h>      // For close() function
#include <string.h>      // For string manipulation
#include <stdio.h>       // For I/O functions like printf(), perror()

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Bind to all network interfaces
    address.sin_port = htons(8080);        // Port number in network byte order

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) { // 3 connection limit
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }
    char buffer[30000] = {0};
    read(new_socket, buffer, 30000);
    printf("Received:\n%s\n", buffer);
    char *response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\nHello World";
    write(new_socket, response, strlen(response));

    close(new_socket);

}
