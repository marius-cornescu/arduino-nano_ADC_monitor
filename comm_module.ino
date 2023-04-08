//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------
bool processReceivedMessage(const char* message);
const char* prepareMessageToSend();

//= VARIABLES ======================================================================================
RtznCommProtocol commProto("NANO_ADC-WORKER", &processReceivedMessage, &prepareMessageToSend);

byte currentActionCode = 0;

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
void comm_ActIfActivity() {
#ifdef UseCOMM
  // NO MESSAGES - I'm not listening
#endif
}
//==================================================================================================
void comm_actOnNewData() {
#ifdef UseCOMM
  commProto.actOnPollMessage();
#endif
}
//==================================================================================================
bool processReceivedMessage(const char* message) {
#ifdef UseCOMM
  bool haveToPublish = false;
  //------------------------------------
  // NO LISTENING
  //------------------------------------
  return haveToPublish;
#endif
}
//==================================================================================================
const char* prepareMessageToSend() {
  //commProto.purgeDataLine(2048, true);

#ifdef UseCOMM
  int CHAR_COUNT = 4;
  int message_size = (CHAR_COUNT * ANALOG_PIN_COUNT) + 1;  // 33 bytes for 8 pins
  char* message = new char[message_size];
  memset(message, 0, message_size);
  //Serial.println("");
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    char voltage_string[CHAR_COUNT + 1];
    sprintf(voltage_string, "%04d", voltage_avg[pinId]);
    //Serial.print(voltage_string); Serial.print(" | ");
    memcpy(&message[pinId * CHAR_COUNT], voltage_string, CHAR_COUNT);
  }
  //Serial.println("");
#ifdef DEBUG
  Serial.println(message);
#endif
  return message;
#endif
}
//==================================================================================================