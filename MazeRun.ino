/*
spawn player at the top left corner of the maze so playerX = 0 and playerY = 0
spawn the exit at the bottom right corner of the maze so exitX = 15 and exitY = 7

*/
const int mazeWidth = 32;
const int mazeHeight = 16;
const char maze[mazeHeight][mazeWidth + 1] = {
  " ###############################",
  "                                ",
  "## ###### ######## ########### #",
  "## #    # #        #           #",
  "######### ######### ############",
  "#                              #",
  "# ######## #########           #",
  "# #      # #        # #        #",
  "# # #### # # ###### # # ###### #",
  "# # #  # # # #    #   # #    # #",
  "#   #  # # # # ## # # # # ## # #",
  "# # #  # # # #    # ### #    # #",
  "# # #### # # ###### # # ###### #",
  "###      # #          #        #",
  "# ### ############## ######### #",
  "# # #  # #                     "
};

int playerX = 0;
int playerY = 0;
int exitX = 29;
int exitY = 15;
int startTime = 0;

void setupMaze(){
    Serial.println("tft.height: " + String(tft.height()) + " tft.width: " + String(tft.width()));
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);  
    tft.setTextColor(TFT_WHITE);
    startTime = millis();
    setupMazeButtons();
    drawMaze();
    drawPlayer();
    playMaze();
}

void setupMazeButtons(){
    btnDown.setPressedHandler([](Button2 & b) {
        movePlayer('D');
    });
    btnUp.setPressedHandler([](Button2 & b) {
        movePlayer('U');
    });
    btnFeed.setPressedHandler([](Button2 & b) {
        movePlayer('L');
    });
    btnPlay.setPressedHandler([](Button2 & b) {
        movePlayer('R');
    });
}

void drawMaze() {
    for (int y = 0; y < mazeHeight; y++) {
        for (int x = 0; x < mazeWidth; x++) {
            if (maze[y][x] == '#') {
                tft.fillRect(x * 8, y * 8, 8, 8, TFT_WHITE);
            }
        }
    }
    tft.fillRect(exitX * 8, exitY * 8, 8, 8, TFT_RED);
}

void clearPlayer(){
    tft.fillRect(playerX * 8+1, playerY * 8+1, 6, 6, TFT_BLACK);
}

void drawPlayer() {
    tft.fillRect(playerX * 8+1, playerY * 8+1, 6, 6, TFT_GREEN);
    // tft.fillRect(playerX * 8+1, playerY * 8+1, 6, 6, TFT_BLACK);
    // tft.fillRect(playerX * 8+1, playerY * 8+1, 6, 6, TFT_GREEN);
    Serial.println("playerX: " + String(playerX) + " playerY: " + String(playerY));
    delay(75);
}

void playMaze(){
    playerX = 0;
    playerY = 0;
    while(playerX != exitX || playerY != exitY){
        btnDown.loop();
        btnUp.loop();
        btnFeed.loop();
        btnPlay.loop();
    }
    displayWinScreen();
}

void movePlayer(char direction) {
    if (direction == 'U' && playerY > 0 && maze[playerY - 1][playerX] != '#') {
        clearPlayer();
        playerY--;
        Serial.println("up");
    } else if (direction == 'D' && playerY < mazeHeight - 1 && maze[playerY + 1][playerX] != '#') {
        clearPlayer();
        playerY++;
        Serial.println("down");
    } else if (direction == 'L' && playerX > 0 && maze[playerY][playerX - 1] != '#') {
        clearPlayer();
        playerX--;
        Serial.println("left");
    } else if (direction == 'R' && playerX < mazeWidth - 1 && maze[playerY][playerX + 1] != '#') {
        clearPlayer();
        playerX++;
        Serial.println("right");
    }
    // tft.fillScreen(TFT_BLACK);
    // drawMaze();
    drawPlayer();
}

void displayWinScreen(){
    int timeTaken = (millis() - startTime) / 1000;
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(tft.height()/2, 20);
    tft.println("Good");
    tft.setCursor(tft.height()/2, tft.getCursorY());
    tft.println("Job!");
    tft.setCursor(tft.height()/2, tft.getCursorY());
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Time taken:");
    tft.setCursor(tft.height()/2, tft.getCursorY());
    tft.println(String(timeTaken) + " seconds");
    delay(2000);
    gameResume();
}