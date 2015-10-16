  // constants used in class
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
    
enum CandleState {Unknown, ShowStd, ShowDiff};

class RGB_Candle 
{
  // pins to read RGB values set by user
  int mRedPin;
  int mGreenPin;
  int mBluePin;
  
  // vars to store RGB values set by user
  int mRedValue;
  int mGreenValue;
  int mBlueValue;
  
  // pins to output candle color
  int mRedLED;
  int mGreenLED;
  int mBlueLED;
  
  // vars to manage state of candle
  CandleState mState;
  unsigned long mPrevMillis;
  int mDelay;
  
  // constants
  const int cColorUnknown = 0;
  const int cColorRed = 1;
  const int cColorGreen = 2;
  const int cColorBlue = 3;
  
 // const int cStateUnknown = 0;
 // const int cStateShowStd = 1;
 // const int cStateShowDiff = 2;
  
  public:
  RGB_Candle(int pRedPin, int pGreenPin, int pBluePin, int pRedLED, int pGreenLED, int pBlueLED) 
  {
    mRedPin = pRedPin;
    mGreenPin = pGreenPin;
    mBluePin = pBluePin;
    
    mRedLED = pRedLED;
    mGreenLED = pGreenLED;
    mBlueLED = pBlueLED;
    
    mState = Unknown;
    mPrevMillis = 0;
    mDelay = 0;
  }
  
  void Flicker(unsigned long pCurrMillis) 
  {
    
    if (mState == ShowStd) 
    {
      if (pCurrMillis - mPrevMillis >= mDelay) 
      {
        readAllValues();
        writeAllValues(mRedValue, mGreenValue, mBlueValue);
        mDelay = random(0,200);
        mState = ShowDiff;
        mPrevMillis = pCurrMillis;
      }
    }
    else if (mState == ShowDiff) 
    {
      if (pCurrMillis - mPrevMillis >= mDelay) 
      {
        // only vary the color on highest value to create flicker effect
        readAllValues();
        switch (determineHighestValue(mRedValue, mGreenValue, mBlueValue)) {
          case 1:
            writeAllValues(calculateFlickerDiff(mRedValue), mGreenValue, mBlueValue);
            break;
          case 2:
            writeAllValues(mRedValue, calculateFlickerDiff(mGreenValue), mBlueValue);
            break;
          case 3:
            writeAllValues(mRedValue, mGreenValue, calculateFlickerDiff(mBlueValue));
            break;
        }
        mDelay = random(0,100);
        mState = ShowStd;
        mPrevMillis = pCurrMillis;
      }
    }
    else if (mState == Unknown) 
    {
      readAllValues();
      mDelay = 0;
      mState = ShowStd;
      mPrevMillis = pCurrMillis;
    }
  }
  
  private:
    void readAllValues() {
      mRedValue = calcRGBDisplayValue(analogRead(mRedPin)); 
      mGreenValue = calcRGBDisplayValue(analogRead(mGreenPin)); 
      mBlueValue = calcRGBDisplayValue(analogRead(mBluePin)); 
    }

    int calcRGBDisplayValue(int pInValue) {
      return pInValue / 4;
    }
    
    void writeAllValues(int pRedValue, int pGreenValue, int pBlueValue) {
      
      analogWrite(mRedLED, pgm_read_byte(&gamma[pRedValue]));  
      analogWrite(mGreenLED, pgm_read_byte(&gamma[pGreenValue])); 
      analogWrite(mBlueLED, pgm_read_byte(&gamma[pBlueValue]));  
    }
  
    int determineHighestValue (int pRed, int pGreen, int pBlue) {
      
      int intReturn = cColorUnknown;
      
      if (pRed > pBlue) {
        if (pRed > pGreen)
          intReturn = cColorRed;
        else
          intReturn = cColorGreen;
      }
      else {
        if (pBlue > pGreen)
          intReturn = cColorBlue;
        else
          intReturn = cColorGreen;
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
        intRange = 40;
      }
      else if (pStartVal <= 166) {
        intRange = 50;
      }
      else {
        intRange = 60;
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
  
};

RGB_Candle candle1(A3, A4, A5, 11, 10, 9);

void setup() {
  
  Serial.begin(9600);
  
  // timer interup set on timer0
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

// method called by interup on timer0
SIGNAL(TIMER0_COMPA_vect) 
{
  unsigned long lCurrMillis = millis();
  candle1.Flicker(lCurrMillis);
}

void loop() 
{ 
}

