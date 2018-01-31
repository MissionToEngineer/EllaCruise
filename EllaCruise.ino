#include <Arduino.h>

#define STOPPED 0
#define SPINLEFT 1
#define SPINRIGHT 2
#define FWDSTRAIGHT 3
#define FWDLEFT 4
#define FWDRIGHT 5

#define STARTDELAY 200

#define LEFTBUT A5
#define RIGHTBUT A3

#define LEFTOUT 10
#define RIGHTOUT 6
#define FWDOUT 11 //7 might work

boolean isCruising = false;
boolean lastState = STOPPED;
bool isMoving = false;
unsigned long lastPress = 0;
unsigned long startTime = 0;
bool isPausing = false;

void setup() {
  pinMode(RIGHTBUT, INPUT_PULLUP);
  pinMode(LEFTBUT, INPUT_PULLUP);

  //And the three output pins that drive the relays
  pinMode(LEFTOUT, OUTPUT);
  pinMode(RIGHTOUT, OUTPUT);
  pinMode(FWDOUT, OUTPUT);
  
  Serial.begin(115200);
  Serial.println(F("Ella PowerChair Code"));
  Serial.println(F("-----------------------------------------"));
}

void setChair(int direction)
{
  if (direction == FWDSTRAIGHT)
  {
      digitalWrite(LEFTOUT, LOW);
      digitalWrite(RIGHTOUT, LOW);
      digitalWrite(FWDOUT, HIGH);
  }
  else if (direction == SPINLEFT)
  {
      digitalWrite(LEFTOUT, HIGH);
      digitalWrite(RIGHTOUT, LOW);
      digitalWrite(FWDOUT, LOW);
  }
  else if (direction == FWDLEFT)
  {
      digitalWrite(LEFTOUT, HIGH);
      digitalWrite(RIGHTOUT, LOW);
      digitalWrite(FWDOUT, HIGH);
  }
  else if (direction == SPINRIGHT)
  {
      digitalWrite(LEFTOUT, LOW);
      digitalWrite(RIGHTOUT, HIGH);
      digitalWrite(FWDOUT, LOW);
  }
  else if (direction == FWDRIGHT)
  {
      digitalWrite(LEFTOUT, LOW);
      digitalWrite(RIGHTOUT, HIGH);
      digitalWrite(FWDOUT, HIGH);
  }
  else if (direction == STOPPED)
  {
      digitalWrite(LEFTOUT, LOW);
      digitalWrite(RIGHTOUT, LOW);
      digitalWrite(FWDOUT, LOW);
  }
}


void loop() {

  boolean leftDown = (digitalRead(LEFTBUT) == LOW);
  boolean rightDown = (digitalRead(RIGHTBUT) == LOW);

  boolean bothDown = leftDown && rightDown;


  /*  Serial.print("Left");
    Serial.print(leftDown);
    Serial.print(" Right");
    Serial.print(rightDown);
    Serial.print(" Both");
    Serial.println(bothDown);*/

  if (leftDown || rightDown)
  {
    if (!isMoving) //delay from stop
    {
      if (!isPausing) //first time during delay
      {
        startTime = millis() + STARTDELAY;
        isPausing = true;
        return;
      }
      else if (startTime > millis())                                           
      {
        return;
      }
    }
    //if we get here, we're moving and not pausing
    isPausing = false;
    isMoving = true;
    if (bothDown)
    {
      isCruising = true;
      setChair(FWDSTRAIGHT);
      //   Serial.println("Moving Straight");
      lastState = FWDSTRAIGHT;
    }
    else if (leftDown)
    {
      if (isCruising)
      {
        setChair(FWDLEFT);
        lastState = FWDLEFT;
      }
      else
      {
        isCruising = false;
        setChair(SPINLEFT);
        lastState = SPINLEFT;
      }
    }
    else if (rightDown)
    {
      if (isCruising)
      {
        setChair(FWDRIGHT);
        lastState = FWDRIGHT;
      }
      else
      {
        isCruising = false;
        setChair(SPINRIGHT);
        lastState = SPINRIGHT;
      }
    }
  }
  else
  {
    if (lastState != STOPPED)
    {
      isMoving = false;
      isPausing = false;
      isCruising = false;
      setChair(STOPPED);
      lastState = STOPPED;
    }
  }

}
