// OPENSOURCE ENYA-GUITAR ESP32 CONTROLLER
//THIS PROJECT ENABLE'S YOU TO CONTROL YOUT ENAY GUITAR OVER BLUETOOTH CHANGE PRESETS AND TURN-ON/OFF PEDALS
//USEFULL FOR CREATING A BLE PEDALBOARD


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>




// UUIDs e endereço MAC
#define SERVICE_UUID        "0000ab11-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ab12-0000-1000-8000-00805f9b34fb"
#define DEVICE_ADDRESS      "XX:XX:XX:XX:XX:XX" //Your guitar mac Addr

// === Define os GPIOs ===
const int presetButtons[4] = {12, 13, 14, 15};      // Presets 0 - 3
const int pedalButtons[5]  = {16, 17, 18, 19, 21};  // Pedals 0 - 4

// === CONTROL VARIABLES DON'T CHANGE UNLESS YOU KNOW WHAT YOU'RE DOING ===
int currentPreset = -1;
bool pedalStates[5] = {false, false, false, false, false};
bool lastPresetButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool lastPedalButtonState[5]  = {HIGH, HIGH, HIGH, HIGH, HIGH};

#define SERVICE_UUID        "0000ab11-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ab12-0000-1000-8000-00805f9b34fb"
#define DEVICE_ADDRESS      "D9:A6:85:71:E2:37"  

// PRESET PACKAGES FROM 0 TO 4 DON'T CHANGE
const uint8_t PACKETS_PRESET[4][2][13] = {
  { 
    {0xAA, 0x55, 0x0A, 0x00, 0x0C, 0x00, 0x00, 0x02, 0x00, 0x02, 0xE5, 0x55, 0xAA},
    {0xAA, 0x55, 0x0A, 0x20, 0x0C, 0x00, 0x00, 0x02, 0x00, 0x02, 0xC5, 0x55, 0xAA}
  },
  { 
    {0xAA, 0x55, 0x0A, 0x00, 0x0C, 0x01, 0x00, 0x02, 0x00, 0x02, 0xE4, 0x55, 0xAA},
    {0xAA, 0x55, 0x0A, 0x20, 0x0C, 0x01, 0x00, 0x02, 0x00, 0x02, 0xC4, 0x55, 0xAA}
  },
  { 
    {0xAA, 0x55, 0x0A, 0x00, 0x0C, 0x02, 0x00, 0x02, 0x00, 0x02, 0xE3, 0x55, 0xAA},
    {0xAA, 0x55, 0x0A, 0x20, 0x0C, 0x02, 0x00, 0x02, 0x00, 0x02, 0xC3, 0x55, 0xAA}
  },
  { 
    {0xAA, 0x55, 0x0A, 0x00, 0x0C, 0x03, 0x00, 0x02, 0x00, 0x02, 0xE2, 0x55, 0xAA},
    {0xAA, 0x55, 0x0A, 0x20, 0x0C, 0x03, 0x00, 0x02, 0x00, 0x02, 0xC2, 0x55, 0xAA}
  }
};

// PEDALS ON/OFF CONSTANT ARRAYS DON'T CHANGE
const uint8_t equalizador_on[]    = {0xAA, 0x55, 0x0C, 0x00, 0x07, 0x01, 0x03, 0x0C, 0x0C, 0x0C, 0x05, 0x05, 0xBA, 0x55, 0xAA};
const uint8_t equalizador_off[]   = {0xAA, 0x55, 0x0C, 0x00, 0x07, 0x00, 0x03, 0x0C, 0x0C, 0x0C, 0x05, 0x05, 0xBB, 0x55, 0xAA};
const uint8_t modulacao_on[]      = {0xAA, 0x55, 0x09, 0x00, 0x08, 0x01, 0x00, 0x1E, 0x14, 0xBB, 0x55, 0xAA};
const uint8_t modulacao_off[]     = {0xAA, 0x55, 0x09, 0x00, 0x08, 0x00, 0x00, 0x1E, 0x14, 0xBC, 0x55, 0xAA};
const uint8_t noise_gate_on[]     = {0xAA, 0x55, 0x0A, 0x00, 0x09, 0x01, 0x42, 0x00, 0x01, 0x64, 0x44, 0x55, 0xAA};
const uint8_t noise_gate_off[]    = {0xAA, 0x55, 0x0A, 0x00, 0x09, 0x00, 0x42, 0x00, 0x01, 0x64, 0x45, 0x55, 0xAA};
const uint8_t delay_on[]          = {0xAA, 0x55, 0x0A, 0x00, 0x0A, 0x01, 0x01, 0x2C, 0x14, 0x1D, 0x8C, 0x55, 0xAA};
const uint8_t delay_off[]         = {0xAA, 0x55, 0x0A, 0x00, 0x0A, 0x00, 0x01, 0x2C, 0x14, 0x1D, 0x8D, 0x55, 0xAA};
const uint8_t reverb_on[]         = {0xAA, 0x55, 0x08, 0x00, 0x0B, 0x01, 0x0F, 0x1E, 0xBE, 0x55, 0xAA};
const uint8_t reverb_off[]        = {0xAA, 0x55, 0x08, 0x00, 0x0B, 0x00, 0x0F, 0x1E, 0xBF, 0x55, 0xAA};

// PEDALS ON/OFF CONTROLL STRUCTS  DON'T CHANGE
struct PedalPacket { 
  const uint8_t* on;
  size_t on_len;
  const uint8_t* off;
  size_t off_len;
};

// ARRAY WITH PEDALS  DON'T CHANGE
const PedalPacket pedals[] = {
  {equalizador_on, sizeof(equalizador_on), equalizador_off, sizeof(equalizador_off)},
  {modulacao_on, sizeof(modulacao_on), modulacao_off, sizeof(modulacao_off)},
  {noise_gate_on, sizeof(noise_gate_on), noise_gate_off, sizeof(noise_gate_off)},
  {delay_on, sizeof(delay_on), delay_off, sizeof(delay_off)},
  {reverb_on, sizeof(reverb_on), reverb_off, sizeof(reverb_off)}
};

BLEClient* pClient;
BLERemoteCharacteristic* pRemoteCharacteristic;

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.println("cONNECTED");
  }

  void onDisconnect(BLEClient* pclient) {
    Serial.println("DICONECTED");
    delay(500);
    pclient->connect(BLEAddress(DEVICE_ADDRESS));
  }
};

bool connectToServer() {
  Serial.println("CONNECTING TO BLE DEVICE...");

  BLEAddress serverAddress(DEVICE_ADDRESS);
  BLEScan* pScan = BLEDevice::getScan();
  pScan->setActiveScan(true);

  BLEScanResults scanResults = *pScan->start(5, false); 

  bool deviceFound = false;
  BLEAdvertisedDevice foundDevice;

  for (int i = 0; i < scanResults.getCount(); i++) {
    BLEAdvertisedDevice device = scanResults.getDevice(i);
    if (device.getAddress().equals(serverAddress)) {
      foundDevice = device;
      deviceFound = true;
      Serial.println("DEVICE FOUND");
      break;
    }
  }

  pScan->clearResults();
  if (!deviceFound) {
    Serial.println("DEVICE NOT FOUND DURING SCAN");
    Serial.println("Check If:");
    Serial.println("1. Device Turned On");
    Serial.println("2. Correct Mac Address");
    Serial.println("3. Device in Range");
    return false;
  }

  delay(1000); 

  pClient = BLEDevice::createClient();
  Serial.println("CREATING BLE CLIENT...");
  pClient->setClientCallbacks(new MyClientCallback());

  if (!pClient->connect(&foundDevice)) {
    Serial.println("FAILED TO CONNECT TO DEVICE");
    return false;
  }

  Serial.println("CONNECTING TO BLE SERVER");

  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.println("SERVICE NOT FOUND");
    pClient->disconnect();
    return false;
  }

  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("CHARACTERISTIC NOT FOUND");
    pClient->disconnect();
    return false;
  }

  Serial.println("CHARACTERISTIC FOUND - READY TO SEND COMMANDS");
  return true;
}

void sendPreset(int preset) {
  if (preset < 0 || preset > 3) {
    Serial.println("INVALID PRESET");
    return;
  }
  
  if (!pClient->isConnected()) {
    Serial.println("DISCONNECTED - TRYING TO RECONNECT...");
    if (!connectToServer()) {
      return;
    }
  }
  
  Serial.print("SENDING PRESET: ");
  Serial.println(preset);
  
  for (int i = 0; i < 2; i++) {
    uint8_t packet[13];
    memcpy(packet, PACKETS_PRESET[preset][i], 13);
    
    pRemoteCharacteristic->writeValue(packet, 13, true);
    Serial.print("PACKAGE ");
    Serial.print(i+1);
    Serial.println(" SENDED");
    delay(100);
  }
}

void sendPedal(int pedalNumber, bool ligar) {
  if (pedalNumber < 0 || pedalNumber >= (int)(sizeof(pedals) / sizeof(pedals[0]))) {
    Serial.println("PEDAL NUMBER INVALID");
    return;
  }

  const uint8_t* packet;
  size_t length;

  if (ligar) {
    packet = pedals[pedalNumber].on;
    length = pedals[pedalNumber].on_len;
  } else {
    packet = pedals[pedalNumber].off;
    length = pedals[pedalNumber].off_len;
  }

  uint8_t buffer[length];
  memcpy(buffer, packet, length);

  pRemoteCharacteristic->writeValue(buffer, length, true);

  Serial.print("PEDAL ");
  Serial.print(pedalNumber);
  Serial.print(ligar ? "TURNED-ON" : " TURNED-OFF");
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  Serial.println("STARTING GUITAR CONTROLL OVER BLE");

  for (int i = 0; i < 4; i++) pinMode(presetButtons[i], INPUT_PULLUP);
  for (int i = 0; i < 5; i++) pinMode(pedalButtons[i], INPUT_PULLUP);

 
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(4, LOW);


  BLEDevice::init("ESP32-GUITAR-CONTROLLER");


  while (!connectToServer()) {
    Serial.println("TRYING TO RECONNECT TO SERVER...");
    delay(5000);
  }

  digitalWrite(4, HIGH);
}

void loop() {
  if (!pClient->isConnected()) {
    digitalWrite(4, LOW);
    Serial.println("BLE CONNECTION LOST - RECONNECTING...");
    if (!connectToServer()) {
      delay(5000);
      return;
    }
    digitalWrite(4, HIGH);
  }

  int presetSelecionado = -1;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(presetButtons[i]) == LOW) {
      presetSelecionado = i;
      break; 
    }
  }

  if (presetSelecionado == -1) {
    presetSelecionado = 1;
  }

  if (presetSelecionado != currentPreset) {
    sendPreset(presetSelecionado);
    Serial.print("PRESET ");
    Serial.print(presetSelecionado);
    Serial.println(" SELECTED");
    currentPreset = presetSelecionado;
  }

  for (int i = 0; i < 5; i++) {
    bool state = digitalRead(pedalButtons[i]);
    if (state == LOW && lastPedalButtonState[i] == HIGH) {
      pedalStates[i] = !pedalStates[i]; 
      sendPedal(i, pedalStates[i]);
    }
    lastPedalButtonState[i] = state;
  }

  delay(50);  
}
