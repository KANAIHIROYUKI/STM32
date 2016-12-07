#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED
/*******************************************
 * encoder ver2.0 2015/10/8
 * Encoder class.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/
class Encoder{
protected:
	int cprData;
	bool revFlag;
	float mltData;
public:
	Encoder(){cprData=1;revFlag=false;mltData=1;};

	//depends on hardware
	virtual int setup()=0;
	virtual int count()=0;

	void rev(bool value);
	void mlt(float mltArg);
	void cpr(int cprArg);
	int cpr();
	float radian();
	float value();
};

#endif // ENCODER_H_INCLUDED
