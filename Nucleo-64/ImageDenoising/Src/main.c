// ===========  PROJECT 2 =================
#include <stdint.h>
#include <STM32_util.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f0xx.h"
#include "DEVICES.h"

long i;
volatile long Sys_Ticks, millisec, CountDown_ms;


enum PORT_CONNECTION
{
	USER_LED_PIN = 5,
	RED_PB,
	BLACK_PB,
	YELLOW_PB,
	WHITE_PB,
	BLUE_PB,
	BLUE_BUTTON_PIN = 13
} __attribute__ ((packed));


/* OLED Macros *****************************************************************/
#define OLED_NUM_PAGE   8
#define OLED_NUM_ROWS  64
#define OLED_NUM_COLS 128

/* 8 rows per page, quotient gives page # */
#define OLED_ROW2PAGE(row) ( row / 8 )
/* 8 vertical pixels per page, row % 8 gives bit position of target pixel */
#define OLED_ROW2PIX(pix)  ( 0x1 << (pix % 8) )


/* Digital Image Macros ********************************************************/
#define IM_NUM_ROWS 16
#define IM_NUM_COLS 32

/* If the image row is odd (LSB = 1), fill the MSByte  */
/* If the image row is even (LSB = 0), fill the LSByte */
#define IM_ROWFILL_MASK(row)	( (row & 0x01) ? 0xF0 : 0x0F )

/* 2 image rows per OLED page, integer division gives appropriate page number */
#define IMROW_2_OLEDPAGE(row)	( row / 2 )

/* Each image column starts with an OLED column that's a multiple of 4 		  */
/* 	e.g., im_col 0 starts at OLED_col 0, im_col 1 starts at OLED_col 4, etc.  */
#define IMCOL_2_OLEDCOL(col)	( 4 * col )


/* Digital Image Data *********************************************************/
uint8_t im_ori[8][128];
uint8_t im_noisy[8][128];
uint8_t im_enhanced[8][128];
uint16_t n = 100;


/* Functions ******************************************************************/
bool Get_Pixel (uint8_t [8][128], uint8_t, uint8_t);
void Set_Pixel (uint8_t [8][128], uint8_t, uint8_t);

void Draw_Block_Image (uint8_t [8][128], uint8_t, uint8_t, bool);
void Draw_Rect_Image (uint8_t [8][128], uint8_t, uint8_t, uint8_t, uint8_t, bool);
void Draw_Image_to_OLED (uint8_t [8][128]);

void Add_Noise_Image (uint8_t [8][128], uint8_t [8][128], uint16_t);
bool Sum_Pixel_Neighbors (uint8_t [8][128], uint8_t, uint8_t);
void Enhance_Image (uint8_t [8][128], uint8_t [8][128]);


int
main (void)
{
	Init_Sys_Tick();


	Init_OLED(0);
	Clear_OLED();


	Draw_Rect_Image (im_ori, 1, 1, 32, 16, true);
	Draw_Rect_Image (im_ori, 3, 3, 30, 14, true);
	Draw_Rect_Image (im_ori, 5, 5, 28, 12, true);
	Draw_Rect_Image (im_ori, 7, 7, 26, 10, true);

	Delay_1ms (10000);

	Add_Noise_Image (im_ori, im_noisy, n);
	Draw_Image_to_OLED (im_noisy);

	Delay_1ms (10000);

	Enhance_Image (im_noisy, im_enhanced);
	Draw_Image_to_OLED (im_enhanced);

	Delay_1ms (10000);

	Clear_OLED();


	for (;;) /* Loop forever */
	{

	}
}


/* Input coordinates correspond to position on the digital image */
/* 	i.e., x in [0,31], y in [0,15] */
void
Draw_Block_Image (uint8_t image[8][128], uint8_t x, uint8_t y, bool fill)
{
	/* Get either top nibble or low nibble, relative to the OLED page (0x0F or 0xF) */
	uint8_t fill_mask = IM_ROWFILL_MASK(y);
	/* If coloring in the block, leave as is; else, invert so that the mask clears the nibble */
	uint8_t fill_value = fill ? fill_mask : 0x0;

	uint8_t page = IMROW_2_OLEDPAGE(y);
	uint8_t col  = IMCOL_2_OLEDCOL(x);

	/* Logical OR with other half of page to preserve the image */
	uint8_t mask = fill_value | (image[page][col] & ~fill_mask);

	Set_Cursor_Pos (col, page);

	image[page][col++] = mask;
	Draw_segm_OLED (mask);

	image[page][col++] = mask;
	Draw_segm_OLED (mask);

	image[page][col++] = mask;
	Draw_segm_OLED (mask);

	image[page][col] = mask;
	Draw_segm_OLED (mask);
}


/* Assumes 1-indexed as shown by Figure 1 */
/* Input coordinates correspond to position on the digital image */
/* 	i.e., x in [0,31], y in [0,15] */
void
Draw_Rect_Image (uint8_t image[8][128], uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool fill)
{
	/* Adjust for 0-indexed */
	x1--, y1--, x2--, y2--;

	uint8_t i;
	/* Top & Bottom Lines */
	for (i = x1; i <= x2; i++)
	{
		Draw_Block_Image (image, i, y1, fill);
		Draw_Block_Image (image, i, y2, fill);
	}

	/* Right & Left Lines */
	for (i = y1; i <= y2; i++)
	{
		Draw_Block_Image (image, x1, i, fill);
		Draw_Block_Image (image, x2, i, fill);
	}
}


/* Takes in bitmap to draw to the screen */
void
Draw_Image_to_OLED (uint8_t image[8][128])
{
	uint8_t i, j;
	for (i = 0; i < OLED_NUM_PAGE; i++)
	{
		Set_Cursor_Pos (0, i);
		for (j = 0; j < OLED_NUM_COLS; j++)
		{
			Draw_segm_OLED (image[i][j]);
		}
	}
}


/* Input coordinates of the pixel (row in [0,63], col in [0,127] */
bool
Get_Pixel (uint8_t image[8][128], uint8_t row, uint8_t col)
{
	return (image[OLED_ROW2PAGE(row)][col] & OLED_ROW2PIX(row));
}


/* Input coordinates of the pixel (row in [0,63], col in [0,127] */
void
Set_Pixel (uint8_t image[8][128], uint8_t row, uint8_t col)
{
	image[OLED_ROW2PAGE(row)][col] |= OLED_ROW2PIX(row);
}


/* Collects n_pixels of random, unique coordinates and inverts to create noise */
void
Add_Noise_Image (uint8_t original[8][128], uint8_t noisy[8][128], uint16_t n_pixels)
{
	uint8_t i, j, rand_row, rand_col;

	/* Ensure space is zeroed out */
	uint8_t noise_mask[8][128];
	for (i = 0; i < OLED_NUM_PAGE; i++)
	{
		for (j = 0; j < OLED_NUM_COLS; j++)
		{
			noise_mask[i][j] = 0x0;
		}
	}

	for (i = 0; i < n_pixels; i++)
	{
		while (1)
		{
			/* Generate point */
			rand_row = (rand() % OLED_NUM_ROWS);
			rand_col = (rand() % OLED_NUM_COLS);
			/* Make sure it's not already used */
			if (Get_Pixel (noise_mask, rand_row, rand_col) == false)
			{
				Set_Pixel (noise_mask, rand_row, rand_col);
				break;
			}

		} /* End while loop */
	} /* End for loop */

	for (i = 0; i < OLED_NUM_PAGE; i++)
	{
		for (j = 0; j < OLED_NUM_COLS; j++)
		{
			noisy[i][j] = original[i][j] ^ noise_mask[i][j];
		}
	}
}

/* Searches around the given pixel to take the average, excludes out-of-bounds neighbors */
bool
Sum_Pixel_Neighbors (uint8_t image[8][128], uint8_t target_row, uint8_t target_col)
{
	uint8_t sum = 0, total_neighbors = 0;
	int8_t row, col;

	for (int i = -1; i <= 1; i++)
	{
		/* -1 = Top; 0 = Middle; +1 = Bottom */
		row = target_row + i;
		if (row >= 0 && row < 64)
		{
			/* Left */
			col = target_col - 1;
			if (col >= 0)
			{
				total_neighbors++;
				sum += Get_Pixel (image, row, col);
			}
			/* Middle */
			col = target_col;
			if (row != target_row)
			{
				total_neighbors++;
				sum += Get_Pixel (image, row, target_col);
			}
			/* Right */
			col = target_col + 1;
			if (col < 128)
			{
				total_neighbors++;
				sum += Get_Pixel (image, row, col);
			}
		}
	}

	/* If more neighbors are 1s, return true */
	return (sum > (total_neighbors / 2));
}

/* Passes a noisy image through an averaging filter and produces an enhanced image */
void Enhance_Image (uint8_t noisy[8][128], uint8_t enhanced[8][128])
{
	uint8_t i, j, page;
	for (i = 0; i < OLED_NUM_ROWS; i++)
	{
		page = OLED_ROW2PAGE(i);
		for (j = 0; j < OLED_NUM_COLS; j++)
		{
			/* If more neighbors are 1s, set the pixel */
			enhanced[page][j] |= Sum_Pixel_Neighbors(noisy, i, j) ? OLED_ROW2PIX(i) : 0x0;
		}
	}
}




