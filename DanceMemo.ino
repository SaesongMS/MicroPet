#include <math.h>

//
void setupDance(){
    tft.fillScreen(TFT_BLACK); // Clear the entire screen
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(tft.height()/2, 20);
    tft.println("Let's");
    tft.setCursor(tft.height()/2, tft.getCursorY());
    tft.println("Dance");
    delay(2000);
    danceGame();
}

//left - 1, up - 2, right - 3
//generate random moves
int* getMoves(int *moves){
    for(int i = 0; i < 4; i++){
        moves[i] = random(1, 4);
        Serial.println(moves[i]);
    }
    return moves;
}

//draw move as arrow
void drawMove(int move, int X, int Y){
    int size = 10;
    if(move == 1){
        //left
        tft.drawLine(X, Y + size/2, X + size, Y + size/2, TFT_WHITE);
        tft.drawLine(X, Y + size/2, X + 5, Y + size/2 - 5, TFT_WHITE);
        tft.drawLine(X, Y + size/2, X + 5, Y + size/2 + 5, TFT_WHITE);
    } else if(move == 2){
        //up
        tft.drawLine(X, Y, X, Y + size, TFT_WHITE);
        tft.drawLine(X, Y, X - 5, Y + 5, TFT_WHITE);
        tft.drawLine(X, Y, X + 5, Y + 5, TFT_WHITE);
    } else if(move == 3){
        //right
        tft.drawLine(X, Y + size/2, X - size, Y + size/2, TFT_WHITE);
        tft.drawLine(X, Y + size/2, X - 5, Y + size/2 - 5, TFT_WHITE);
        tft.drawLine(X, Y + size/2, X - 5, Y + size/2 + 5, TFT_WHITE);
    }
}

//draw random generated moves
void displayMoves(int moves[]){
    //draw moves as arrows
    tft.fillScreen(TFT_BLACK);
    int X = tft.width()/4, Y = tft.height()/4 + 20, size = 10;
    for(int i = 0; i < 4; i++){
        if(moves[i] == 1){
            drawMove(1, X, Y);
        } else if(moves[i] == 2){
            drawMove(2, X, Y);
        } else if(moves[i] == 3){
            drawMove(3, X, Y);
        }
        X += 30 + size;
    }
    delay(2000);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(tft.height()/2, 20);
    tft.println("Repeat");
}

int* inputMoves = new int[4]; //store user input moves
int inputIndex = 0; //check how many buttons have been pressed
int inputX = tft.height()/4, inputY = tft.width()/2 - 20; //position of user input moves for drawing

//draw  waves next to or above the sprite, depending of the move
void drawSpriteMove(int move){
    int scale = 2;
    int spriteX = (tft.width() - 16 * scale) / 2;
    int spriteY = tft.height() - 16 * scale - 10;

    if(move == 1)
       displaySprite(left);
    else if(move == 2)
        displaySprite(jump);
    else if(move == 3)
        displaySprite(right);
}

//setup handlers for game buttons
void setupGameButtons(){
    btnFeed.setPressedHandler([](Button2 & b) {
        Serial.println("Left button pressed");
        drawMove(1, inputX, inputY);
        inputX += 30 + 10;
        drawSpriteMove(1);
        inputMoves[inputIndex] = 1;
        inputIndex++;
    });
    btnSleep.setPressedHandler([](Button2 & b) {
        Serial.println("Up button pressed");
        drawMove(2, inputX, inputY);
        inputX += 30 + 10;
        drawSpriteMove(2);
        inputMoves[inputIndex] = 2;
        inputIndex++;
    });
    btnPlay.setPressedHandler([](Button2 & b) {
        Serial.println("Right button pressed");
        drawMove(3, inputX, inputY);
        inputX += 30 + 10;
        drawSpriteMove(3);
        inputMoves[inputIndex] = 3;
        inputIndex++;
    });
}

void danceGame(){
    int moves[4];
    getMoves(moves);
    inputIndex = 0;
    inputX = tft.height()/4;
    Serial.print("inputY: ");
    Serial.println(inputY);
    displayMoves(moves);
    displaySprite(musicnotes);
    setupGameButtons();
    while(inputIndex < 4){
        btnFeed.loop();
        btnPlay.loop();
        btnSleep.loop();
    }
    delay(500);
    int correct = 1;
    for(int i = 0; i < 4; i++){
        if(inputMoves[i] != moves[i]){
            correct = 0;
            break;
        }
    }
    Serial.print("Correct: ");
    Serial.println(correct);
    if(correct){
        tft.fillScreen(TFT_BLACK);
        tft.setTextSize(4);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setCursor(tft.height()/2, 20);
        tft.println("Good");
        tft.setCursor(tft.height()/2, tft.getCursorY());
        tft.println("Job!");
        delay(2000);
        gameResume();
    } else {
        tft.fillScreen(TFT_BLACK);
        tft.setTextSize(4);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setCursor(tft.height()/2, 20);
        tft.println("Try");
        tft.setCursor(tft.height()/2, tft.getCursorY());
        tft.println("Again!");
        delay(2000);
        gameResume();
    }
}