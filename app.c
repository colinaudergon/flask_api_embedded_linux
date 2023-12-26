#define _DEFAULT_SOURCE // usleep wouldnt work without this
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

#define DS400 11 // chipbank 1
#define DS401 14 // chipbank 3
#define DS402 13 // chipbank 3
#define DS403 4  // chipbank 3

#define S400 10 // chipbank 1
#define S401 31 // chipbank 1
#define S402 15 // chipbank 1
#define S403 7  // chipbank 3

#define JOYSTICK_ADDR 0x34

#define JOYSTICK_b_LEFT_RIGHT 0x21
#define JOYSTICK_b_UP_DOWN 0x23

#define JOYSTICK_A_LEFT_RIGHT 0x25
#define JOYSTICK_A_UP_DOWN 0x27

// Socket
#define UNIX_PATH_MAX 108 // UNIX LIKE SOCKET (unused atm)
#define PORT 5050
// #define SOCKET_READY 0xFF

/*****************************************************************************/
/* Global Variables						             						 */
/*****************************************************************************/

// GPIO reading
int ret0, ret1, ret2, ret3;
int retS0, retS1, retS2, retS3;

pthread_t readInputThread;

unsigned int led_lines[] = {DS400, DS401, DS402, DS403};
unsigned int ledOnOff[] = {0, 0, 0, 0};
int num_leds = sizeof(led_lines) / sizeof(led_lines[0]);
// Socket
struct sockaddr_un address;
int socket_fd;
bool runSocket = true;

/*****************************************************************************/
/* Static Variables (Module variable)				             	         */
/*****************************************************************************/
// GPIO reading
static struct gpiohandle_request req;
struct gpiohandle_request req0;
struct gpiohandle_request req1;
struct gpiohandle_request req2;
struct gpiohandle_request req3;

struct gpiohandle_request reqS0;
struct gpiohandle_request reqS1;
struct gpiohandle_request reqS2;
struct gpiohandle_request reqS3;

static struct gpiohandle_data data;

struct gpioevent_request event_req;

static char chrdev_name[20];
static bool running = true;

static const char GPIO_BANK1[] = "/dev/gpiochip1";
static const char GPIO_BANK3[] = "/dev/gpiochip3";

const char I2C_DEVICE[] = "/dev/i2c-0";

/* File descriptor to i2cdev */
static int32_t i2c_fd;

/* i2c communication buffer */
static uint8_t i2cCommBuffer[8];
static uint8_t revID, devID;

static int fd;

/*******************************************************************************/
/* Function prototype							       						   */
/*******************************************************************************/
// GPIO reading
int initialize_gpio_output(const char *gpio_chip, unsigned int gpio_line, int ret);
int initialize_gpio_input(const char *gpio_chip, unsigned int gpio_line, int ret);
void turnOnLed(int ledIndice);
void turnOffLed(int ledIndice);
void cleanupRelease(void);

int setUpAdcValue(uint8_t setup);
int readAdcValue();

uint8_t readInputs();
int readSwitch(int retSwitch, struct gpiohandle_request reqSwitch);
void pulseLed(int ledIndice);

void sleep_ms(int milliseconds);

// Socket
bool establishConnection();

int main(int argc, char **argv)
{
    printf("Hello Python!\n");
    // GPIO reading
    uint8_t inputState = 0x00;

    // LED Init
    ret0 = initialize_gpio_output(GPIO_BANK1, DS400, ret0);
    ret1 = initialize_gpio_output(GPIO_BANK3, DS401, ret1);
    ret2 = initialize_gpio_output(GPIO_BANK3, DS402, ret2);
    ret3 = initialize_gpio_output(GPIO_BANK3, DS403, ret3);

    // Switch init
    retS0 = initialize_gpio_input(GPIO_BANK1, S400, retS0);
    retS1 = initialize_gpio_input(GPIO_BANK1, S401, retS1);
    retS2 = initialize_gpio_input(GPIO_BANK1, S402, retS2);
    retS3 = initialize_gpio_input(GPIO_BANK3, S403, retS3);

    // I2c init
    i2c_fd = open(I2C_DEVICE, O_RDWR);
    if (i2c_fd < 0)
    {
        perror("i2cOpen");
        printf("No such device found!\n");
        return -1;
    }

    /* Set the I2C slave address for all subsequent I2C device transfers */
    if (ioctl(i2c_fd, I2C_SLAVE_FORCE, JOYSTICK_ADDR) < 0)
    {
        perror("i2cSetAddress");
        return -1;
    }

    // Socket
    // Specify the socket file path for real application
    const char *socket_path = "/tmp/socket";
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_path, UNIX_PATH_MAX - 1);

    int n;
    // Call the function to establish the connection
    if (!establishConnection())
    {
        // Handle the failure appropriately
        cleanupRelease();
        return 1;
    }
    else
    {
        runSocket = true;
    }

    while (1)
    {
        inputState = readInputs();
        // Send 8-bit integer
        n = write(socket_fd, &inputState, sizeof(inputState));
        if (n < 0)
        {
            perror("ERROR writing to socket");
            return 1;
        }
        printf("input state: 0x%02X", inputState);
        sleep(1);
    }

    cleanupRelease();
    return 0;
}

/*****************************************************************************/
/* GPIO output Initialization Function											 */
/*****************************************************************************/
int initialize_gpio_output(const char *gpio_chip, unsigned int gpio_line, int ret)
{
    // /*  Open gpio device: gpiochipX */
    strcpy(chrdev_name, gpio_chip);
    fd = open(chrdev_name, 0);
    if (fd == -1)
    {
        ret = -errno;
        fprintf(stderr, "Failed to open %s\n", chrdev_name);

        return ret;
    }

    /* Setup variables for a request */
    req.lineoffsets[0] = gpio_line;
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    memcpy(req.default_values, &data, sizeof(req.default_values));
    snprintf(req.consumer_label, sizeof(req.consumer_label), "led_%u", gpio_line);
    req.lines = 1;

    /* Request GPIO line: GPIO_H_11 --> LED-DS400 */
    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (ret == -1)
    {
        ret = -errno;
        fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
                ret);
    }
    if (close(fd) == -1)
        perror("Failed to close GPIO character device file");

    /* Store the file descriptor in the global req variable */
    if (gpio_line == DS400)
    {
        req0 = req;
    }
    else if (gpio_line == DS401)
    {
        req1 = req;
    }
    else if (gpio_line == DS402)
    {
        req2 = req;
    }
    else if (gpio_line == DS403)
    {
        req3 = req;
    }
    return ret;
}

/*****************************************************************************/
/* GPIO input Initialization Function											 */
/*****************************************************************************/
int initialize_gpio_input(const char *gpio_chip, unsigned int gpio_line, int ret)
{

    // Known working Input init:
    // Open GPIO device
    strcpy(chrdev_name, gpio_chip);
    fd = open(chrdev_name, 0);
    if (fd == -1)
    {
        ret = -errno;
        fprintf(stderr, "Failed to open %s\n", chrdev_name);
        return ret;
    }

    // Setup variables for a request
    // struct gpiohandle_request req;
    req.lineoffsets[0] = gpio_line;
    req.flags = GPIOHANDLE_REQUEST_INPUT;
    snprintf(req.consumer_label, sizeof(req.consumer_label), "btn_%u", gpio_line);
    req.lines = 1;

    // Request GPIO line
    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (ret == -1)
    {
        ret = -errno;
        fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n", ret);
    }

    // Store the file descriptor in the global req variable
    if (gpio_line == S400)
    {
        reqS0 = req;
    }
    else if (gpio_line == S401)
    {
        reqS1 = req;
    }
    if (gpio_line == S402)
    {
        reqS2 = req;
    }
    if (gpio_line == S403)
    {
        reqS3 = req;
    }
    // Close GPIO device (not needed after acquiring the line handle)
    if (close(fd) == -1)
        perror("Failed to close GPIO character device file");

    return ret;
}
/*****************************************************************************/
/*Setup ADC Function											 */
/*****************************************************************************/
int setUpAdcValue(uint8_t setup)
{
    i2cCommBuffer[0] = setup;
    if (write(i2c_fd, i2cCommBuffer, 1) != 1)
    {
        perror("write error!");
        close(i2c_fd);
        return -1;
    }
    return 0;
}

/*****************************************************************************/
/* Read adc value Function											 */
/*****************************************************************************/
int readAdcValue()
{
    if (read(i2c_fd, i2cCommBuffer, 1) != 1)
    {
        perror("read error");
        close(i2c_fd);
        return -1;
    }
    uint8_t value = i2cCommBuffer[0];
    return value;
}
/*****************************************************************************/
/* GPIO and I2C clean up Function											 */
/*****************************************************************************/
void cleanupRelease(void)
{
    close(i2c_fd);
    ret0 = close(req0.fd);
    if (ret0 == -1)
    {
        perror("Failed to close GPIO LINEHANDLE device file");
        ret0 = -errno;
    }
    ret1 = close(req1.fd);
    if (ret1 == -1)
    {
        perror("Failed to close GPIO LINEHANDLE device file");
        ret1 = -errno;
    }
    ret2 = close(req2.fd);
    if (ret1 == -1)
    {
        perror("Failed to close GPIO LINEHANDLE device file");
        ret2 = -errno;
    }
    ret3 = close(req3.fd);
    if (ret3 == -1)
    {
        perror("Failed to close GPIO LINEHANDLE device file");
        ret3 = -errno;
    }
}
/*****************************************************************************/
/* Read Inputs Function											 */
/*****************************************************************************/
/*Reads the gpio inputs, outputs a varible inputState in hexa, binary code represents inputs value
b7  |b6  |b5  |b4  |b3| b2 | b1 | b0
S400|S401|S402|S403|up|down|left|right
TODO: add range for joystic input
*/
uint8_t readInputs()
{
    uint8_t adcValue;
    int8_t inputState = 0x00;

    uint8_t joystickUpReading = 0xFC;    // 252
    uint8_t joystickDownReading = 0xF1;  // 241
    uint8_t joystickRightReading = 0xFC; // 252
    uint8_t joystickLeftReading = 0xF2;  // 242

    uint8_t joystickUp = 0x08;
    uint8_t joystickDown = 0x04;
    uint8_t joystickRight = 0x02;
    uint8_t joystickLeft = 0x01;

    uint8_t startPressed = 0x80;
    uint8_t selectPressed = 0x40;
    uint8_t aPressed = 0x20;
    uint8_t bPressed = 0x10;
    uint8_t errorCode = 0xFF;

    if (setUpAdcValue(JOYSTICK_A_UP_DOWN) < 0)
    {
        printf("Failed to configure ADC");
        perror("Setup ADC");
        return errorCode;
        // isReading = false;
    }
    adcValue = readAdcValue();
    printf("U/D Value: %d\n", adcValue);

    if (adcValue == joystickUpReading)
    {
        inputState |= joystickUp;
    }
    if (adcValue == joystickLeftReading)
    {
        inputState |= joystickDown;
    }

    sleep_ms(20);
    if (setUpAdcValue(JOYSTICK_A_LEFT_RIGHT) < 0)
    {
        printf("Failed to configure ADC");
        perror("Setup ADC");
        // isReading = false;
        return errorCode;
    }
    adcValue = readAdcValue();
    printf("L/R Value: %d\n", adcValue);

    // if (adcValue == joystickRightReading)
    // {
    //     inputState |= joystickRight;
    // }
    // if (adcValue == joystickLeftReading)
    // {
    //     inputState |= joystickLeft;
    // }

    // if (!readSwitch(retS0, reqS0))
    // {
    //     inputState |= startPressed;
    //     pulseLed(0);
    //     printf("Switch S400 active\n");
    // }
    // if (!readSwitch(retS1, reqS1))
    // {
    //     inputState |= selectPressed;
    //     pulseLed(1);
    //     printf("Switch S401 active\n");
    // }
    // if (!readSwitch(retS2, reqS2))
    // {
    //     inputState |= aPressed;
    //     pulseLed(2);
    //     printf("Switch S402 active\n");
    // }
    // if (!readSwitch(retS3, reqS3))
    // {
    //     inputState |= bPressed;
    //     pulseLed(3);
    //     printf("Switch S403 active\n");
    // }
    // sleep_ms(10);
    printf("Inputs state: 0x%02X\n", inputState);
    return inputState;
}
/*****************************************************************************/
/* Read switches Function											 */
/*****************************************************************************/
int readSwitch(int retSwitch, struct gpiohandle_request reqSwitch)
{
    int value_switch = 0;
    retSwitch = ioctl(reqSwitch.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &value_switch);
    if (retSwitch == -1)
    {
        retSwitch = -errno;
        fprintf(stderr, "Failed to get line values for S (%d)\n", retSwitch);
    }
    return value_switch;
}

/*****************************************************************************/
/* Pulse a led for 10ms											 */
/*****************************************************************************/
void pulseLed(int ledIndice)
{
    // Turn on the specified LED
    ledOnOff[ledIndice] = 1;
    ret0 = ioctl(req0.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[0]);
    ret1 = ioctl(req1.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[1]);
    ret2 = ioctl(req2.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[2]);
    ret3 = ioctl(req3.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[3]);

    sleep_ms(10); // Wait for a short duration

    // Turn off the specified LED
    ledOnOff[ledIndice] = 0;
    ret0 = ioctl(req0.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[0]);
    ret1 = ioctl(req1.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[1]);
    ret2 = ioctl(req2.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[2]);
    ret3 = ioctl(req3.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ledOnOff[3]);
}

/*****************************************************************************/
/* Sleep in ms Function											 */
/*****************************************************************************/
void sleep_ms(int milliseconds)
{
    usleep(milliseconds * 1000);
}

/*****************************************************************************/
/* Socket establishment Function											 */
/*****************************************************************************/
// connect(socket_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un)) != 0
// connect(socket_fd, (struct sockaddr *)&address, sizeof(address))
// if (connect(socket_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un)) == 0)
bool establishConnection()
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