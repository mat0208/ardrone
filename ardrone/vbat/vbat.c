/*
    vbat.c - AR.Drone battery voltage driver

    Copyright (C) 2011 Hugo Perquin - http://blog.perquin.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include "i2c-dev.h"
#include "vbat.h"

#define VBAT_ADDRESS 0x4a

#define I2CDEV "/dev/i2c-1"

int fd;

float vbat_get(unsigned char channel) 
{
	if(channel>9) return -1;

	unsigned lower = i2c_smbus_read_byte_data(fd, 0x37 );
	unsigned upper = i2c_smbus_read_byte_data(fd, 0x38 );
	
	printf ("U: %02X L: %02X\n",upper,lower);
	
	unsigned value = upper<<2 | lower;
	//VREF Reference Voltage Internally connected to VDDC pin. 1.8V +/- 0.05V
	//Measured Input Scaling Factor External inputs ANA{0,1,2,3}  0.25  V/V  	ch 0-3
	//Measured Input Scaling Factor VDD{0,1,2,3,4} inputs 0.4  V/V 				ch 4-8
	//Measured Input Scaling Factor VINSYS input 0.25 V/V 						ch 9
	float factor=1.0;
	
	float v = value * factor;
	//printf("Channel=%d Vbat=%.2fVolt RawValue=%d RawHiByte=0x%x RawLiByte=0x%x\n",(int)channel,v,value,upper,lower);
	
	return v;
}



int vbat_init(vbat_struct *vbat)
{
	fd = open( I2CDEV, O_RDWR );

	if( ioctl( fd, I2C_SLAVE_FORCE, VBAT_ADDRESS ) < 0 )
	{
	fprintf( stderr, "Failed to set slave address: %m\n" );
	return 1;
	}

	//SET_POWER_ON
	if( i2c_smbus_write_byte_data( fd, 0x0, 0x1))   {	
		fprintf( stderr, "Failed to write to I2C device (1)\n" );
		return 2;
	}

	//SELECT CHANNEL
	if( i2c_smbus_write_byte_data( fd, 0x6, 0x1))   {	
		fprintf( stderr, "Failed to write to I2C device (2)\n" );
		return 2;
	}
	
	
	//ENABLE AVERAGING
	if( i2c_smbus_write_byte_data( fd, 0x8, 0x1))   {	
		fprintf( stderr, "Failed to write to I2C device (3)\n" );
		return 2;
	}
	
	
	//DISABLE (?) INTERRUPT
	if( i2c_smbus_write_byte_data( fd, 0x62, 0xF))   {	
		fprintf( stderr, "Failed to write to I2C device (4)\n" );
		return 2;
	}
	
	
	//START CONVERSION
	if( i2c_smbus_write_byte_data( fd, 0x12, 0x20))   {	
		fprintf( stderr, "Failed to write to I2C device (4)\n" );
		return 2;
	}
	
	
	return 0;
}

int vbat_read(vbat_struct *vbat)
{
	vbat->vbat=vbat_get(0);
}