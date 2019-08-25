unsigned int m;
unsigned int dTime;
unsigned int sTime;
unsigned int irTimeStampBuffer[100];
unsigned int irRealTimeBuffer[100];
uint32_t irKeyCode = 0;
const int irPin = 2;
const int relayPin = 13;

void setup() 
{
  pinMode(relayPin, OUTPUT);
  pinMode(irPin, INPUT);
  Serial.begin(115200); 
  attachInterrupt(digitalPinToInterrupt(irPin), irPulse, CHANGE);
}

void loop() 
{
  if (m) 
  { 
    m = 0;
    Serial.println("RAW TIMING DATA (MARK & SPACE): [68 BITS]");
    for (int n = 0; n < 67; n++)
    { 
      dTime = irTimeStampBuffer[n + 1] - irTimeStampBuffer[n];
      Serial.print(dTime);
      Serial.print(" ");
      if (n == 17 || n == 34 || n == 51)
        Serial.print("\n");
    }
    for (int n = 2; n < 66; n++)
    { 
        dTime = irTimeStampBuffer[n + 1] - irTimeStampBuffer[n];
        irRealTimeBuffer[n - 2] = dTime;
    }
    Serial.print("\n--------------------------------------------------------------------\n");
    Serial.print("BINARY DATA: [32 BITS]\n");
    for (int n = 0; n < 64; n += 2)
    {
      sTime = irRealTimeBuffer[n] + irRealTimeBuffer[n + 1];
      if (sTime > 2200 && sTime < 2300)
      {
        Serial.print(1);
        irKeyCode <<= 1;
        irKeyCode |= 1;
      }
      else if (sTime > 1100 && sTime < 1200)
      {
        Serial.print(0);
        irKeyCode <<= 1;
      }
    }
    Serial.print("\n--------------------------------------------------------------------\n");
    Serial.print("DECIMAL DATA: [4 BYTES]\n");
    Serial.print(irKeyCode);
    Serial.print("\n--------------------------------------------------------------------\n");
    Serial.print("HEXADECIMAL DATA: [4 BYTES]\n");
    Serial.print(irKeyCode, HEX);
    Serial.print("\n\n");  
    irKeyCode = 0;
    clearTimeStampBuffer();
  }
  delay(1000);
}

void clearTimeStampBuffer()
{
  for (int k = 0; k < 100; k++)
    irTimeStampBuffer[k] = 0;
}

void irPulse() 
{
  irTimeStampBuffer[m++] = micros();
}