#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
bool displayOk = false;

const int startButton = 2;
const int buzzerPin = 8;

const int led1 = 12;
const int led2 = 4;
const int led3 = 7;

const int player1Button = 3;
const int player1Led = 5;
const int player2Button = 9;
const int player2Led = 10;

unsigned long reactionTime = 0;

void setup() {
  pinMode(startButton, INPUT_PULLUP);
  pinMode(player1Button, INPUT_PULLUP);
  pinMode(player2Button, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(player1Led, OUTPUT);
  pinMode(player2Led, OUTPUT);

  displayOk = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // game still works if screen isn't found
  showScreen("PRESS", "START", "");
}

void loop() {
  if (digitalRead(startButton) == LOW) {
    digitalWrite(player1Led, LOW);
    digitalWrite(player2Led, LOW);

    showScreen("GET", "READY", "");
    runLights();

    int winner = waitForWinner();

    char timeText[16];
    snprintf(timeText, sizeof(timeText), "%lu ms", reactionTime);
    if (winner == 1) {
      showScreen("PLAYER 1", "WINS!", timeText);
    } else {
      showScreen("PLAYER 2", "WINS!", timeText);
    }

    delay(5000);                          // winner's LED stays on for 5 seconds
    digitalWrite(player1Led, LOW);
    digitalWrite(player2Led, LOW);
    showScreen("PRESS", "START", "");

    while (digitalRead(startButton) == LOW) {
    }
  }
}

void runLights() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  delay(1000);

  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  delay(1000);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(500);

  digitalWrite(led3, LOW);
}

int waitForWinner() {
  showScreen("GO!", "", "");              // draw first, so the screen update doesn't delay detection
  digitalWrite(buzzerPin, HIGH);          // "GO!" starts, players can press right now
  unsigned long buzzStart = millis();

  while (true) {
    if (millis() - buzzStart >= 1000) {
      digitalWrite(buzzerPin, LOW);       // buzzer ends after 1 second
    }

    if (digitalRead(player1Button) == LOW) {
      reactionTime = millis() - buzzStart;
      digitalWrite(buzzerPin, LOW);
      digitalWrite(player1Led, HIGH);
      return 1;
    }
    if (digitalRead(player2Button) == LOW) {
      reactionTime = millis() - buzzStart;
      digitalWrite(buzzerPin, LOW);
      digitalWrite(player2Led, HIGH);
      return 2;
    }
  }
}

void showScreen(const char* line1, const char* line2, const char* line3) {
  if (!displayOk) return;
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);                 // 10 characters per line at this size
  display.setCursor(0, 0);
  display.println(line1);
  display.setCursor(0, 22);
  display.println(line2);
  display.setCursor(0, 44);
  display.println(line3);
  display.display();
}