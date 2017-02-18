#include "i2c.hpp"

int I2cInterface::i2cInterfaceSetup(I2c *i2c){
	this->i2c=i2c;
	return 0;
}

int I2cInterface::i2cSetup(){
	return i2c->setup();
}
int I2cInterface::i2cWrite(char address,unsigned char *value,char dataSize,bool txrx){
	return i2c->write(address,value,dataSize,txrx);
}
