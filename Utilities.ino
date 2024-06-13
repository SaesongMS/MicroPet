void playSound(int frequency, int duration) {
  digitalWrite(buzzerPin2, HIGH);
  ledcAttachPin(buzzerPin, 0);
  ledcSetup(0, frequency, 8);
  ledcWriteTone(0, frequency);
  delay(duration);
  digitalWrite(buzzerPin2, LOW);
}

//mode 1 = on, mode 2 = blink, mode 3 = off
void updateLed(int mode){
  if(mode == 1){
    digitalWrite(ledPin, HIGH);
    ledBlink = false;
    blinker.detach();
  }else if(mode == 2){
    ledBlink = true;
    blinker.attach(blinkerPace, blink);
  }else if(mode == 3){
    digitalWrite(ledPin, LOW);
    ledBlink = false;
    blinker.detach();
  }
}

void blink() {
  digitalWrite(ledPin, !digitalRead(ledPin));
}