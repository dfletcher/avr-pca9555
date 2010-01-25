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

/**
 @defgroup fletchtronics_pca9555 PCA9555 Driver
 @code #include <PCA9555.h> @endcode
  
 @brief PCA9555 Driver

 AVR driver for PCA9555. Wraps all PCA9555 functions in a simple C interface.

 The PCA9555 driver requres Peter Fleury's I2C master library, which is
 included in this package. AVRs with a TWI interface can replace the
 i2cmaster.S source in the makefile with twimaster.c, which uses the AVR's i2c
 hardware. If using the software i2cmaster.S version, configure the I2C pins
 at the top of the i2cmaster.S file.

 An example schematic, Cadsoft Eagle files, and other details can be found at:
 http://fletchtronics.net/pca9555

 @author Dave Fletcher <fletch@fletchtronics.net>  http://fletchtronics.net

 @par API Usage Example
  The following code shows typical usage of this library. This example uses the
  PCINT interrupt structure for at90usb162. There are more examples in the root
  directory of this distribution.

 @code
  #include <PCA9555.h>
  #include <util/delay.h>
  #include <avr/interrupt.h>

  uint8_t pca9555interrupt = 0; // global PCA9555 INT detector
  SIGNAL(PCINT1_vect) { pca9555interrupt = 1; } // PCA9555 INT changed

  int main(void) {
    int pca9555val = 0; // value for port 0
    PCA9555_init(); // start up
    PCA9555_dir(PCA9555_DEV_000, PCA9555_PORT_0, 0b00000000); // port 0 output
    PCA9555_dir(PCA9555_DEV_000, PCA9555_PORT_1, 0b11111111); // port 1 input
    PCMSK1 |= (1<<PCINT11); // enable PCINT11, connect PCA9555 INT to it
    PCICR |= (1<<PCIE1); // enable PCIE1, the PCINT1 interrupt vector
    sei(); // enable interrupts
    while (1) {
      _delay_ms(10); // wait a bit so the counting on output goes slow
      PCA9555_set(PCA9555_DEV_000, PCA9555_PORT_0, pca9555val++); // count
      if (pca9555interrupt) {
        uint8_t value = PCA9555_get(PCA9555_DEV_000, PCA9555_PORT_1); // read
        // Do something useful with 'value' here.
        pca9555interrupt = 0; // reset so we only process the interrupt once
      }
      return 0;
    }
  }
 @endcode

*/

/**@{*/

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

#ifndef PCA9555
#define PCA9555

#include <inttypes.h>

#define PCA9555_DEVICE_ADDRESS 0x40

/**
 *  Eight possible device addresses. The rightmost (least significant)
 *  bit is A0 on the device.
 */
typedef enum {
  PCA9555_DEV_000               = 0b000,
  PCA9555_DEV_001               = 0b001,
  PCA9555_DEV_010               = 0b010,
  PCA9555_DEV_011               = 0b011,
  PCA9555_DEV_100               = 0b100,
  PCA9555_DEV_101               = 0b101,
  PCA9555_DEV_110               = 0b110,
  PCA9555_DEV_111               = 0b111,
} PCA9555_DEVICE;

/**
 *  Two ports on a PCA9555, port 0 and port 1.
 */
typedef enum {
  PCA9555_PORT_0                = 0,
  PCA9555_PORT_1                = 1,
} PCA9555_PORT;

/**
 * Registers in a PCA9555.
 */
typedef enum {
  PCA9555_INPUT_0               = 0,
  PCA9555_INPUT_1               = 1,
  PCA9555_OUTPUT_0              = 2,
  PCA9555_OUTPUT_1              = 3,
  PCA9555_POLARITY_INV_0        = 4,
  PCA9555_POLARITY_INV_1        = 5,
  PCA9555_DIRECTION_0           = 6,
  PCA9555_DIRECTION_1           = 7,
} PCA9555_REGISTER;

/**
 *  Initialize the PCA9555 driver.
 */
void PCA9555_init(void);

/**
 *  Low level write command. Uses i2cmaster library to communicate with the
 *  device. Calls i2c_start_wait, which waits for ACK from the device, so if
 *  the specified device is not on the i2c bus, it will wait forever.
 *  @param dev 3 bit device address.
 *  @param reg Register to write.
 *  @param value Value to write.
 */
void PCA9555_write(PCA9555_DEVICE dev, PCA9555_REGISTER reg, uint8_t value);

/**
 *  Low level read command. Uses i2cmaster library to communicate with the
 *  device. Calls i2c_start_wait, which waits for ACK from the device, so if
 *  the specified device is not on the i2c bus, it will wait forever.
 *  @param dev 3 bit device address.
 *  @param reg Register to read.
 *  @return value Value read from the device.
 */
uint8_t PCA9555_read(PCA9555_DEVICE dev, PCA9555_REGISTER reg);

/**
 *  Set the direction of a port. Implemented as a macro.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @param dir 8 bit direction of the pins. 1 means input, 0 output.
 */
void PCA9555_dir(PCA9555_DEVICE dev, PCA9555_PORT port, uint8_t dir);

/**
 *  Invert the polarity of pins on a port. Take care when using this function
 *  as it inverts the current polarity settings, it is not an absolute value.
 *  If the PCA9555 has not been reset but the microcontroller has been, this
 *  might not contain the value you expect. This is complicated by the fact
 *  that this chip does not have any RESET line or i2c command that can be
 *  accessed from a microcontroller. Implemented as a macro.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @param pol 8 bit polarity of the pins. 1 means invert, 0 no change. */
void PCA9555_pol(PCA9555_DEVICE dev, PCA9555_PORT port, uint8_t pol);

/**
 *  Set values on a port. Implemented as a macro.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @param value New 8 bit value for the port.
 */
void PCA9555_set(PCA9555_DEVICE dev, PCA9555_PORT port, uint8_t value);

/**
 *  Get values from a port.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @return 8 bit value on the port.
 */
uint8_t PCA9555_get(PCA9555_DEVICE dev, PCA9555_PORT port);

#include "PCA9555_api.h"

#endif /* [PCA9555] */

/**@}*/
