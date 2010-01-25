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

#define PCA9555_dir(dev,port,dir) PCA9555_write( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_DIRECTION_0 : PCA9555_DIRECTION_1, \
  dir \
)

#define PCA9555_pol(dev,port,pol) PCA9555_write( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_POLARITY_INV_0 : PCA9555_POLARITY_INV_1, \
  pol \
)

#define PCA9555_set(dev,port,value) PCA9555_write( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_OUTPUT_0 : PCA9555_OUTPUT_1, \
  value \
)

#define PCA9555_get(dev,port) PCA9555_read( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_INPUT_0 : PCA9555_INPUT_1 \
)
