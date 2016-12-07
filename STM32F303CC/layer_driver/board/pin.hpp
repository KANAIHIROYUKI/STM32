#ifndef PIN_HPP_INCLUDED
#define PIN_HPP_INCLUDED

/*
チンパンジーでもわかる！
ichigoplus基板ドライバの作り方！

何をしたい？
１、ピンの配置を変えたい　→　pin.cppへ
２、新しいピンを追加したい　→　１４行目へ
３、ピンを消したい　→　１９行目へ
４、基板の名前が変わった　→　pin.cppへ

２，新しいピンを追加したい
　追加したいピンと同じ機能のクラスを何かコピーして貼り付け、クラス名だけ書き換えて下さい。
　例えば、D10を作りたければD0をコピーしてD0って書いてある部分を全部D10に置き換える感じで。
　終わったらpin.cppを見て下さい。

３，ピンを消したい
　消したいピンのクラスを全部消して下さい。
　例えば、D10を消したければclass D10{...}; という一連のクラスまるごと消して下さい。
　終わったらpin.cppを見て下さい。
 */

#ifdef __cplusplus
extern "C" void std_char_out(char data);
extern "C" int std_char_out_setup();
extern "C" char* board_name();
#else
void std_char_out(char data);
int std_char_out_setup();
char* board_name();
#endif
#ifdef __cplusplus

#include "layer_driver/base/serial.hpp"
#include "layer_driver/base/digital.hpp"
#include "layer_driver/base/analog.hpp"
#include "layer_driver/base/encoder.hpp"
#include "layer_driver/base/pwm.hpp"

class A0:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A0(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A1:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A1(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A2:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A2(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A3:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A3(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A4:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A4(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A5:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A5(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A6:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A6(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A7:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A7(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};
/*消しました
class A8:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A8(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};

class A9:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	A9(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};
*/
/*
class C5:public Digital,public Analog{
private:
	enum{
		MODE_NULL,
		MODE_ANALOG,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupAnalogIn();
	float _analogRead();
public:
	C5(){mode=0;};

	int analogResolution(){return 4095;};
	float analogVoltage(){return 3.3;};
	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		return analogRead();
	}
};*/

class D0:public Digital{
private:
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();
public:

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator int() {
		return digitalRead();
	}
};

class D1:public Digital{
private:
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();
public:

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator int() {
		return digitalRead();
	}
};

class D2:public Digital{
private:
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();
public:

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator int() {
		return digitalRead();
	}
};

class D3:public Digital{
private:
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();
public:

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator int() {
		return digitalRead();
	}
};

/*消しました
   class D4:public Digital{
private:
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();
public:

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator int() {
		return digitalRead();
	}
};
*/
class Pwm0:public Digital,public Pwm{
private:
	enum{
		MODE_NULL,
		MODE_PWM,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupPwmOut(float frequency,float duty);
	void _pwmWrite(float duty);
	float _pwmWrite();
public:
	Pwm0(){mode=0;}

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	Pwm& operator= (float value) {
		pwmWrite(value);
		return *this;
	}

	Pwm& operator= (Pwm& pwm) {
		pwm.pwmWrite(pwm.pwmWrite());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		if (mode==MODE_PWM)return pwmWrite();
		else return 0.0;
	}
};

//////////////////////////////
class Pwm1:public Digital,public Pwm{
private:
	enum{
		MODE_NULL,
		MODE_PWM,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupPwmOut(float frequency,float duty);
	void _pwmWrite(float duty);
	float _pwmWrite();
public:
	Pwm1(){mode=0;}

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	Pwm& operator= (float value) {
		pwmWrite(value);
		return *this;
	}

	Pwm& operator= (Pwm& pwm) {
		pwm.pwmWrite(pwm.pwmWrite());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		if (mode==MODE_PWM)return pwmWrite();
		else return 0.0;
	}
};
//////////////////////////////
class Pwm2:public Digital,public Pwm{
private:
	enum{
		MODE_NULL,
		MODE_PWM,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupPwmOut(float frequency,float duty);
	void _pwmWrite(float duty);
	float _pwmWrite();
public:
	Pwm2(){mode=0;}

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	Pwm& operator= (float value) {
		pwmWrite(value);
		return *this;
	}

	Pwm& operator= (Pwm& pwm) {
		pwm.pwmWrite(pwm.pwmWrite());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		if (mode==MODE_PWM)return pwmWrite();
		else return 0.0;
	}
};
//////////////////////////////
class Pwm3:public Digital,public Pwm{
private:
	enum{
		MODE_NULL,
		MODE_PWM,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupPwmOut(float frequency,float duty);
	void _pwmWrite(float duty);
	float _pwmWrite();
public:
	Pwm3(){mode=0;}

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	Pwm& operator= (float value) {
		pwmWrite(value);
		return *this;
	}

	Pwm& operator= (Pwm& pwm) {
		pwm.pwmWrite(pwm.pwmWrite());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		if (mode==MODE_PWM)return pwmWrite();
		else return 0.0;
	}
};
//////////////////////////////
class Pwm4:public Digital,public Pwm{
private:
	enum{
		MODE_NULL,
		MODE_PWM,
		MODE_DIGITAL
	};
	int mode;
	int _digitalWrite();
	void _digitalWrite(int value);
	int _digitalRead();
	int _setupDigitalIn();
	int _setupDigitalOut();
	int _setupDigitalInPullUp();
	int _setupDigitalInPullDown();

	int _setupPwmOut(float frequency,float duty);
	void _pwmWrite(float duty);
	float _pwmWrite();
public:
	Pwm4(){mode=0;}

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	Pwm& operator= (float value) {
		pwmWrite(value);
		return *this;
	}

	Pwm& operator= (Pwm& pwm) {
		pwm.pwmWrite(pwm.pwmWrite());
		return *this;
	}

	operator bool(){
		return digitalRead();
	}

	operator int() {
		return digitalRead();
	}

	operator float() {
		if (mode==MODE_PWM)return pwmWrite();
		else return 0.0;
	}
};
//////////////////////////////

class Enc0:public Encoder{
public:
	int setup();
	int count();

	operator int() {
		return count();
	}
};

class Enc1:public Encoder{
public:
	int setup();
	int count();

	operator int() {
		return count();
	}
};

class Enc2:public Encoder{
public:
	int setup();
	int count();

	operator int() {
		return count();
	}
};



class Serial0:public Serial{
private:
	static SerialInterface *interface;
	static RingBuffer<char,256> txBuf;
	static int isTransmitting;
	int _setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength);
	friend int std_char_out_setup();
	friend void serial0_rxInterrupt();
	friend void serial0_txInterrupt();
public:
	static void transmit(char);
	void writeChar(char value);
};

class Serial1:public Serial{
private:
	static SerialInterface *interface;
	static RingBuffer<char,256> txBuf;
	static int isTransmitting;
	int _setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength);
	//friend int std_char_out_setup();
	friend void serial1_rxInterrupt();
	friend void serial1_txInterrupt();
public:
	static void transmit(char);
	void writeChar(char value);
};

class Serial2:public Serial{
private:
	static SerialInterface *interface;
	static RingBuffer<char,256> txBuf;
	static int isTransmitting;
	int _setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength);
	//friend int std_char_out_setup();
	friend void serial2_rxInterrupt();
	friend void serial2_txInterrupt();
public:
	static void transmit(char);
	void writeChar(char value);
};

class Serial3:public Serial{
private:
	static SerialInterface *interface;
	static RingBuffer<char,256> txBuf;
	static int isTransmitting;
	int _setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength);
	//friend int std_char_out_setup();
	friend void serial3_rxInterrupt();
	friend void serial3_txInterrupt();
public:
	static void transmit(char);
	void writeChar(char value);
};

class WatchDog{
public:
	int setup();
	void cycle();
};

#include "can.hpp"

#define NUMBER_OF_CAN_INTERFACE 30
class Can0:public Can{
public:
	Can0();
	int setup();
	int setupLoopBack();
	int addInterface(CanInterface &interfaceArg);
	int setId(int id);
	int setIdAll();
	int write(int id,int number,unsigned char data[8]);
private:
	int filter_number;
	short filter[14*4];
	bool setuped;
	static CanInterface *canInterface[30];
	static int canInterfaceCursor;
	static int read(int id,int number,unsigned char data[8]);
	friend void Can0_Interrupt();
};

class Can1:public Can{
public:
	Can1();
	int setup();
	int setupLoopBack();
	int addInterface(CanInterface &interfaceArg);
	int setId(int id);
	int setIdAll();
	int write(int id,int number,unsigned char data[8]);
private:
	int filter_number;
	short filter[14*4];
	bool setuped;
	static CanInterface *canInterface[30];
	static int canInterfaceCursor;
	static int read(int id,int number,unsigned char data[8]);
	friend void Can1_Interrupt();
};

#endif // __cplusplus
#endif // PIN_H_INCLUDED
