int EN = PB11;
int RS = PB10;
const int pinMap[] = {PA0, PA1, PA2, PA3, PB11, PB10};

void setup() {

  for (int i = 0; i < 6; i++) {
    pinMode(pinMap[i], OUTPUT); // Set pins 0 to 3 as output
  }
  digitalWrite(RS, LOW);
  digitalWrite(EN, LOW);
  init_lcd();
  char c2 = '>';
  writeCharBits(c2);
  writeCharBits(c2);

  String val = String(1001, DEC);
  print_lcd(val);
  

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

void print_lcd(String val) {
    int len = val.length();
    for (int j = 0; j < len ; j++) {
      writeCharBits(val[j]);
    }
  }
void loop() {

}
