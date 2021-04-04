#ifndef MultiFunctionShield_h
  #define MultiFunctionShield_h

#if not defined (__AVR_ATmega328P__)  // Arduino UNO, NANO
  #warning Library only for Arduino Uno, Nano and so on with ATmega328
#endif

#include "Arduino.h"
#include <avr/interrupt.h>

class MultiFunctionShield
{
  public:
    MultiFunctionShield();
    void begin(void);
    void Display (uint16_t val);

  private:
    void WriteNumberToSegment(uint8_t digit, uint8_t value);
};

static const uint8_t LATCH = 4;
static const uint8_t CLK = 7;
static const uint8_t DATA = 8;
static const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};    // Segmente, die leuchten sollen pro Zahlwert (Low-Aktiv), & 0x7F Verknüpfen fuer Dezimalpunkt
static const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};                               // Ziffernposition (gemeinsame Anode, LSB)

volatile uint8_t ActDigit = 0;

#endif
