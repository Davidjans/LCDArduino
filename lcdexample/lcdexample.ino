#include <LiquidCrystal.h>

LiquidCrystal lcd(4,6,10,11,12,13);

#define LEFTBUTTON 2
#define RIGHTBUTTON 3

int score = 0;
int heroPositionX = 5;
int heroPositionY = 2;
bool leftButtonLastFrame;
bool rightButtonLastFrame;
// 0 = left 1 = idle 2 = right
float heroDirection;

float idleTimer;
float idleOriginalTimer = 1000;

float animationTimer = 200;
float originalAnimationTimer = 400;

float monsterXPosition = 0;
float monsterYPosition = 2;

bool animationState = false;
bool shouldDraw = true;
bool victory = false;

unsigned long current = 0;
unsigned long last = 0;

float deltaTime;

byte hero[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

byte heroWalkRightOne[8] = {
  B01100,
  B01100,
  B00000,
  B01110,
  B11100,
  B01100,
  B11010,
  B10011
};

byte heroWalkRightTwo[8] = {
  B01100,
  B01100,
  B00000,
  B01100,
  B01100,
  B01100,
  B01100,
  B01110
};

byte heroWalkLeftOne[8] = {
  B00110,
  B00110,
  B00000,
  B01110,
  B00111,
  B00110,
  B01011,
  B11001
};

byte heroWalkLeftTwo[8] = {
  B00110,
  B00110,
  B00000,
  B00110,
  B00110,
  B00110,
  B00110,
  B01110
};

byte heroIdle[8] = {
  B00100,
  B00100,
  B00000,
  B11111,
  B01110,
  B01110,
  B01010,
  B11011
};

byte enemy[8] = {
  B10001,
  B01010,
  B10001,
  B00000,
  B00000,
  B01110,
  B10001,
};

void ClearPosition(float positionX, float positionY){
  lcd.setCursor(positionX, positionY);
  lcd.write(" ");
}

void WriteScore(){
  String scoreDisplay = "Score: " + String(score);
  lcd.setCursor(0, 0);
  lcd.write(scoreDisplay.c_str());
}

void SetTimers(){
  current = millis();

  deltaTime = current - last;

  idleTimer = idleTimer - deltaTime; 
  animationTimer = animationTimer - deltaTime;
}

void CheckIdle(){
   if(idleTimer <= 0){
    heroDirection = 1;
    shouldDraw = true;
    idleTimer = 9999999;
  }
}

void CheckAnimation(){
  if(animationTimer <= 0){
    animationState = !animationState;
    animationTimer = originalAnimationTimer;
    shouldDraw = true;
  }
}

void DrawHero(){
  if(shouldDraw){
     if(heroDirection == 0 && animationState == false){
      ClearPosition(heroPositionX, heroPositionY);
      lcd.setCursor(heroPositionX, heroPositionY);
      lcd.write(byte(1));
     }
     else if(heroDirection == 0 && animationState == true){
      lcd.setCursor(heroPositionX, heroPositionY);
      lcd.write(byte(2));
     }
    
    if(heroDirection == 1){
      ClearPosition(heroPositionX, heroPositionY);
      lcd.setCursor(heroPositionX, heroPositionY);
      lcd.write(byte(0));
    }
  
    if(heroDirection == 2 && animationState == false){
      ClearPosition(heroPositionX, heroPositionY);
      lcd.setCursor(heroPositionX, heroPositionY);
      lcd.write(byte(3));
    }
    else if(heroDirection == 2 && animationState == true){
      ClearPosition(heroPositionX, heroPositionY);
      lcd.setCursor(heroPositionX, heroPositionY);
      lcd.write(byte(4));
    }
   shouldDraw = false;
  }
}

void CheckMonsterKill(){
 if(heroPositionX == monsterXPosition){
  score += 1;
  monsterXPosition = rand() % 18;  
  lcd.setCursor(monsterXPosition, heroPositionY);
  lcd.write(byte(5));
 }
}

void MovePosition(){
  bool leftButtonDown = (digitalRead(LEFTBUTTON) == HIGH);
  bool rightButtonDown = (digitalRead(RIGHTBUTTON) == HIGH);
  
  if(leftButtonDown && leftButtonLastFrame == false && heroPositionX >= 1){
    ClearPosition(heroPositionX, heroPositionY);
    heroPositionX--;
    heroDirection = 0;
    idleTimer = idleOriginalTimer;
    shouldDraw = true;
  }
  else if(rightButtonDown && rightButtonLastFrame == false && heroPositionX <= 18){
    ClearPosition(heroPositionX, heroPositionY);
    heroPositionX++;
    heroDirection = 2;
    idleTimer = idleOriginalTimer;
    shouldDraw = true;
  }
  leftButtonLastFrame = leftButtonDown;
  rightButtonLastFrame = rightButtonDown;
}

void CheckForVictory(){
  if(score >= 10){
    victory = true;
    for (int x = 0; x < 19; x++ ) {
      for (int y = 0; y < 3; y++ ) {
        ClearPosition(x,y);
      }
    }
    lcd.setCursor(5, 0);
    lcd.write("Victory!!!");
    lcd.setCursor(5, 1);
    lcd.write("Victory!!!");
    lcd.setCursor(5, 2);
    lcd.write("Victory!!!");
    lcd.setCursor(5, 3);
    lcd.write("Victory!!!");
  }
}
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  lcd.createChar(0, heroIdle);
  lcd.createChar(1, heroWalkLeftOne);
  lcd.createChar(2, heroWalkLeftTwo);
  lcd.createChar(3, heroWalkRightOne);
  lcd.createChar(4, heroWalkRightTwo);
  
  lcd.createChar(5, enemy);
  lcd.begin(20,4);

  lcd.setCursor(0, 2);
  lcd.write(byte(5));

  lcd.setCursor(heroPositionX, heroPositionY);
  lcd.write(byte(0));
  
  pinMode(LEFTBUTTON, INPUT);
  pinMode(RIGHTBUTTON, INPUT);
}

void loop() {  
  if(!victory){
    WriteScore();
  
    SetTimers();
    CheckIdle();
    CheckAnimation();
    
    DrawHero();
    
    CheckMonsterKill();
  
    MovePosition();

    CheckForVictory();
    last = current;
  }
  delay(15);
}
