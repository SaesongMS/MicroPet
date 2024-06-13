void exitSleepMode() {
  isSleeping = false;
  healthChange = 0;
  hungerChange = 0;
  happinessChange = 0;
  displayPetStats();
  displaySprite(happyPet); // Show awake sprite
}

void toggleSleep() {
  isSleeping = !isSleeping;
  if (isSleeping) {
    // Sleeping increases health but decreases hunger and happiness
    healthChange = 20;
    hungerChange = -10;
    happinessChange = -5;
    health = constrain(health + 20, 0, 100);
    hunger = constrain(hunger - 10, 0, 100);
    happiness = constrain(happiness - 5, 0, 100);
  } else {
    healthChange = 0;
    hungerChange = 0;
    happinessChange = 0;
  }
  checkHealth(); // Check health after modifying states
  displayPetStats();
  displaySprite(isSleeping ? sleepingPet : happyPet); // Show appropriate sprite
}

void changeHunger(int amount) {
  hungerChange = amount;
  hunger = constrain(hunger + amount, 0, 100);
  displaySprite(eatingPet); // Show eating sprite
  delay(1000);  // Show eating sprite for 1 second
  displaySprite(happyPet); // Show happy sprite after eating
  checkHealth(); // Check health after modifying hunger
  displayPetStats();
}

void changeHappiness(int amount) {
  happinessChange = amount;
  happiness = constrain(happiness + amount, 0, 100);
  displayPetStats();
}

void checkHealth() {
  healthChange = 0;
  if(health > 0)
    updateLed(1) ;
  if (hunger == 0 || happiness == 0) {
    healthChange = -10;
    health = constrain(health - 10, 0, 100);
    updateLed(2);
    displaySprite(sad);
    displayPetStats();
  }
  if (health <= 0) {
    displayGameOver();
    updateLed(3);
  }
}

void updateGameState() {
  hungerChange = 0;
  happinessChange = 0;
  healthChange = 0;
  if (!isSleeping) {
    // Decrease hunger and happiness gradually over time
    hungerChange = -5;
    happinessChange = -3;
    hunger = max(0, hunger - 5);
    happiness = max(0, happiness - 3);
    checkHealth(); // Check if low hunger or happiness affects health
  } else {
    // Increase health gradually over time while sleeping
    healthChange = 2;
    health = min(100, health + 2);
  }
  if(!gameOver)
    displayPetStats();
}