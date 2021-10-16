const byte DOWN_PIN = 2;
const byte UP_PIN = 3;
const byte LEFT_PIN = 4;
const byte RIGHT_PIN = 5;
const byte EASY_MODE_LED = 8;
const byte HARD_MODE_LED = 9;

const byte VERTICAL_SIZE = 8;
const byte HORIZONTAL_SIZE = 8;
const int BOARD_SIZE = VERTICAL_SIZE * HORIZONTAL_SIZE;

const int EASY_MODE_INTERVAL_IN_MS = 500;
const int EASY_MODE_SUBTRACTOR_IN_MS = 4;
const int HARD_MODE_INTERVAL_IN_MS = 450;
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
short intervalSubstractorInMs = EASY_MODE_SUBTRACTOR_IN_MS;
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
    intervalSubstractorInMs = HARD_MODE_SUBTRACTOR_IN_MS;
    updateIntervalInMs = HARD_MODE_INTERVAL_IN_MS;
    digitalWrite(HARD_MODE_LED, HIGH);
    return true;
  }
  if (digitalRead(DOWN_PIN)) {
    intervalSubstractorInMs = EASY_MODE_SUBTRACTOR_IN_MS;
    updateIntervalInMs = EASY_MODE_INTERVAL_IN_MS;
    digitalWrite(EASY_MODE_LED, HIGH);
    return true;
  }
  return false;
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
      if (hasReachedSnakeHead(i)) {
        validFoodCoordinates = true;
      }
    }
    food = {x, y};
  }
}

bool hasFoodGenerationCollisionWithSnake(int i, byte foodX, byte foodY) {
  return snake[i].x == foodX && snake[i].y == foodY;
}

bool hasReachedSnakeHead(int i) {
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
  readInputsAndUpdateSnakeDirection();
  int head = snakeLength - 1;
  
  if (direction == RIGHT) {
    byte x = snake[head].x + 1 > HORIZONTAL_SIZE ? 1 : snake[head].x + 1;
    snake[snakeLength] = { x , snake[head].y };
  } else if (direction == LEFT) {
    byte x = snake[head].x - 1 < 1 ? HORIZONTAL_SIZE : snake[head].x - 1;
    snake[snakeLength] = { x , snake[head].y };
  } else if (direction == UP) {
    byte y = snake[head].y - 1 < 1 ? VERTICAL_SIZE : snake[head].y - 1;
    snake[snakeLength] = { snake[snakeLength - 1].x , y };
  } else if (direction == DOWN) {
    byte y = snake[head].y + 1 > VERTICAL_SIZE ? 1 : snake[head].y + 1;
    snake[snakeLength] = { snake[head].x , y };
  }

  snakeLength++;   
}

int increaseSnakeSpeed() {
    updateIntervalInMs -= intervalSubstractorInMs; 
}

void moveSnake() {
  for (int i = 0; i < BOARD_SIZE - 1; i++) {
    if (direction == RIGHT) {
      moveRight(i);
    } else if (direction == LEFT) {
      moveLeft(i);
    } else if (direction == UP) {
      moveUp(i);
    } else if (direction == DOWN) {
      moveDown(i);
    }
  }
}

void moveRight(int i) {
  if (hasReachedSnakeHead(i)) {
    byte x = snake[i].x + 1 > HORIZONTAL_SIZE ? 1 : snake[i].x + 1;
    snake[i] = { x , snake[i].y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }
}

void moveLeft(int i) {
  if (hasReachedSnakeHead(i)) {
    byte x = snake[i].x - 1 < 1 ? HORIZONTAL_SIZE : snake[i].x - 1;
    snake[i] = { x , snake[i].y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }
}

void moveUp(int i) {
  if (hasReachedSnakeHead(i)) {
    byte y = snake[i].y - 1 < 1 ? VERTICAL_SIZE : snake[i].y - 1;
    snake[i] = { snake[i].x , y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }
}

void moveDown(int i) {
  if (hasReachedSnakeHead(i)) {
    byte y = snake[i].y + 1 > VERTICAL_SIZE ? 1 : snake[i].y + 1;
    snake[i] = { snake[i].x , y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }  
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
  if (direction == RIGHT || direction == LEFT) {
    if (digitalRead(UP_PIN)) {
      direction = UP;
    } else if (digitalRead(DOWN_PIN)) {
      direction = DOWN;
    }
  }
               
  if (direction == UP || direction == DOWN) {
    if (digitalRead(RIGHT_PIN)) {
      direction = RIGHT;
    } else if (digitalRead(LEFT_PIN)) {
      direction = LEFT;
    }
  }
}
