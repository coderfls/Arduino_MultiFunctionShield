/*
 * Florian Schäffer
 * https://www.blafusel.de
 * 
 * https://github.com/coderfls/Arduino_MultiFunctionShield
 * 
 * https://www.heise.de/select/make/2018/2/1524540425550478
 * https://heise.de/-4031506
 */

#include <MultiFunctionShield.h>
#ifdef ARDUINO_R4
#include "FspTimer.h"
FspTimer display_timer;
#endif

const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};    // Segmente, die leuchten sollen pro Zahlwert (Low-Aktiv), & 0x7F Verknüpfen fuer Dezimalpunkt
const uint8_t SEGMENT_BLANK = 0xFF;
const uint8_t SEGMENT_MINUS = 0xBF;
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};                               // Ziffernposition (gemeinsame Anode, LSB)
volatile uint8_t ActDigit = 0;
volatile uint8_t SEGMENT_VALUE[4];

static MultiFunctionShield *instance;

MultiFunctionShield::MultiFunctionShield(void)  //constructor
{
  instance = this;
}

#ifdef ARDUINO_R4
// callback method used by timer
void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  instance->ISRFunc();
}
bool beginTimer(float rate) {
  uint8_t timer_type = GPT_TIMER;
  int8_t tindex = FspTimer::get_available_timer(timer_type);
  if (tindex < 0){
    tindex = FspTimer::get_available_timer(timer_type, true);
  }
  if (tindex < 0){
    return false;
  }

  FspTimer::force_use_of_pwm_reserved_timer();

  if(!display_timer.begin(TIMER_MODE_PERIODIC, timer_type, tindex, rate, 0.0f, timer_callback)){
    return false;
  }

  if (!display_timer.setup_overflow_irq()){
    return false;
  }

  if (!display_timer.open()){
    return false;
  }

  if (!display_timer.start()){
    return false;
  }
  return true;
}
#else
ISR(TIMER1_COMPA_vect)          // interrupt service routine 
{
  instance->ISRFunc();
} 
#endif

void MultiFunctionShield::begin(void)
{
#ifdef ARDUINO_R4
  pinMode(BUZZER_PIN, OUTPUT);  
  digitalWrite(BUZZER_PIN, HIGH);   // second! else forever sound
#else
  digitalWrite(BUZZER_PIN, HIGH);   // first! else short sound
  pinMode(BUZZER_PIN, OUTPUT);                                    
#endif

  pinMode(LATCH_PIN,OUTPUT);
  pinMode(CLK_PIN,OUTPUT);
  pinMode(DATA_PIN,OUTPUT);
    
#ifdef ARDUINO_R4
	beginTimer(1000); // 1kHz
#else
  TCCR1A = 0;                                           // Register loeschen
  OCR1A = 1000;                                         // Vergleichswert x = (CPU / (2 x Teiler x f)) - 1
  TCCR1B |= (1 << CS10) | (1 << CS11) | (1 << WGM12);   // CTC-Mode, Teiler = 64
  TIMSK1 |= (1 << OCIE1A);                              // Output Compare A Match Interrupt Enable
  sei();                                                // IRQ Enable
#endif

  Clear();
}

void MultiFunctionShield::Display (int16_t value)
{
  if ((value > 9999) || (value < -999))   // out of range
  {
    SEGMENT_VALUE[0] = SEGMENT_MINUS;
    SEGMENT_VALUE[1] = SEGMENT_MINUS;
    SEGMENT_VALUE[2] = SEGMENT_MINUS;
    SEGMENT_VALUE[3] = SEGMENT_MINUS;
  }
  else    // possible range
  {
    if (value >= 0)   // positive values
    {
      if (value > 999)
        SEGMENT_VALUE[0] = SEGMENT_MAP [(uint8_t) (value / 1000)];
      else
        SEGMENT_VALUE[0] = SEGMENT_BLANK;
    
      if (value > 99)
        SEGMENT_VALUE[1] = SEGMENT_MAP [(uint8_t) ((value / 100) % 10)];
      else
        SEGMENT_VALUE[1] = SEGMENT_BLANK;
    
      if (value > 9) 
        SEGMENT_VALUE[2] = SEGMENT_MAP [(uint8_t) ((value / 10) % 10)];
      else
        SEGMENT_VALUE[2] = SEGMENT_BLANK;
    
      SEGMENT_VALUE[3] = SEGMENT_MAP [(uint8_t) (value % 10)];
      
    }
    if (value < 0)      // negative values: "-" left
    {
      value *= -1;
      SEGMENT_VALUE[0] = SEGMENT_MINUS;
    
      if (value > 99)
        SEGMENT_VALUE[1] = SEGMENT_MAP [(uint8_t) ((value / 100) % 10)];
      else
        SEGMENT_VALUE[1] = SEGMENT_BLANK;
    
      if (value > 9) 
        SEGMENT_VALUE[2] = SEGMENT_MAP [(uint8_t) ((value / 10) % 10)];
      else
        SEGMENT_VALUE[2] = SEGMENT_BLANK;
    
      SEGMENT_VALUE[3] = SEGMENT_MAP [(uint8_t) (value % 10)];
    }
  }
}

void MultiFunctionShield::Clear(void) 
{
  SEGMENT_VALUE[0] = SEGMENT_BLANK;
  SEGMENT_VALUE[1] = SEGMENT_BLANK;
  SEGMENT_VALUE[2] = SEGMENT_BLANK;
  SEGMENT_VALUE[3] = SEGMENT_BLANK;
}

void MultiFunctionShield::WriteNumberToSegment(uint8_t digit)
{
  digitalWrite(LATCH_PIN,LOW);                                      // Uebernahme-Takt: Ausgang Aus
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, SEGMENT_VALUE[digit]);      // Segmente passend zum Zahlwert rausschieben
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, SEGMENT_SELECT[digit]);     // Welche Stelle leuchten soll hinterher schieben
  digitalWrite(LATCH_PIN,HIGH);                                     // Uebernahme-Takt: Ausgang Ein
}

void MultiFunctionShield::ISRFunc(void) 
{
  switch (++ActDigit)
  {
    case 1 : WriteNumberToSegment(0); break;     
    case 2 : WriteNumberToSegment(1); break; 
    case 3 : WriteNumberToSegment(2); break; 
    case 4 : WriteNumberToSegment(3); ActDigit = 0; break; 
  }
}
    