#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int switchPin = 6;
const int rockStart = 13;
//input info
int state = 0;
int prevState = 0;
int score = 0;
bool inMenu = true;
bool gameOn = false;
bool lost = false;
bool clearScreen = false;
bool is_hit = false;

unsigned long time;

class Entity {
  public:
    Entity(int c, int r) {
      col = c;
      row = r;
    };
    int getCol() {
      return col;
    };
    int getRow() {
      return row;
    };
    void setCol(int c) {
      col = c;
    };
    void setRow(int r) {
      row = r;
    };
  private:
    int col;
    int row;
};

Entity player(1, 0);
Entity rock(rockStart, 0);
Entity lives(15, 1);

int numLives = 3;

void printPlayer() {
  int col = player.getCol();
  int row = player.getRow();
  lcd.setCursor(col, row);
  lcd.print(">");
}
void printRock() {
  int col = rock.getCol();
  int row = rock.getRow();
  lcd.setCursor(col, row);
  lcd.print("O");
}
void printLives() {
  lcd.setCursor(14, 0);
  lcd.print("<3");
  lcd.setCursor(14, 1);
  lcd.print("x");
  lcd.setCursor(15, 1);
  lcd.print(numLives);
}

void printEntity() {
  lcd.clear();
  if (numLives > 0) {
    printPlayer();
    printRock();
    printLives();
  }
  else {
    lcd.print("u lost Score: ");
    lcd.print(score - 2);
    lcd.setCursor(0, 1);
    lcd.print("push 2 restart");
  }
}

void movePlayer() {
  int row = player.getRow();

  if (row == 1)
    player.setRow(0);
  else if (row == 0)
    player.setRow(1);
}

void moveRock() {
  int col = rock.getCol();
  col--;
  rock.setCol(col);
  if (col < 0) {
    rock.setCol(rockStart);
    rock.setRow(random(2));
    if(is_hit == false && numLives > 0)
      score++;
  }
}

void setup() {
  lcd.begin(16, 2);
  pinMode(switchPin, INPUT);

  lcd.print("Play Game ? ");
  lcd.setCursor(0, 1);
  lcd.print("Push button!");
}

void loop() {
  state = digitalRead(switchPin);

  time = millis();

  if (state == HIGH && prevState == LOW && lost == false) {
    if (inMenu) {
      inMenu = false;
      gameOn = true;
    }
    if (gameOn) {
      movePlayer();
    }
    if (numLives < 0) {
      numLives = 3;
      score = 0;
    }
  }

  if (time % 80 == 0 && player.getRow() == rock.getRow() && player.getCol() == rock.getCol()) {
    numLives--;
    is_hit = true;
  }

  if (time % 80 == 0 && gameOn && lost == false) {
    printEntity();
    moveRock();
  }
  
  is_hit = false;
  prevState = state;
}


