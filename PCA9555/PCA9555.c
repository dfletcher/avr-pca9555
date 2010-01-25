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
#include "i2cmaster.h"

void PCA9555_init(void) {
  i2c_init();
}

void PCA9555_write(PCA9555_DEVICE dev, PCA9555_REGISTER reg, uint8_t value) {
  i2c_start_wait(PCA9555_DEVICE_ADDRESS + dev + I2C_WRITE);
  i2c_write(reg);
  i2c_write(value);
  i2c_stop();
}

uint8_t PCA9555_read(PCA9555_DEVICE dev, PCA9555_REGISTER reg) {
  uint8_t rval;
  i2c_start_wait(PCA9555_DEVICE_ADDRESS + dev + I2C_WRITE);
  i2c_write(reg);
  i2c_rep_start(PCA9555_DEVICE_ADDRESS + dev + I2C_READ);
  rval = i2c_readNak();
  i2c_stop();
  return rval;
}
