# This project is still under developpement !


# Docs - Motorized Stage Version 1.1

## How to use this stage ?

 * Normal Mod
 * Follow Me Mod
 * Set-up Mod

## Global

![Template](https://github.com/EG-Julien/MotorizedStage/blob/master/Capture.PNG)

On the picture, we can see three different parts :
 * The potentiometer
 * LCD
 * Buttons

The potentiometer is used in Set-up Mod and Follow Me Mod in order to set-up the different position. Read Follow Me Mod and Set up Mod documentation for more information.

_The LCD can display some information about the current mod of the stage, current position of the cart etc ..._

Buttons are used to enter in different mods or **to go to** preset positions. The button named "S" is used to enter in **set-up mod** or while the cart is moving to go on a new position, **to stop** the traveling. Buttons 1 to 3 are used to set or go on positions 1 to 3. If the button 3 is pressed in the same time as the S's button the stage will enter in the **follow me mod**.

_**Note : All mod can be exited with a press on S button.**_

## Power Up

Controls enable :
 * ~~Potentiometer~~
 * LCD
 * ~~Buttons~~

When you power up the module by **simply** connect the power supply _(there is no switch button)_, it will **immediately** enter in calibration mod. This mod will put the cart at the **position zero**. While the cart is moving to the position 0 the LCD is displaying "Calibration ...". When the cart is at the position zero, a message is displayed by the LCD : "Position Homed !". After a second of delay you are now in the normal mod.

## Normal Mod

> This mod is used by default.

Controls enable :
 * ~~Potentiometer~~
 * LCD
 * Buttons

In this mod you have access to **three preset positions** that you can **change** through the two others mod. When you will push the Button 1 (See reference on the picture above), the cart will go on the position 1. During the travel time of the cart you have an **instant** feedback of the **current position** and direction which is refreshed every 500 ms. Moreover you **can't press any buttons** during the travel time except the button S. When this button is pressed the current travel is **immediately stopped**.

LCD Scheme :
* "At fwd pos 0 rot" => Cart is in position **0** and **can't go** farther.
* "At rev pos 17300" => Cart is in the **last** position and **can't go** farther.
* "Dir:1 Pos: 9500" => Current **direction** is from the motor to the end of the stage, and **current position** is 9500 _(about the half of the stage)_.
* _Line 2_ "1000  9250 17300" => Position 1 : 1000, Position 2 : 9250 and the third one is 17300 _(Preset position on power up)_

## Follow Me Mod

> Press button 3 and S simultaneously to enter in this mod while you're in normal mod.

Controls enable :
 * Potentiometer
 * LCD
 * Buttons

In this mod the cart will **follow** the potentiometer position in live. You have an **instant** feedback of the current position of the cart ("CR" on the LCD screen) and the potentiometer ("R" on the LCD screen). This feedback is refreshed every 250ms. When you find a good position you can **save** it in one of the **three preset position**. To do it, you just have to **press** the button of the preset position you want to change by the new value. After that the LCD will display a confirmation message with the name of the **preset position that was changed and it's new value**.

To **exit** this mod just press the S button. The LCD will freeze a second with the message that confirm you return into the normal mod.

## Set up Mod _To be improved_

> Press S button to enter in this mod while you're in normal mod.

Controls enable :
 * Potentiometer
 * LCD
 * Buttons

This mod allows you to change the **three preset position without move the cart**. Just after the entrance in this mod the position 1 displayed by the LCD will **blink** _(Too fast ...)_ then you can **change** this value by **rotate** the potentiometer. When you have found the value you wanted for this position just press S button to **change the next value**. When you're on the third value, press S button will let you **exit** this mod and enter in the normal mod (Again ...).

## Bluetooth Mod _Not implemented yet_

> Press S while pressing button 2 in order to enter in this mod.

Controls enable :
 * ~Potentiometer~
 * ~LCD~
 * ~Buttons~

This mod will enable the bluetooth module to connect the stage to a phone / computer or what ever you want. Thanks to [this](//) application you will be able to control the stage from any bluetooth devices.

## Errors 

### Position Homed freeze

> Error :

Sometimes just after the calibration, the LCD freezes while displaying the position's homed message. 

> Solution :

Just unplug and plug again the power supply.

### Follow Me Mod Blank LCD freeze

> Error :

Sometimes when you will enter in the Follow Me mod the LCD will be blank during a while. This is caused (most of the times) by the time needed for the motor to be sync. with the value of the potentiometer.

> Solution : 

Just wait the motor to equals potentiometer's value. If this change nothing turn the potentiomemter in order to stress the system.

# Dev

## Change the default preset value

To **change the preset value** you have to download the code [here](https://github.com/EG-Julien/MotorizedStage/blob/master/Motor.c).

Open it with the Arduino IDE ([Download link](https://downloads.arduino.cc/arduino-1.8.3-windows.exe))

Go to line 33 :

```c
int pos1 = 1000; // Preset position 1
int pos2 = 9250; // Preset position 2
int pos3 = 17300; // Preset position 3
```

By for example :

```c
int pos1 = 0; // Preset position 1
int pos2 = 4589; // Preset position 2
int pos3 = 12659; // Preset position 3
```

**Verify** the code and **upload** it on the Arduino NANO which is in the blue box.

## Want to create it ? _Arduino Nano Version_

### Needed pieces
* Arduino Nano
* A4988 Stepper Driver
* 1 potentiometer
* 4 Buttons
* 1 I2C LCD 2x16
* 2 Limit Switch
* A stepper Motor (NEMA 17) 
* A stage like [this](https://www.mcmaster.com/#catalog/123/1151/=18s754l)
* A gimbal
* A 5V Regulator (LM078)
* 2 Resistors (a 1k one and a 2k ohms)

_ Note : You'll need a 3D printer _

### Schematic :

![Schematic](https://github.com/EG-Julien/MotorizedStage/raw/master/Schematic.PNG)

[Download](https://github.com/EG-Julien/MotorizedStage/raw/master/Schematic.PNG) this schematic

You just have to solder the components on a PCB card and upload the code ([here](https://github.com/EG-Julien/MotorizedStage/blob/master/Motor.c))

[Download](https://github.com/EG-Julien/MotorizedStage/raw/master/PCB%20V1.1.zip) Gerber files

## Want to create it ? _ATMega328 Version_

**Coming soon !**
