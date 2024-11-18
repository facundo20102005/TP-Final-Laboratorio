#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Definición de colores
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

#define LCD_MOSI 23 
#define LCD_SCLK 18 
#define LCD_CS 15
#define LCD_DC 2 
#define LCD_RST 4 
#define LCD_BLK 32

Adafruit_ST7789 lcd = Adafruit_ST7789(LCD_CS, LCD_DC, LCD_RST);

uint16_t ccenterx, ccentery;  // Centro del reloj
const uint16_t cradius = 63;  // Radio del reloj
const float scosConst = 0.0174532925;
float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx, osy, omx, omy, ohx, ohy;
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;  // Para el temporizador de 1 segundo
uint8_t hh, mm, ss;  // Contenedores para la hora actual

void drawClockFace() {
  lcd.fillCircle(ccenterx, ccentery, cradius, BLUE);
  lcd.fillCircle(ccenterx, ccentery, cradius - 4, BLACK);
  
  // Dibujar 12 líneas (marcas del reloj)
  for (int i = 0; i < 360; i += 30) {
    sx = cos((i - 90) * scosConst);
    sy = sin((i - 90) * scosConst);
    x0 = sx * (cradius - 4) + ccenterx;
    yy0 = sy * (cradius - 4) + ccentery;
    x1 = sx * (cradius - 11) + ccenterx;
    yy1 = sy * (cradius - 11) + ccentery;
    lcd.drawLine(x0, yy0, x1, yy1, BLUE);
  }

  // Dibujar 60 puntos (minutos)
  for (int i = 0; i < 360; i += 6) {
    sx = cos((i - 90) * scosConst);
    sy = sin((i - 90) * scosConst);
    x0 = sx * 55 + ccenterx;
    yy0 = sy * 55 + ccentery;
    
    lcd.drawPixel(x0, yy0, BLUE);
    if (i == 0 || i == 180) lcd.fillCircle(x0, yy0, 1, CYAN);
    if (i == 90 || i == 270) lcd.fillCircle(x0, yy0, 1, CYAN);
  }
}

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') v = *p - '0';
  return 10 * v + *++p - '0';
}

void setup(void) {
  pinMode(LCD_BLK, OUTPUT);
  digitalWrite(LCD_BLK, HIGH);  // Activar la retroiluminación
  lcd.init(170, 320);
  lcd.fillScreen(ST77XX_BLACK);
  
  lcd.setTextColor(WHITE, BLACK);
  ccenterx = lcd.width() / 2;
  ccentery = lcd.height() / 2;
  osx = ccenterx;
  osy = ccentery;
  omx = ccenterx;
  omy = ccentery;
  ohx = ccenterx;
  ohy = ccentery;

  drawClockFace();  // Dibujar la cara del reloj
  
  // Obtener la hora actual del compilador
  hh = conv2d(__TIME__);
  mm = conv2d(__TIME__ + 3);
  ss = conv2d(__TIME__ + 6);
  targetTime = millis() + 1000; 
}

void drawClockHands(uint8_t h, uint8_t m, uint8_t s) {
  // Pre-calcular los grados de las agujas y las coordenadas
  sdeg = s * 6;  // 0-59 -> 0-354
  mdeg = m * 6 + sdeg * 0.01666667;  // Incluir segundos
  hdeg = h * 30 + mdeg * 0.0833333;  // Incluir minutos y segundos
  hx = cos((hdeg - 90) * scosConst);    
  hy = sin((hdeg - 90) * scosConst);
  mx = cos((mdeg - 90) * scosConst);    
  my = sin((mdeg - 90) * scosConst);
  sx = cos((sdeg - 90) * scosConst);    
  sy = sin((sdeg - 90) * scosConst);

  // Borrar las posiciones antiguas de las agujas
  lcd.drawLine(ohx, ohy, ccenterx + 1, ccentery + 1, BLACK);  
  lcd.drawLine(omx, omy, ccenterx + 1, ccentery + 1, BLACK);  
  lcd.drawLine(osx, osy, ccenterx + 1, ccentery + 1, BLACK);

  // Dibujar las nuevas posiciones de las agujas
  lcd.drawLine(hx * (cradius - 28) + ccenterx + 1, hy * (cradius - 28) + ccentery + 1, ccenterx + 1, ccentery + 1, WHITE);
  lcd.drawLine(mx * (cradius - 17) + ccenterx + 1, my * (cradius - 17) + ccentery + 1, ccenterx + 1, ccentery + 1, WHITE);
  lcd.drawLine(sx * (cradius - 14) + ccenterx + 1, sy * (cradius - 14) + ccentery + 1, ccenterx + 1, ccentery + 1, RED);
  lcd.fillCircle(ccenterx + 1, ccentery + 1, 3, RED);

  // Actualizar las coordenadas antiguas
  osx = sx * (cradius - 14) + ccenterx + 1;
  osy = sy * (cradius - 14) + ccentery + 1;
  omx = mx * (cradius - 17) + ccenterx + 1;
  omy = my * (cradius - 17) + ccentery + 1;
  ohx = hx * (cradius - 28) + ccenterx + 1;
  ohy = hy * (cradius - 28) + ccentery + 1;
}

void loop() {
  if (targetTime < millis()) {
    targetTime = millis() + 1000;
    ss++;
    if (ss == 60) {
      ss = 0;
      mm++;
      if (mm > 59) {
        mm = 0;
        hh++;
        if (hh > 23) hh = 0;
      }
    }
    drawClockHands(hh, mm, ss);
  }
}
