const byte DOWN_PIN = 2;
const byte UP_PIN = 3;
const byte LEFT_PIN = 4;
const byte RIGHT_PIN = 5;
const byte EASY_MODE_LED = 8;
const byte HARD_MODE_LED = 9;

const byte VERTICAL_SIZE = 8;
const byte HORIZONTAL_SIZE = 8;
const int BOARD_SIZE = VERTICAL_SIZE * HORIZONTAL_SIZE;

const int EASY_MODE_INTERVAL_IN_MS = 1000;
const int EASY_MODE_SUBTRACTOR_IN_MS = 4;
const int HARD_MODE_INTERVAL_IN_MS = 900;
const int HARD_MODE_SUBTRACTOR_IN_MS = 5;

enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT 
};

struct Point {
  byte x;
  byte y;
};

Point food = { 0, 0 };
Point snake[BOARD_SIZE] = { { 4, 3 }, { 4, 4 }, { 4, 5 } };
int snakeLength = 3;
Direction direction = RIGHT;

int updateIntervalInMs = EASY_MODE_INTERVAL_IN_MS;
short intervalSubtractorInMs = EASY_MODE_SUBTRACTOR_IN_MS;
unsigned long timeElapsedInMs = 0;

void setup() {
  pinMode(DOWN_PIN, INPUT);
  pinMode(UP_PIN, INPUT);
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  pinMode(EASY_MODE_LED, OUTPUT);
  pinMode(HARD_MODE_LED, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Press the down button for easy mode or up button for hard mode...");
  while(!isModeChosen());
  Serial.println("Press the right button to start...");
  while(!canBeginGame());
  generateFood();
}

void loop() {
  if (canUpdateGameState()) {
    updateGameState();
  }
  readInputsAndUpdateSnakeDirection();
}

bool isModeChosen() {
  if (digitalRead(UP_PIN)) {
     return selectHardMode();
  }
  if (digitalRead(DOWN_PIN)) {
    return selectEasyMode();
  }
  return false;
}

bool selectHardMode() {
  intervalSubtractorInMs = HARD_MODE_SUBTRACTOR_IN_MS;
  updateIntervalInMs = HARD_MODE_INTERVAL_IN_MS;
  digitalWrite(HARD_MODE_LED, HIGH);
  return true;
}

bool selectEasyMode() {
  intervalSubtractorInMs = EASY_MODE_SUBTRACTOR_IN_MS;
  updateIntervalInMs = EASY_MODE_INTERVAL_IN_MS;
  digitalWrite(EASY_MODE_LED, HIGH);
  return true;
}

bool canBeginGame() {
  return digitalRead(RIGHT_PIN);  
}

void generateFood() {
  randomSeed(analogRead(0));
  bool validFoodCoordinates = false;
  
  while(!validFoodCoordinates) {
    byte x = random(1, HORIZONTAL_SIZE + 1);
    byte y = random(1, VERTICAL_SIZE + 1);
    
    for (int i = 0; i < snakeLength; i++) {
      if (hasFoodGenerationCollisionWithSnake(i, x, y)) {
        break;
      }
      if (isSnakeHead(i)) {
        validFoodCoordinates = true;
      }
    }
    food = {x, y};
  }
}

bool hasFoodGenerationCollisionWithSnake(int i, byte foodX, byte foodY) {
  return snake[i].x == foodX && snake[i].y == foodY;
}

bool isSnakeHead(int i) {
  return i+1 == snakeLength;
}

bool canUpdateGameState() {
  if (millis() > timeElapsedInMs + updateIntervalInMs) {
    timeElapsedInMs = millis();
    return true;
  }
  return false;
}

void updateGameState() {
  if (hasFoodCollision()) {
    handleFoodCollision();
  } else {
    moveSnake();
  }
  checkGameOver();
  renderState();
}

bool hasFoodCollision() {
  int head = snakeLength - 1;
  return food.x == snake[head].x && food.y == snake[head].y;
}

void handleFoodCollision() {
  increaseSnakeLength();
  increaseSnakeSpeed();
  generateFood(); 
}

void increaseSnakeLength() {
  int headPosition = snakeLength;
  moveHead(headPosition);
  snakeLength++;   
}

int increaseSnakeSpeed() {
  updateIntervalInMs -= intervalSubtractorInMs; 
}

void moveSnake() {
  moveTail();
  int headPosition = snakeLength - 1;
  moveHead(headPosition);
}

void moveTail() {
  for (int i = 0; i < BOARD_SIZE - 1; i++) {
    if (isSnakeTail(i)){
      snake[i] = snake[i+1];
    }
  }
}

bool isSnakeTail(int i) {
  return i+1 < snakeLength;  
}

void moveHead(int toPosition) {
  if (direction == RIGHT) {
    moveHeadRight(toPosition);
  } else if (direction == LEFT) {
    moveHeadLeft(toPosition);
  } else if (direction == UP) {
    moveHeadUp(toPosition);
  } else if (direction == DOWN) {
    moveHeadDown(toPosition);
  }  
}

void moveHeadRight(int toPosition) {
  int head = snakeLength - 1;
  byte x = snake[head].x + 1 > HORIZONTAL_SIZE ? 1 : snake[head].x + 1;
  snake[toPosition] = { x , snake[head].y };
}

void moveHeadLeft(int toPosition) {
  int head = snakeLength - 1;
  byte x = snake[head].x - 1 < 1 ? HORIZONTAL_SIZE : snake[head].x - 1;
  snake[toPosition] = { x , snake[head].y };
}

void moveHeadUp(int toPosition) {
  int head = snakeLength - 1;
  byte y = snake[head].y - 1 < 1 ? VERTICAL_SIZE : snake[head].y - 1;
  snake[toPosition] = { snake[head].x , y };
}

void moveHeadDown(int toPosition) {
  int head = snakeLength - 1;
  byte y = snake[head].y + 1 > VERTICAL_SIZE ? 1 : snake[head].y + 1;
  snake[toPosition] = { snake[head].x , y };
}

void checkGameOver() {
  if (hasPlayerWin()) {
    displayWinMessage();
  }
  if (hasPlayerLost()) {
    displayLostMessage();
  }  
}

bool hasPlayerWin() {
  return snakeLength >= BOARD_SIZE;
}

bool hasPlayerLost() {
  for (int i = 0; i < snakeLength - 1; i++) {
    if (hasSnakeCollision(i)) {
      return true;
    }
  }   
  return false;
}

bool hasSnakeCollision(int tail) {
  int head = snakeLength - 1;
  return snake[head].x == snake[tail].x && snake[head].y == snake[tail].y; 
}

void displayWinMessage() {
  // ------------------------------------------------------------
  // SUBSTITUIR PRINTS PELA INTERFACE DE COMUNICACAO COM A MATRIZ
  // ------------------------------------------------------------
  Serial.println("You win! :)");
  while(true) {
    digitalWrite(EASY_MODE_LED, HIGH);
    digitalWrite(HARD_MODE_LED, LOW);
    delay(300);
    digitalWrite(EASY_MODE_LED, LOW);
    digitalWrite(HARD_MODE_LED, HIGH);
    delay(300);
  };
}

void displayLostMessage() {
  // ------------------------------------------------------------
  // SUBSTITUIR PRINTS PELA INTERFACE DE COMUNICACAO COM A MATRIZ
  // ------------------------------------------------------------
  Serial.println("You lost... :(");
  while(true) {
    digitalWrite(EASY_MODE_LED, HIGH);
    digitalWrite(HARD_MODE_LED, HIGH);
    delay(300);
    digitalWrite(EASY_MODE_LED, LOW);
    digitalWrite(HARD_MODE_LED, LOW);
    delay(300);
  };
}

void renderState() {
  // ------------------------------------------------------------
  // SUBSTITUIR PRINTS PELA INTERFACE DE COMUNICACAO COM A MATRIZ
  // ------------------------------------------------------------
  Serial.println("");
  Serial.print("FOOD: ");
  Serial.print(food.x);
  Serial.print(" ");
  Serial.println(food.y);
  Serial.println("");
  Serial.println("SNAKE: ");
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (i < snakeLength) {
      Serial.print(snake[i].x);
      Serial.print(" ");
      Serial.println(snake[i].y);
    }
  }
  Serial.println("");
}

void readInputsAndUpdateSnakeDirection() {
  if (isSnakeMovingHorizontally()) {
    checkVerticalButtons();
  }
  if (isSnakeMovingVertically()) {
    checkHorizontalButtons();
  }
}

bool isSnakeMovingHorizontally() {
  return direction == RIGHT || direction == LEFT;
}

void checkVerticalButtons() {
  if (digitalRead(UP_PIN)) {
    direction = UP;
  } else if (digitalRead(DOWN_PIN)) {
    direction = DOWN;
  }
}

bool isSnakeMovingVertically() {
  return direction == UP || direction == DOWN;
}

void checkHorizontalButtons() {
  if (digitalRead(RIGHT_PIN)) {
    direction = RIGHT;
  } else if (digitalRead(LEFT_PIN)) {
    direction = LEFT;
  }
}
