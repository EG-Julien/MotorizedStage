#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DEBUG false

#define en   6
#define step 5
#define dir  4

#define bp1 9
#define bp2 8
#define bp3 7
#define bp4 10

#define pot  0

#define endS1 1
#define endS2 0

#define MAXROT 17300

int pos1 = 1000;
int pos2 = 9250;
int pos3 = 17300;

int currentpos = 0;
int direction;

int mil1 = millis();
int mil2;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

    if (DEBUG == true) {
        Serial.begin(115200);
    }

    lcd.begin();

    pinMode(step, OUTPUT);
    pinMode(dir, OUTPUT);
    pinMode(endS1, INPUT);
    pinMode(endS2, INPUT);
    pinMode(en, OUTPUT);

    pinMode(bp1, INPUT_PULLUP);
    pinMode(bp2, INPUT_PULLUP);
    pinMode(bp3, INPUT_PULLUP);
    pinMode(bp4, INPUT_PULLUP);


    digitalWrite(dir, LOW);

    lcd.clear();

    if (digitalRead(endS1) != LOW) {
        startCalibration();
    } else {
        lcdState();
    }

    lcd.setCursor(0, 1);
    lcd.print(pos1);
    lcd.setCursor(6, 1);
    lcd.print(pos2);
    lcd.setCursor(11, 1);
    lcd.print(pos3);
}

void loop() {

    if (digitalRead(bp4) == LOW) {

        //SET POS 1

        do {
            lcd.setCursor(0, 1);
            lcd.print("     ");
            pos1 = map(analogRead(pot), 0, 1023, 0, MAXROT);
            lcd.setCursor(0, 1);
            lcd.print(pos1);
            delay(100);
        } while (digitalRead(bp4) != LOW);

        delay(500);

        //SET POS 2

        do {
            lcd.setCursor(6, 1);
            lcd.print("     ");
            pos2 = map(analogRead(pot), 0, 1023, 0, MAXROT);
            lcd.setCursor(6, 1);
            lcd.print(pos2);
            delay(100);
        } while (digitalRead(bp4) != LOW);

        delay(500);

        //SET POS 3

        do {
            lcd.setCursor(11, 1);
            lcd.print("     ");
            pos3 = map(analogRead(pot), 0, 1023, 0, MAXROT);
            lcd.setCursor(11, 1);
            lcd.print(pos3);
            delay(100);
        } while (digitalRead(bp4) != LOW);

        delay(500);

    }

    getPos(bp1);
    getPos(bp2);
    getPos(bp3);

    lcdState();

    digitalWrite(en, HIGH);
}

void startCalibration() {

    lcd.clear();
    lcd.print("Calibration ...");

    digitalWrite(en, LOW);
    digitalWrite(dir, LOW);
    while (digitalRead(endS1) != LOW) {
        doStep(1);
    }

    currentpos = 0;

    lcd.clear();
    lcd.print("Position Homed !");
    delay(1000);
    lcdState();
}

void doStep(int dely) {

    digitalWrite(en, LOW);

    if ((digitalRead(endS1) == LOW && digitalRead(dir) == LOW) || (digitalRead(endS2) == LOW && digitalRead(dir) == HIGH)) {
        lcdState();
    } else {
        if (digitalRead(dir) == HIGH) {
            currentpos++;
        } else {
            currentpos--;
        }
        digitalWrite(step, HIGH);
        delay(dely);
        digitalWrite(step, LOW);
        delay(dely);
    }

}

void lcdState() {

    mil2 = millis();

    if ((mil2 - mil1) > 500) {
        mil1 = millis();
        int __pos = currentpos;
        int __dir = direction;

        lcd.setCursor(0, 0);

        if (__pos == 0 || digitalRead(endS1) == LOW) {
            lcd.print("At fwd pos 0 rot");
        } else if (__pos == MAXROT || digitalRead(endS2) == LOW) {
            lcd.print("At rev pos ");
            lcd.print(MAXROT);
            lcd.print(" r");
        } else {
            lcd.print("Dir:");
            lcd.print(__dir);
            lcd.print(" ");
            lcd.print("Pos:      ");
            lcd.setCursor(11, 0);
            lcd.print(__pos);
        }
        /*
            lcd.setCursor(0, 1);
            lcd.print(pos1);
            lcd.setCursor(6, 1);
            lcd.print(pos2);
            lcd.setCursor(12, 1);
            lcd.print(pos3);    */
    }

}

void getPos(int bp) {
  int nb = 0;
    if (digitalRead(bp) == LOW) {
        if (bp == bp1) {
            nb = pos1 - currentpos;
        } else if (bp == bp2) {
            nb = pos2 - currentpos;
        } else if (bp == bp3) {
            nb = pos3 - currentpos;
        }

        if (nb < 0) {
            digitalWrite(dir, LOW);
            direction = 0;
            nb = abs(nb);
        } else {
            digitalWrite(dir, HIGH);
            direction = 1;
        }

        for (int i = 0; i < nb; ++i) {
            if (digitalRead(bp4) == LOW) {
                delay(500);
                return;
            }
            doStep(1);
            lcdState();
        }

    }
}