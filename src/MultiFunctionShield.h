#ifndef MultiFunctionShield_h
  #define MultiFunctionShield_h

// TODO: better define
#define ARDUINO_R4 1

#ifdef ARDUINO_R4

#else
#ifndef __AVR_ATmega328P__  // Arduino UNO, NANO
  #error : Library only for Arduino Uno, Nano and so on with ATmega328
#endif
#include <avr/interrupt.h>
#endif

#include "Arduino.h"

const uint8_t LED_1_PIN=13;
const uint8_t LED_2_PIN=12;
const uint8_t LED_3_PIN=11;
const uint8_t LED_4_PIN=10;
const uint8_t POT_PIN=0;
const uint8_t BUZZER_PIN=3;
const uint8_t BUTTON_1_PIN=A1;
const uint8_t BUTTON_2_PIN=A2;
const uint8_t BUTTON_3_PIN=A3;
const uint8_t LATCH_PIN=4;
const uint8_t CLK_PIN=7;
const uint8_t DATA_PIN=8;
const uint8_t LM35_PIN=A4;

class MultiFunctionShield
{
  public:
    /**
      @brief  Constructor
      @param  none
      @return none
    */
    MultiFunctionShield(void);

    /**
      @brief  Initialization
      @param  none
      @return none
    */
    void begin(void);

    /**
      @brief  Write a given int value on the display (in the next IRQ)
      @param  value
      @return none
    */
    void Display (int16_t);

    /**
      @brief  Clears the display ("    ");
      @param  none
      @return none
    */
    void Clear (void);

    /**
      @brief  Called by ISR. Do not use
      @param  none
      @return none
    */
    void ISRFunc(void);

  private:
    /**
      @brief  Writes the value for one segment to the display. The value is saved in a lokal variable
      @param  Display position (0..3, 0=left)
      @return none
    */
     void WriteNumberToSegment(uint8_t);
};

#endif
