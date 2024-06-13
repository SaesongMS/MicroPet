#include <TFT_eSPI.h>  // Include TFT_eSPI library
#include <Button2.h>   // Include library for button handling
#include <Ticker.h>   // Include library for periodic events

TFT_eSPI tft = TFT_eSPI();  // Create a display object

// Define button pins
#define feedButtonPin   32
#define playButtonPin   33
#define sleepButtonPin  25

Button2 btnFeed(feedButtonPin);
Button2 btnPlay(playButtonPin);
Button2 btnSleep(sleepButtonPin);

// Internal buttons
#define buttonDownPin   0
#define buttonUpPin     35

Button2 btnDown(buttonDownPin);
Button2 btnUp(buttonUpPin);


// Define buzzer pin
#define buzzerPin   27
#define buzzerPin2  12

//Define LED pin
#define ledPin      15

// Game state variables
int health = 80, hunger = 50, happiness = 50;
bool isSleeping = false;
bool gameOver = false;
int hungerChange = 0, happinessChange = 0, healthChange = 0; // Track changes for display

// Led blink
Ticker blinker;
bool ledBlink = false;
float blinkerPace = 0.5;  //seconds

// Sprites
#include "PetSprites.h"

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 5000; // Update every 5000 milliseconds (5 seconds)

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);  // Set the rotation which fits your display
  tft.fillScreen(TFT_BLACK);
  drawBackground();    // Draw initial background

  //set pin to output
  pinMode(buzzerPin2, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Draw the initial pet sprite and stats
  displayPetStats();
  displayMessage("Hello! Press a button to interact");
  displaySprite(happyPet);  // Initial sprite

  // Button setup
  setupButtons();

  updateLed(1);
}

void loop() {
  if (!gameOver) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime > updateInterval) {
      updateGameState();
      lastUpdateTime = currentTime;
    }
  }

  btnFeed.loop();
  btnPlay.loop();
  btnSleep.loop();
  btnDown.loop();
  btnUp.loop();
}

void displayPetStats() {
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  // Clear the area where the stats are displayed
  clearTextArea();

  // Print the updated stats
  tft.setCursor(0, 0);
  tft.printf("Health: %d ", health);
  displayChangeIndicator(healthChange);
  tft.printf("\nHunger: %d ", hunger);
  displayChangeIndicator(hungerChange);
  tft.printf("\nHappiness: %d ", happiness);
  displayChangeIndicator(happinessChange);
}

void displayChangeIndicator(int change) {
  int startX = tft.getCursorX();
  int startY = tft.getCursorY();
  int arrowSize = 4; // Change this to change the size of the arrow
  if (change > 0) {
    tft.drawLine(startX, startY + 3*arrowSize, startX, startY, TFT_GREEN);
    tft.drawLine(startX - arrowSize/2, startY + arrowSize/2, startX, startY, TFT_GREEN);
    tft.drawLine(startX + arrowSize/2, startY + arrowSize/2, startX, startY, TFT_GREEN);
  } else if (change < 0) {
    tft.drawLine(startX, startY, startX, startY + 3*arrowSize, TFT_RED);
    tft.drawLine(startX - arrowSize/2, startY + 3*arrowSize - arrowSize/2, startX, startY + 3*arrowSize, TFT_RED);
    tft.drawLine(startX + arrowSize/2, startY + 3*arrowSize - arrowSize/2, startX, startY + 3*arrowSize, TFT_RED);
  } else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(" ", tft.getCursorX(), tft.getCursorY());
  }
}

void clearTextArea() {
  // Assuming stats are displayed at the top of the screen with a height of 60 pixels
  tft.fillRect(0, 0, tft.width(), 60, TFT_BLACK);
}

void displaySprite(const unsigned char* sprite) {
  int scale = 2; // Define the scale factor
  // Assuming sprite size is 16x16 and you want to display it at the bottom center
  int spriteX = (tft.width() - 16 * scale) / 2;
  int spriteY = tft.height() - 16 * scale - 10;

  // Calculate the enlarged sprite size
  int scaledWidth = 16 * scale;
  int scaledHeight = 16 * scale;

  // Correctly set the address window for the sprite's size
  tft.setAddrWindow(spriteX, spriteY, scaledWidth, scaledHeight);
  
  // Create a buffer to hold pixel color data for the enlarged sprite
  uint16_t buffer[scaledWidth * scaledHeight]; // Scaled size

  // Convert monochrome bitmap to color bitmap suitable for TFT with scaling
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      uint16_t color = (pgm_read_byte_near(sprite + y * 2 + (x / 8)) & (1 << (7 - x % 8))) ? TFT_WHITE : TFT_BLACK;
      for (int dy = 0; dy < scale; dy++) {
        for (int dx = 0; dx < scale; dx++) {
          buffer[(y * scale + dy) * scaledWidth + (x * scale + dx)] = color;
        }                           
      }
    }
  }

  // Draw the enlarged image from the buffer
  tft.pushImage(spriteX, spriteY, scaledWidth, scaledHeight, buffer);
}

void setupButtons() {
  btnFeed.setPressedHandler([](Button2 & b) {
    if (!gameOver) {
      if (isSleeping) {
        exitSleepMode();
      }
      changeHunger(25);
      playSound(1000, 200);  // Play a sound when feeding
      displaySprite(happyPet);  // Change sprite to happy when fed
      displayMessage("Fed the pet");
    }
  });

  btnPlay.setPressedHandler([](Button2 & b) {
    if (!gameOver) {
      if (isSleeping) {
        exitSleepMode();
      }
      playSound(1200, 200);  // Play a sound when playing
      displaySprite(playfulPet);  // Change sprite to playful when playing
      menu();
      displayMessage("Played with pet");
    }
  });

  btnSleep.setPressedHandler([](Button2 & b) {
    if (!gameOver) {
      toggleSleep();  // Toggle sleep state
      playSound(800, 500);  // Play a sound when sleeping or waking up
      displayMessage(isSleeping ? "Pet is sleeping" : "Pet woke up");
    }
  });
}

void displayGameOver() {
  tft.fillScreen(TFT_BLACK); // Clear the entire screen
  tft.setTextSize(4);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(tft.width()/2-60, 20);
  tft.println("GAME");
  tft.setCursor(tft.width()/2-60, tft.getCursorY());
  tft.println("OVER");
  // delay(2000); // Wait for 2 seconds before resetting the game
  displaySprite(rip);
  gameOver = true; // Set gameOver flag
}

void resetGame() {
  health = 100;
  hunger = 50;
  happiness = 50;
  isSleeping = false;
  gameOver = false;
  tft.fillScreen(TFT_BLACK);
  drawBackground(); // Draw background again
  displayPetStats();
  displaySprite(happyPet); // Show initial happy pet sprite
}

void displayMessage(const char* message) {
  // Assuming the message area starts at (1, 50)
  int messageX = 1;
  int messageY = 50;
  int messageWidth = tft.width();
  int messageHeight = 10; // Height for one line of text

  // Clear only the area where the message will be displayed
  tft.fillRect(messageX, messageY, messageWidth, messageHeight, TFT_BLACK);
  
  // Display the message
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(messageX, messageY);
  tft.println(message);
  delay(1000);  // Display the message for 1 second
  
  // Clear the message area
  tft.fillRect(messageX, messageY, messageWidth, messageHeight, TFT_BLACK);
  
  // Redraw the pet stats and sprite
  displayPetStats();
  displaySprite(isSleeping ? sleepingPet : happyPet);
}

void drawBackground() {
  // Draw a gradient background
  for (int y = 0; y < tft.height(); y++) {
    uint16_t color = tft.color565(y * 255 / tft.height(), 0, 255 - (y * 255 / tft.height()));
    tft.drawFastHLine(0, y, tft.width(), color);
  }
}

//return old button handlers, draw main screen
void gameResume(){
    setupButtons();
    tft.fillScreen(TFT_BLACK);
    drawBackground(); // Draw background again
    displayPetStats();
    displaySprite(happyPet); // Show initial happy pet sprite
    changeHappiness(25);
}
