

#include "arduinoFFT.h"
arduinoFFT FFT;
/*
  These values can be changed in order to evaluate the functions
*/
const uint16_t samples = 1024; //This value MUST ALWAYS be a power of 2
//const double signalFrequency = 5000;
const double samplingFrequency = 40000;
//const uint8_t amplitude = 100;
/*
  These are the input and output vectors
  Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

int EN = PB11;
int RS = PB10;
int DI = PB1;

const int pinMap[] = {PA0, PA1, PA2, PA3, PB11, PB10};
void setup()
{
  Serial.begin(9600);
  delay(1000);
  for (int i = 0; i < 6; i++) {
    pinMode(pinMap[i], OUTPUT); // Set pins 0 to 3 as output
  }
  pinMode(DI, INPUT); 
  digitalWrite(RS, LOW);
  digitalWrite(EN, LOW);
  init_lcd();
  char c2 = '>';
  writeCharBits(c2);
  writeCharBits(c2);
  String val = "Calc: ";

  int len = val.length();
  for(int j = 0; j <len ; j++){
    writeCharBits(val[j]);
    }
  
  Serial.println("Ready");
}

void loop()
{
  /* Build raw data */
  double cycles = (((samples - 1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
  for (uint16_t i = 0; i < samples; i++)
  {
//    vReal[i] = int8_t((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0) +int8_t((0.5 *amplitude * (sin((i * (twoPi * cycles/2)) / samples))) / 2.0);/* Build data with positive and negative values*/
    vReal[i] = uint8_t(analogRead(DI));/* Build data displaced on the Y axis to include only positive values*/
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }



  FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency); /* Create FFT object */
  /* Print the results of the simulated sampling according to time */
  Serial.println("Data:");
  PrintVector(vReal, samples, SCL_TIME);
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);  
  Serial.println("Weighed data:");
  PrintVector(vReal, samples, SCL_TIME);
  FFT.Compute(FFT_FORWARD); 
  Serial.println("Computed Real values:");
  PrintVector(vReal, samples, SCL_INDEX);
  Serial.println("Computed Imaginary values:");
  PrintVector(vImag, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(); /* Compute magnitudes */
  Serial.println("Computed magnitudes:");
  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  double x = FFT.MajorPeak();
  Serial.println(x, 6);

  ``
    
  while (1); /* Run Once */
  // delay(2000); /* Repeat after delay */
}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
        break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
        break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
        break;
    }
    Serial.print(abscissa, 6);
    if (scaleType == SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}

void writeCharBits(char c) {
  digitalWrite(RS, HIGH);
  digitalWrite(EN, LOW);
  byte firstFourBits = (c >> 4) & 0x0F; // Get the first 4 bits of the ASCII character
  byte lastFourBits = c & 0x0F; // Get the last 4 bits of the ASCII character

  for (int i = 0; i < 4; i++) {
    digitalWrite(pinMap[i], (firstFourBits >> i) & 0x01); // Write the first 4 bits to pins 0 to 3
  }
  delay(50); // Wait for 0.1 second
  digitalWrite(EN, HIGH); delay(50); digitalWrite(EN, LOW);
  delay(100);
  for (int i = 0; i < 4; i++) {
    digitalWrite(pinMap[i], (lastFourBits >> i) & 0x01); // Write the last 4 bits to pins 0 to 3
  }
  delay(50);
  digitalWrite(EN, HIGH); delay(50); digitalWrite(EN, LOW);
  delay(100);
  digitalWrite(RS, LOW);
}

void writeHexBits(byte hexValue, bool high) {
  byte lastFourBits = (hexValue >> 4) & 0x0F; // Get the first 4 bits of the hex value
  byte firstFourBits = hexValue & 0x0F; // Get the last 4 bits of the hex value
  digitalWrite(RS, LOW);
  digitalWrite(EN, LOW);

  for (int i = 0; i < 4; i++) {
    digitalWrite(pinMap[i], (firstFourBits >> i) & 0x01); // Write the first 4 bits to pins 0 to 3
  }
  delay(50);
  digitalWrite(EN, HIGH); delay(50); digitalWrite(EN, LOW);
  delay(100);
  if (high) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(pinMap[i], (lastFourBits >> i) & 0x01); // Write the last 4 bits to pins 0 to
    }
    delay(50);
    digitalWrite(EN, HIGH); delay(50); digitalWrite(EN, LOW);
    delay(100); // Wait for 1 second
  }
}

void init_lcd()
{
  digitalWrite(RS, LOW);
  digitalWrite(EN, LOW);
  writeHexBits(0x02, false);    // initialize the display into 4-bit mode

  writeHexBits(0x10, true);    // clear the display

  writeHexBits(0x20, true);    // clear the display

  writeHexBits(0xD0, true);    // clear the display

}
