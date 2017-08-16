#ifndef _ENC_H_
#define _ENC_H_

#include "system.h"
#include "base.h"

class Enc:public CountIn{
public:
	void setup();
	void add(int64_t value);
	void write(int64_t value);
	int64_t read();
	void reset();
	void reverce(int16_t dir = 1);

	int16_t direction;
	int64_t countValue;

};

#endif
