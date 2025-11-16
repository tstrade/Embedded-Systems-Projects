#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI ();

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

int x_axis, y_axis, z_axis;

char ascii_delete[] = { 0x7F, 0x00 };
char ascii_enter[] = { 0x0A, 0x0D, 0x00 };
char ascii_clear[] = { 0x0C, 0x00 };


TFT_eSPI_Button a = TFT_eSPI_Button ();
TFT_eSPI_Button b = TFT_eSPI_Button ();
TFT_eSPI_Button c = TFT_eSPI_Button ();
TFT_eSPI_Button d = TFT_eSPI_Button ();
TFT_eSPI_Button e = TFT_eSPI_Button ();
TFT_eSPI_Button f = TFT_eSPI_Button ();
TFT_eSPI_Button g = TFT_eSPI_Button ();
TFT_eSPI_Button h = TFT_eSPI_Button ();
TFT_eSPI_Button i = TFT_eSPI_Button ();
TFT_eSPI_Button j = TFT_eSPI_Button ();
TFT_eSPI_Button k = TFT_eSPI_Button ();
TFT_eSPI_Button l = TFT_eSPI_Button ();
TFT_eSPI_Button m = TFT_eSPI_Button ();
TFT_eSPI_Button n = TFT_eSPI_Button ();
TFT_eSPI_Button o = TFT_eSPI_Button ();
TFT_eSPI_Button p = TFT_eSPI_Button ();
TFT_eSPI_Button q = TFT_eSPI_Button ();
TFT_eSPI_Button r = TFT_eSPI_Button ();
TFT_eSPI_Button s = TFT_eSPI_Button ();
TFT_eSPI_Button t = TFT_eSPI_Button ();
TFT_eSPI_Button u = TFT_eSPI_Button ();
TFT_eSPI_Button v = TFT_eSPI_Button ();
TFT_eSPI_Button w = TFT_eSPI_Button ();
TFT_eSPI_Button x = TFT_eSPI_Button ();
TFT_eSPI_Button y = TFT_eSPI_Button ();
TFT_eSPI_Button z = TFT_eSPI_Button ();
TFT_eSPI_Button shf = TFT_eSPI_Button ();
TFT_eSPI_Button ent = TFT_eSPI_Button ();
TFT_eSPI_Button del = TFT_eSPI_Button ();

TFT_eSPI_Button keyboard_btn[29] =
{
  q, w, e, r, t, y, u, i, o, p,
  shf, a, s, d, f, g, h, j, k, l,
  del, z, x, c, v, b, n, m, ent
};

char *keyboard_str_low[29] = 
{
  "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
  "shf", "a", "s", "d", "f", "g", "h", "j", "k", "l",
  "del", "z", "x", "c", "v", "b", "n", "m", "ent"
};

char *keyboard_str_up[29] = 
{
  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
  "SHF", "A", "S", "D", "F", "G", "H", "J", "K", "L",
  "DEL", "Z", "X", "C", "V", "B", "N", "M", "ENT"
};

char **active_keyboard = keyboard_str_low;

bool is_shift = false;
bool prev_shift = false;
bool caps_lock = false;
int curr_key;
int prev_key;

void 
init_keyboard ( void )
{
  // 32x32 pixel keys
  int i;
  int x_val = 0;
  int y_val = 144;
  for (i = 0; i < 28; i++)
  {
    keyboard_btn[i].initButtonUL ( &tft, x_val, y_val, 32, 32, TFT_BLACK, TFT_LIGHTGREY, TFT_BLACK, active_keyboard[i], 1 );
    keyboard_btn[i].drawButton ( false, active_keyboard[i] );
    x_val += 32;

    if (i == 9)
    {
      x_val = 0;
      y_val += 32;
      continue;
    }
    
    if (i == 19)
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
  keyboard_btn[28].initButtonUL ( &tft, x_val, y_val, 48, 32, TFT_BLACK, TFT_LIGHTGREY, TFT_BLACK, active_keyboard[28], 1 );
  keyboard_btn[28].drawButton ( false, active_keyboard[i] );
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
  for (i = 0; i < 29; i++)
    keyboard_btn[i].drawButton ( false, active_keyboard[i] );
  
  delay (50);
}

void 
setup() {
  Serial.begin(115200);

  touchscreenSPI.begin (XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin (touchscreenSPI);
  touchscreen.setRotation (1);

  tft.init();

  tft.setRotation(1);
  tft.fillScreen (TFT_WHITE);
  init_keyboard ();
  Serial.println (ascii_clear);
}

void 
loop() {
  // put your main code here, to run repeatedly:
  if (touchscreen.tirqTouched () && touchscreen.touched ())
  {
    TS_Point p = touchscreen.getPoint ();
    static int btn_ul_x, btn_ul_y;

    x_axis = map ( p.x, 200, 3700, 1, SCREEN_WIDTH );
    y_axis = map ( p.y, 240, 3800, 1, SCREEN_HEIGHT );
    z_axis = p.z;

    if (y_axis < 120)
    {
      Serial.print (" ");
    }
    else
    {
      static int i = 0;
      for (i = 0; i < 29; i++ )
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
          btn_ul_y = 144;
        }
        else if (i < 19)
        {
          btn_ul_x = 32 * (i - 10);
          btn_ul_y = 176;
        }
        else if (i == 20)
        {
          btn_ul_x = 0;
          btn_ul_y = 208;
        }
        else if (i < 28)
        {
          btn_ul_x = (32 * (i - 21)) + 48;
          btn_ul_y = 208;
        }
        else 
        {
          btn_ul_x = 272;
          btn_ul_y = 208;
        }

        prev_key = curr_key;
        curr_key = i;

        keyboard_btn[i].drawButton ( true, active_keyboard[i] );
        delay (100);
        
        switch (i)
        {
        case 10:
          if (prev_key == 10)
          {
            caps_lock = true;
            active_keyboard[10] = "CAP";
          }
          else
          {
            caps_lock = false;
            active_keyboard[10] = "SHF";
          }

          shift_keyboard ();
          break;
        case 20:
          Serial.print (ascii_delete);
          break;
        case 28:
          Serial.print (ascii_enter);
          break;
        default:
          Serial.print (active_keyboard[i]);
          break;
        }
        
        keyboard_btn[i].drawButton ( false, active_keyboard[i] );
        keyboard_btn[i].press (false);
        if (is_shift && i != 10 && !caps_lock)
          shift_keyboard ();
      }
    }
    delay (100);
  }
}
