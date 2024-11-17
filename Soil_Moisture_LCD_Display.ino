#include <Wire.h>               // Biblioteca para comunicação I2C
#include <LiquidCrystal_I2C.h>  // Biblioteca para o LCD I2C

// Definição do LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Configura o LCD com endereço I2C 0x3F, 16 colunas e 2 linhas

#define soilMoisture A0

void setup() {
  // Inicializa o LCD
  lcd.init();
  lcd.backlight();  // Ativa o backlight do LCD

  // Configuração inicial do monitor serial
  Serial.begin(9600);

  // Mensagem inicial no LCD
  lcd.setCursor(0, 0);
  lcd.print(" Umidade do Solo");
}

void loop() {
  // Leitura da umidade do solo
  int umidity = analogRead(soilMoisture);  // Lê o valor analógico do sensor de umidade
  int percentageUmidity = map(umidity, 1023, 200, 0, 100);  // Mapeia o valor lido de 0 a 100%

  // Atualiza a exibição no LCD
  lcd.setCursor(20, 0);
  lcd.print("Umidade: ");
  lcd.print(percentageUmidity);
  lcd.print("%   ");

  lcd.setCursor(20, 1);
  lcd.print("Valor analog: ");
  lcd.print(umidity);
  lcd.print("   ");

  delay(1000);  // Atraso de 1 segundo entre as leituras
}
