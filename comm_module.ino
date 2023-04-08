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
  Serial.println(">>> COMM:Setup");
#endif
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActIfActivity() {
#ifdef UseCOMM
  if (commProto.hasMessageInInboxThenReadMessage()) {
    commProto.actOnMessage();
    if (commProto.isHaveToPublish()) {
      __actOnPartnerDataChanged();
    }
  }
#endif
}
//==================================================================================================
void comm_actOnNewAction(/*const Action* action*/) {
#ifdef UseCOMM
  //currentActionCode = action->actionCode;

  commProto.actOnPollMessage();
#endif
}
//==================================================================================================
void __actOnPartnerDataChanged() {
#ifdef UseCOMM
  // new data came in
#endif
}
//==================================================================================================
bool processReceivedMessage(const char* message) {
#ifdef UseCOMM
  bool haveToPublish = false;
  //------------------------------------
  byte newVentSpeed = message[0] - (byte)'0';
  // ignore - the meat is in the action
  //------------------------------------
  byte newActionCode = message[1] - (byte)'0';
  if (currentActionCode != newActionCode) {
    if (newActionCode > ACTION_NOP) {
      currentActionCode = newActionCode;
      haveToPublish = true;
    }
  }
  //------------------------------------
  return haveToPublish;
#endif
}
//==================================================================================================
const char* prepareMessageToSend() {
#ifdef UseCOMM
  char* message = new char[4];
  memset(message, 0, 4);
  //message[0] = currentVentSpeed + (byte)'0';
  message[1] = currentActionCode + (byte)'0';

  return message;
#endif
}
//==================================================================================================