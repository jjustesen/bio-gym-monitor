#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o LCD no endereço 0x27 (pode ser diferente no seu módulo)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pino do potenciômetro
const int potPin = A0;
int lastPotValue = 0;
int potValue = 0;
int mappedValue = 0;
int repetitionCount = 0;
bool movingUp = false;

// Ajuste estas constantes conforme necessário
const int MIN_THRESHOLD = 15;  // Valor mínimo para considerar uma mudança significativa
const int UPPER_ZONE = 85;    // Limite superior para considerar "zona alta"
const int LOWER_ZONE = 15;    // Limite inferior para considerar "zona baixa"
const int READING_SAMPLES = 5; // Número de amostras para média (reduz ruído)

// Variável para estado da repetição
enum RepState {WAITING_BOTTOM, WAITING_TOP, WAITING_RETURN};
RepState repState = WAITING_BOTTOM;

// Caracteres personalizados para barra de progresso
byte barChars[8][8] = {
  {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000}, // 0 - vazio
  {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000}, // 1 - 1/5
  {0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000}, // 2 - 2/5
  {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100}, // 3 - 3/5
  {0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110}, // 4 - 4/5
  {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}  // 5 - cheio
};

void setup() {
  // Inicializa comunicação serial
  Serial.begin(9600);
  
  // Inicializa o LCD
  lcd.init();
  lcd.backlight();
  
  // Cria caracteres personalizados para barra de progresso
  for (int i = 0; i < 6; i++) {
    lcd.createChar(i, barChars[i]);
  }
  
  // Mensagem inicial
  lcd.setCursor(0, 0);
  lcd.print("Fitness Tracker");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
}

void loop() {
  // Lê múltiplas amostras para reduzir ruído
  int rawValue = 0;
  for(int i = 0; i < READING_SAMPLES; i++) {
    rawValue += analogRead(potPin);
    delay(2);  // pequeno delay entre leituras
  }
  potValue = rawValue / READING_SAMPLES;
  
  // Mapeia para 0-100 para facilitar visualização
  mappedValue = map(potValue, 0, 1023, 0, 100);
  
  // Sistema de estado para detecção de repetições mais robusto
  switch(repState) {
    case WAITING_BOTTOM:
      // Esperamos que esteja na zona baixa primeiro
      if(mappedValue <= LOWER_ZONE) {
        repState = WAITING_TOP;
      }
      break;
      
    case WAITING_TOP:
      // Agora esperamos que alcance a zona alta
      if(mappedValue >= UPPER_ZONE) {
        repState = WAITING_RETURN;
      }
      break;
      
    case WAITING_RETURN:
      // Quando retorna para a zona baixa, conta 1 repetição
      if(mappedValue <= LOWER_ZONE) {
        repetitionCount++;
        repState = WAITING_TOP; // Pronto para a próxima subida
      }
      break;
  }
  
  // Atualiza o display LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Forca:");
  lcd.print(mappedValue);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("Rep:");
  lcd.print(repetitionCount);
  
  // Desenha barra de progresso
  drawProgressBar(9, 1, 7, mappedValue);
  
  // Adiciona o status do estado ao envio serial para depuração
  Serial.print("{\"force\":");
  Serial.print(mappedValue);
  Serial.print(",\"reps\":");
  Serial.print(repetitionCount);
  Serial.print(",\"state\":");
  Serial.print(repState);
  Serial.println("}");
  
  delay(50); // Reduzido para melhorar a resposta
}

// Função para desenhar barra de progresso no LCD
void drawProgressBar(int col, int row, int length, int percent) {
  // Mapeia a porcentagem para o número de pixels da barra (cada caractere tem 5 pixels de largura)
  int numPixels = map(percent, 0, 100, 0, length * 5);
  
  // Posiciona cursor no ponto inicial da barra
  lcd.setCursor(col, row);
  
  // Desenha cada caractere da barra de progresso
  for (int i = 0; i < length; i++) {
    int pixelsInChar = min(5, max(0, numPixels - (i * 5)));
    lcd.write(pixelsInChar);
  }
}