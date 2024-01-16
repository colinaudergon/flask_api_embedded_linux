/*
 ***************************************************************************
 * \brief   Embedded Linux BTE5446 FrameBuffer Exercise.
 *	        Fill the screen on the Linux frame buffer device with a color.
 *	        Not optimized and only a minimal error handling is implemented.
 * \file    AppFramefufferShapes.c
 * \version 1.1
 * \date    24/01/2011
 * \author  Colin Audergon (audec2@bfh.ch)
 *
 * \remark  Last Modifications:
 * \remark  V1.1, 24/01/2011 (DD/MM/YY) Colin Audergon (audec2@bfh.ch)
 *  \remark add argument input to display array of
 ***************************************************************************
 *
 * Stop console blinking cursor
 * @target sudo echo 0 > /sys/class/graphics/fbcon/cursor_blink
 *
 ******************************************************************************
 */

/* Declare the function prototypes headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <linux/fb.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

/******************************************************************************/
/* Macros								      */
/******************************************************************************/

#define CONVERT_RGB24(red, green, blue) \
  ((red << 16) | (green << 8) | (blue))

/******************************************************************************/
/* Define some constants						      */
/******************************************************************************/

#define BPP32 32

/*
 * Color definitions   	        R    G    B
 */

#define BLACK CONVERT_RGB24(0, 0, 0)
#define RED CONVERT_RGB24(255, 0, 0)
#define GREEN CONVERT_RGB24(0, 255, 0)
#define YELLOW CONVERT_RGB24(255, 255, 0)
#define BLUE CONVERT_RGB24(0, 0, 255)
#define MAGENTA CONVERT_RGB24(255, 0, 255)
#define CYAN CONVERT_RGB24(0, 255, 255)
#define GREY CONVERT_RGB24(192, 192, 192)
#define WHITE CONVERT_RGB24(255, 255, 255)

/******************************************************************************/
/* Define some data types						      */
/******************************************************************************/

struct RGB_COLOR
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

/******************************************************************************/
/* Static Variables							      */
/******************************************************************************/
static uint16_t h;
static uint16_t w;
/* Screen Info */
static struct fb_var_screeninfo fbVarScreenInfo;

static struct RGB_COLOR *pfb_rgb; // Updated to use RGB_COLOR structure

/******************************************************************************/
/* Function prototypes						      */
/******************************************************************************/
int validateImageArray(struct RGB_COLOR *rgbValues, int height, int width);

/*
 ******************************************************************************
 * main
 ******************************************************************************
 */

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *inputFile = fopen(argv[1], "r");
  if (!inputFile)
  {
    perror("Error opening input file");
    return 1;
  }

  int height, width;
  if (fscanf(inputFile, "%d %d", &height, &width) != 2)
  {
    fprintf(stderr, "Error reading height and width from input file\n");
    fclose(inputFile);
    return 1;
  }

  // Allocate memory for pfb_rgb
  // pfb_rgb = malloc(fbVarScreenInfo.yres_virtual * fbVarScreenInfo.xres_virtual * sizeof(struct RGB_COLOR));
  pfb_rgb = malloc(fbVarScreenInfo.yres * fbVarScreenInfo.xres * sizeof(struct RGB_COLOR));
  if (pfb_rgb == NULL)
  {
    perror("Error: failed to allocate memory for pfb_rgb");
    fclose(inputFile);
    return 1;
  }

  /* Figure out the size of the screen in bytes */
  // int32_t screensize = (fbVarScreenInfo.xres * fbVarScreenInfo.yres * fbVarScreenInfo.bits_per_pixel) / 8;

  /*
   * Map the frame buffer device memory to user space.
   */
  int32_t fb_fd = open("/dev/fb0", O_RDWR);
  if (fb_fd == -1)
  {
    perror("Error: cannot open framebuffer device");
    exit(errno);
  }

  if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbVarScreenInfo) == -1)
  {
    perror("Error reading variable information");
    close(fb_fd);
    exit(errno);
  }

  // int32_t screensize = (fbVarScreenInfo.yres_virtual * fbVarScreenInfo.xres_virtual * fbVarScreenInfo.bits_per_pixel) / 8;
  int32_t screensize = (fbVarScreenInfo.xres * fbVarScreenInfo.yres * fbVarScreenInfo.bits_per_pixel) / 8;
  int32_t *pfb32 = (int32_t *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
  printf("X pixels: %d; Y pixels: %d\n", fbVarScreenInfo.xres, fbVarScreenInfo.yres);
  // printf("Screensize: %d\npfb32: %p\n",screensize, temp);
  if (pfb32 == (int32_t *)-1)
  {
    perror("Error: failed to map 32-BPP framebuffer device to memory");
    exit(errno);
  }
  if (fbVarScreenInfo.bits_per_pixel == BPP32)
  {
    /* Fill the screen with 32 bpp, do it for all [x,y] pixel with desired color */
      for (h = 0; h < fbVarScreenInfo.yres; h++)
    {
    for (w = 0; w < fbVarScreenInfo.xres; w++)
      {
        pfb32[w + h * fbVarScreenInfo.xres] = CONVERT_RGB24(w, h, 0);
        // printf("pixel position HV: %d\n", w + h * fbVarScreenInfo.xres);
      }
    }

    // for (h = 0; h < height; h++)
    // {
    //   for (w = 0; w < width; w++)
    //   {
    //     struct RGB_COLOR rgbValue;
    //     if (fscanf(inputFile, "%hhu %hhu %hhu", &rgbValue.r, &rgbValue.g, &rgbValue.b) != 3)
    //     {
    //       fprintf(stderr, "Error reading RGB values from input file\n");
    //       fclose(inputFile);
    //       free(pfb_rgb);
    //       return 1;
    //     }
    //     // pfb32[w + h * fbVarScreenInfo.xres] = CONVERT_RGB24(rgbValue.r, rgbValue.g, rgbValue.b);
    //     pfb32[w + h * fbVarScreenInfo.xres] = CONVERT_RGB24(w, h, 0);
    //   }
      // int32_t pos = h + w * fbVarScreenInfo.xres;
      // printf("PosX: %d; PosY: %d\nPosTot\n\n: %d", w, h, pos);
    }
    // // Set the rotation
    // fbVarScreenInfo.rotate = FB_ROTATE_CW; // Adjust this line based on your specific rotation value

    // // Write back the updated information
    // if (ioctl(fb_fd, FBIOPUT_VSCREENINFO, &fbVarScreenInfo) == -1)
    // {
    //   perror("Error setting rotation");
    //   close(fb_fd);
    //   exit(errno);
    // }
  }
  else
  {
    printf("fbVarScreenInfo.bits_per_pixel not correctly configured");
    return EXIT_FAILURE;
  }

  /* Cleanup */
  free(pfb_rgb);
  munmap(pfb32, screensize);
  close(fb_fd);
  fclose(inputFile);

  return EXIT_SUCCESS;
}

// Function to check if rgbValues is a valid array with expected length
int validateImageArray(struct RGB_COLOR *rgbValues, int height, int width)
{
  if (rgbValues == NULL)
  {
    fprintf(stderr, "Error: rgbValues is NULL\n");
    return 0;
  }

  int expectedLength = height * width;
  if (expectedLength <= 0)
  {
    fprintf(stderr, "Error: Invalid height or width\n");
    return 0;
  }

  return 1; // rgbValues is valid
}
