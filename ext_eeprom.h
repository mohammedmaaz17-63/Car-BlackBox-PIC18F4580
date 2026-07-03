#ifndef EXT_EEPROM_H
#define EXT_EEPROM_H

void write_ext_eeprom(unsigned char address,unsigned char data);
unsigned char read_ext_eeprom(unsigned char address);

void write_ext_eeprom_array(unsigned char address,char *data,unsigned char len);
void read_ext_eeprom_array(unsigned char address,char *data,unsigned char len);


#endif