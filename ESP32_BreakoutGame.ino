#include <TFT_eSPI.h>
#include <SPI.h>
#include "BluetoothSerial.h"
// Initialize the display
TFT_eSPI tft = TFT_eSPI();
BluetoothSerial SerialBT;

// Button pins
#define LEFT_BUTTON_PIN   32
#define RIGHT_BUTTON_PIN  33

// Game variables
int barWidth = 20;
int barHeight = 5;
int barX = 50;
int barY;

int ballRadius = 3;
int ballX = 60;
int ballY = 80;
int ballSpeedX = 1;
int ballSpeedY = -1;

// Block configuration for 8x3 blocks
#define NUM_BLOCKS_X 8
#define NUM_BLOCKS_Y 4
int blockWidth;
int blockHeight = 10; // Adjusted for 3 rows
int blocks[NUM_BLOCKS_X][NUM_BLOCKS_Y];

const int buzzer = 25; // Pinul la care este conectat buzzerul

const int ledgameover = 26 ; // Pinul la care este conectat LED-ul
const int ledwin = 27; // Pinul la care este conectat LED-ul
// Initialize blocks
void initBlocks() {
  blockWidth = 16;//tft.width() / NUM_BLOCKS_X;  // Calculate block width to fit 8 blocks
  for (int i = 0; i < NUM_BLOCKS_X; i++) {
    for (int j = 0; j < NUM_BLOCKS_Y; j++) {
      blocks[i][j] = 1; // 1 = active, 0 = destroyed
    }
  }
}

// Draw the blocks
void drawBlocks() {
  for (int i = 0; i < NUM_BLOCKS_X; i++) {
    for (int j = 0; j <NUM_BLOCKS_Y; j++) {
      int x = i * blockWidth;
      int y = j * blockHeight;
      if (blocks[i][j]) {
        tft.fillRect(x+32, y, blockWidth - 2, blockHeight - 2, TFT_BLUE);
      } else {
        tft.fillRect(x+32, y, blockWidth - 2, blockHeight - 2, TFT_BLACK);
      }
    }
  }
}

// Draw the bar
void drawBar(int x, int y) {
  tft.fillRect(x, y, barWidth, barHeight, TFT_WHITE);
}

// Clear the bar
void clearBar(int x, int y) {
  tft.fillRect(x, y, barWidth, barHeight, TFT_BLACK);
}

// Draw the ball
void drawBall(int x, int y) {
  tft.fillCircle(x, y, ballRadius, TFT_RED);
}

// Clear the ball
void clearBall(int x, int y) {
  tft.fillCircle(x, y, ballRadius, TFT_BLACK);
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  //Serial.println("The device started, now you can pair it with bluetooth!");
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  delay(5000);
  start();

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
pinMode(buzzer, OUTPUT); // Set the buzzer pin as output 
pinMode(ledgameover, OUTPUT); // Set the Game Over LED pin as output
pinMode(ledwin, OUTPUT); // Set the Win LED pin as output
  barY = tft.height() - barHeight - 5;
  initBlocks();
  drawBlocks();
  drawBar(barX, barY);
  drawBall(ballX, ballY);
}
bool checkWin() {
  for (int i = 0; i < NUM_BLOCKS_X; i++) {
    for (int j = 0; j < NUM_BLOCKS_Y; j++) {
      if (blocks[i][j] == 1) {
        return false;  // Active blocks still exist
      }
    }
  }
  return true;  // All blocks have been destroyed.
}
void start() {
   tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(37, 50);
  tft.println("START GAME");
  delay(2000);
  tft.fillScreen(TFT_BLACK);
  // start the game
}
void playWinMelody() {
  int melody[] = {
  262, 294, 330, 349, 392, 440, 494, 523
};

// Note durations (in milliseconds)
int noteDurations[] = {
  200, 200, 200, 200, 200, 200, 200, 400
};
  for (int i = 0; i < 8; i++) {
    tone(buzzer, melody[i], noteDurations[i]);
    delay(noteDurations[i] * 1.3); // Delay
    noTone(buzzer);
  }
}
void win() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(60, 50);
  tft.println("WIN!!!");

  digitalWrite(ledwin, HIGH); // Turn on the Green LED
   playWinMelody();
  delay(2000);
  setup(); // Restart the game
}
void playGameOverMelody() {
  int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
  int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }
}

void gameOver() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(2);
  tft.setCursor(45, 50);
  tft.println("GAME OVER");
 
  digitalWrite(ledgameover, HIGH); // Turn on the Red LED
  playGameOverMelody();  // "GAME OVER" melody starts
  delay(2000);
  setup(); // Restart the game
}
int readBT=0;

void loop() {
  
  if (SerialBT.available()) {
    readBT=SerialBT.read();
    Serial.write(readBT);
    if(readBT=='1'){
      clearBar(barX, barY);
      barX -= 7;
    }
    else if(readBT=='2'){
      clearBar(barX, barY);
      barX+=7;
      }
  }
  // Control bar with buttons
  if (digitalRead(LEFT_BUTTON_PIN) == LOW && barX >= 30) {
    clearBar(barX, barY);
    barX -= 3;
  }
  if (digitalRead(RIGHT_BUTTON_PIN) == LOW && barX < tft.width() - barWidth) {
    clearBar(barX, barY);
    barX += 3;
  }

  // Move the ball
  clearBall(ballX, ballY);
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Bounce off walls
  if (ballX - ballRadius <= 30 || ballX + ballRadius >= tft.width()) {
    ballSpeedX = -ballSpeedX;
  }
  if (ballY - ballRadius <= 0) {
    ballSpeedY = -ballSpeedY;
  }

  // Bounce off the bar
  if (ballY + ballRadius >= barY && ballX >= (barX) && ballX <= (barX + barWidth)) {
    ballSpeedY = -ballSpeedY;
  }

  // Check for collision with blocks
  for (int i = 0; i < NUM_BLOCKS_X; i++) {
    for (int j = 0; j < NUM_BLOCKS_Y; j++) {
      int blockX = i * blockWidth;
      int blockY = j * blockHeight;
      if (blocks[i][j] &&
          ballX >(blockX+32) && ballX < (blockX + blockWidth+32) &&
          ballY > blockY && ballY < blockY + blockHeight) {
        blocks[i][j] = 0;
        ballSpeedY = -ballSpeedY;
        drawBlocks();
        if (checkWin()) {
        win();
      }
      }
    }
  }

  // Game over if ball falls off the screen
  if (ballY - ballRadius > tft.height()) {
    gameOver();

  }

  // Draw updated bar and ball
  drawBar(barX, barY);
  drawBall(ballX, ballY);
  delay(20);
}
