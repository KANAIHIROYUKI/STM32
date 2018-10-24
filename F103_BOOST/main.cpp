#include "main.hpp"

int main(void)
{
	setup();
	while(1){
		if(printTime < millis()){
			printTime += 100;
			adcVal = vin.read();
			voltage = adcVal * 140 / 819;
			serial.printf("%d,",PWM_CNT*16/voltage);
			serial.printf("pwm = %4d,adc = %4d,,voltage = %3d,pw = %d\n\r",pwmDuty,adcVal,voltage,(int)sqrtf(voltage*voltage - oldVoltage*oldVoltage));
			oldVoltage = voltage;
		}


		if(serial.available()){
			char rxData;
			rxData = serial.read();
			if(rxData == 'c'){
				serial.printf("chargeStart");
				chargeEn = 1;
				chargeTimer = millis();
			}
			if(rxData == 's'){
				serial.printf("chargeStop");
				chargeEn = 0;
				chargeTimer = millis();
			}
		}
		vave.stack(vin.read());
		if(fbTime < millis()){
			fbTime += 10;
			adcVal = vave.read();
			voltage = adcVal * 140 / 819;
			pwmDuty = PWM_CNT - (16*PWM_CNT/voltage);
			if(pwmDuty < 0)pwmDuty = PWM_CNT/2;
			if(voltage > 450){
				chargeEn = 0;
				serial.printf("chargeTime = %d,",millis() - chargeTimer);
				pwmDuty = 0;
			}
			if(chargeEn == 0)pwmDuty = 0;
			//pwm.duty(pwmDuty);

			if(pwmDuty != 0){
				pwm.pwmReset(PWM_CNT + PWM_CNT*16/voltage + 20);
				pwm.duty(PWM_CNT);
			}else{
				pwm.duty(0);
			}
		}
	}
}
