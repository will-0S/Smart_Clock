 #include <BluetoothSerial.h>
 
 //Create a BluetoothSerial object
 BluetoothSerial SerialBT;
 
 void setup() {
   // Start serial communication at 115200 baud rate
   Serial.begin(115200);
   
   // Start Bluetooth serial communication with the device name "ESP32"
   SerialBT.begin("smartcock"); 
   Serial.println("The device started, now you can pair it with Bluetooth!");
 }
 
 void loop() {
   // Check if there is an incoming message
   if (SerialBT.available()) {
     // Read the incoming message
     String incomingMessage = SerialBT.readString();
 
     // Print the incoming message to the serial monitor
     Serial.print("Received: ");
     Serial.println(incomingMessage);
 
    // Check if the incoming message is "hello"
    incomingMessage.trim(); // Remove any leading/trailing whitespace or newline characters

    if (incomingMessage == "hello") {
      // Print "Hello World" to the serial monitor
      Serial.println("Hello World");
    } else {
      Serial.println("Message did not match 'hello'");
    }
   }
   
   // Small delay to prevent overwhelming the serial output
   delay(20);
 }

/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
  Ported to Arduino ESP32 by Evandro Copercini
*/
//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEServer.h>
//
//#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println("Starting BLE work!");
//
//  BLEDevice::init("SmartCock");
//  
//  BLEServer *pServer = BLEDevice::createServer();
//  BLEService *pService = pServer->createService(SERVICE_UUID);
//  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//                                         CHARACTERISTIC_UUID,
//                                         BLECharacteristic::PROPERTY_READ |
//                                         BLECharacteristic::PROPERTY_WRITE
//                                       );
//
//  pCharacteristic->setValue("Hello World says the clock");
//  pService->start();
//
//  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//  pAdvertising->addServiceUUID(SERVICE_UUID);
//  pAdvertising->setScanResponse(true);
//  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
//  pAdvertising->setMinPreferred(0x12);
//  BLEDevice::startAdvertising();
//  Serial.println("Characteristic defined! Now you can read it in your phone!");
//}
//
//void loop() {
//  delay(2000);
//}
