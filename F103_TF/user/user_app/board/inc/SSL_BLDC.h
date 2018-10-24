#ifndef SSL_BLDC_H_
#define SSL_BLDC_H_

#include "system.h"
#include "app.h"
#include "util.hpp"

class bldc{
public:
	void setup(DRV8305 &gd);

	DRV8305 *gd;
};


#endif
