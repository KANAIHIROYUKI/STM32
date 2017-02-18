#ifndef CAN_HPP
#define CAN_HPP

/*******************************************
 * can ver2.0 2015/10/8
 * Base classes of Controller Area Network communication.
 * CanInterface declaration.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/

class Can;
class CanInterface{
public:
	int canSetup();
	int canWrite(int idArg,int numberArg,unsigned char dataArg[8]);
	int canSetId(int id);
	int canSetIdAll();
	virtual int canRead(int id,int number,unsigned char data[8])=0;
	virtual int canId(int id){return 0;};

	int canInterfaceSetup(Can *can);
private:
	Can *can;
};

class Can{
public:
	virtual int setup()=0;
	virtual int setupLoopBack()=0;
	int addInterface(CanInterface *interfaceArg){return addInterface(*interfaceArg);};
	virtual int addInterface(CanInterface &interfaceArg)=0;
	virtual int setId(int id)=0;
	virtual int setIdAll()=0;
	virtual int write(int id,int number,unsigned char data[8])=0;
};

#endif//CAN_HPP
