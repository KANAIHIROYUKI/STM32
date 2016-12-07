#ifndef DIGITAL_H_INCLUDED
#define DIGITAL_H_INCLUDED
/*******************************************
 * digital ver2.0 2015/10/8
 * Digital class.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/
class Digital{
protected:
//***************override******************
    virtual int _digitalWrite()=0;
    virtual void _digitalWrite(int)=0;
    virtual int _digitalRead()=0;
    virtual int _setupDigitalIn()=0;
	virtual int _setupDigitalOut()=0;
	virtual int _setupDigitalInPullUp()=0;
	virtual int _setupDigitalInPullDown()=0;
//***************override******************

    enum{
        DIGITAL_MODE_NULL,
        DIGITAL_MODE_IN,
        DIGITAL_MODE_OUT,
        DIGITAL_MODE_PULLUP,
        DIGITAL_MODE_PULLDOWN,
    };
    int _digitalMode;
public:
	virtual int setupDigitalIn();
	virtual int setupDigitalOut();
	virtual int setupDigitalInPullUp();
	virtual int setupDigitalInPullDown();

	virtual void digitalWrite(int value);
	virtual int digitalRead();
	virtual void digitalHigh();
	virtual void digitalLow();
	virtual void digitalToggle();

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




#endif // DIGITAL_H_INCLUDED
