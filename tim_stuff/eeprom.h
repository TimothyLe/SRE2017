#ifndef EEPROM_H
#define EEPROM_H

#include <stdio.h>
#include "IO_EEPROM.h"
#include "IO_Driver.h"
#include "IO_CAN.h"
using namespace std;

class EEPROM{
public:
	/*Declarations*/
	void readEP(ubyte2 offset, ubyte2 length, ubyte1 data);
	void writeEP(ubyte2 offset, ubyte2 length, ubyte1 * data);
};

#endif //EEPROM_H