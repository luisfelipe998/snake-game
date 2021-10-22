#include <LedControl.h>

const byte DOWN_PIN = 2;
const byte UP_PIN = 3;
const byte LEFT_PIN = 4;
const byte RIGHT_PIN = 5;
const byte EASY_MODE_LED = 6;
const byte HARD_MODE_LED = 7;

const byte DISPLAY_DATA_IN_PIN = 8;
const byte DISPLAY_CLOCK_PIN = 10;
const byte DISPLAY_CS_PIN = 9;

const byte VERTICAL_SIZE = 8;
const byte HORIZONTAL_SIZE = 8;
const int BOARD_SIZE = VERTICAL_SIZE * HORIZONTAL_SIZE;

const int EASY_MODE_INTERVAL_IN_MS = 700;
const int EASY_MODE_SUBTRACTOR_IN_MS = 6;
const int HARD_MODE_INTERVAL_IN_MS = 620;
const int HARD_MODE_SUBTRACTOR_IN_MS = 7;

const PROGMEM bool snakeMessage[8][69] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const PROGMEM bool lostMessage[8][67] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}
};

const PROGMEM bool winMessage[8][28] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 }
};

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
Point snake[BOARD_SIZE] = { { 3, 4 }, { 4, 4 }, { 5, 4 } };
const byte INITIAL_SNAKE_LENGTH = 3;
int snakeLength = INITIAL_SNAKE_LENGTH;
Direction direction = RIGHT;

int updateIntervalInMs = EASY_MODE_INTERVAL_IN_MS;
short intervalSubtractorInMs = EASY_MODE_SUBTRACTOR_IN_MS;
unsigned long timeElapsedInMs = 0;

bool buttonAlreadyPressedForNextRender = false;

LedControl matrix = LedControl(DISPLAY_DATA_IN_PIN, DISPLAY_CLOCK_PIN, DISPLAY_CS_PIN);

void setup() {
  Serial.begin(9600);
  setupPins();
  setupDisplay();
  setupGame();
}

void loop() {
  if (canUpdateGameState()) {
    updateGameState();
  }
  readInputsAndUpdateSnakeDirection();
}

void setupPins() {
  pinMode(DOWN_PIN, INPUT);
  pinMode(UP_PIN, INPUT);
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  pinMode(EASY_MODE_LED, OUTPUT);
  pinMode(HARD_MODE_LED, OUTPUT);
}

void setupDisplay() {
  matrix.shutdown(0, false);
  matrix.setIntensity(0, 0);
  matrix.clearDisplay(0);
}

void setupGame() {
  displaySnakeMessage();
  Serial.println("Press the down button for easy mode or up button for hard mode...");
  while (!isModeChosen());
  Serial.println("Press the right button to start...");
  while (!canBeginGame());
  generateFood();
}

void displaySnakeMessage() {
  matrix.clearDisplay(0);
  for (int k = 0; k < sizeof(snakeMessage[0]) - 7; k++) {
    for (int column = 0; column < 8; column++) {
      for (int row = 0; row < 8; row++) {
        matrix.setLed(0, row, column, pgm_read_byte(&(snakeMessage[row][column + k])));
      }
    } 
    delay(60);
  }
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

  while (!validFoodCoordinates) {
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
  return i + 1 == snakeLength;
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
    if (isSnakeTail(i)) {
      snake[i] = snake[i + 1];
    }
  }
}

bool isSnakeTail(int i) {
  return i + 1 < snakeLength;
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
  } else if (hasPlayerLost()) {
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
  delay(1000);
  Serial.println("You win! :)");
  while (true) {
    matrix.clearDisplay(0);
    for (int k = 0; k < sizeof(winMessage[0]) - 7; k++) {
      for (int column = 0; column < 8; column++) {
        for (int row = 0; row < 8; row++) {
          matrix.setLed(0, row, column, pgm_read_byte(&(winMessage[row][column + k])));
        }
      } 
      delay(50);
    }
  }
}

void displayLostMessage() {
  delay(1000);
  Serial.println("You lost... :(");
  while (true) {
    matrix.clearDisplay(0);
    for (int k = 0; k < sizeof(lostMessage[0]) - 7; k++) {
      for (int column = 0; column < 8; column++) {
        for (int row = 0; row < 8; row++) {
          matrix.setLed(0, row, column, pgm_read_byte(&(lostMessage[row][column + k])));
        }
      } 
      delay(50);
    }
  }
}

void renderState() {
  clearState();
  renderFood();
  renderSnake();
}

void clearState() {
  buttonAlreadyPressedForNextRender = false;
  matrix.clearDisplay(0);
}

void renderFood() {
  matrix.setLed(0, food.y - 1, food.x - 1, true);
}

void renderSnake() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (i < snakeLength) {
      matrix.setLed(0, snake[i].y - 1, snake[i].x - 1, true);
    }
  }
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
  return (direction == RIGHT || direction == LEFT) && !buttonAlreadyPressedForNextRender;
}

void checkVerticalButtons() {
  if (digitalRead(UP_PIN)) {
    direction = UP;
    buttonAlreadyPressedForNextRender = true;
  } else if (digitalRead(DOWN_PIN)) {
    direction = DOWN;
    buttonAlreadyPressedForNextRender = true;
  }
}

bool isSnakeMovingVertically() {
  return (direction == UP || direction == DOWN) && !buttonAlreadyPressedForNextRender;
}

void checkHorizontalButtons() {
  if (digitalRead(RIGHT_PIN)) {
    direction = RIGHT;
    buttonAlreadyPressedForNextRender = true;
  } else if (digitalRead(LEFT_PIN)) {
    direction = LEFT;
    buttonAlreadyPressedForNextRender = true;
  }
}
