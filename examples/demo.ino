#include <MultiFunctionShield.h>

void setup ()
{}
 
void loop()
{
  MultiFunctionShield MFS;
  MFS.begin();
  
  while (1)
  {
    MFS.Display (1234);
    delay (2000);
    MFS.Display (5678);
    delay (2000);
  }  
}
