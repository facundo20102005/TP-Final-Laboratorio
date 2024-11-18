// Definición de pines del teclado
const uint8_t ROWS = 4;  // Número de filas
const uint8_t COLS = 4;  // Número de columnas

// Distribución de las teclas en el teclado 4x4
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Definición de los pines correspondientes a las filas y columnas
uint8_t rowPins[ROWS] = {22, 21, 19, 5};   // Pines para filas
uint8_t colPins[COLS] = {17, 16, 13, 12};  // Pines para columnas

void setup() {
  Serial.begin(9600);

  // Configura los pines de filas como entradas con resistencias pull-up
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }

  // Configura los pines de columnas como salidas en estado alto
  for (int j = 0; j < COLS; j++) {
    pinMode(colPins[j], OUTPUT);
    digitalWrite(colPins[j], HIGH); // Inicializa las columnas en alto
  }
}

void loop() {
  for (int i = 0; i < COLS; i++) {
    // Activa una columna a la vez
    digitalWrite(colPins[i], LOW);

    for (int j = 0; j < ROWS; j++) {
      // Lee el estado de cada fila
      if (digitalRead(rowPins[j]) == LOW) {
        // Muestra la tecla correspondiente en el Monitor Serial
        Serial.print("Tecla presionada: ");
        Serial.println(keys[j][i]);
        delay(200); // Pausa para evitar lecturas repetidas
      }
    }

    // Desactiva la columna actual antes de pasar a la siguiente
    digitalWrite(colPins[i], HIGH);
  }
}
