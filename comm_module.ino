//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------
bool processReceivedMessage(const char* message);
void prepareMessageToSend(char* message);

//= VARIABLES ======================================================================================
RtznCommProtocol commProto("NANO_ADC-WORKER", &processReceivedMessage, &prepareMessageToSend);

//==================================================================================================
//**************************************************************************************************
void comm_Setup() {
#ifdef UseCOMM
#ifdef DEBUG
  Serial.println("COMM:Setup >>>");
#endif
  //..............................
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  while (!Serial) { ; }
  //..............................
#ifdef DEBUG
  Serial.println("COMM:Setup <<<");
#endif
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActIfReceivedMessage() {
#ifdef UseCOMM
  // NO MESSAGES - I'm not listening
#endif
}
//==================================================================================================
void comm_ActOnNewDataToSend() {
#ifdef UseCOMM
  commProto.actOnPollMessage();
#endif
}
//==================================================================================================
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
#ifdef UseCOMM
  //------------------------------------
  // NO LISTENING
  //------------------------------------
#endif
  return haveToPublish;
}
//==================================================================================================
void prepareMessageToSend(char* message) {
  //commProto.purgeDataLine(2048, true);

#ifdef UseCOMM
  int CHAR_COUNT = 2;
  //Serial.println("");
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    int value = voltage_avg[pinId];
    byte byte1 = round(value / 100);
    byte byte2 = (value - (round(value / 100) * 100));

    //Serial.print(byte1);Serial.print(" | ");Serial.println(byte2);

    message[pinId * CHAR_COUNT] = (char)(byte1 + byte('0'));
    message[pinId * CHAR_COUNT + 1] = (char)(byte2 + byte('0'));
  }
  //Serial.println("");
#ifdef DEBUG
  Serial.println(message);
#endif
#endif
}
//==================================================================================================