#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED
/*******************************************
 * can ver2.0 2015/10/8
 * Pulse Width Modulation
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/

class Pwm{
protected:
//***************override******************
    virtual int _setupPwmOut(float frequency,float duty)=0;
    virtual void _pwmWrite(float duty)=0;
    virtual float _pwmWrite()=0;
//***************override******************
public:
    virtual int setupPwmOut(float frequency,float duty);
    virtual void pwmWrite(float duty);
    virtual float pwmWrite();
};

#endif // PWM_H_INCLUDED
