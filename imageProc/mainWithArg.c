/*
 ***************************************************************************
 * \brief   Embedded Linux BTE5446 FrameBuffer Exercise.
 *	        Fill the screen on the Linux frame buffer device with a color.
 *	        Not optimized and only a minimal error handling is implemented.
 * \file    AppFramefufferShapes.c
 * \version 1.0
 * \date    28.10.2013
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.1, 11/01/24 (DD/MM/YY) Colin Audergon (audec2@bfh.ch)
 *  \remark add argument input to display array of
 ***************************************************************************
 *
 * Stop console blinking cursor
 * @target sudo echo 0 > /sys/class/graphics/fbcon/cursor_blink
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>

struct RGB_COLOR
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

static struct RGB_COLOR *pfb_rgb;
static uint16_t y, x;
static struct fb_var_screeninfo fbVarScreenInfo;
static int32_t *pfb32;
static int32_t screensize;

int validateImageArray(uint8_t ***imageArray, int height, int width);

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
  pfb_rgb = malloc(height * width * sizeof(struct RGB_COLOR));
  if (pfb_rgb == NULL)
  {
    perror("Error: failed to allocate memory for pfb_rgb");
    fclose(inputFile);
    return 1;
  }

  int fb_fd = open("/dev/fb0", O_RDWR);
  if (fb_fd == -1)
  {
    perror("Error: cannot open framebuffer device");
    fclose(inputFile);
    free(pfb_rgb);
    return 1;
  }

  if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbVarScreenInfo) == -1)
  {
    perror("Error reading variable information");
    close(fb_fd);
    fclose(inputFile);
    free(pfb_rgb);
    return 1;
  }

  screensize = (fbVarScreenInfo.xres_virtual * fbVarScreenInfo.yres_virtual * fbVarScreenInfo.bits_per_pixel) / 8;

  pfb32 = (int32_t *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
  if (pfb32 == (int32_t *)-1)
  {
    perror("Error: failed to map 32-BPP framebuffer device to memory");
    close(fb_fd);
    fclose(inputFile);
    free(pfb_rgb);
    return 1;
  }

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      struct RGB_COLOR rgbValue;
      if (fscanf(inputFile, "%hhu %hhu %hhu", &rgbValue.r, &rgbValue.g, &rgbValue.b) != 3)
      {
        fprintf(stderr, "Error reading RGB values from input file\n");
        fclose(inputFile);
        munmap(pfb32, screensize);
        close(fb_fd);
        free(pfb_rgb);
        return 1;
      }

      uint32_t pixel_pos = x + y * fbVarScreenInfo.xres_virtual;
      uint32_t pixel = (rgbValue.r << 16) | (rgbValue.g << 8) | rgbValue.b;

      pfb_rgb[pixel_pos].r = rgbValue.r;
      pfb_rgb[pixel_pos].g = rgbValue.g;
      pfb_rgb[pixel_pos].b = rgbValue.b;
    }
  }

  fclose(inputFile);
  munmap(pfb32, screensize);
  close(fb_fd);
  free(pfb_rgb);

  return EXIT_SUCCESS;
}

