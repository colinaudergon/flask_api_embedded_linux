// reading button S400 to S403 -> GPIO poll
// 7segment display Ds300 to DS303
// pot reading PR1
// HOW TO USE:
/*
    In project root folder:
    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-none-linux-aarch64-gcc.cmake
    make
    (if needed: sudo mount -a)
    make install
*/
#define _DEFAULT_SOURCE // usleep wouldnt work without this
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/gpio.h>
#include <linux/i2c-dev.h>

#include <math.h>
#include <sys/stat.h>

#include "spi.h"
#include "gpiolib.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/types.h>

/******************************************************************************/
/* Defines and Typedefs							      						  */
/******************************************************************************/

#pragma GCC diagnostic ignored "-Wunused-result"
// #pragma GCC diagnostic ignored "-Wunused-result"

#define NR_OF_SEGMENTS 4
#define BUFFER_SIZE 16 * 1024

#define SEVEN_SEG_ENABLE 41

// BTN
#define S400 10 // chipbank 1
#define S401 31 // chipbank 1
#define S402 15 // chipbank 1
#define S403 7  // chipbank 3

// LED
#define DS400 11 // chipbank 1
#define DS401 14 // chipbank 3
#define DS402 13 // chipbank 3
#define DS403 4  // chipbank 3

#define MS 1000
#define TRUE 1
#define FALSE 0

#define JOYSTICK_ADDR 0x34

#define JOYSTICK_b_LEFT_RIGHT 0x21
#define JOYSTICK_b_UP_DOWN 0x23

#define JOYSTICK_A_LEFT_RIGHT 0x25
#define JOYSTICK_A_UP_DOWN 0x27
#define UNIX_PATH_MAX 108
#define SOCKET_READY 0xFF

/*****************************************************************************/
/* Global Variables						             						 */
/*****************************************************************************/

uint8_t joystickUpReading = 0xFA;    // 250
uint8_t joystickDownReading = 0xF4;  // 244
uint8_t joystickRightReading = 0xFA; // 252
uint8_t joystickLeftReading = 0xF4;  // 244

uint8_t joystickUp = 0x08;
uint8_t joystickDown = 0x04;
uint8_t joystickRight = 0x02;
uint8_t joystickLeft = 0x01;

uint8_t startPressed = 0x80;
uint8_t selectPressed = 0x40;
uint8_t aPressed = 0x20;
uint8_t bPressed = 0x10;
uint8_t errorCode = 0xFF;
/******************************************************************************/
/* Static variables																  */
/******************************************************************************/

/* Seven Segment Table Index */

static bool running = true;

/* File descriptor to i2cdev */
static int32_t i2c_fd;
/* i2c communication buffer */
static uint8_t i2cCommBuffer[8];
static uint8_t revID, devID;

static int fd;

static const char GPIO_BANK1[] = "/dev/gpiochip1";
static const char GPIO_BANK3[] = "/dev/gpiochip3";

const char I2C_DEVICE[] = "/dev/i2c-0";

static char chrdev_name[20];
static int fd;
// static struct gpiohandle_request req;

typedef struct gpio_desc
{
    struct gpiohandle_request req;
    char gpiobank[sizeof(GPIO_BANK1)];
    int direction; // Set direction to 2 = high output, 1 low output, 0 input
    int ret;
    int status;
} gpio_desc;

/******************************************************************************/
/* Variables																  */
/******************************************************************************/
int adc_fd;
int charRead;
char adcBuffer[BUFFER_SIZE];
/******************************************************************************/
/* Function prototype							      						  */
/******************************************************************************/

int init7SegDisplay(spi_t *spi);
void release7SegDisplay(spi_t *spi, int sevenSegEn);

int initADC(void);
char *readADCChar(int verbose);
int readADCInt(int verbose);

struct gpio_desc initGpioInput(const char *gpio_chip, unsigned int gpio_line);
struct gpio_desc initGpioOutput(const char *gpio_chip, unsigned int gpio_line);
int readBtn(struct gpio_desc gpio);
int controlGpioOut(struct gpio_desc gpio, int value);

int setUpAdcValue(uint8_t setup);
int readAdcValue();

void signal_callback_handler(int signum);
// void exitProgramm();
bool establishConnection(void);


bool runSocket = true;

struct sockaddr_un address;
int socket_fd;
/******************************************************************************/
/* main()																      */
/******************************************************************************/

int main(void)
{
    /* Register signal and signal handler */
    signal(SIGINT, signal_callback_handler);

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

    // BTN init
    struct gpio_desc s400 = initGpioInput(GPIO_BANK1, S400);
    if (s400.status < 0)
    {
        printf("Error init S400");
    }
    struct gpio_desc s401 = initGpioInput(GPIO_BANK1, S401);
    if (s401.status < 0)
    {
        printf("Error init S401");
    }
    struct gpio_desc s402 = initGpioInput(GPIO_BANK1, S402);
    if (s402.status < 0)
    {
        printf("Error init S402");
    }
    struct gpio_desc s403 = initGpioInput(GPIO_BANK3, S403);
    if (s403.status < 0)
    {
        printf("Error init S403");
    }

    // LED init
    struct gpio_desc ds403 = initGpioOutput(GPIO_BANK3, DS403);
    if (ds403.status < 0)
    {
        printf("Error init ds400");
    }

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
    int swval = 1;
    while (running)
    {

        swval = readBtn(s400);
        controlGpioOut(ds403, swval);

        n = write(socket_fd, &swval, sizeof(valueToSend));
        if (n < 0)
        {
            perror("ERROR writing to socket");
            return 1;
        }
        printf("input state: 0x%02X\n", swval);
    }

    close(adc_fd);
    return EXIT_SUCCESS;
}

struct gpio_desc initGpioInput(const char *gpio_chip, unsigned int gpio_line)
{
    int ret;
    int status;
    struct gpiohandle_request req;
    strcpy(chrdev_name, gpio_chip);
    fd = open(chrdev_name, 0);
    if (fd == -1)
    {
        ret = -errno;
        fprintf(stderr, "Failed to open %s\n", chrdev_name);

        status = -1;
    }
    else
    {
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
            status = -1;
        }
        // Close GPIO device (not needed after acquiring the line handle)
        if (close(fd) == -1)
        {
            perror("Failed to close GPIO character device file");
            status = -1;
        }
        else
        {
            status = 0;
        }
    }
    struct gpio_desc gpio;
    strcpy(gpio.gpiobank, gpio_chip);
    gpio.direction = 0;
    gpio.req = req;
    gpio.ret = ret;
    gpio.status = status;
    return gpio;
}

struct gpio_desc initGpioOutput(const char *gpio_chip, unsigned int gpio_line)
{
    int ret;
    int status;
    struct gpiohandle_request req;
    static struct gpiohandle_data data;

    // /*  Open gpio device: gpiochipX */
    strcpy(chrdev_name, gpio_chip);
    fd = open(chrdev_name, 0);
    printf("prout0\n");
    if (fd == -1)
    {
        ret = -errno;
        fprintf(stderr, "Failed to open %s\n", chrdev_name);
        status = -1;
    }
    else
    {
        printf("prout1\n");
        /* Setup variables for a request */
        req.lineoffsets[0] = gpio_line;
        req.flags = GPIOHANDLE_REQUEST_OUTPUT;
        memcpy(req.default_values, &data, sizeof(req.default_values));
        snprintf(req.consumer_label, sizeof(req.consumer_label), "led_%u", gpio_line);
        req.lines = 1;

        /* Request GPIO line: GPIO_H_11 --> LED-DS400 */
        ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
        printf("prout2\n");
        if (ret == -1)
        {
            ret = -errno;
            status = -1;
            fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
                    ret);
        }
        printf("prout3\n");
        if (close(fd) == -1)
        {
            perror("Failed to close GPIO character device file");
            status = -1;
        }
    }
    printf("prout4!\n");
    struct gpio_desc gpio;
    strcpy(gpio.gpiobank, gpio_chip);
    gpio.direction = 1;
    gpio.req = req;
    gpio.ret = ret;
    gpio.status = status;
    return gpio;
}

int controlGpioOut(struct gpio_desc gpio, int value)
{
    gpio.ret = ioctl(gpio.req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &value);
    return gpio.ret;
}

int readBtn(struct gpio_desc gpio)
{

    int value_switch = 0;
    gpio.ret = ioctl(gpio.req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &value_switch);
    if (gpio.ret == -1)
    {
        gpio.ret = -errno;
        fprintf(stderr, "Failed to get line values(%d)\n", gpio.ret);
    }
    printf("Value: %d\n", value_switch);
    return value_switch;
}

void signal_callback_handler(int signum)
{
    /* Set termination flag */
    running = false;
    printf("\nProgram terminated by Ctrl-C!\n\n");
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
