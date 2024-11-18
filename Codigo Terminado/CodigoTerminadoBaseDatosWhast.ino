#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>
#include <Callmebot_ESP32.h>
#include <time.h>

// Configuración WiFi y CallMeBot
const char* ssid = "paola vilca 2.4Ghz";
const char* password = "01430150960";
String phoneNumber = "+5491120441514";
String apiKey = "8454542";

// Configuración para obtener la hora
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

// Configuración de los pines para el ST7789
#define TFT_CS     15
#define TFT_DC     2
#define TFT_RST    4
#define TFT_SCLK   18
#define TFT_MOSI   23
#define TFT_BLK    32

// Inicialización de la pantalla TFT
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Definición de colores
#define BLACK   0x0000
#define BLUE    0x001F
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0

// Configuración del teclado 4x4
const uint8_t ROWS = 4;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t rowPins[ROWS] = {22, 21, 19, 5};
uint8_t colPins[COLS] = {17, 16, 13, 12};

// Configuración de la contraseña
String clave = "";
const String PASSWORD = "1234A";

// Configuración de pines para LEDs y sensor
#define LED_CORRECTO 14
#define LED_INCORRECTO 27
#define SENSOR_PIN 26
const int BUZZER = 25;

int state, lastState = HIGH; // Variables para leer el estado del sensor y detectar cambio

void setup() {
  Serial.begin(115200);

  // Configurar la pantalla TFT
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH);
  tft.init(170, 320);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  mostrarTextoInicial();

  // Configurar LEDs, buzzer y sensor
  pinMode(LED_CORRECTO, OUTPUT);
  pinMode(LED_INCORRECTO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SENSOR_PIN, INPUT_PULLUP);

  digitalWrite(LED_CORRECTO, LOW);
  digitalWrite(LED_INCORRECTO, LOW);

  // Configurar teclado
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  for (int j = 0; j < COLS; j++) {
    pinMode(colPins[j], OUTPUT);
    digitalWrite(colPins[j], HIGH);
  }

  // Conectar al WiFi
  conectarWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}
String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "Error al obtener la hora";
  }
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

void loop() {
  // Verificar el estado del sensor magnético
  state = digitalRead(SENSOR_PIN);

  if (state == HIGH) {
    activarAlarma("Ventana Abierta");
  } else if (lastState == HIGH && state == LOW) {
    apagarAlarma();
    clave = "";
    mostrarTextoInicial();
  }

  lastState = state;

  // Escaneo del teclado
  for (int i = 0; i < COLS; i++) {
    digitalWrite(colPins[i], LOW);

    for (int j = 0; j < ROWS; j++) {
      if (digitalRead(rowPins[j]) == LOW) {
        char tecla = keys[j][i];
        Serial.print("Tecla presionada: ");
        Serial.println(tecla);

        if (tecla == '*') {
          if (clave.length() > 0) {
            clave.remove(clave.length() - 1);
          }
        } else if (clave.length() < 5) {
          clave += tecla;
        }

        tft.fillRect(20, 140, 200, 40, BLACK);
        tft.setTextColor(WHITE);
        tft.setCursor(20, 140);
        tft.setTextSize(2);
        tft.print(clave);

        delay(200);

        if (clave.length() == 5) {
          validarClave();
        }
      }
    }
    digitalWrite(colPins[i], HIGH);
  }
}

void activarAlarma(String mensaje) {
  String currentTime = getCurrentTime();
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(30, 120);
  tft.setTextSize(3);
  tft.print("Alarma!");

  digitalWrite(LED_INCORRECTO, HIGH);
  tone(BUZZER, 400);

  Callmebot.whatsappMessage(phoneNumber, apiKey, mensaje + " (" + currentTime + ")");
  Serial.println("Mensaje enviado: " + mensaje);

  delay(1000); // Esperar 3 segundos antes de reiniciar
  apagarAlarma();
}

void apagarAlarma() {
  digitalWrite(LED_INCORRECTO, LOW);
  noTone(BUZZER);
  mostrarTextoInicial();
}

void mostrarTextoInicial() {
  tft.fillScreen(BLACK);
  tft.setTextColor(BLUE);
  tft.setTextSize(2);
  tft.setCursor(20, 100);
  tft.print("Poner Clave:");
}

void validarClave() {
  String currentTime = getCurrentTime();
  if (clave == PASSWORD) {
    tft.fillScreen(BLACK);
    tft.setTextColor(GREEN);
    tft.setCursor(40, 120);
    tft.setTextSize(3);
    tft.print("Clave Correcta");
    digitalWrite(LED_CORRECTO, HIGH);
    
    Callmebot.whatsappMessage(phoneNumber, apiKey, "Acceso Permitido (" + currentTime + ")");
    Serial.println("Mensaje enviado: Acceso Permitido");

    delay(2000);
    digitalWrite(LED_CORRECTO, LOW);
  } else {
    activarAlarma("Intruso detectado");
  }
  clave = "";
  mostrarTextoInicial();
}