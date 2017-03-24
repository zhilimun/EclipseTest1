/*
 * We test the git function
 */
#include <SPI.h>
#include <AccelStepper.h>
#include <stdio.h>
#include <avr/iom2560.h>

const int Proximity_Head = 23;
const int Proximity_Tail = 22;
const int Proximity_Top = 26;
const int Proximity_Bottom = 27;

AccelStepper stepper_blade_L(1, 4, 24);    // direction Digital 2 (CW), pulses Digital 3 (CLK)
AccelStepper stepper_Z_L (1, 5, 25);


void setup()
{
	// This part is for setting up the prescalor for the two PWM pins
    //prescaler = 1 ---> PWM frequency is 31000 Hz                      timer 0 (controls pin 13, 4);
    //prescaler = 2 ---> PWM frequency is 4000 Hz
    //prescaler = 3 ---> PWM frequency is 490 Hz (default value)        timer 1 (controls pin 12, 11);
    //prescaler = 4 ---> PWM frequency is 120 Hz                        timer 2 (controls pin 10, 9);
    //prescaler = 5 ---> PWM frequency is 30 Hz                         timer 3 (controls pin 5, 3, 2);
    //prescaler = 6 ---> PWM frequency is <20 Hz                        timer 4 (controls pin 8, 7, 6);

	// Timer 0, 62K PWM frequency -- PIN 4 for the vertical motor
    int myEraser = 7;             // this is 111 in binary and is used as an eraser
    TCCR0B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
    int myPrescaler = 1;         // this could be a number in [1 , 6]. In this case, 3 corresponds in binary to 011.
    TCCR0B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011

    // Timer 3, 31K PWM frequency -- PIN 5 for the vertical motor
    TCCR3B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
    TCCR3B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011

    pinMode(Proximity_Head, INPUT);
    pinMode(Proximity_Tail, INPUT);
    pinMode(Proximity_Top, INPUT);
    pinMode(Proximity_Bottom, INPUT);

    // Serial communication to Laptop
    Serial.begin(115200);

    // Best Maxspeed 700 with Acceleration 1000;
    // Blade motor
    stepper_blade_L.setMaxSpeed(1000);
    stepper_blade_L.setAcceleration(200);
    stepper_blade_L.setCurrentPosition(0);


    // Horizontal motor
    stepper_Z_L.setMaxSpeed(1000);
    stepper_Z_L.setAcceleration(200);
    stepper_Z_L.setCurrentPosition(0);

}

//declare reset function at address 0
void(* resetFunc) (void) = 0;

void loop() {


	//if (stepper_blade_L.distanceToGo() == 0)
	//	stepper_blade_L.moveTo(-stepper_blade_L.currentPosition());
	//stepper_blade_L.run();

	//if (stepper_Z_L.distanceToGo() == 0)
	//	stepper_Z_L.moveTo(-stepper_Z_L.currentPosition());
	//stepper_Z_L.run();
    //stepper_blade_L.move(100);
	if(digitalRead(Proximity_Head)!=HIGH)
	{
	    //stepper_Z_L.setSpeed(-100);
	    stepper_Z_L.moveTo(-90000);
	    stepper_Z_L.enableOutputs();

	    stepper_blade_L.moveTo(-90000);
	    stepper_blade_L.enableOutputs();

		stepper_blade_L.run();
		//stepper_Z_L.move(100);
		stepper_Z_L.run();

		//stepper_Z_L.runSpeed();
		Serial.print("The position of the vertical and horizontal motor: ");
		Serial.print(stepper_blade_L.currentPosition());
		Serial.print(" ");
		Serial.print(stepper_Z_L.currentPosition());
		Serial.print("\n\r");
		delay(20);
	}
	else
	{
		stepper_blade_L.stop();
		//stepper_Z_L.move(100);
		stepper_Z_L.stop();

		//stepper_Z_L.runSpeed();
		Serial.print("Motor Stopped!");
		Serial.print("\n\r");
		delay(20);
	}

	stepper_Z_L.stop();
	stepper_blade_L.stop();
	 //stepper_blade_L.runSpeed();
	 //Serial.print(digitalRead(Proximity_Head));
	 //delay(100);
}
