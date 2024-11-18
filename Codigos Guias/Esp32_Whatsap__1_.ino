#include <WiFi.h>
#include <Callmebot_ESP32.h>
#include <time.h>

const char* ssid = "";  //Wifi
const char* password = ""; // Contraseña del Wifi
String phoneNumber = ""; //Numero del Celular
String apiKey = ""; // Codigo que te da el bot

const int buttonPin = 14;  // Pin al que está conectado el botón

int buttonState = HIGH;     // Variable para almacenar el estado actual del botón
int lastButtonState = HIGH; // Variable para almacenar el último estado del botón
const int debounceDelay = 50;  // Tiempo de debounce en milisegundos
unsigned long lastDebounceTime = 0;  // Tiempo de la última lectura estable del botón

// Configuración para obtener la hora
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800; // Ajuste para la zona horaria (-3 GMT para Argentina)
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  // Configuración del botón
  pinMode(buttonPin, INPUT_PULLUP);

  // Conectar al WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Configurar NTP para obtener la hora
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
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
  int reading = digitalRead(buttonPin);

  // Si el estado del botón ha cambiado
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Reinicia el contador de debounce
  }

  // Si ha pasado suficiente tiempo desde el último cambio
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Si el estado ha cambiado
    if (reading != buttonState) {
      buttonState = reading;

      // Si el botón está presionado (LOW)
      if (buttonState == LOW) {
        // Obtener la fecha y hora actuales
        String currentTime = getCurrentTime();
        String mensaje = "Acceso permitido (" + currentTime + ")";

        // Enviar mensaje por WhatsApp
        Callmebot.whatsappMessage(phoneNumber, apiKey, mensaje);
        Serial.println("Mensaje enviado: " + mensaje);
        Serial.println(Callmebot.debug());

        delay(1000);  // Esperar para evitar múltiples envíos
      }
    }
  }

  lastButtonState = reading;  // Actualizar el último estado del botón
}
