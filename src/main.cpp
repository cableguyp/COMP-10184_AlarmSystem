// COMP-10184 – Mohawk College 
// Alarm System
//  
// This program will check for any motion using a motion sensor. If there is motion
// then a 10 second timer starts counting down while an LED blinks. If the user clicks the 
// button within the 10 second then the alarm is disabled. If they do not click it
// then the alarm goes off (the LED stays lit). Once the alarm is disabled the user
// can reenable the alarm by clicking the button again. 
// 
// @author  Caleb Priede
// @id   000803812
// 
// I created this work and I have not shared it with anyone else. 
// I reached stage 5 of the assignment
 
#include <Arduino.h> 
 
// Motion detector digital pin D5
#define PIN_PIR D5 

// Switch button digital pin D6
#define PIN_BUTTON D6 

// Delay in MS after the switch button is pressed
#define BUTTON_DELAY 200

// Delay in MS after the LED status is flipped
#define LIGHT_DELAY 125

// The inital value for the alarm trigger counter
#define INITIAL_ALARM 0

// The value that will trigger the alarm once the count reaches it
#define TRIGGER_ALARM 80 // this is equivalent to 10 seconds. There is 8 cycles per second!

// This state is when the alarm has been turned off by the user
#define ALARM_DISABLED 0

// This state is when the alarm is watching for motion
#define ALARM_ENABLED 1

// This state is when the alarm is counting down from 10 seconds
#define ALARM_COUNTDOWN 2

// The state for when the alarm is active (you cannot get out of this state unless the device is reset)
#define ALARM_ACTIVE 3


// Holds the current state of the alarm
int iAlarmState;

// Holds the current count for when the alarm detects motion
int iCount;

// The motion sensor digital value is written to this variable
bool bPIR;

// The switch button digital value is stored in this variable
bool bSwitch;
 
// ************************************************************* 
// Set inital values and set digital pins
void setup() { 
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT); 
 
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 
 
  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP); 

  Serial.println("\n");
  Serial.println("COMP-10184 - Alarm System");
  Serial.println("Name: Caleb Priede");
  Serial.println("Student ID: 000803812");
  Serial.println(" ");
  // Set the inital alarm state to enabled
  iAlarmState = ALARM_ENABLED;
} 
 

//  ******************************************************************************
//  This functions purpose is to get the digital values from the
//  connected sensors and store them in global variables.
//
void collectInputs(){
    bPIR = digitalRead(PIN_PIR);
    bSwitch = digitalRead(PIN_BUTTON);
}

//  ******************************************************************************
//  This functions purpose is to handle the input values in different ways according
//  to the alarms current state. Each state is watching for different inputs to trigger
//  other states in the alarm
//
void checkAlarmState(){
  switch(iAlarmState){
    // when the alarm is enabled check for any motion and make sure the LED is OFF
    case ALARM_ENABLED:
      if(bPIR){
        iCount = INITIAL_ALARM;
        iAlarmState = ALARM_COUNTDOWN;
      }
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    // when the alarm is disabled check if the button is pressed which will reenable the alarm
    case ALARM_DISABLED:
      if(!bSwitch){
        Serial.println("Button pressed: Alarm Enabled");
        delay(BUTTON_DELAY);
        iAlarmState = ALARM_ENABLED;
      }
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    // when the alarm is in countdown mode wait for the user to turn it off from a button press
    // if they do not press the button is a set amount of time then the alarm will go off
    case ALARM_COUNTDOWN:
      if(iCount == INITIAL_ALARM){
        Serial.println("Motion Detected! - Alarm will sound in 10s!");
      }
      if(iCount == TRIGGER_ALARM){
        Serial.println("Alarm activated!");
        iAlarmState = ALARM_ACTIVE;
      }
      if(!bSwitch){
        Serial.println("Button pressed: Alarm Disabled");
        delay(BUTTON_DELAY);
        iAlarmState = ALARM_DISABLED;
        break;
      }
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(LIGHT_DELAY);
      iCount++;
      break;
    // when the alarm is active activate the LED (the alarm cannot go to any other state once it reached here, you must reset the device)
    case ALARM_ACTIVE:
      digitalWrite(LED_BUILTIN, LOW);
      break;
    default:
      digitalWrite(LED_BUILTIN, HIGH);
  }
}

//  ************************************************************
void loop() { 
  // get button and motion sensor input values
  collectInputs();

  // check alarm state and handle inputs accordingly
  checkAlarmState();
} 