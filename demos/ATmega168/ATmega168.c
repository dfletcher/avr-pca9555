/*-----------------------------------------------------------------------------

   PCA9555 AVR Driver
   Copyright 2010 Dave Fletcher
   <fletch@fletchtronics.net> http://fletchtronics.net

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. 

NOTE: The PCA9555 driver code and demos are distributed under the terms of the
Apache 2.0 software license. The other pieces of this package including Dean
Camera's LUFA and Peter Fleury's I2C Master are redistributed under the
respective authors' terms.

-----------------------------------------------------------------------------*/

#include "PCA9555.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define DEMO_LED_PORT  PORTC
#define DEMO_LED_DDR   DDRC
#define DEMO_LED_PIN   PC2
#define DEMO_PCMSK     PCMSK1
#define DEMO_PCINT     PCINT11
#define DEMO_GIMSK     PCICR
#define DEMO_PCIE      PCIE1
#define DEMO_VECT      PCINT1_vect

uint8_t pca9555interrupt = 0; // global PCA9555 INT detector
SIGNAL(DEMO_VECT) { pca9555interrupt = 1; } // PCA9555 INT changed

int main(void) {
  int pca9555val = 0; // value for port 0
  MCUSR &= ~(1 << WDRF); // disable watchdog
  wdt_disable(); // disable watchdog
  clock_prescale_set(clock_div_1); // disable prescalar (8Mhz)
  DEMO_LED_DDR |= (1 << DEMO_LED_PIN); // DEMO_LED_PIN output
  PCA9555_init(); // start up
  PCA9555_dir(PCA9555_DEV_000, PCA9555_PORT_0, 0b00000000); // port 0 output
  PCA9555_dir(PCA9555_DEV_000, PCA9555_PORT_1, 0b11111111); // port 1 input
  DEMO_PCMSK |= (1<<DEMO_PCINT); // enable DEMO_PCINT, connect to PCA9555 INT
  DEMO_GIMSK |= (1<<DEMO_PCIE); // enable PCIE0, the PCINT0 interrupt vector
  sei(); // enable interrupts
  while (1) {
    _delay_ms(5); // wait a bit so the counting on output goes slow
    PCA9555_set(PCA9555_DEV_000, PCA9555_PORT_0, pca9555val++); // count
    if (pca9555interrupt) {
      uint8_t value = PCA9555_get(PCA9555_DEV_000, PCA9555_PORT_1); // read
      // Do something useful with 'value' here.
	  DEMO_LED_PORT |= (1 << DEMO_LED_PIN);
      for (uint8_t i = 0; i < 20; i++) _delay_ms(30); // flash the LED
	  DEMO_LED_PORT &= ~(1 << DEMO_LED_PIN);
      pca9555interrupt = 0; // reset so we only process the interrupt once
    }
  }
  return 0;
}
