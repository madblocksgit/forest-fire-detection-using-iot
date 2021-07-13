#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xAC, 0x67, 0xB2, 0x36, 0xEB, 0xEC};
float smoke;
float fire;
float vibration;
String success;

typedef struct struct_message
{
    float smke;
    float fir;
    float vib;
} struct_message;

struct_message sensorReadings;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
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
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() 
{
  getReadings();
  sensorReadings.smke = smoke;
  sensorReadings.fir = fire;
  sensorReadings.vib = vibration;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &sensorReadings, sizeof(sensorReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}

void getReadings()
{
  smoke =digitalRead(21);
  fire = digitalRead(23);
  vibration = digitalRead(22);
}
