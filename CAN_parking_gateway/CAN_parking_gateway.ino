#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
MCP2515 mcp2515_2(4);

struct can_frame canMsg2;
MCP2515 mcp2515_1(5);

//везде провод с узлом - плюсовой или CAN-H
//провод с 1 узлом - ММ шина
//провод с 2 узлами - MS шина
//провод без узлов - питание от синего провода магнитолы

//Байт, который хотим перехватить
canid_t forRead = 0x131;
struct can_frame canTx;

bool isDebugMode = true;


void setup() {
  Serial.begin(230400);

  delay(500);

  SPI.setClockDivider(SPI_CLOCK_DIV2); 
  SPI.begin();

  mcp2515_1.reset();
  mcp2515_1.setBitrate(CAN_125KBPS, MCP_8MHZ);

  /*mcp2515_1.setConfigMode();
  mcp2515_1.setFilterMask(MCP2515::MASK0, false, 0x7FF);
  mcp2515_1.setFilter(MCP2515::RXF0, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF1, false, 0x131);
  mcp2515_1.setFilterMask(MCP2515::MASK1, false, 0x7FF);
  mcp2515_1.setFilter(MCP2515::RXF2, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF3, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF4, false, 0x131);
  mcp2515_1.setFilter(MCP2515::RXF5, false, 0x131);*/

  mcp2515_1.setNormalMode();

  mcp2515_2.reset();
  mcp2515_2.setBitrate(CAN_125KBPS, MCP_8MHZ);

  /*mcp2515_2.setConfigMode();
  mcp2515_2.setFilterMask(MCP2515::MASK0, false, 0x7FF);
  mcp2515_2.setFilter(MCP2515::RXF0, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF1, false, 0x131);
  mcp2515_2.setFilterMask(MCP2515::MASK1, false, 0x7FF);
  mcp2515_2.setFilter(MCP2515::RXF2, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF3, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF4, false, 0x131);
  mcp2515_2.setFilter(MCP2515::RXF5, false, 0x131);*/

  mcp2515_2.setNormalMode();


}

void loop() {
  canMsg2.can_id  = 0x0F6;
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x20;
  canMsg2.data[1] = 0x21;
  canMsg2.data[2] = 0x22;
  canMsg2.data[3] = 0x22;
  canMsg2.data[4] = 0x23;
  canMsg2.data[5] = 0x20;
  canMsg2.data[6] = 0x20;
  canMsg2.data[7] = 0x27;
  mcp2515_2.sendMessage(&canMsg2);
  delay(1);
  readCan1();
}

//Читаем MS шину
void readCan1() {
  if (mcp2515_1.readMessage(&canMsg1) == MCP2515::ERROR_OK) {
    //Находим нужный нам пакет с id 131
    //printMessage("CAN1", &canMsg1);
    //    if (canMsg1.can_id == forRead) {
    if (isDebugMode) {
      send_data(0, &canMsg1);
    }



    /*if (canMsg1.data[0] == 0xD3) {
      if(canMsg1.data[7] != 0 or canMsg1.data[6] != 0  or canMsg1.data[5] != 0 )
      {
        tone(6, 550, 1000);

      }
    }*/

    //Записываем пакет с ID 131 в MM шину.
    ///mcp2515_2.sendMessage(&canMsg1);
    //    }
  }
}

void readCan2() {
  if (isDebugMode && mcp2515_2.readMessage(&canMsg2) == MCP2515::ERROR_OK) {
    send_data("CAN2", &canMsg2);
  }
}


void send_data(uint8_t canName, can_frame* canMsg)
{
  Serial.write(canName);

  Serial.write(canMsg->can_id>>24);
  Serial.write((canMsg->can_id>>16)&0xff);
  Serial.write((canMsg->can_id>>8)&0xff);
  Serial.write((canMsg->can_id)&0xff);

  Serial.write(canMsg->can_dlc);

  for (int i = 0; i < 8; i++)  { // print the data

    Serial.write(canMsg->data[i]);
  }

  Serial.println();
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
