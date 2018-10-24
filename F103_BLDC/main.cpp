#include "main.hpp"

int main(void)
{
	setup();

	pwm.duty(50);

	enc.write(mag.getValue() - 95);
	//enc.write(mag.getValue() - 71);
	isCW = 0;
	dwell.write(0);

	while(1){
		//sys.cycle();
		//led[0].cycle();
		//led[1].cycle();
		//mag.cycle();


		if(intervalTime < millis() && nFault.read() == 0){
			intervalTime += 100;
			for(int i=1;i<12;i++){
				serial.printf("%x,",drv.read(i));
			}

			serial.printf("pw=,%d,",pwGd.read());
			//if( == 0)serial.printf("Fault");
			serial.printf("\n\r");

			//ゲートドライバのSPI書き込みデバッグ
			//ss.write(0);
			//serial.printf("%4d,\n\r",(int)(((spi.transfer(0) << 3) + (spi.transfer(0) >> 5)) & 0x3FF));
			//ss.write(1);
			//drv.readFault();
			//serial.printf("%d,%d,%d,%d,%d,%d\n\r",drv.data[0],drv.data[1],drv.data[2],drv.data[3],nFault.read(),pwGd.read());


		}

		if(intervalTime < millis()){
			intervalTime += 100;
			serial.printf("val = %4d,speed = %4drps,",val,speed);
			serial.printf("val = %2d,",val);
			serial.printf("t = %d,",(millis()/500)%12);
			serial.printf("offset = %d,",encVal/2000);
			serial.printf("%d,\n\r",enc.read());
		}

		if(serial.available()){
			uint8_t rxData = serial.read();
			switch(rxData){
			case 'w':
				pwmDuty *= 1.1;
				break;
			case 's':
				pwmDuty /= 1.1;
				break;
			case 'r':
				enc.reset();
				break;
			case 'e':
				if(isCW == 0){
					isCW = 1;
					enc.write(enc.read() + 71);
				}
				//TIM3->CNT++;
				offsetCnt++;
				//enc.write(enc.read() + 1);
				//serial.printf("mag = %d\n\r",mag.getValue());
				break;
			case 'q':
				if(isCW == 1){
					isCW = 0;
					enc.write(enc.read() - 71);

				}
				//TIM3->CNT--;
				offsetCnt--;
				//enc.write(enc.read() - 1);
				break;
			}
			serial.printf("                     pwm duty = %d,offset = %d\n\r",pwmDuty,offsetCnt);
			pwm.duty(pwmDuty);
		}

		if(speedCycleTome < millis()){
			speedCycleTome += 5;
			speed = (enc.read() - oldCnt)/24;
			oldCnt = enc.read();
		}

		//7サイクルで一周
		//1024カウント
		//146
		//

		encVal = enc.read();
		//encVal -= (encVal/512);
		encVal += speed;
		val = -encVal%1023;
		val = (int)((float)(-val%(1023/7))/((float)1023/7)*12);		//ここの数字は関係ないっぽい｡丸め誤差か？
		if(val < 0)val +=12;
		switch(val){
		case 0:
			GPIO_ResetBits(State0);
			GPIO_SetBits(State1);
			GPIO_SetBits(State2);

			GPIO_ResetBits(Dowel);

			/*
			state[0].write(0);
			state[1].write(1);
			state[2].write(1);
*/
			dwell.write(0);
			break;

		case 1:
			GPIO_ResetBits(State0);
			GPIO_SetBits(State1);
			GPIO_ResetBits(State2);

			GPIO_SetBits(Dowel);
			/*
			state[0].write(0);
			state[1].write(1);
			state[2].write(0);


			dwell.write(1);*/
			break;

		case 2:
			GPIO_ResetBits(State0);
			GPIO_SetBits(State1);
			GPIO_ResetBits(State2);

			GPIO_ResetBits(Dowel);
			/*
			state[0].write(0);
			state[1].write(1);
			state[2].write(0);

			dwell.write(0);*/
			break;

		case 3:
			GPIO_SetBits(State0);
			GPIO_SetBits(State1);
			GPIO_ResetBits(State2);

			GPIO_SetBits(Dowel);
			/*
			state[0].write(1);
			state[1].write(1);
			state[2].write(0);

			dwell.write(1);*/
			break;

		case 4:
			GPIO_SetBits(State0);
			GPIO_SetBits(State1);
			GPIO_ResetBits(State2);

			GPIO_ResetBits(Dowel);

			/*
			state[0].write(1);
			state[1].write(1);
			state[2].write(0);

			dwell.write(0);*/
			break;

		case 5:
			GPIO_SetBits(State0);
			GPIO_ResetBits(State1);
			GPIO_ResetBits(State2);

			GPIO_SetBits(Dowel);
			/*
			state[0].write(1);
			state[1].write(0);
			state[2].write(0);

			dwell.write(1);*/
			break;

		case 6:
			GPIO_SetBits(State0);
			GPIO_ResetBits(State1);
			GPIO_ResetBits(State2);

			GPIO_ResetBits(Dowel);

			/*
			state[0].write(1);
			state[1].write(0);
			state[2].write(0);

			dwell.write(0);*/
			break;

		case 7:

			GPIO_SetBits(State0);
			GPIO_ResetBits(State1);
			GPIO_SetBits(State2);

			GPIO_SetBits(Dowel);

			/*
			state[0].write(1);
			state[1].write(0);
			state[2].write(1);

			dwell.write(1);*/
			break;

		case 8:
			GPIO_SetBits(State0);
			GPIO_ResetBits(State1);
			GPIO_SetBits(State2);

			GPIO_ResetBits(Dowel);

			/*
			state[0].write(1);
			state[1].write(0);
			state[2].write(1);

			dwell.write(0);*/
			break;

		case 9:
			GPIO_ResetBits(State0);
			GPIO_ResetBits(State1);
			GPIO_SetBits(State2);

			GPIO_SetBits(Dowel);

			/*
			state[0].write(0);
			state[1].write(0);
			state[2].write(1);

			dwell.write(1);*/
			break;

		case 10:
			GPIO_ResetBits(State0);
			GPIO_ResetBits(State1);
			GPIO_SetBits(State2);

			GPIO_ResetBits(Dowel);
			/*
			state[0].write(0);
			state[1].write(0);
			state[2].write(1);

			dwell.write(0);*/
			break;

		case 11:
			GPIO_ResetBits(State0);
			GPIO_SetBits(State1);
			GPIO_SetBits(State2);

			GPIO_SetBits(Dowel);
			/*
			state[0].write(0);
			state[1].write(1);
			state[2].write(1);

			dwell.write(1);*/
			break;
		}
	}
}
