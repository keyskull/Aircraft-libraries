#include <Arduino.h>
#include <Serial.h>
#include <Wire.h>

#define BMP085_ADDRESS 0x77  // I2C address of BMP085
const unsigned char OSS = 0x01;  // Oversampling Setting

// Calibration values
short ac1;
short ac2;
short ac3;
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1;
short b2;
short mb;
short mc;
short md;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).

long b5;
long temperature;
long pressure;

// Read 1 byte from the BMP085 at 'address'
unsigned char bmp085Read(unsigned char reg)
{
		  unsigned char data;

		    Wire.beginTransmission(BMP085_ADDRESS);
			  Wire.write(byte(reg));
			    Wire.endTransmission();

				  Wire.requestFrom(BMP085_ADDRESS, 1);
				    while(Wire.available())
							   {
									   		data = Wire.read();
											   }
					   return data;
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
short bmp085ReadInt(unsigned char address)
{
		  unsigned char msb, lsb;
		    //char i=0;
		    //char val[10];

		    Wire.beginTransmission(BMP085_ADDRESS);
			  Wire.write(address);
			    Wire.endTransmission();

				  Wire.requestFrom(BMP085_ADDRESS, 2);
				    while(Wire.available()<2);
					 //  {
					//		val[i++]=Wire.read();
					//    }
					  msb = Wire.read();
					  lsb = Wire.read();
					    return (short( msb<<8 | lsb));
}

// Read the uncompensated temperature value
short bmp085ReadUT()
{
		  short ut;

		    // Write 0x2E into Register 0xF4
		    // This requests a temperature reading
		    Wire.beginTransmission(BMP085_ADDRESS);
			  Wire.write(0xF4);
			    Wire.write(0x2E);
				  Wire.endTransmission();

				    // Wait at least 4.5ms
				    delay(5);

					  // Read two bytes from registers 0xF6 and 0xF7
					  ut = bmp085ReadInt(0xF6);
					    return ut;
}


// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
	ac1 = bmp085ReadInt(0xAA);
	ac2 = bmp085ReadInt(0xAC);
	ac3 = bmp085ReadInt(0xAE);
	ac4 = bmp085ReadInt(0xB0);
	ac5 = bmp085ReadInt(0xB2);
	ac6 = bmp085ReadInt(0xB4);
	b1 = bmp085ReadInt(0xB6);
	b2 = bmp085ReadInt(0xB8);
	mb = bmp085ReadInt(0xBA);
	mc = bmp085ReadInt(0xBC);
	md = bmp085ReadInt(0xBE);
}

// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
long bmp085GetTemperature(long ut)
{
		  long x1, x2;

		    x1 = ( (ut - ac6) * ac5 ) >> 15;
			  x2 = (mc << 11)/(x1 + md);
			    b5 = x1 + x2;

				  return ((b5 + 8)>>4);
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
		  long x1, x2, x3, b3, b6, p,b7;
		    unsigned long b4;

			  b6 = b5 - 4000;
			    // Calculate B3
			    x1 = (b2 * (b6 * b6)>>12)>>11;
				  x2 = (ac2 * b6)>>11;
				    x3 = x1 + x2;
					  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

					    // Calculate B4
					    x1 = (ac3 * b6)>>13;
						  x2 = (b1 * ((b6 * b6)>>12))>>16;
						    x3 = ((x1 + x2) + 2)>>2;
							  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

							    b7 = ((unsigned long)(up - b3) * (50000>>OSS));
								  if (b7 < 0x80000000)
										      p = (b7<<1)/b4;
								    else
											    p = (b7/b4)<<1;

									  x1 = (p>>8) * (p>>8);
									    x1 = (x1 * 3038)>>16;
										  x2 = (-7357 * p)>>16;
										    p += (x1 + x2 + 3791)>>4;

											  return p;
}

// Read the uncompensated pressure value
long bmp085ReadUP()
{
		  unsigned char msb, lsb, xlsb;
		    unsigned long up = 0;

			  // Write 0x34+(OSS<<6) into register 0xF4
			  // Request a pressure reading w/ oversampling setting
			  Wire.beginTransmission(BMP085_ADDRESS);
			    Wire.write(0xF4);
				  Wire.write(0x34 + (OSS<<6));
				    Wire.endTransmission();

					  // Wait for conversion, delay time dependent on OSS
					  delay(5);

					    // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
					    Wire.beginTransmission(BMP085_ADDRESS);
						  Wire.write(0xF6);
						    Wire.endTransmission();
							  Wire.requestFrom(BMP085_ADDRESS, 3);

							    // Wait for data to become available
							    while(Wire.available() < 3)
										    ;
								  msb = Wire.read();
								    lsb = Wire.read();
									  xlsb = Wire.read();

									    up = ( (long( msb << 16)) | (long( lsb << 8)) | (long(xlsb)) ) >> (8-OSS);

										  return up;
}

void setup()
{
		Wire.setBusFreq(100000);
		  Wire.begin();
		    bmp085Calibration();
}

void loop()
{
		 temperature = bmp085GetTemperature(bmp085ReadUT());
		   pressure = bmp085GetPressure(bmp085ReadUP());
		     printf("Temperature: ");
			   printf("%ld %d",temperature, DEC);
			     printf(" *0.1 deg C \n");
				   printf("Pressure: ");
				     printf("%ld %d",pressure, DEC);
					   printf(" Pa \n");
					     printf("\n");
						   delay(1000);
}


