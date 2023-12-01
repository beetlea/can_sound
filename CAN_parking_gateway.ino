#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
MCP2515 mcp2515_2(10);

struct can_frame canMsg2;
MCP2515 mcp2515_1(9);

//везде провод с узлом - плюсовой или CAN-H
//провод с 1 узлом - ММ шина
//провод с 2 узлами - MS шина
//провод без узлов - питание от синего провода магнитолы

//Байт, который хотим перехватить
canid_t forRead = 0x131;
struct can_frame canTx;

bool isDebugMode = true;


void setup() {
  Serial.begin(115200);
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
  delay(500);

  SPI.begin();

  mcp2515_1.reset();
  mcp2515_1.setBitrate(CAN_125KBPS, MCP_8MHZ);

  mcp2515_1.setConfigMode();
  mcp2515_1.setFilterMask(MCP2515::MASK0, false, 0x7FF);
  mcp2515_1.setFilter(MCP2515::RXF0, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF1, false, 0x131);
  mcp2515_1.setFilterMask(MCP2515::MASK1, false, 0x7FF);
  mcp2515_1.setFilter(MCP2515::RXF2, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF3, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF4, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF5, false, 0x131);

  mcp2515_1.setNormalMode();

  mcp2515_2.reset();
  mcp2515_2.setBitrate(CAN_125KBPS, MCP_8MHZ);

  mcp2515_2.setConfigMode();
  mcp2515_2.setFilterMask(MCP2515::MASK0, false, 0x7FF);
  mcp2515_2.setFilter(MCP2515::RXF0, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF1, false, 0x131);
  mcp2515_2.setFilterMask(MCP2515::MASK1, false, 0x7FF);
  mcp2515_2.setFilter(MCP2515::RXF2, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF3, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF4, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF5, false, 0x131);

  mcp2515_2.setNormalMode();


}

void loop() {
  readCan1();
}

//Читаем MS шину
void readCan1() {
  if (mcp2515_1.readMessage(&canMsg1) == MCP2515::ERROR_OK) {
    //Находим нужный нам пакет с id 131
    //printMessage("CAN1", &canMsg1);
    //    if (canMsg1.can_id == forRead) {
    if (isDebugMode) {
      printMessage("CAN1", &canMsg1);
    }



    if (canMsg1.data[0] == 0xD3) {
      if(canMsg1.data[7] != 0 or canMsg1.data[6] != 0  or canMsg1.data[5] != 0 )
      {
        tone(6, 550, 1000);

      }
    }

    //Записываем пакет с ID 131 в MM шину.
    ///mcp2515_1.sendMessage(&canMsg1);
    //    }
  }
}

void readCan2() {
  if (isDebugMode && mcp2515_2.readMessage(&canMsg2) == MCP2515::ERROR_OK) {
    printMessage("CAN2", &canMsg2);
  }
}

void printMessage(String canName, can_frame* canMsg) {
  Serial.print(canName);
  Serial.print(" Data: ");
  Serial.print(canMsg->can_id, HEX); // print ID
  Serial.print(" ");
  Serial.print(canMsg->can_dlc, HEX); // print DLC
  Serial.print(" ");

  for (int i = 0; i < canMsg->can_dlc; i++)  { // print the data
    Serial.print(canMsg->data[i], HEX);
    Serial.print(" ");
  }

  Serial.println();
}
