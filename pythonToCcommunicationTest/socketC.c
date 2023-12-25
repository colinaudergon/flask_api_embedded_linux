#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define UNIX_PATH_MAX 108

bool runSocket = true;

int main(void)
{
    printf("Hello python!");
    struct sockaddr_un address;
    int socket_fd;
    char buffer[256];
    int n;

    while (1)
    {
        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

        if (socket_fd < 0)
        {
            perror("socket() failed");
            return 1;
        }

        printf("Socket initiated");

        memset(&address, 0, sizeof(struct sockaddr_un));
        address.sun_family = AF_UNIX;
        snprintf(address.sun_path, UNIX_PATH_MAX, "/tmp/demo_socket");
        // int isConnected = 1;

        if (connect(socket_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un)) != 0)
        {
            perror("connect() failed");
            return 1;
        }
        
        while (runSocket == true)
        {
            strcpy(buffer, "Hello from the C client");
            n = write(socket_fd, buffer, strlen(buffer));
            if (n < 0)
            {
                perror("ERROR writing to socket");
                return 1;
            }

            bzero(buffer, 256);
            n = read(socket_fd, buffer, 255);
            if (n < 0)
            {
                perror("ERROR reading from socket");
                return 1;
            }
            if (strcmp(buffer, "exit") == 0)
            {
                runSocket = false;
                printf("Runsocket value: %d\n", runSocket);
                printf("Received message from server: %s\n", buffer);
            }
            else
            {
                printf("Received message from server: %s\n", buffer);
            }
        }
        printf("Closing socket\n");
        close(socket_fd);
    }
    return 0;
}
