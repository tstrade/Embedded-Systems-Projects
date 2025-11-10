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

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define FONT_SIZE       2

int x, y, z;
bool started = false;

void 
printTouchToSerial (int touchX, int touchY, int touchZ)
{
  Serial.print ("X = ");
  Serial.print (touchX);
  Serial.print (" | Y = ");
  Serial.print (touchY);
  Serial.print (" | Pressure = ");
  Serial.print (touchZ);
  Serial.println ();
}


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
drawMenuBar ( void )
{
  tft.fillSmoothRoundRect (20, 20, SCREEN_WIDTH - 40, 50, 10, TFT_BLUE, TFT_WHITE);
}

void setup() {
  Serial.begin (115200);

  touchscreenSPI.begin (XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin (touchscreenSPI);

  touchscreen.setRotation (1);

  tft.init ();
  tft.setRotation (1);

  tft.fillScreen (TFT_WHITE);
  tft.setTextColor (TFT_BLACK, TFT_BLUE);

  int centerX = SCREEN_WIDTH >> 1;
  int centerY = SCREEN_HEIGHT >> 1;

  drawMenuBar ();
  //tft.drawCentreString ("Hello, world!", centerX, 30, FONT_SIZE);
  tft.drawCentreString ("Press here to begin testing", centerX, 35, 3);
  //tft.setTextColor (TFT_BLACK, TFT_WHITE);
}

void loop() {
  if (touchscreen.tirqTouched () && touchscreen.touched ())
  {
    TS_Point p = touchscreen.getPoint ();

    x = map (p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map (p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    if (!started && x > 20 && x < (SCREEN_WIDTH - 40) && y > 20 && y < 70)
    {
      started = true;
      delay (200);
    }
    
    if (started)
    {
      printTouchToSerial (x, y, z);
      printTouchToDisplay (x, y, z);
    }

    delay (100);
  }

}
