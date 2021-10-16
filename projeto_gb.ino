const int down_pin = 2;
const int up_pin = 3;
const int left_pin = 4;
const int right_pin = 5;

const int VERTICAL_SIZE = 8;
const int HORIZONTAL_SIZE = 8;
const int BOARD_SIZE = VERTICAL_SIZE * HORIZONTAL_SIZE;

enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT 
};

struct Point {
  short x;
  short y;
};

Point food = { 0, 0 };
Point snake[BOARD_SIZE] = { { 4, 3 }, { 4, 4 }, { 4, 5 } };
short snakeLength = 3;
Direction direction = RIGHT;

const int INITIAL_SNAKE_SPEED = 500 - snakeLength*6;
int updateInterval = INITIAL_SNAKE_SPEED;
unsigned long timeElapsed = 0;

void setup() {
  pinMode(down_pin, INPUT);
  pinMode(up_pin, INPUT);
  pinMode(left_pin, INPUT);
  pinMode(right_pin, INPUT);
  
  Serial.begin(9600);

  Serial.println("Press the right button to start...");
  while(!canBeginGame());
  generateFood();
}

void loop() {
  if (shouldUpdateState()) {
    moveSnake();
    checkFoodCollision();
    renderState();
    checkGameOver();
  }
  readInputsAndUpdateDirection();
}

bool canBeginGame() {
  return digitalRead(right_pin);  
}

void generateFood() {
  randomSeed(analogRead(0));
  bool validFoodCoordinates = false;
  
  while(!validFoodCoordinates) {
    short x = random(1, HORIZONTAL_SIZE + 1);
    short y = random(1, VERTICAL_SIZE + 1);
    
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

bool hasFoodGenerationCollisionWithSnake(int i, short foodX, short foodY) {
  return snake[i].x == foodX && snake[i].y == foodY;
}

bool hasReachedSnakeHead(int i) {
  return i+1 == snakeLength;
}

bool shouldUpdateState() {
  if (millis() > timeElapsed + updateInterval) {
    timeElapsed = millis();
    return true;
  }
  return false;
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
    short x = snake[i].x + 1 > HORIZONTAL_SIZE ? 1 : snake[i].x + 1;
    snake[i] = { x , snake[i].y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }
}

void moveLeft(int i) {
  if (hasReachedSnakeHead(i)) {
    short x = snake[i].x - 1 < 1 ? HORIZONTAL_SIZE : snake[i].x - 1;
    snake[i] = { x , snake[i].y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }
}

void moveUp(int i) {
  if (hasReachedSnakeHead(i)) {
    short y = snake[i].y - 1 < 1 ? VERTICAL_SIZE : snake[i].y - 1;
    snake[i] = { snake[i].x , y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }
}

void moveDown(int i) {
  if (hasReachedSnakeHead(i)) {
    short y = snake[i].y + 1 > VERTICAL_SIZE ? 1 : snake[i].y + 1;
    snake[i] = { snake[i].x , y };
  } else if (i+1 < snakeLength){
    snake[i] = snake[i+1];
  }  
}

void checkFoodCollision() {
  if (hasFoodCollision()) {
    increaseSnakeLength();
    increaseSnakeSpeed();
    generateFood();
  }  
}

boolean hasFoodCollision() {
  int head = snakeLength - 1;
  return food.x == snake[head].x && food.y == snake[head].y;
}

void increaseSnakeLength() {
  readInputsAndUpdateDirection();
  int head = snakeLength - 1;
  
  if (direction == RIGHT) {
    short x = snake[head].x + 1 > HORIZONTAL_SIZE ? 1 : snake[head].x + 1;
    snake[snakeLength] = { x , snake[head].y };
  } else if (direction == LEFT) {
     short x = snake[head].x - 1 < 1 ? HORIZONTAL_SIZE : snake[head].x - 1;
     snake[snakeLength] = { x , snake[head].y };
  } else if (direction == UP) {
     short y = snake[head].y - 1 < 1 ? VERTICAL_SIZE : snake[head].y - 1;
     snake[snakeLength] = { snake[snakeLength - 1].x , y };
  } else if (direction == DOWN) {
      short y = snake[head].y + 1 > VERTICAL_SIZE ? 1 : snake[head].y + 1;
      snake[snakeLength] = { snake[head].x , y };
  }

  snakeLength++;   
}

int increaseSnakeSpeed() {
  updateInterval = 500 - snakeLength*6; 
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
  Serial.print("SNAKE: ");
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (i < snakeLength) {
      Serial.print(snake[i].x);
      Serial.print(" ");
      Serial.println(snake[i].y);
    }
  }
  Serial.println("");
}

void checkGameOver() {
    if (hasPlayerWin()) {
      displayWinMessage();
    }
    if (hasPlayerLost()) {
      displayLostMessage();
    }  
}

boolean hasPlayerWin() {
  return snakeLength >= BOARD_SIZE;
}

boolean hasPlayerLost() {
  for (int i = 0; i < snakeLength - 1; i++) {
    if (hasSnakeCollision(i)) {
      return true;
    }
  }   
  return false;
}

boolean hasSnakeCollision(int tail) {
  int head = snakeLength - 1;
  return snake[head].x == snake[tail].x && snake[head].y == snake[tail].y; 
}

void displayWinMessage() {
  // ------------------------------------------------------------
  // SUBSTITUIR PRINTS PELA INTERFACE DE COMUNICACAO COM A MATRIZ
  // ------------------------------------------------------------
  Serial.println("You win! :)");
  while(true);
}

void displayLostMessage() {
  // ------------------------------------------------------------
  // SUBSTITUIR PRINTS PELA INTERFACE DE COMUNICACAO COM A MATRIZ
  // ------------------------------------------------------------
  Serial.println("You lost... :(");
  while(true);
}

void readInputsAndUpdateDirection() {
  if (direction == RIGHT || direction == LEFT) {
    if (digitalRead(up_pin)) {
      direction = UP;
    } else if (digitalRead(down_pin)) {
      direction = DOWN;
    }
  }
               
  if (direction == UP || direction == DOWN) {
    if (digitalRead(right_pin)) {
      direction = RIGHT;
    } else if (digitalRead(left_pin)) {
      direction = LEFT;
    }
  }
}
