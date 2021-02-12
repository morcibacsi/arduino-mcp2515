#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;

SPIClass *spi;
MCP2515 *mcp2515;

uint8_t CS_PIN = 10;

void setup() {
  Serial.begin(115200);

  spi = new SPIClass();

  #ifdef ARDUINO_ARCH_AVR
    // start spi on Arduino boards
    spi->begin();
  #endif

  #ifdef ARDUINO_ARCH_ESP32
      // You can select which pins to use as SPI on ESP32 boards by passing the SCK, MISO, MOSI, SS as arguments (in this order) to the spi->begin() method
      const uint8_t SCK_PIN = 25;
      const uint8_t MISO_PIN = 5;
      const uint8_t MOSI_PIN = 33;
      CS_PIN = 32;

      spi->begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  #endif
  
  mcp2515 = new MCP2515(10, spi);
  mcp2515->reset();
  mcp2515->setBitrate(CAN_125KBPS);
  mcp2515->setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  if (mcp2515->readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      Serial.print(canMsg.data[i],HEX);
      Serial.print(" ");
    }

    Serial.println();      
  }
}
