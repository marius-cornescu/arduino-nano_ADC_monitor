/*
  PIN CONNECTIONS
  -------------------------------
  A0 - A7 - analog input pins
  -------------------------------
  D1 -> RX
  D0 -> TX
  -------------------------------
*/
//= DEFINES ========================================================================================
//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DEBUG
//#define UseIoT

#define SEC 1000  // 1 second

//= INCLUDES =======================================================================================
#include <Wire.h>

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
//------------------------------------------------
const float OPERATING_VOLTAGE = 5.0;
const byte ANALOG_PIN_COUNT = 8;    // Arduino NANO has 8 analog pins
//
#ifdef DEBUG
const byte TIME_TICK = 100;
#else
const byte TIME_TICK = 10;
#endif
//------------------------------------------------

//= VARIABLES ======================================================================================
byte analog_pin[ANALOG_PIN_COUNT] = {A0, A1, A2, A3, A4, A5, A6, A7};
int voltage[ANALOG_PIN_COUNT];

//##################################################################################################
//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println("START-UP >>>");
#endif
  //..............................
  // initialize digital pin LED_INDICATOR_PIN as an output.
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  //
  adc_Setup();
  //..............................
#ifdef DEBUG
  Serial.println(">>> START-UP");
#endif
}
//**************************************************************************************************
void adc_Setup() {
#ifdef DEBUG
  Serial.println("NANO-ADC:Setup >>>");
#endif
  //..............................
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    pinMode(analog_pin[pinId], INPUT);
    voltage[pinId] = 0;
  }
  //..............................
  iot_Setup();
  //..............................
#ifdef DEBUG
  Serial.println(">>> NANO-ADC:Setup");
#endif
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  digitalWrite(LED_INDICATOR_PIN, LOW);
  //..............................
  //
  _readAnalogPinData();
  //
  _publishAnalogPinData();
  //..............................
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  //
  delay(10 * TIME_TICK);
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
void _readAnalogPinData() {
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    int raw_analog = analogRead(analog_pin[pinId]); // integer values between 0 and 1023
    float measured_voltage = _computeVoltage(raw_analog);
    pinMode(analog_pin[pinId], INPUT);
    voltage[pinId] = _asMilliVolts(measured_voltage);
  }
}
//==================================================================================================
void _publishAnalogPinData() {
#ifdef DEBUG
  Serial.println("---------------------------------------");
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    Serial.print("Pin [A");
    Serial.print(pinId);
    Serial.print("] = ");
    Serial.print(voltage[pinId]);
    Serial.println(" mV");
  }
  Serial.println("---------------------------------------");
#endif
}
//==================================================================================================
int _asMilliVolts(float measured_voltage) {
  return round(measured_voltage * 1000);  // round to milli-volts
}
//==================================================================================================
float _computeVoltage(int raw_analog_value) {
  float raw_voltage = raw_analog_value * OPERATING_VOLTAGE / 1024.0;
  // read correct supply voltage
  float supply_voltage = _readVcc() / 1000.0;
  float corrected_voltage = supply_voltage / OPERATING_VOLTAGE * raw_voltage;
  return corrected_voltage;
}
//==================================================================================================
long _readVcc() {
  long result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
  return result;
}
//==================================================================================================