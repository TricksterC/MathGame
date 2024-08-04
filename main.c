#include <LiquidCrystal.h>
#include <Keypad.h>

byte smile[8] =

{

  0b00000,

  0b00000,

  0b01010,

  0b00000,

  0b10001,

  0b01110,

  0b00000,

  0b00000

};
byte sad[8] =

{

  0b00000,

  0b01010,

  0b00000,

  0b00100,

  0b00000,

  0b01110,

  0b10001,

  0b00000

};
char  incomingData = ' ';
char symbolsArr[] = {'+', '-', '*', '/'};
int num1, num2, calcResult, symbolsArrIndex, temp, inputAnswer, resultCursorPos = 8;
int buzzer = 44;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 52, d5 = 50, d6 = 48, d7 = 46;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//keypad setup
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char Keys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {36, 34, 32, 30}; //{R1, R2, R3, R4}
byte colPins[COLS] = {44, 42, 40, 38};//{C1, C2, C3, C4}


Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);

void buzzForCorrectAnswer() {
  tone (buzzer, 4000);
  delay(500);
  noTone(buzzer);
  delay(200);
}

void buzzForWrongAnswer() {
  tone (buzzer, 250);
  delay(100);
  noTone(buzzer);
  delay(150);
  tone (buzzer, 250);
  delay(100);
  noTone(buzzer);
  delay(150);
}

void checkAnswer() {
  lcd.setCursor(0, 1);
  if (inputAnswer == calcResult) {
    lcd.print("Correct!");
    lcd.setCursor(9, 10);
    lcd.write(1);
    //buzzForCorrectAnswer();
  }
  else {
    lcd.print("Wrong!");
    lcd.setCursor(7 , 10);
    lcd.write(2);
    //buzzForWrongAnswer();
  }
}

void randomQuestionInit() {
  randomSeed(analogRead(0));
  lcd.begin(16, 2);
  // Print a message to the LCD.
  symbolsArrIndex = random(0, 4);

  inputAnswer = 0;
  resultCursorPos = 8;
  //Prints random in serial
  num1 = random(1, 10);
  lcd.setCursor(0, 0);
  num2 = random(1, 10);

  //make sure num1 is always bigger to avoid negative numbers
  if (num2 > num1) {
    temp = num1;
    num1 = num2;
    num2 = temp;
  }


  lcd.print(num1);
  lcd.print(" ");
  lcd.print(symbolsArr[symbolsArrIndex]);
  lcd.print(" ");
  lcd.print(num2);
  lcd.print(" = ");
  Serial.println(num1);
  Serial.println(symbolsArrIndex);
  Serial.println(symbolsArr[symbolsArrIndex]);
  Serial.println(num2);

  switch (symbolsArr[symbolsArrIndex]) {
    case '+': calcResult = num1 + num2;
      break;
    case '-': calcResult = num1 - num2;
      break;
    case '*': calcResult = num1 * num2;
      break;
    case '/': calcResult = num1 / num2;
      break;
  }
  Serial.println(calcResult);
}


void setup()
{
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  analogWrite(10, 100);  // set up the LCD's number of columns and rows:
  randomQuestionInit();
  lcd.createChar(1, smile);
  lcd.createChar(2, sad);
}

void loop() {

  char customKeyInChar = customKeypad.getKey();
  int customKey = int(customKeyInChar);
  if (customKey) {
    Serial.println(customKey);
    if (customKey > 48 && customKey < 58) {
      int inputDigit = customKey - 48;
      Serial.println(inputDigit);
      lcd.setCursor(resultCursorPos, 0);
      lcd.print(inputDigit);
      resultCursorPos++;
      inputAnswer = inputAnswer * 10 + inputDigit;
    } else if (customKey == 35) {
      randomQuestionInit();
    } else if (customKey == 65) {
      Serial.println(inputAnswer);
      checkAnswer();
    }
  }

}
