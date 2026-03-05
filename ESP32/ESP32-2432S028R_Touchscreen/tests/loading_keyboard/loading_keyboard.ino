#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "Free_Fonts.h"

#define XPT2046_IRQ   36 // T_IRQ
#define XPT2046_MOSI  32 // T_DIN
#define XPT2046_MISO  39 // T_OUT
#define XPT2046_CLK   25 // T_CLK
#define XPT2046_CS    33 // T_CS

#define BITS_PER_PIXEL    1           
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define SCREEN_DIAGONAL 400
#define FONT_SIZE         1

char ascii_delete[] = { 0x7F, 0x00 };
char ascii_enter[] = { 0x0A, 0x0D, 0x00 };
char ascii_clear[] = { 0x0C, 0x00 };
int centerX = 160;
int centerY = 120;

TFT_eSPI tft = TFT_eSPI ();
SPIClass touchscreenSPI = SPIClass (VSPI);
XPT2046_Touchscreen touchscreen (XPT2046_CS, XPT2046_IRQ);


TFT_eSPI_Button keyboard_btn[40] =
{
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button (),
  TFT_eSPI_Button ()
};

char *keyboard_str_low[40] = 
{
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
  "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
  "shf", "a", "s", "d", "f", "g", "h", "j", "k", "l",
  "del", "z", "x", "c", "v", "b", "n", "m", "ent", " "
};

char *keyboard_str_up[40] = 
{
  "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
  "SHF", "A", "S", "D", "F", "G", "H", "J", "K", "L",
  "DEL", "Z", "X", "C", "V", "B", "N", "M", "ENT", " "
};

char **active_keyboard = keyboard_str_low;
bool is_shift = false, prev_shift = false, caps_lock = false;
int curr_key, prev_key;

void init_keyboard ( void );
void shift_keyboard ( void );
void update_keyboard ( int, int );
void animateLetterVU ( int, int, int, const char * );
unsigned int rainbow ( byte value );
void load_screen ( void );


void
setup ( void )
{
    Serial.begin (115200);
    load_screen ();
    init_keyboard ();
}

void
loop ( void )
{
    static int x_axis, y_axis, z_axis;
    if (touchscreen.tirqTouched () && touchscreen.touched ())
    {
        TS_Point p = touchscreen.getPoint ();

        x_axis = map ( p.x, 200, 3700, 1, SCREEN_WIDTH );
        y_axis = map ( p.y, 240, 3800, 1, SCREEN_HEIGHT );
        z_axis = p.z;

        update_keyboard ( x_axis, y_axis );
        delay (100);
    }
}

void
load_screen ( void )
{
    touchscreenSPI.begin (XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin (touchscreenSPI);
    touchscreen.setRotation (1);

    tft.init();

    tft.setRotation(1);
    tft.fillScreen (TFT_BLACK);

    tft.setTextColor (TFT_WHITE, TFT_BLACK, false);
    tft.setTextSize (1);
    tft.setFreeFont (FF1);
        
    centerX -= 5;
    animateLetterVU (centerX - 45, SCREEN_HEIGHT, centerY - 8, "a");
    animateLetterVU (centerX - 30, SCREEN_HEIGHT, centerY - 8, "k");
    animateLetterVU (centerX - 15, SCREEN_HEIGHT, centerY - 8, "a");
    animateLetterVU (centerX + 00, SCREEN_HEIGHT, centerY - 8, "t");
    animateLetterVU (centerX + 15, SCREEN_HEIGHT, centerY - 8, "o");
    animateLetterVU (centerX + 30, SCREEN_HEIGHT, centerY - 8, "s");
    animateLetterVU (centerX + 45, SCREEN_HEIGHT, centerY - 8, "h");
    centerX += 5;

    delay (100);

    uint32_t color, index = random (0, 191);
    for (int i = 0; i < 360; )
    {
        color = rainbow (index);
        tft.drawSmoothArc (centerX, centerY, 100, 90, i, ++i, color, color, false);
        index = (i % 2 == 0) ? (index + 1) % 197 : index;
        delay (5);
    }
}


void
animateLetterVU (int x, int yi, int yf, const char *str)
{
  int i;
  for (i = yi; i > yf; i--)
  {
    tft.setTextColor (TFT_WHITE, TFT_BLACK, false);
    tft.drawString (str, x, i);
    delay (3);
    tft.setTextColor (TFT_BLACK, TFT_BLACK, false);
    tft.drawString (str, x, i);
  }
  
  tft.setTextColor (TFT_WHITE, TFT_BLACK, false);
  tft.drawString (str, x, yf);
}


unsigned int 
rainbow( byte value )
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

void 
init_keyboard ( void )
{
    tft.fillScreen (TFT_WHITE);
    // 32x32 pixel keys
    int i;
    int x_val = 0;
    int y_val = 80;
    for (i = 0; i < 38; i++)
    {
        keyboard_btn[i].initButtonUL ( &tft, x_val, y_val, 32, 32, TFT_BLACK, TFT_LIGHTGREY, TFT_BLACK, active_keyboard[i], 1 );
        keyboard_btn[i].drawButton ( false, active_keyboard[i] );
        x_val += 32;

        if (i == 9)
        {
          x_val = 0;
          y_val += 32;
        }
        else if (i == 19)
        {
          x_val = 0;
          y_val += 32;
          continue;
        }
        else if (i == 29)
        {
          x_val = 0;
          y_val += 32;
          i++;
          keyboard_btn[i].initButtonUL ( &tft, x_val, y_val, 48, 32, TFT_BLACK, TFT_LIGHTGREY, TFT_BLACK, active_keyboard[i], 1 );
          keyboard_btn[i].drawButton ( false, active_keyboard[i] );
          x_val = 48;
          continue;
        }
    }
    keyboard_btn[38].initButtonUL ( &tft, x_val, y_val, 48, 32, TFT_BLACK, TFT_LIGHTGREY, TFT_BLACK, active_keyboard[28], 1 );
    keyboard_btn[38].drawButton ( false, active_keyboard[38] );

    keyboard_btn[39].initButtonUL ( &tft, 48, 208, 228, 32, TFT_BLACK, TFT_LIGHTGREY, TFT_BLACK, " ", 1 );
    keyboard_btn[39].drawButton ( false, active_keyboard[39] );

    Serial.println (ascii_clear);
}


void
shift_keyboard ( void )
{
  prev_shift = is_shift;
  if (is_shift && caps_lock)
    return;

  is_shift = !is_shift;

  if (is_shift == false)
    active_keyboard = keyboard_str_low;
  else
    active_keyboard = keyboard_str_up;

  int i;
  for (i = 0; i < 40; i++)
    keyboard_btn[i].drawButton ( false, active_keyboard[i] );
  
  delay (50);
}

void 
update_keyboard ( int x_axis, int y_axis )
{
    int i = 0;
    int btn_ul_x, btn_ul_y;

    for (i = 0; i < 40; i++ )
    {
      if (!keyboard_btn[i].contains ( x_axis, y_axis ))
        continue;
      
      keyboard_btn[i].press (true);
      delay (200);

      if (!keyboard_btn[i].isPressed ())
        continue;
      
      if (i < 10)
      {
        btn_ul_x = 32 * i;
        btn_ul_y = 80;
      }
      else if (i < 19)
      {
        btn_ul_x = 32 * (i - 10);
        btn_ul_y = 112;
      }
      else if (i < 30)
      {
        btn_ul_x = 32 * (i - 20);
        btn_ul_y = 144;
      }
      else if (i == 30)
      {
        btn_ul_x = 0;
        btn_ul_y = 176;
      }
      else if (i < 38)
      {
        btn_ul_x = (32 * (i - 31)) + 48;
        btn_ul_y = 176;
      }
      else if (i == 38)
      {
        btn_ul_x = 272;
        btn_ul_y = 176;
      }
      else
      {
        btn_ul_x = 48;
        btn_ul_y = 208;
      }

      prev_key = curr_key;
      curr_key = i;

      keyboard_btn[i].drawButton ( true, active_keyboard[i] );
      delay (100);
      
      switch (i)
      {
      case 20:
        if (prev_key == 20 && !caps_lock)
        {
          caps_lock = true;
          active_keyboard[20] = "CAP";
        }
        else
        {
          caps_lock = false;
          active_keyboard[20] = "SHF";
        }

        shift_keyboard ();
        break;
      case 30:
        Serial.print (ascii_delete);
        break;
      case 38:
        Serial.print (ascii_enter);
        break;
      default:
        Serial.print (active_keyboard[i]);
        break;
      }
      
      keyboard_btn[i].drawButton ( false, active_keyboard[i] );
      keyboard_btn[i].press (false);
      if (is_shift && i != 20 && !caps_lock)
        shift_keyboard ();
  }
}
