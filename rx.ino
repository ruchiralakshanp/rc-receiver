
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
const uint64_t pipeIn = 0xE8E8F0F0E1LL;

RF24 radio(9, 10);

//We could use up to 32 channels
struct MyData {
  int throttle;
  int yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
};

MyData data;

void resetData()
{

  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2 = 0;

}

/**************************************************/

void setup()
{
  Serial.begin(250000);
  resetData();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);

  radio.openReadingPipe(1, pipeIn);

  radio.startListening();

}

/**************************************************/

unsigned long lastRecvTime = 0;

void recvData()
{
  while ( radio.available() ) {
    radio.read(&data, sizeof(MyData));
    lastRecvTime = millis(); //here we receive the data
  }
}

/**************************************************/

void loop()
{
  recvData();
  unsigned long now = millis();
  //Here we check if we've lost signal, if we did we reset the values
  if ( now - lastRecvTime > 1000 ) {
    // Signal lost?
    resetData();
  }

  Serial.print("Throttle: "); Serial.print(data.throttle);  Serial.print("    ");
  Serial.print("Yaw: ");      Serial.print(data.yaw);       Serial.print("    ");
  Serial.print("Pitch: ");    Serial.print(data.pitch);     Serial.print("    ");
  Serial.print("Roll: ");     Serial.print(data.roll);      Serial.print("    ");
  Serial.print("Aux1: ");     Serial.print(data.AUX1);      Serial.print("    ");
  Serial.print("Aux2: ");     Serial.print(data.AUX2);      Serial.print("\n");



}

/**************************************************/
