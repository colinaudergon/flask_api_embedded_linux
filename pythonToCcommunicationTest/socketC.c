#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/gpio.h>
#include <linux/i2c-dev.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/types.h>

#define UNIX_PATH_MAX 108
#define SOCKET_READY 0xFF

bool establishConnection(void);
bool runSocket = true;

struct sockaddr_un address;
int socket_fd;

int main(void)
{
    printf("Hello Python!\n");

    // Specify the socket file path
    const char *socket_path = "/tmp/socket";
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_path, UNIX_PATH_MAX - 1);

    char buffer[256];
    int n;
    uint8_t valueToSend;

    if (!establishConnection())
    {
        // Handle the failure appropriately
        // cleanupRelease();
        return 1;
    }
    else
    {
        runSocket = true;
        valueToSend = SOCKET_READY;
    }
    printf("Value to send is set\n");

    uint8_t inputState = 0x00;
    while (1)
    {
        n = write(socket_fd, &inputState, sizeof(valueToSend));
        if (n < 0)
        {
            perror("ERROR writing to socket");
            return 1;
        }
        printf("input state: 0x%02X\n", inputState);

        sleep(1);
    }
    return 0;
}

/*****************************************************************************/
/* Socket establishment Function											 */
/*****************************************************************************/
bool establishConnection(void)
{
    int max_connection_attempts = 10;
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("Error creating socket");
        return false;
    }

    while (max_connection_attempts > 0)
    {
        if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) == 0)
        {
            printf("Connected to the server\n");
            return true; // Exit the function with success
        }
        else
        {
            perror("Error connecting to the server");
            max_connection_attempts--;

            // Wait for a short duration before retrying
            sleep(1);
        }
    }

    // Connection attempts exhausted, handle the error as needed
    fprintf(stderr, "Failed to establish a connection\n");
    close(socket_fd); // Close the socket if connection fails
    return false;     // Exit the function with failure
}
