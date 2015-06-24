/*
PWMGenerator

This arduino code allows you to generate a PWM signal.

This code uses two ways for that :
  - a potentiometer (pin A0)
  - two buttons for fine-tuning (pin 2 and 3).
  
Button 2 allows you to decrease the PWM signal of one step.
Button 3 allows you to increase the PWM signal of one step.

By pusshing the button 2 and 3 at the same time, you will cancel the fine tuning.

You can estimate the PWM signal by the brightness of led 13.
*/

#include <SoftPWM.h>

const int analogInPin = A0;
const int pwmPin = 9;
const int buttonPlusPin = 3;
const int buttonMinusPin = 2;
const int debounceDelay = 150;

int potValue = 0;
int buttonsValue = 0;

int buttonPlusDebounce = 0;
int buttonMinusDebounce = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(analogInPin, INPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(buttonPlusPin, INPUT_PULLUP);
  pinMode(buttonMinusPin, INPUT_PULLUP);

  //Soft PWM for Led 13
  SoftPWMBegin();
  SoftPWMSet(13, 0);
  SoftPWMSetFadeTime(13, 0, 0);
}

void loop() {
  // map analog read to the range of the analog out:
  potValue = 255 - map(analogRead(analogInPin), 0, 1023, 0, 255);
  
  // handle buttons
  if(digitalRead(buttonPlusPin) == LOW && digitalRead(buttonMinusPin) == LOW){
    buttonsValue = 0;
  }
  else{
    if(digitalRead(buttonPlusPin) == LOW && millis() >= buttonPlusDebounce + debounceDelay){
      buttonsValue++;
      buttonPlusDebounce = millis();
    }
    if(digitalRead(buttonMinusPin) == LOW && millis() >= buttonMinusDebounce + debounceDelay){
      buttonsValue--;
      buttonMinusDebounce = millis();
    }
  }

  //correct button value if necessary
  int outputValue = potValue + buttonsValue;
  if(outputValue < 0){
    buttonsValue = -potValue;
  }
  if(outputValue > 255){
    buttonsValue = 255 - potValue;
  }
  
  // change the analog out value:
  analogWrite(pwmPin, outputValue);
  
  // Set the led brightness
  SoftPWMSet(13, outputValue);
  
  // Write value to serial
  Serial.println(outputValue);
  
  delay(20);
}
