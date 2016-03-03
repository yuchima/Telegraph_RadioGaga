/**
 * morse-code.ino
 *
 * Author: Ryan Jacobs <ryan.mjacobs@gmail.com>
 * Author: Kyle Domen <kdomen99@gmail.com>
 */

// how many milliseconds to delay after each letter
#define SPEED 300

const int led = 4;
enum { SHORT, LONG, OFF };

const byte morse_dict[26][4] = {
    { SHORT, LONG,  OFF,   OFF   }, // a
    { LONG,  SHORT, SHORT, SHORT }, // b
    { LONG,  SHORT, LONG,  SHORT }, // c
    { LONG,  SHORT, SHORT, OFF   }, // d
    { SHORT, OFF,   OFF,   OFF   }, // e
    { SHORT, SHORT, LONG,  SHORT }, // f
    { LONG,  LONG,  SHORT, OFF   }, // g
    { SHORT, SHORT, SHORT, SHORT }, // h
    { SHORT, SHORT, OFF,   OFF   }, // i
    { SHORT, LONG,  LONG,  LONG  }, // j
    { LONG,  SHORT, LONG,  OFF   }, // k
    { SHORT, LONG,  SHORT, SHORT }, // l
    { LONG,  LONG,  OFF,   OFF   }, // m
    { LONG,  SHORT, OFF,   OFF   }, // n
    { LONG,  LONG,  LONG,  OFF   }, // o
    { SHORT, LONG,  LONG,  SHORT }, // p
    { LONG,  LONG,  SHORT, LONG  }, // q
    { SHORT, LONG,  SHORT, OFF   }, // r
    { SHORT, SHORT, SHORT, OFF   }, // s
    { LONG,  OFF,   OFF,   OFF   }, // t
    { SHORT, SHORT, LONG,  OFF   }, // u
    { SHORT, SHORT, SHORT, LONG  }, // v
    { SHORT, LONG,  LONG,  OFF   }, // w
    { LONG,  SHORT, SHORT, LONG  }, // x
    { LONG,  SHORT, LONG,  LONG  }, // y
    { LONG,  LONG,  SHORT, SHORT }, // z 
};

void blink_letter(const char letter) {
    for (byte i = 0; i < 4; i++) {
        switch (morse_dict[letter-97][i]) {
            case SHORT:
                digitalWrite(led, HIGH); delay(500);
                digitalWrite(led, LOW);  delay(500 + SPEED);
                break;
            case LONG:
                digitalWrite(led, HIGH); delay(750);
                digitalWrite(led, LOW);  delay(750 + SPEED);
                break;
            case OFF:
                digitalWrite(led, LOW);
                break;
        }
    }
}

void blink_string(const char *str) {
    unsigned int i = 0;

    while (str[i++] != '\0') {
        Serial.print(str[i]);
        blink_letter(str[i]);
    }
    Serial.println();
}

void setup(void) {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop(void) {
    blink_string(" HELLO");
//    delay(2000);
}
