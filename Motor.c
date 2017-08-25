/*

    Writed by Julien GENESTE - Under MIT License
                    08/24/2k17


    Documetation Arduino : https://www.arduino.cc/en/Reference/HomePage
    Controller Documetation : https://github.com/EG-Julien/MotorizedStage/wiki

 */


#include <Wire.h> // Needed to control I2C Lcd
#include <LiquidCrystal_I2C.h> // Needed to control I2C Lcd

#define DEBUG false

#define en   6 // Enable pin for A4988
#define step 5 // Step pin for A4988
#define dir  4 // Direction pin for A4988

#define bp1 9 // Button Position 1
#define bp2 8 // Button position 2
#define bp3 7 // Button position 3
#define bp4 10 // Setup button

#define pot  0 // /!\ ANALOG PIN A0 /!\ - Potentiometer

#define endS1 1 // End swith 1 (closest from the motor)
#define endS2 0 // End swith 2

#define MAXROT 17300 // Maximal number of steps (Software limit)

int pos1 = 1000; // Preset position 1
int pos2 = 9250; // Preset position 2
int pos3 = 17300; // Preset position 3

int currentpos = 0; // Current position of the cart
int direction; // Current set direction for the motor

int mil1 = millis(); // Interuption system
int mil2;
int mil3 = millis();
int mil4;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialisation of the lcds

void setup() { // Runing once

    if (DEBUG == true) {
        Serial.begin(115200); // If in debugging, start serial communication software
    }

    lcd.begin(); // Strating of the lcd

    pinMode(step, OUTPUT); // These pins are INPUT or OUTPUT ?s
    pinMode(dir, OUTPUT);
    pinMode(endS1, INPUT);
    pinMode(endS2, INPUT);
    pinMode(en, OUTPUT);

    pinMode(bp1, INPUT_PULLUP); // INPUT_PULLUP => Input with DEFAULT HIGH logical state (5V)
    pinMode(bp2, INPUT_PULLUP);
    pinMode(bp3, INPUT_PULLUP);
    pinMode(bp4, INPUT_PULLUP);


    digitalWrite(dir, LOW); // Set the dir pin to 0Vs

    lcd.clear(); // Erase all content displayed by the LCD

    if (digitalRead(endS1) != LOW) { // If the cart is not at position 0
        startCalibration(); // We start the calibrations
    } else {
        lcdState(); // Else, we display information about current position of the cart and direction
    }

    presetPos(); // We display preset position
}

void loop() { // This code is runing in a loop

    // Follow me ! function
    // Button 1 and 4 pressed at the same time

    if ((digitalRead(bp4) == LOW) && (digitalRead(bp3) == LOW)) {
        followMe(); // Execute Follow me function
    }

    if (digitalRead(bp4) == LOW && digitalRead(bp3) == HIGH) { // If the setup button has been pressed

        //First we change the fisrt preset position

        do {
            lcd.setCursor(0, 1); // We place the cursor at the right position (Row 0, line 1)
            lcd.print("     "); // Erasing last position
            pos1 = map(analogRead(pot), 0, 1023, 0, MAXROT); // Getting potentiometer value to set the position, but this value is between 0 and 1023 (10 Bits ADC) and we need a value between 0 and MAXROT, so we're using map function
            lcd.setCursor(0, 1);
            lcd.print(pos1); // We print the position on the screen
            delay(100);
        } while (digitalRead(bp4) != LOW); // And we do this until the setup's button is pressed once again

        delay(500); // We wait 500 ms before the next loop

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

    getPos(bp1); // Check if the first position's button has been presseds
    getPos(bp2); // Some for the second potion's button
    getPos(bp3); // And finaly for the third one

    lcdState(); // Display the current information

    digitalWrite(en, HIGH); // Put the enable pin to HIGH logical state AKA 5V
}

void presetPos() { // Function to display preset position
    lcd.setCursor(0, 1); // We display preset position 1
    lcd.print(pos1);
    lcd.setCursor(6, 1); // 2
    lcd.print(pos2);
    lcd.setCursor(11, 1); // And 3
    lcd.print(pos3);
}

void startCalibration() { // Function to calibrate the system

    lcd.clear();
    lcd.print("Calibration ...");

    digitalWrite(en, LOW); // Activation of the A4988
    digitalWrite(dir, LOW); // Set the good direction
    while (digitalRead(endS1) != LOW) {
        doStep(1); // And we turn on the motor until the end switch which is the near the motor is activated
    }

    currentpos = 0; // Now we have set the position 0

    lcd.clear();
    lcd.print("Position Homed !");
    delay(1000); // We wait a second
    lcdState();
}

void doStep(int dely) { // Function to make the motor run forward / backward (1 step = 1/200 rot)

    digitalWrite(en, LOW); // Make sure the A4988 is not sleeping

    if ((digitalRead(endS1) == LOW && digitalRead(dir) == LOW) || (digitalRead(endS2) == LOW && digitalRead(dir) == HIGH)) { // Check if the stage can do the step without hurting the end of the cariage
        lcdState();
    } else {
        if (digitalRead(dir) == HIGH) { // We increse / decrease the current pos in fuction of direction status
            currentpos++; // Equals to currentpos = currentpos + 1;
        } else {
            currentpos--;
        }

        // Generation of the square wave which controls the A4988

        digitalWrite(step, HIGH);
        delay(dely); // Wait for the delay given in parameter, (Higher delay = slower speed)
        digitalWrite(step, LOW);
        delay(dely); // Wait for the delay given in parameter, (Higher delay = slower speed)
    }

}

void doStepWL(int dely) { // Function to make the motor run forward / backward (1 step = 1/200 rot)

    digitalWrite(en, LOW); // Make sure the A4988 is not sleeping

    if ((digitalRead(endS1) == LOW && digitalRead(dir) == LOW) || (digitalRead(endS2) == LOW && digitalRead(dir) == HIGH)) { // Check if the stage can do the step without hurting the end of the cariage
    } else {
        if (digitalRead(dir) == HIGH) { // We increse / decrease the current pos in fuction of direction status
            currentpos++; // Equals to currentpos = currentpos + 1;
        } else {
            currentpos--;
        }

        // Generation of the square wave which controls the A4988

        digitalWrite(step, HIGH);
        delay(dely); // Wait for the delay given in parameter, (Higher delay = slower speed)
        digitalWrite(step, LOW);
        delay(dely); // Wait for the delay given in parameter, (Higher delay = slower speed)
    }

}

void lcdState() { // This function sends informations to the LCD

    mil2 = millis(); // Interuption system

    if ((mil2 - mil1) > 500) { // If the last refresh is older then 500 ms we refresh the LCD's informations else we do noting
        mil1 = millis(); // Reset he interuption system
        int __pos = currentpos; // Internal variable
        int __dir = direction; // Internal variable

        lcd.setCursor(0, 0);

        if (__pos == 0 || digitalRead(endS1) == LOW) { // If the cart is at the 0 pos or it's touching the switch which is close to the motor we displays these informations on line one of the lcd
            lcd.print("At fwd pos 0 rot");
        } else if (__pos == MAXROT || digitalRead(endS2) == LOW) { // Same as before but for the second switch AKA the opposit side
            lcd.print("At rev pos ");
            lcd.print(MAXROT);
            lcd.print(" r");
        } else { // If the cart is able to moove, we send direction and position informations
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

void getPos(int bp) { // This funciton allows us to know which button was pressed :1, 2 or 3
  int nb = 0;
    if (digitalRead(bp) == LOW) { // Calculates positions
        if (bp == bp1) {
            nb = pos1 - currentpos;
        } else if (bp == bp2) {
            nb = pos2 - currentpos;
        } else if (bp == bp3) {
            nb = pos3 - currentpos;
        }

        if (nb < 0) { // If the position is before the cart we reverse the direction of the motor
            digitalWrite(dir, LOW);
            direction = 0;
            nb = abs(nb);
        } else { // Otherwise we kept the current direction (We set it again to be sure !)
            digitalWrite(dir, HIGH);
            direction = 1;
        }

        for (int i = 0; i < nb; ++i) { // We run into a loop untill the end of the step's sequence
            if (digitalRead(bp4) == LOW) { // If the button 4 is pressed we immediately stop the motor and exit the loop
                delay(500);
                return;
            }
            doStep(1);
            lcdState();
        }

    }
}

void followMe() {
    lcd.setCursor(0, 0); // Some text to prevent the user
    lcd.print("  Follow me  !  ");
    lcd.setCursor(0, 1);
    lcd.print("                ");

    delay(500);

    int i = 0;
    do {
        int rot = map(analogRead(pot), 0, 1023, 0, MAXROT); // Getting the position of the potentiometter
        int nb = rot - currentpos; // Calculating of the number of rotation to do

        if (nb < 0) { // Calculating direction to choose
            digitalWrite(dir, LOW);
            direction = 0;
        } else {
            digitalWrite(dir, HIGH);
            direction = 1;
        }

        mil4 = millis();

        if (mil4 - mil3 > 250) { // We refresh the screen every 250 ms
            lcd.setCursor(0, 1);
            lcd.print("                ");
            lcd.setCursor(0, 1);
            lcd.print("CP:"); // Displaying current position
            lcd.print(currentpos);
            lcd.setCursor(9, 1);
            lcd.print("R:"); // Displaying position to reach
            lcd.print(rot);
            mil3 = millis();
        }


        if (currentpos != rot) { // If the current position is different than the position to reach we turn the motor until those position are equals
            doStepWL(1); // Same function than doStep but without the LCD return
        } else {
            digitalWrite(en, HIGH); // Disable A4988
            if (digitalRead(bp1) == LOW) { // If Bp 1 is pressed
                pos1 = currentpos; // Setting the current position as position 1
                lcd.clear();
                lcd.print("Pos 1 set:"); // Print the new position 1 on the screen
                lcd.print(pos1);
                delay(800);
            }

            if (digitalRead(bp2) == LOW) { // If bp 2 is pressed
                pos2 = currentpos; // Setting current position as position 2
                lcd.clear();
                lcd.print("Pos 2 set:"); // Print the new position 2 on the screen
                lcd.print(pos2);
                delay(800);
            }

            if (digitalRead(bp3) == LOW) { // If bp 3 is pressed
                pos3 = currentpos; // Setting current position as position 3
                lcd.clear();
                lcd.print("Pos 3 set:"); // Print the new position 3 on the screen
                lcd.print(pos3);
                delay(800);
            }
        }

    } while (digitalRead(bp4) != LOW); // We repeat this until the button 4 is pressed

    lcd.clear();
    lcd.print("   Normal Mod   "); // Then the screen is cleared and tell the user he switchs into the normal mod
    delay(500);
    lcd.clear();
    mil1 = millis() + 1000;
    lcdState(); // Display the current state of the cart
    presetPos(); // Print preset position

    digitalWrite(en, HIGH); // Disable A4988
}