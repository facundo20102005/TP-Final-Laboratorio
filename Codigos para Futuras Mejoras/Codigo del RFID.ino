#include <WiFi.h>              // Biblioteca para ESP32
#include <HTTPClient.h>         // Para enviar solicitudes HTTP
#include <SPI.h>
#include <MFRC522.h>            // Biblioteca para el módulo MFRC522

// Definición de los pines para ESP32
#define SS_PIN  5    //--> SDA / SS (GPIO5)
#define RST_PIN 22   //--> RST (GPIO22)
#define SCK_PIN 18   //--> SCK (GPIO18)
#define MOSI_PIN 23  //--> MOSI (GPIO23)
#define MISO_PIN 19  //--> MISO (GPIO19)

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instanciamos el objeto MFRC522

#define ON_Board_LED 2  // LED integrado en la placa (GPIO2)

const char* ssid = "";
const char* password = "";

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

//-------------------------------------SETUP------------------------------------//
void setup() {
  Serial.begin(115200);       // Iniciar la comunicación serial
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);  // Iniciar el bus SPI con los pines correspondientes
  mfrc522.PCD_Init();         // Iniciar el módulo MFRC522
  
  delay(500);

  WiFi.begin(ssid, password); // Conectar a la red WiFi
  Serial.println("");
  
  pinMode(ON_Board_LED, OUTPUT);
  digitalWrite(ON_Board_LED, HIGH); // Apagar LED mientras se conecta al Wi-Fi

  //----------------------------------------Esperar conexión Wi-Fi
  Serial.print("Conectando a la red Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Hacer parpadear el LED mientras se conecta
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  digitalWrite(ON_Board_LED, HIGH); // Apagar el LED cuando esté conectado
  
  Serial.println("");
  Serial.print("Conectado a: ");
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Por favor, escanea una tarjeta o llavero para ver el UID.");
  Serial.println("");
}
//------------------------------------------------------------------//

//----------------------LOOP---------------------------//
void loop() {
  readsuccess = getid();  // Intentar leer el UID de una tarjeta
  
  if (readsuccess) {  
    digitalWrite(ON_Board_LED, LOW);  // Apagar el LED cuando se detecta una tarjeta
    
    HTTPClient http;  // Crear objeto HTTPClient
    
    String UIDresultSend, postData;
    UIDresultSend = StrUID;
   
    // Datos a enviar
    postData = "UIDresult=" + UIDresultSend;
  
      http.begin("http://'DireccionIP'/NodeMCU_RC522_Mysql/getUID.php");  // Dirección de destino
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Cabecera para datos formateados
    
    int httpCode = http.POST(postData);   // Enviar la solicitud POST
    String payload = http.getString();    // Obtener la respuesta
    
    // Mostrar la información de la solicitud en el Serial Monitor
    Serial.println("Enviado UID: " + UIDresultSend);
    Serial.println("Código HTTP: " + String(httpCode));   // Código de estado HTTP
    Serial.println("Respuesta del servidor: " + payload);    // Respuesta del servidor
    
    http.end();  // Cerrar la conexión HTTP
    delay(1000);
    digitalWrite(ON_Board_LED, HIGH); // Encender el LED cuando termine
  }
}
//-----------------------------------------------------------------//

//----------Procedimiento para leer y obtener un UID de una tarjeta o llavero------------------//
int getid() {  
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return 0;  // No se detectó una tarjeta
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return 0;  // No se pudo leer la tarjeta
  }
  
  Serial.print("El UID de la tarjeta escaneada es: ");
  
  for (int i = 0; i < 4; i++) {
    readcard[i] = mfrc522.uid.uidByte[i]; // Almacenar el UID en el arreglo
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();  // Detener la lectura de la tarjeta
  return 1;  // Se leyó una tarjeta correctamente
}
//----------------------------------------------------------------------------//

//----------Procedimiento para convertir el UID en un string-----------------//
void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len*2] = '\0';  // Finalizar la cadena
}
