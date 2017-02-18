#include "layer_driver/base/can.hpp"
#include "console.hpp"
#include "layer_driver/board/pin.hpp"
#include "util.hpp"
#include "math.h"

class CanKey{
public:
	char key;
	char *canName;
	Can *can;
};

class CanTest:public ConsoleCommand,public CanInterface{
public:
	CanTest(Console &console);
	int setup();
	int canRead(int id,int number,unsigned char data[8]){
		consolePrintf((char*)"get  a data:%x %x %x %x %x %x %x %x %x %x\r\n",id,number,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);return 0;
	};
	void setCanStatus(Can *can,char key,char *canName);
	int commandCompare(char *command){return ((!(strcmp(command,"can"))));};
	int commandCycle(){return 0;};
	int commandRequest(int argc,char **argv);
	void consoleReadChar(char data){};
	int canId(int id);
private:
	bool dataLoad;
	int cans;
	int ids[30];
	int idNum;
	bool can0SetupFlag;
	ConsoleCommand *command;
	CanKey cankey[2];
};
