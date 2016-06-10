#include "U8glib.h"
#include <DS3231.h>
#include <Wire.h>

/*
 * Pin numbers for hardware.
 */
const int button1 = 8;
const int button2 = 6;
const int externalLED = 9;
const int vibrate = 4;

/*
 * Size of player boxes in pixels.
 */
int boxHeight = 20;
int boxWidth = 5;

/*
 * Player options.
 */
int playerPos = 0;
int playerSpeed = 2;

/*
 * AI options.
 */
int aiPos = 0;
int aiSpeed = 2;

/*
 * Ball options.
 */
int ballPosX = 64;
int ballPosY = 32;
int ballRadius = 1;
int ballSpeed = 4;

/*
 * Ball properties.
 */
boolean isBallGoingRight = true;
boolean isBallGoingDown = true;

/*
 * Game state.
 */
boolean isGameOver = false;

/*
 * Screen.
 */
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);

/*
 * Setup function (run once at beginning).
 */
void setup(){
  Serial.begin(9600);
  Wire.begin();
  
    //Button number 1 ************************************************************************************
  pinMode(button1, INPUT_PULLUP);           // set pin to input
  digitalWrite(button1, HIGH);


  //Button number 2 ************************************************************************************
  pinMode(button2, INPUT_PULLUP);           // set pin to input
  digitalWrite(button2, HIGH);

  // FlashLight  ************************************************************************************
  pinMode(externalLED, OUTPUT);
  digitalWrite(externalLED , LOW);

  //Vibration Motor should be connected to GND  ************************************************************************************
  pinMode(vibrate, OUTPUT);
  digitalWrite(vibrate, LOW);
}

/*
 * Loop function (run every time it finishes).
 */
void loop(){
  if (isGameOver){ // Check if game is over
    if (digitalRead(button1) == LOW) { // wait for restart button to be pressed and then restart.
      ballPosX = 64;
      ballPosY = 32;
      isGameOver = false;
    }
  } else { // game is not over
    checkButtons(); // check to see if any buttons are pressed.
    updateBall(); // update the ball position
    updateAI(); // update ai
    updateState(); // check to see if game is over
    drawGraphics(); // draw new game state to screen
  }
}

/*
 * Update AI with new position.
 */
void updateAI(){
  if (isBallGoingRight){
    if (ballPosY<aiPos+boxHeight/2){
      if (aiPos>0)
        aiPos -= aiSpeed;
    } else {
      if (aiPos<u8g.getHeight()-boxHeight)
        aiPos += aiSpeed;
    }
  }
}

/*
 * Update ball position.
 */
void updateBall(){
  if (isBallGoingDown){
    ballPosY += ballSpeed;
  } else {
    ballPosY -= ballSpeed;
  }
  if (isBallGoingRight){
    ballPosX += ballSpeed;
  } else {
    ballPosX -= ballSpeed;
  }
}

/*
 * Update game state.
 */
void updateState(){
  if (ballPosX==0 || ballPosX==u8g.getWidth()){
    isBallGoingRight = !isBallGoingRight;
    isGameOver = true;
  }
  if (ballPosY==0 || ballPosY==u8g.getHeight()){
    isBallGoingDown = !isBallGoingDown;
  }
  if (ballPosY>playerPos && ballPosY<playerPos+boxHeight && ballPosX<boxWidth){
    isBallGoingRight = !isBallGoingRight;
  }
  if (ballPosY>aiPos && ballPosY<aiPos+boxHeight && ballPosX>u8g.getWidth()-boxWidth){
    isBallGoingRight = !isBallGoingRight;
  }
}

/*
 * Button functions
 */
void checkButtons(){
  if (digitalRead(button1) == LOW) {
    if (playerPos<u8g.getHeight()-boxHeight)
      playerPos += playerSpeed;
  }
  if (digitalRead(button2) == LOW) {
    if (playerPos>0)
      playerPos -= playerSpeed;
  }
}


/*
 * Graphics functions.
 */
void drawGraphics(){
  u8g.firstPage();
  do {
    u8g.drawBox(0, playerPos, boxWidth, boxHeight); // Player box
    u8g.drawBox(u8g.getWidth()-boxWidth, aiPos, boxWidth, boxHeight); // AI box
    u8g.drawDisc(ballPosX, ballPosY, ballRadius); // Ball
  }
  while ( u8g.nextPage() );
}

