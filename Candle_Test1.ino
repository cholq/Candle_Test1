


int redPin = A3;    
int greenPin = A4;
int bluePin = A5;
int redLED = 11;
int greenLED = 10;
int blueLED = 9;

int redValue = 0;  
int greenValue = 0;
int blueValue = 0;

int redDisplay = 0;  
int greenDisplay = 0;
int blueDisplay = 0;

const int cUnknown = 0;
const int cRed = 1;
const int cGreen = 2;
const int cBlue = 3;

void setup() {
  Serial.begin(9600);
}

void loop() {
  readAllValues(); 
  changeAllValues();
  writeAllValues();
  flicker();
  writeAllValues();
  delay(50);
  
}

void readAllValues() {
  redValue = analogRead(redPin); 
  greenValue = analogRead(greenPin); 
  blueValue = analogRead(bluePin); 
}

void changeAllValues() {
  redDisplay = redValue / 4;
  greenDisplay = greenValue / 4;
  blueDisplay = blueValue / 4;
}

void flicker() {
  
  int intSleep = 0;
  int intDiff = 0;
  int intColor = cUnknown;
  
  if (redDisplay != blueDisplay || redDisplay != greenDisplay ) {
    
    // determine which color has the highest value
    intColor = determineHighestValue(redDisplay, greenDisplay, blueDisplay);
  
    intSleep = random(0,200);
    delay(intSleep);
  
    // only vary the color on highest value to create flicker effect
    switch (intColor) {
      case cRed:
        redDisplay = calculateFlickerDiff(redDisplay);
        break;
      case cGreen:
        greenDisplay = calculateFlickerDiff(greenDisplay);
        break;
      case cBlue:
        blueDisplay = calculateFlickerDiff(blueDisplay);
        break;
    }
    
  }
}

void writeAllValues() {
  analogWrite(redLED, redDisplay);  
  analogWrite(greenLED, greenDisplay); 
  analogWrite(blueLED, blueDisplay);  
  Serial.println(greenDisplay);
}

int determineHighestValue (int pRed, int pGreen, int pBlue) {
  
  int intReturn = cUnknown;
  
  if (pRed > pBlue) {
    if (pRed > pGreen)
      intReturn = cRed;
    else
      intReturn = cGreen;
  }
  else {
    if (pBlue > pGreen)
      intReturn = cBlue;
    else
      intReturn = cGreen;
  }
  
  return intReturn;
}

int calculateFlickerDiff (int pStartVal) {
  
  int intReturn = 0;
  int intRange = 0;
  int intLow = pStartVal;
  int intHigh = pStartVal;
  
  // calc range used for random function
  if (pStartVal <= 83) {
    intRange = 30;
  }
  else if (pStartVal <= 166) {
    intRange = 40;
  }
  else {
    intRange = 50;
  }
  
  // calculate return value
  if (pStartVal >= (255 - intRange)) {
    intLow = pStartVal - intRange;
    intHigh = 255;
  }
  else if (pStartVal <= intRange) {
    intLow = 0;
    intHigh = pStartVal + intRange;
  }
  else {
    intLow = pStartVal - intRange;
    intHigh = pStartVal + intRange;
  }
  
  intReturn = random(intLow, intHigh);
  
  return intReturn;
}
