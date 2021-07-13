#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xAC, 0x67, 0xB2, 0x38, 0x23, 0xF4};
float incomingSmoke;
float incomingFire;
float incomingVib;
int sFlag=0,fFlag=0,vFlag=0;

typedef struct struct_message
{
    float smke;
    float fir;
    float vib;
} struct_message;

struct_message incomingReadings;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingSmoke = incomingReadings.smke;
  incomingFire = incomingReadings.fir;
  incomingVib = incomingReadings.vib;
  
  if(incomingSmoke==0.0 && sFlag==0)
  {
      sFlag=1;
      Serial.println("Smoke Detected");
      delay(1000);

      Serial.println("ATE0");
      delay(1000);
      Serial.println("AT+CMGF=1");
      delay(1000);
      Serial.print("AT+CMGS=");
      delay(4000);
      Serial.write('"');
      Serial.print("8341571529");
      Serial.write('"');
      Serial.println();
      delay(4000);
      Serial.println("Smoke Detected");
      delay(2000);
      Serial.write(0x1A);
      delay(1000);  
  } else if (incomingSmoke==1.0&&sFlag==1) {
    sFlag=0;
  }
  
  if(incomingFire==0.0 && fFlag==0)
  {
      fFlag=1;
      Serial.println("Fire Detected");
      delay(1000);

      Serial.println("ATE0");
      delay(1000);
      Serial.println("AT+CMGF=1");
      delay(1000);
      Serial.print("AT+CMGS=");
      delay(4000);
      Serial.write('"');
      Serial.print("8341571529");
      Serial.write('"');
      Serial.println();
      delay(4000);
      Serial.println("Fire Detected");
      delay(2000);
      Serial.write(0x1A);
      delay(1000);  
      
  } else if(incomingFire==1.0 && fFlag==1) {
    fFlag=0;
  }
  if(incomingVib==0.0 && vFlag==0)
  {
      vFlag=1;
      Serial.println("Vib Detected");
      delay(1000);

      Serial.println("ATE0");
      delay(1000);
      Serial.println("AT+CMGF=1");
      delay(1000);
      Serial.print("AT+CMGS=");
      delay(4000);
      Serial.write('"');
      Serial.print("8341571529");
      Serial.write('"');
      Serial.println();
      delay(4000);
      Serial.println("Vib Detected");
      delay(2000);
      Serial.write(0x1A);
      delay(1000);  
  } else if(incomingVib==1.0 && vFlag==1) {
    vFlag=0;
  }
}
 
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
void loop() 
{
  //updateDisplay();
  delay(1000);
}

void updateDisplay()
{
  Serial.println("INCOMING READINGS");
  Serial.print("Smoke : ");
  Serial.print(incomingReadings.smke);
  Serial.println(" ");
  Serial.print("fire: ");
  Serial.print(incomingReadings.fir);
  Serial.println(" ");
  Serial.print("vibration: ");
  Serial.print(incomingReadings.vib);
  Serial.println(" ");
}


