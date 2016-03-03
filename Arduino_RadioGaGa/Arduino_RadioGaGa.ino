#define BUTTON 2
//#define LCD_PINS 12, 11, 10, 9, 8, 7
//#define LCD_COLUMNS 16
//#define LCD_ROWS 2

#define PULSE_THRESHOLD 200
#define LETTER_SEPARATION 500
#define WORD_SEPARATION 2000

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);

  initMorse(BUTTON, PULSE_THRESHOLD, LETTER_SEPARATION, WORD_SEPARATION);

//  initLCD(LCD_COLUMNS, LCD_ROWS, LCD_PINS);
}

void loop()
{
  // used to skip the first space
  static boolean firstLoop = true;

  char c = getNextChar();

  if (firstLoop)
  {
    firstLoop = false;
    if (c == ' ')
      return;
  }

  Serial.print(c);
}





#define DOT false
#define DASH true

#define MAX_CHAR_LENGTH 6

const struct MorseTree *tree = NULL;

int button = -1;
int pulseThreshold = 0;
int letterSeparation = 0;
int wordSeparation = 0;

void initMorse(int lbutton, int lpulseThreshold, int lletterSeparation, int lwordSeparation)
{
  tree = generateMorseTree();
  button = lbutton;
  pulseThreshold = lpulseThreshold;
  letterSeparation = lletterSeparation;
  wordSeparation = lwordSeparation;
}


struct MorseTree
{
  // '-' unless this is a leaf node
  char character;
  struct MorseTree *dotChild;
  struct MorseTree *dashChild;
};

struct MorseData
{
  char character;
  bool code[MAX_CHAR_LENGTH];
  byte codeLength;
};

void initMorseTree(struct MorseTree *tree)
{
  tree -> character = '-';
  tree -> dotChild = NULL;
  tree -> dashChild = NULL;
}

struct MorseTree *newMorseTree()
{
  struct MorseTree *tree = (struct MorseTree *) malloc(sizeof(struct MorseTree));
  initMorseTree(tree);
  return tree;
}

void addTreeMember(struct MorseTree *tree, struct MorseData &data)
{
  struct MorseTree *current = tree;
  for (byte i = 0; i < data.codeLength; i++)
  {
    boolean currentSymbol = data.code[i];
    if (currentSymbol == DOT)
    {
      if (current -> dotChild == NULL)
        current -> dotChild = newMorseTree();
      current = current -> dotChild;
    }
    else
    {
      if (current -> dashChild == NULL)
        current -> dashChild = newMorseTree();
      current = current -> dashChild;
    }
  }

  // now current must be a leaf node
  current -> character = data.character;
}

void addTreeMembers(struct MorseTree *tree, struct MorseData data[], byte dataLength)
{
  for (byte i = 0; i < dataLength; i++)
    addTreeMember(tree, data[i]);
}

void addAlphabet(struct MorseTree *tree)
{
  struct MorseData data[] = {
    {'A', {DOT, DASH}, 2},
    {'B', {DASH, DOT, DOT, DOT}, 4},
    {'C', {DASH, DOT, DASH, DOT}, 4},
    {'D', {DASH, DOT, DOT}, 3},
    {'E', {DOT}, 1},
    {'F', {DOT, DOT, DASH, DOT}, 4},
    {'G', {DASH, DASH, DOT}, 3},
    {'H', {DOT, DOT, DOT, DOT}, 4},
    {'I', {DOT, DOT}, 2},
    {'J', {DOT, DASH, DASH, DASH}, 4},
    {'K', {DASH, DOT, DASH}, 3},
    {'L', {DOT, DASH, DOT, DOT}, 4},
    {'M', {DASH, DASH}, 2},
    {'N', {DASH, DOT}, 2},
    {'O', {DASH, DASH, DASH}, 3},
    {'P', {DOT, DASH, DASH, DOT}, 4},
    {'Q', {DASH, DASH, DOT, DASH}, 4},
    {'R', {DOT, DASH, DOT}, 3},
    {'S', {DOT, DOT, DOT}, 3},
    {'T', {DASH}, 1},
    {'U', {DOT, DOT, DASH}, 3},
    {'V', {DOT, DOT, DOT, DASH}, 4},
    {'W', {DOT, DASH, DASH}, 3},
    {'X', {DASH, DOT, DOT, DASH}, 4},
    {'Y', {DASH, DOT, DASH, DASH}, 4},
    {'Z', {DASH, DASH, DOT, DOT}, 4},
  };

  addTreeMembers(tree, data, 26);
}

void addNumbers(struct MorseTree *tree)
{
  struct MorseData data[] = {
    {'1', {DOT, DASH, DASH, DASH, DASH}, 5},
    {'2', {DOT, DOT, DASH, DASH, DASH}, 5},
    {'3', {DOT, DOT, DOT, DASH, DASH}, 5},
    {'4', {DOT, DOT, DOT, DOT, DASH}, 5},
    {'5', {DOT, DOT, DOT, DOT, DOT}, 5},
    {'6', {DASH, DOT, DOT, DOT, DOT}, 5},
    {'7', {DASH, DASH, DOT, DOT, DOT}, 5},
    {'8', {DASH, DASH, DASH, DOT, DOT}, 5},
    {'9', {DASH, DASH, DASH, DASH, DOT}, 5},
    {'0', {DASH, DASH, DASH, DASH, DASH}, 5},
  };

  addTreeMembers(tree, data, 10);
}

void addPunctuation(struct MorseTree *tree)
{
  struct MorseData data[] = {
    {'.', {DOT, DASH, DOT, DASH, DOT, DASH}, 6},
    {',', {DASH, DASH, DOT, DOT, DASH, DASH}, 6},
    {'?', {DOT, DOT, DASH, DASH, DOT, DOT}, 6},
  };

  addTreeMembers(tree, data, 3);
}

struct MorseTree *generateMorseTree()
{
  // since this will live for the life of the program anyway, we won't even bother to have a strategy for freeing it.
  struct MorseTree *tree = newMorseTree();
  initMorseTree(tree);

  addAlphabet(tree);
  addNumbers(tree);
  addPunctuation(tree);

  return tree;
}

// ---------------------------------------
// Arduino related stuff:
// ---------------------------------------

// waits for the given pin to go to the given state. To reduce noise,
// the pin must read the required value twice, with a short delay
// between the readings, to count as a reading.
void waitFor(int pin, int state)
{
  do
  {
    // spin until the pin reads the given state
    while (digitalRead(pin) != state) { }
    // delay, to verify the reading
    delay(5);
    // continue if the reading has changed
  } while (digitalRead(pin) != state);
}

boolean getNextSymbol()
{
  waitFor(button, HIGH);
  unsigned long start = millis();

  waitFor(button, LOW);
  unsigned long end = millis();

  unsigned long pulseLength = end - start;

  // Serial.println(pulseLength);
  if (pulseLength > pulseThreshold)
    return DASH;
  else
    return DOT;
}

boolean shouldTimeOut()
{
  unsigned long start = millis();
  while (digitalRead(BUTTON) == LOW)
  {
    if (millis() - start > letterSeparation)
      return true;
  }

  return false;
}

boolean shouldBeSpace()
{
  unsigned long start = millis();
  while (digitalRead(BUTTON) == LOW)
  {
    if (millis() - start > wordSeparation)
      return true;
  }

  return false;
}

char getNextChar()
{
  static boolean lastCharWasSpace = false;

  const struct MorseTree *current = tree;

  byte symbolCount = 0;

  if (!lastCharWasSpace && shouldBeSpace())
  {
    lastCharWasSpace = true;
    return ' ';
  }

  lastCharWasSpace = false;

  while (true)
  {
    symbolCount++;
    boolean currentSymbol = getNextSymbol();
//    Serial.println(currentSymbol == DOT ? "DOT" : "DASH");
    if (currentSymbol == DOT)
      current = current -> dotChild;
    else
      current = current -> dashChild;

    if (current == NULL)
      return '-';

    if (shouldTimeOut() || symbolCount >= MAX_CHAR_LENGTH)
      return current -> character;
  }

  // should never get here
  return '!';
}
