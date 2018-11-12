int togglePinAssignment = 20;
int tempo = 0;
int ledArray[4] = {29, 30, 31, 32};
int buttonArray [4] = { 39, 38, 37, 36 };
int togglePin2 = 33;
int togglePin = 34;
int currentStep = 0;
unsigned long lastStepTime = 0;
int potArray[4] = {A20, A19, A18, A17};
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean nextLastButtonState = LOW;
boolean nextButtonState = LOW;
boolean prevLastButtonState = LOW;
boolean prevButtonState = LOW;

boolean on[3][4] = {
  { LOW, LOW, LOW, LOW },
  { LOW, LOW, LOW, LOW },
  { LOW, LOW, LOW, LOW }
};
int nextChannelButtonPin = 15;
int prevChannelButtonPin = 14;
int channelDisplayed = 0;
int midiNotes[3] = { 36, 40, 46 };




void setup() {
  Serial.begin(9600);
  delay(2000);
  pinMode(ledArray[0], OUTPUT);  //set pin 11, array element 0, as output
  pinMode(ledArray[1], OUTPUT);  //set pin 10, array element 1, as output
  pinMode(ledArray[2], OUTPUT);  //set pin 9, array element 2, as output
  pinMode(ledArray[3], OUTPUT);  //set pin 6, array element 3, as output
  pinMode(buttonArray [0], INPUT);
  pinMode(buttonArray [1], INPUT);
  pinMode(buttonArray [2], INPUT);
  pinMode(buttonArray [3], INPUT);
  pinMode(togglePin, INPUT);
  pinMode(togglePin2, INPUT);
  pinMode(nextChannelButtonPin, INPUT);
  pinMode(prevChannelButtonPin, INPUT);


}

void loop() {
  checkNextChannelButton();
  checkPrevChannelButton();
  checkButton();
  sequence();
  setLed();

}




void sequence() {
  tempo = analogRead(A16);

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...

    //turn last things off from last step
    // digitalWrite(ledArray[currentStep], LOW);  //turn off the current led




    //increment up
    if (digitalRead(togglePin2) == HIGH) {
      currentStep = currentStep + 1;         //increment to the next step
      if (currentStep > 3) {
        currentStep = 0;
      }
      // backwards

    } else {
      currentStep = currentStep - 1;        //increment to the next step
      if (currentStep < 0) {
        currentStep = 3;
      }
    }
    lastStepTime = millis();               //set lastStepTime to the current time
    //  digitalWrite(ledArray[currentStep], HIGH); //turn on the new led
    for (int i = 0; i < 3; i++) {
      if (on[i][currentStep] == true) {
        usbMIDI.sendNoteOff(midiNotes[i], 0, 1);
        usbMIDI.sendNoteOn(midiNotes[i], 127, 1);

      }
    }
  }
}

void checkButton() {
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonArray[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (on[channelDisplayed][i] == false) {
        on[channelDisplayed][i] = true;
      } else if (on[channelDisplayed][i] == true) {
        on[channelDisplayed][i] = false;
      }
    }
  }
}
void setLed() {
  for (int i = 0; i < 4; i++) {
    if (on[channelDisplayed][i] == true || currentStep == i) {
      digitalWrite(ledArray[i], HIGH);

    } else {//if (on[channelDisplayed][i] == false || currentStep == i) {
      digitalWrite(ledArray[i], LOW);

    }
  }
}
void checkNextChannelButton() {
  nextLastButtonState = nextButtonState;
  nextButtonState = digitalRead(nextChannelButtonPin);

  if (nextButtonState == HIGH && nextLastButtonState == LOW) {
    Serial.println("next");
    Serial.println(channelDisplayed);



    channelDisplayed = channelDisplayed + 1;         //increment to the next step
    if (channelDisplayed > 2) {
      channelDisplayed = 0;
    }
  }

}

void checkPrevChannelButton() {
  prevLastButtonState = prevButtonState;
  prevButtonState = digitalRead(prevChannelButtonPin);

  if (prevButtonState == HIGH && prevLastButtonState == LOW) {
    Serial.println("prev");
    Serial.println(channelDisplayed);

    channelDisplayed = channelDisplayed - 1;        //increment to the next step
    if (channelDisplayed < 0) {
      channelDisplayed = 2;


    }

  }

}
