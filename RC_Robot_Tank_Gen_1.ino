
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h" //stuff to run the motors
#include <PS2X_lib.h>  //backend stuff to detect the controller, can be downloaded fram this site: http://www.billporter.info/2010/06/05/playstation-2-controller-arduino-library-v1-0/

PS2X ps2x; // create PS2 Controller Class

int error = 0; //defining different variables for use later
byte type = 0;
byte vibrate = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //defining motor shield
Adafruit_DCMotor *myMotor = AFMS.getMotor(1); //defining motors
Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

int turn = 80;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  error = ps2x.config_gamepad(13, 11, 10, 12, true, true); //setup pins and settings:  GamePad(clock (blue), command (orange), attention (yellow), data (pink)) check for error
  AFMS.begin(); //telling the motor shield to start
  Serial.println("motor shield activated");

  if (error == 0) { //the serial monitor output if the gamepad was found
    Serial.println("Found Controller, configured successful");
  }

  else if (error == 1) //serial monitor output if the controller isn't found, which would set the error variable to '1'
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2) //serial monitor output if the controller is not working properly, which would set the error variable to '2'
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3) //serial monitor output if the controller can't detect pressure, error variable '3'
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType(); //naming different types
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
  //error = 0;
  //type = 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (error == 1) //skip loop if no controller found
    return;

  else { //DualShock Controller

    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed


    if (ps2x.Button(PSB_PAD_UP)) {        //detect when one of the D-Pad buttons are pressed and run the motors accordingly
      Serial.print("UP held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      myMotor->run(FORWARD);
      myOtherMotor->run(FORWARD);
      myMotor->setSpeed(turn * 1.1);
      myOtherMotor->setSpeed(turn);
    }

    if (ps2x.Button(PSB_PAD_RIGHT)) {
      Serial.print("RIGHT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      myMotor->run(FORWARD);
      myOtherMotor->run(BACKWARD);
      myMotor->setSpeed(turn / 2);
      myOtherMotor->setSpeed(turn / 2);
    }

    if (ps2x.Button(PSB_PAD_LEFT)) {
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      myMotor->run(BACKWARD);
      myOtherMotor->run(FORWARD);
      myMotor->setSpeed(turn / 2);
      myOtherMotor->setSpeed(turn / 2);
    }

    if (ps2x.Button(PSB_PAD_DOWN)) {

      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      myMotor->run(BACKWARD);
      myOtherMotor->run(BACKWARD);
      myMotor->setSpeed(turn * 1.1);
      myOtherMotor->setSpeed(turn);
    }





    if (ps2x.ButtonReleased(PSB_PAD_UP)) {           //code to turn the wheels off once the button is released
      Serial.println("UP just released");
      myMotor->run(RELEASE);
      myOtherMotor->run(RELEASE);
    }

    if (ps2x.ButtonReleased(PSB_PAD_RIGHT)) {
      Serial.println("RIGHT just released");
      myMotor->run(RELEASE);
      myOtherMotor->run(RELEASE);
    }

    if (ps2x.ButtonReleased(PSB_PAD_LEFT)) {
      Serial.println("LEFT just released");
      myMotor->run(RELEASE);
      myOtherMotor->run(RELEASE);
    }

    if (ps2x.ButtonReleased(PSB_PAD_DOWN)) {
      Serial.println("DOWN just released");
      myMotor->run(RELEASE);
      myOtherMotor->run(RELEASE);
    }




    vibrate = ps2x.Analog(PSAB_R1);        //this will set the large motor vibrate speed based on
    //how hard you press R1

  }
  delay(50);

}
