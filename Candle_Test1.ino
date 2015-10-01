
// pin declarations
int redPin = A3;    
int greenPin = A4;
int bluePin = A5;
int redLED = 11;
int greenLED = 10;
int blueLED = 9;

// variables
int redValue = 0;  
int greenValue = 0;
int blueValue = 0;

int redDisplay = 0;  
int greenDisplay = 0;
int blueDisplay = 0;

// constants
const int cUnknown = 0;
const int cRed = 1;
const int cGreen = 2;
const int cBlue = 3;

const uint8_t PROGMEM gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

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
  result = pgm_read_byte(&gamma[input]);
  
  analogWrite(redLED, pgm_read_byte(&gamma[redDisplay]));  
  analogWrite(greenLED, pgm_read_byte(&gamma[greenDisplay])); 
  analogWrite(blueLED, pgm_read_byte(&gamma[blueDisplay]));  
  //Serial.println(greenDisplay);
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
