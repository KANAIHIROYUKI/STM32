#ifndef I2C_HPP
#define I2C_HPP

#define TX true
#define RX false

class I2c;
class I2cInterface{
public:
	int i2cSetup();
	int i2cWrite(char address,unsigned char *value,char dataSize,bool txrx);
	virtual void i2cRead(unsigned char *data)=0;
	virtual void i2cReStartSend(char data)=0;
	virtual int i2cAddress(int address){return 0;};
	int i2cInterfaceSetup(I2c *i2c);
private:
	I2c *i2c;
};

class I2c{
public:
	virtual int setup()=0;
	virtual int write(char address,unsigned char *value,char dataSize,bool txrx)=0;
	int addInterface(I2cInterface *interfaceArg){return addInterface(*interfaceArg);};
	virtual int addInterface(I2cInterface &interfaceArg)=0;
};
#endif
