#include <xc.h>
#include "i2c.h"
#include "ext_eeprom.h"

#define EEPROM_WRITE    0xA0
#define EEPROM_READ     0xA1

void write_ext_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();

    i2c_write(EEPROM_WRITE);
    i2c_ack();

    i2c_write(address);
    i2c_ack();

    i2c_write(data);
    i2c_ack();

    i2c_stop();

    /* EEPROM internal write cycle (5 ms max) */
    for(unsigned int i = 0; i < 5000; i++);
}

unsigned char read_ext_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();

    i2c_write(EEPROM_WRITE);
    i2c_ack();

    i2c_write(address);
    i2c_ack();

    i2c_rep_start();

    i2c_write(EEPROM_READ);
    i2c_ack();

    data = i2c_read();

    i2c_stop();

    return data;
}

void write_ext_eeprom_array(unsigned char address, char *data, unsigned char len)
{
    unsigned char i;

    for(i = 0; i < len; i++)
    {
        write_ext_eeprom(address++, data[i]);
    }
}

void read_ext_eeprom_array(unsigned char address, char *data, unsigned char len)
{
    unsigned char i;

    for(i = 0; i < len; i++)
    {
        data[i] = read_ext_eeprom(address++);
    }

    data[len] = '\0';
}