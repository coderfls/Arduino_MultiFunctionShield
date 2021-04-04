#include <MultiFunctionShield.h>

volatile uint16_t DisplayValue = 0;

MultiFunctionShield::MultiFunctionShield()  //constructor
{
}

void MultiFunctionShield::begin(void)
{
  pinMode(LATCH,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(DATA,OUTPUT);

  TCCR1A = 0;                                           // Register loeschen
  OCR1A = 1000;                                         // Vergleichswert x = (CPU / (2 x Teiler x f)) - 1
  TCCR1B |= (1 << CS10) | (1 << CS11) | (1 << WGM12);   // CTC-Mode, Teiler = 64
  TIMSK1 |= (1 << OCIE1A);                              // Output Compare A Match Interrupt Enable
  sei();                                                // IRQ Enable
}

void MultiFunctionShield::Display (uint16_t val)
{
  DisplayValue = val;
}

void WriteNumberToSegment(uint8_t digit, uint8_t value)
{
  digitalWrite(LATCH,LOW);                                // Uebernahme-Takt: Ausgang Aus
  shiftOut(DATA, CLK, MSBFIRST, SEGMENT_MAP[value]);      // Segmente passend zum Zahlwert rausschieben
  shiftOut(DATA, CLK, MSBFIRST, SEGMENT_SELECT[digit]);   // Welche Stelle leuchten soll hinterher schieben
  digitalWrite(LATCH,HIGH);                               // Uebernahme-Takt: Ausgang Ein
}

ISR(TIMER1_COMPA_vect)          // interrupt service routine 
{
  switch (++ActDigit)
  {
    case 1 : WriteNumberToSegment(0, DisplayValue / 1000); break;     
    case 2 : WriteNumberToSegment(1, (DisplayValue / 100) % 10); break; 
    case 3 : WriteNumberToSegment(2, (DisplayValue / 10) % 10); break; 
    case 4 : WriteNumberToSegment(3, DisplayValue % 10); ActDigit = 0; break; 
  }
} 
