/*
 * File:   ext_eeprom_pro1.c
 * Author: surya
 *
 * Created on 17 April, 2025, 2:07 PM
 */


#include "black_box.h"


void write_ext_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(0XA0);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    
    for(unsigned int i=3000;i--;);
}

unsigned char read_ext_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(0XA0);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(0XA1);
	data = i2c_read();
	i2c_stop();

	return data;
}