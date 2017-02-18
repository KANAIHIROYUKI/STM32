#ifndef CAN_VALVE_H_INCLUDED
#define CAN_VALVE_H_INCLUDED
#include "layer_driver/board/pin.hpp"
#include "valve.hpp"

class CanValve:public CanInterface{
private:
	int canId;
	class CanValvePin:public Valve{
		friend class CanValve;
	private:
		CanValve *canValve;
		int status;
		int number;
		int canStatus(int canId,CanValve *canValve);
	public:
		int setup(bool turnOrve=false);
		void on();
		void off();
		void toggle();
		int read(){return status;};
	};
	public:
	int runSetupOnce;
	CanValve(Can &can,int canId);
	int setup();
	void canValveTransmit(int arg,unsigned char dataArg[8]);
	int canRead(int id,int number,unsigned char data[8]){return 0;};

	CanValvePin pin0;
	CanValvePin pin1;
	CanValvePin pin2;
	CanValvePin pin3;
	CanValvePin pin4;
	CanValvePin pin5;
	CanValvePin pin6;
	CanValvePin pin7;
};



#endif // CAN_VALVE_H_INCLUDED
