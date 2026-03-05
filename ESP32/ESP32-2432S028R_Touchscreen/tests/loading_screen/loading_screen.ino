/*
  Sketch to show creation of a 1bpp sprite with a transparent
  background, then plot it on the TFT.

  Example for library:
  https://github.com/Bodmer/TFT_eSPI

  A Sprite is notionally an invisible graphics screen that is
  kept in the processors RAM. Graphics can be drawn into the
  Sprite just as it can be drawn directly to the screen. Once
  the Sprite is completed it can be plotted onto the screen in
  any position. If there is sufficient RAM then the Sprite can
  be the same size as the screen and used as a frame buffer.

  A 1-bit Sprite occupies (width * height)/8 bytes in RAM. So,
  for example, a 320 x 240 pixel Sprite occupies 9600 bytes.
*/
// A new setBitmapColor(fg_color, bg_color) function allows
// any 2 colours to be used for the 1 bit sprite.
// One colour can also be defined as transparent when
// rendering to the screen.


#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI    tft = TFT_eSPI();         // Create object "tft"

#define XPT2046_IRQ   36 // T_IRQ
#define XPT2046_MOSI  32 // T_DIN
#define XPT2046_MISO  39 // T_OUT
#define XPT2046_CLK   25 // T_CLK
#define XPT2046_CS    33 // T_CS

SPIClass touchscreenSPI = SPIClass (VSPI);
XPT2046_Touchscreen touchscreen (XPT2046_CS, XPT2046_IRQ);

#define BITS_PER_PIXEL    1             // How many bits per pixel in Sprite
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define SCREEN_DIAGONAL 400
#define FONT_SIZE         1

int centerX = 160;
int centerY = 120;

int x, y, z;

void
printTouchToDisplay (int touchX, int touchY, int touchZ)
{
  tft.fillScreen (TFT_WHITE);
  tft.setTextColor (TFT_BLACK, TFT_WHITE);

  int centerX = SCREEN_WIDTH >> 1;
  int textY = 80;

  String tempText = "X = " + String (touchX);
  tft.drawCentreString (tempText, centerX, textY, FONT_SIZE);
  
  textY += 20;
  tempText = "Y = " + String (touchY);
  tft.drawCentreString (tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Pressure = " + String (touchZ);
  tft.drawCentreString (tempText, centerX, textY, FONT_SIZE);

  textY += 50;
  tft.drawSmoothArc (centerX, textY, 20, 12, 0, map(touchZ, 300, 2500, 0, 360), TFT_BLUE, TFT_WHITE, true);
}


void
drawBorder (int16_t thickness, uint32_t index)
{
  uint32_t origin_color = rainbow (index);
  uint32_t start_color = origin_color;
  index = (index + 10) % 192;
  uint32_t next_color = rainbow (index);
  index = (index + 10) % 192;

  // Top Border
  tft.fillRectHGradient (0, 0, SCREEN_WIDTH, thickness, start_color, next_color);
  start_color = next_color;
  next_color = rainbow (index);
  index = (index + 10) % 192;

  // Right Border
  tft.fillRectVGradient (SCREEN_WIDTH - thickness, 0, thickness, SCREEN_HEIGHT, start_color, next_color);
  start_color = next_color;
  next_color = rainbow (index);
  index = (index + 10) % 192;

  // Bottom Border
  tft.fillRectHGradient (0, SCREEN_HEIGHT - thickness, SCREEN_WIDTH, thickness, next_color, start_color);
  start_color = next_color;
  next_color = rainbow (index);
  index = (index + 10) % 192;

  // Left Border
  tft.fillRectVGradient (0, 0, thickness, SCREEN_HEIGHT, origin_color, start_color);
}

// =========================================================================
void setup(void) {
  Serial.begin(115200);

  touchscreenSPI.begin (XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin (touchscreenSPI);
  touchscreen.setRotation (1);

  tft.init();

  tft.setRotation(1);
  tft.fillScreen (TFT_BLACK);

  tft.setTextColor (TFT_WHITE, TFT_BLACK, false);
  tft.setTextSize (2);
    
  //drawBorder (5, random (0, 191));
  centerX -= 5;
  tft.drawString ("a", centerX - 45, centerY - 8);
  delay (150);
  tft.drawString ("k", centerX - 30, centerY - 8);
  delay (150);
  tft.drawString ("a", centerX - 15, centerY - 8);
  delay (150);
  tft.drawString ("t", centerX, centerY - 8);
  delay (150);
  tft.drawString ("o", centerX + 15, centerY - 8);
  delay (150);
  tft.drawString ("s", centerX + 30, centerY - 8);
  delay (150);
  tft.drawString ("h", centerX + 45, centerY - 8);
  centerX += 5;
  delay (400);

  uint32_t color, index = random (0, 191);
  for (int i = 0; i < 360; )
  {
    color = rainbow (index);
    tft.drawSmoothArc (centerX, centerY, 100, 90, i, ++i, color, color, false);
    index = (i % 2 == 0) ? (index + 1) % 197 : index;
    delay (10);
  }


  delay (100000);

  tft.fillScreen (TFT_WHITE);
  printTouchToDisplay (0, 0, 0);
}
// =========================================================================
void loop()
{
  if (touchscreen.tirqTouched () && touchscreen.touched ())
  {
    TS_Point p = touchscreen.getPoint ();

    x = map (p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map (p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    printTouchToDisplay (x, y, z);

    delay (100);
  }
  
}


unsigned int rainbow(byte value)
{
  // If 'value' is in the range 0-159 it is converted to a spectrum colour
  // from 0 = red through to 127 = blue to 159 = violet
  // Extending the range to 0-191 adds a further violet to red band
 
  value = value%192;
  
  byte red   = 0; // Red is the top 5 bits of a 16-bit colour value
  byte green = 0; // Green is the middle 6 bits, but only top 5 bits used here
  byte blue  = 0; // Blue is the bottom 5 bits

  byte sector = value >> 5;
  byte amplit = value & 0x1F;

  switch (sector)
  {
    case 0:
      red   = 0x1F;
      green = amplit; // Green ramps up
      blue  = 0;
      break;
    case 1:
      red   = 0x1F - amplit; // Red ramps down
      green = 0x1F;
      blue  = 0;
      break;
    case 2:
      red   = 0;
      green = 0x1F;
      blue  = amplit; // Blue ramps up
      break;
    case 3:
      red   = 0;
      green = 0x1F - amplit; // Green ramps down
      blue  = 0x1F;
      break;
    case 4:
      red   = amplit; // Red ramps up
      green = 0;
      blue  = 0x1F;
      break;
    case 5:
      red   = 0x1F;
      green = 0;
      blue  = 0x1F - amplit; // Blue ramps down
      break;
  }
  return red << 11 | green << 6 | blue;
}
