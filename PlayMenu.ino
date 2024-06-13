/*
after button pressed, display the menu
button up/down navigate menu
buttonPlay to confirm choice
if choice is 1, changeHappiness(25)
if choice is 2, start by using startScreen() from DanceMemo.ino
*/

int confirm = 0;
int choice = 1;
int inMenu = 0;

//up - 1, down - 2
void changeChoice(int dir){
    if(dir == 1){
        choice = choice == 1 ? 3 : choice - 1;
    }
    else{
        choice = choice == 3 ? 1 : choice + 1;
    }
    displayMenu();
}

void setupInternalButtons(){
    btnDown.setPressedHandler([](Button2 & b) {
        Serial.println("Down button pressed");
        if(inMenu)
            changeChoice(2);
    });
    btnUp.setPressedHandler([](Button2 & b) {
        Serial.println("Up button pressed");
        if(inMenu)
            changeChoice(1);
    });
    btnPlay.setPressedHandler([](Button2 & b) {
        Serial.println("Play button pressed");
        if(inMenu){
            confirm = 1;
            inMenu = 0;
        }
    });
}

void displayMenu(){
    clearTextArea();
    tft.setCursor(0, 5);
    tft.setTextSize(2);
    choice == 1 ? tft.setTextColor(TFT_GREEN, TFT_BLACK) : tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("1. Quick Play");
    choice == 2 ? tft.setTextColor(TFT_GREEN, TFT_BLACK) : tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("2. Dance Memo");
    choice == 3 ? tft.setTextColor(TFT_GREEN, TFT_BLACK) : tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("3. Maze Runner");
}

void menu(){
    Serial.println("Menu");
    inMenu = 1;
    confirm = 0;
    setupInternalButtons();
    displayMenu();
    while(!confirm){
        btnDown.loop();
        btnUp.loop();
        btnPlay.loop();
    }
    if(choice == 1){
        changeHappiness(25);
        setupButtons();
    }
    else if(choice == 2)
        setupDance();
    else
        setupMaze();
}
