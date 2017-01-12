//Read EEPROM
#ifndef EEPROM_H
#define EEPROM_H

#include "IO_Driver.h"

IO_EEPROM_Init (void);
IO_EEPROM_DeInit (void);
IO_EEPROM_Read (ubyte2 offset, ubyte2 length, ubyte1 *const data);
IO_EEPROM_Write (ubyte2 offset, ubyte2 length, const ubyte1 *const data);
IO_EEPROM_GetStatus (void);

#endif //EEPROM_H
