#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI ();

#define XPT2046_IRQ   36 // T_IRQ
#define XPT2046_MOSI  32 // T_DIN
#define XPT2046_MISO  39 // T_OUT
#define XPT2046_CLK   25 // T_CLK
#define XPT2046_CS    33 // T_CS

SPIClass touchscreenSPI = SPIClass (VSPI);
XPT2046_Touchscreen touchscreen (XPT2046_CS, XPT2046_IRQ);
TFT_eSPI_Button start_btn = TFT_eSPI_Button ();
TFT_eSprite buddy = TFT_eSprite (&tft);

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define FONT_SIZE       1

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


bool
create_buddy ( void )
{
  buddy.createSprite (1, 1, 1);
  if (!buddy.create ())
    return false;

  buddy.setPaletteColor (0, TFT_RED);
  buddy.setBitmapColor (TFT_PURPLE, TFT_WHITE);
}

void setup() {
  Serial.begin (115200);

  touchscreenSPI.begin (XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin (touchscreenSPI);

  touchscreen.setRotation (1);

  tft.init ();
  tft.setRotation (1);

  tft.fillScreen (TFT_WHITE);
  tft.setTextColor (TFT_BLACK, TFT_WHITE);

  int centerX = SCREEN_WIDTH >> 1;
  int centerY = SCREEN_HEIGHT >> 1;

  start_btn.initButton (&tft, centerX, centerY, 300, 50, TFT_BLACK, TFT_BLUE, TFT_WHITE, "Start Button", 2);
  start_btn.drawButton (false, "Press here to start!");
}

void loop() {
  if (touchscreen.tirqTouched () && touchscreen.touched ())
  {
    TS_Point p = touchscreen.getPoint ();

    x = map (p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map (p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    if (start_btn.contains (x, y))
    {
      start_btn.press (true);
      delay (200);
    }
    
    if (start_btn.isPressed ())
    {
      printTouchToDisplay (x, y, z);
    }

    delay (100);
  }

}
