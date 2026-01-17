#include <LiquidCrystal.h>
#include "config.h"

LiquidCrystal lcd(
  LCD_RS,
  LCD_E,
  LCD_D4,
  LCD_D5,
  LCD_D6,
  LCD_D7
);

bool bombActive = false;
int bombTimer = 0;

bool defusing = false;
int defuseTimer = 0;

unsigned long lastSecondTick = 0;
unsigned long lastLedTick = 0;
bool ledState = false;


void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("CS2 Bomb Timer");
  delay(2000);
  lcd.clear();
}


void loop() {
  handleSerial();
  updateTimer();
  updateLed();
}

void handleSerial() {
  if (!Serial.available()) return;

  String command = Serial.readStringUntil('\n');
  command.trim();

  if (command.startsWith("BOMB_PLANTED:")) {
    int time = command.substring(13).toInt();
    startBomb(time);
  }
  else if (command.startsWith("DEFUSING:")) {
    int time = command.substring(9).toInt();
    startDefuse(time);
  }
  else if (command == "BOMB_DEFUSED") {
    stopBomb("Defused");
  }
  else if (command == "BOMB_EXPLODED") {
    stopBomb("Exploded");
  }
}



void startBomb(int timeSeconds) {
  bombActive = true;
  bombTimer = timeSeconds;

  lastSecondTick = millis();
  lastLedTick = millis();

  digitalWrite(LED_PIN, LOW);
  lcd.clear();
}

void stopBomb(const char* message) {
  bombActive = false;
  defusing = false;

  digitalWrite(LED_PIN, LOW);

  lcd.clear();
  lcd.print("Bomb ");
  lcd.print(message);
}



void updateTimer() {
  if (!bombActive) return;

  unsigned long now = millis();

  if (now - lastSecondTick >= 1000) {
    lastSecondTick = now;

    bombTimer--;
    if (defusing) defuseTimer--;

    lcd.setCursor(0, 0);

    if (defusing) {
      lcd.print("DEFUSING...    ");
    } else {
      lcd.print("Bomb Planted   ");
    }

    lcd.setCursor(0, 1);

    if (defusing) {
      lcd.print("Defuse: ");
      lcd.print(defuseTimer);
      lcd.print("s   ");
    } else {
      lcd.print("Time: ");
      lcd.print(bombTimer);
      lcd.print("s   ");
    }

    if (bombTimer <= 0) {
      stopBomb("Exploded");
    }

    if (defusing && defuseTimer <= 0) {
      stopBomb("Defused");
    }
  }
}


void updateLed() {
  if (!bombActive) return;

  if (defusing) {
    digitalWrite(LED_PIN, HIGH);
    return;
  }

  unsigned long now = millis();
  int interval;

  if (bombTimer > 20) {
    interval = 1000;
  } else if (bombTimer > 10) {
    interval = 500;
  } else {
    digitalWrite(LED_PIN, HIGH);
    return;
  }

  if (now - lastLedTick >= interval) {
    lastLedTick = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}


void startDefuse(int timeSeconds) {
  defusing = true;
  defuseTimer = timeSeconds;
  lastSecondTick = millis();
}

