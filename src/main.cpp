#include <Arduino.h>
#include <SPI.h>
#include "display.h"



void setup() {
  display_pwr(0);
  display_init();
  display_set("1234");

}


void loop() {
  
}