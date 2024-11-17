#include <Wire.h>               // Biblioteca para comunicação I2C
#include <LiquidCrystal_I2C.h>  // Biblioteca para o LCD I2C

// Definições de pinos
#define buttonPin 12    // Pino onde o botão está conectado
#define ledPin 7        // Pino onde o LED está conectado
#define relayPin 4      // Pino onde o relé está ligado
#define soilMoisture A0 // Pino do sensor de umidade do solo

// Configuração do LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // LCD com endereço I2C 0x3F, 16 colunas e 2 linhas

// Variáveis globais
bool relayOn = false;         // Estado do relé
unsigned long lowStartTime = 0;  // Tempo em que a umidade ficou <= 40%
unsigned long highStartTime = 0; // Tempo em que a umidade ficou >= 90%
bool ledActive = false;       // Indica se o LED foi ativado por baixa umidade

void setup() {
   // Inicializa o LCD
  lcd.init();
  lcd.backlight();

  // Configurações de pinos
  pinMode(buttonPin, INPUT_PULLUP); // Configura botão com resistor pull-up
  pinMode(ledPin, OUTPUT);          // Configura LED como saída
  pinMode(relayPin, OUTPUT);        // Configura relé como saída

  // Estado inicial
  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, HIGH);

  // Mensagem inicial no LCD
  lcd.setCursor(0, 0);
  lcd.print("Umidade do Solo");
  delay(1000);
}

void loop() {
  // Leitura da umidade do solo
  int umidity = analogRead(soilMoisture);  // Lê o valor analógico do sensor de umidade
  int percentageUmidity = map(umidity, 1023, 200, 0, 100);  // Mapeia o valor lido de 0 a 100%

  // Atualiza exibição no LCD
  lcd.setCursor(0, 1);
  lcd.print("Umidade: ");
  lcd.print(percentageUmidity);
  lcd.print("%   ");

  if (percentageUmidity <= 40) {
    if (lowStartTime == 0) {
      lowStartTime = millis(); // Inicia o tempo de contagem
    }
    if (millis() - lowStartTime >= 10000) { // 10 segundos se passaram
    digitalWrite(ledPin, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Umidade: ");
    lcd.print(percentageUmidity);
    lcd.print("%   ");
    lcd.setCursor(20, 1);
    lcd.print("Regar!");
    ledActive = true;
    }
  } else {
    lowStartTime = 0; // Reseta o tempo de baixa umidade
    if (!relayOn) {
      digitalWrite(ledPin, LOW); // Desliga o LED se o relé não estiver acionado
      ledActive = false;
    }
  }

  // Controle do relé
  if (ledActive && digitalRead(buttonPin) == LOW && !relayOn) {
    digitalWrite(relayPin, LOW);
    relayOn = true;
  }

   // Caso a umidade seja maior ou igual a 90%
  if (percentageUmidity >= 70 && relayOn) {
    if (highStartTime == 0) {
      highStartTime = millis(); // Inicia o tempo de contagem
    }
    if (millis() - highStartTime >= 3000) { // 3 segundos se passaram
      digitalWrite(relayPin, HIGH);
      relayOn = false;
      highStartTime = 0; // Reseta o tempo
      lcd.setCursor(0, 1);
      lcd.print("Umidade: ");
      lcd.print(percentageUmidity);
      lcd.print("%   ");
      lcd.setCursor(20, 1);
      lcd.print("        ");
    }
  } else {
    highStartTime = 0; // Reseta o tempo se a umidade não estiver alta
  }

  delay(100); // Pequeno atraso para estabilidade
}
