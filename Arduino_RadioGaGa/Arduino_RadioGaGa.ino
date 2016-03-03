const byte inPin = 7;

//letters (0-25), digits (26-36)
const byte MORSECODE [][5] = {{1, 2, 0, 0, 0}, {2, 1, 1, 1, 0}, {2, 1, 2, 1, 0}, {2, 1, 1, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 2, 1, 0}
  , {2, 2, 1, 0, 0}, {1, 1, 1, 1, 0}, {1, 1, 0, 0, 0} , {1, 2, 2, 2, 0} , {2, 1, 2, 0, 0}, {1, 2, 1, 1, 0}
  , {2, 2, 0, 0, 0}, {2, 1, 0, 0, 0}, {2, 2, 2, 0, 0}, {1, 2, 2, 1, 0}, {2, 2, 1, 2, 0}, {1, 2, 1, 0, 0}
  , {1, 1, 1, 0, 0}, {2, 0, 0, 0, 0}, {1, 1, 2, 0, 0}, {1, 1, 1, 2, 0}, {1, 2, 2, 0, 0}, {2, 1, 1, 2, 0}
  , {2, 1, 2, 2, 0}, {2, 2, 1, 1, 0}
  , {1, 2, 2, 2, 2}, {1, 1, 2, 2, 2}, {1, 1, 1, 2, 2}, {1, 1, 1, 1, 2}, {1, 1, 1, 1, 1}
  , {2, 1, 1, 1, 1}, {2, 2, 1, 1, 1}, {2, 2, 2, 1, 1}, {2, 2, 2, 2, 1}, {2, 2, 2, 2, 2}
};

const char LETTERS [] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
byte button = 0;
unsigned long startTime = 0;
unsigned long stopTime = 0;
unsigned long time = 0;

byte beepArray [] = {0, 0, 0, 0, 0};
byte subIndex = 0;
byte letterIndex = 0;
char letter = '0';
String str = "";

void setup() {
  Serial.begin(9600);

  pinMode(inPin, INPUT);
}

byte letterSearch (byte array []) {
  boolean match = 0;
  for (byte index = 0; index < 36; index++ ) {
    for (byte i = 0; i < 5; i++) {
      match = 0;
      if (array[i] != MORSECODE[index][i]) {
        break;
      }
      match = 1;
    }
    if (match) return index;
  }
  return 255;
}

void clearArray(byte array []) {
  for (byte i = 0; i < 5; i++) {
    array[i] = 0;
  }
}

void printArray(byte array []) {
  for (byte i = 0; i < 5; i++) {
    Serial.print(array[i]);
  }
  Serial.println("");
}


void loop() {

  startTime = millis();
  while (button == HIGH) {
    button = digitalRead(inPin);
  }
  stopTime = millis();
  time = stopTime - startTime;

  if (time > 20 && time < 300) {
    beepArray[subIndex] = 1;
    subIndex ++;
  }
  else if (time > 300 && time < 800) {
    beepArray[subIndex] = 2;
    subIndex ++;
  }

  startTime = millis();
  while (button == LOW) {
    button = digitalRead(inPin);
  }
  stopTime = millis();
  time = stopTime - startTime;

  if (time > 800) {
    letterIndex = letterSearch(beepArray);
    if (letterIndex != 255) {
      letter = LETTERS[letterIndex];
      str += letter;
      Serial.print(letter);
    }
    clearArray(beepArray);
    subIndex = 0;
  }
  if (time > 2000) {
    str += " ";
    Serial.print(" ");
  }
}

