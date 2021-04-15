/*
 * Show two values => LED1 on => wait for S1 pressed => sound => LED off => repeat
 */

#include <MultiFunctionShield.h>

void setup ()
{}
 
void loop()
{
  uint8_t i;
  
  MultiFunctionShield MFS;
  MFS.begin();
  pinMode (BUTTON_1_PIN, INPUT);    // insert J2 on board for pull-ups!
  digitalWrite(LED_1_PIN, HIGH);    // off
  pinMode (LED_1_PIN, OUTPUT);  
  
  while (1)
  {
    MFS.Display (1234);
    delay (2000);
    MFS.Display (-56);
    delay (2000);
    
    digitalWrite(LED_1_PIN, LOW);    
    while (digitalRead(BUTTON_1_PIN));     // wait until S1 (low active)
    
    for (i = 0; i < 10; i++)    
    {
      digitalWrite(BUZZER_PIN, LOW);
      delay (50);
      digitalWrite(BUZZER_PIN, HIGH);
      delay (10);
    }
    digitalWrite(LED_1_PIN, HIGH);    
  }  
}
