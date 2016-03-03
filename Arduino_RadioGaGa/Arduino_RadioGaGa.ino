
int key = 7;  // Pin 4 and using it as digital
int speaker = 4; // Speaker will be hooked between pin 11 and ground

int val = 0;       // A value for key up and down
int myTone = 4000;  // Freq. of our tone

void setup() {
  pinMode(key, INPUT);
  pinMode(speaker, OUTPUT);
}

void loop() {
  val = digitalRead(key);        // Read the state of the key
  if (val) {
    // If it is down play the tone
    tone(speaker, myTone);
  }

  if (!val) {
    // If it is not down stop the tone
    noTone(speaker);
  }
}
