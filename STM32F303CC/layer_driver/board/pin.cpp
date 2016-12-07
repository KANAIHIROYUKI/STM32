#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


/*
チンパンジーでもわかる！
ichigoplus基板ドライバの作り方！

何をしたい？
１、ピンの配置を変えたい　→15行目へ
２、新しいピンを追加したい　→　ｎ行目へ
３、ピンを消したい　→　ｎ行目へ
４、基板の名前が変わった　→　ｎ行目へ

１、ピンの配置を変えたい
　１、デジタルピンかアナログピンの場合
　　例)#define D0_PIN		  GPIOE,GPIO_Pin_4
　　D0_PINはD0という名前のクラスのピンの名前を記したdefineです。
　　もしD1ならD1_PINとなります。
　　後ろのGPIOE,GPIO_Pin_4はピンの名前で、データシートのPC4を指します
　　例えばPA0を指定したければここにGPIOA,GPIO_Pin_0を入力してください。

　　アナログピンであればA0_PINみたいな名前になっているはずです。
　　これもA0という名前のクラスのピンの名前を記したdefineです。
　　もしA1ならA1_PINとなります。
　　後ろのGPIOE,GPIO_Pin_4はデータシートのPC4を指します
　　例えばPA0を指定したければここにGPIOA,GPIO_Pin_0を入力してください。

　２，エンコーダの場合
　　例)
　　#define ENC0TIM TIM5
　　#define ENC0		GPIOA,GPIO_Pin_0 | GPIO_Pin_1

　　ENC0TIMはEnc0という名前のクラスが使うタイマーを記したdefineです。
　　もしEnc1ならENC1TIMとなります。
　　後ろのTIM5はタイマーの名前で、データシートのTIM5を指します。
　　例えばTIM4を指定したければここにTIM4を入力してください。

　　ENC0はEnc0という名前のクラスが使うピンを記したdefineです。
　　もしEnc1ならENC1となります。
　　後ろのGPIOA,GPIO_Pin_0 | GPIO_Pin_1は使うピンの名前で、データシートのPA0,PA1を指します。
　　例えば、PE1,PE2を指したければGPIOE, GPIO_Pin_0 | GPIO_Pin_1 と入力してください。

　３，PWMの場合
　　例)
　　#define PWM0		    GPIOB,GPIO_Pin_14
　　#define PWM0SET			TIM12,PWM0
　　#define PWM0TIMCH		TIM12,1
　　PWM0はPwm0という名前のクラスが使うピンを記したdefineです。
　　デジタルと同じです。
　　PWM0SETはPwm0という名前のクラスが使うタイマーを記したdefineです。
　　もしPwm1ならPWM1SETとなります。
　　後ろのTIM12は使うタイマーの名前で、データシートのTIM12を指します。
　　PWM0は上で解説したdefineです。もしPWM1SETならPWM1としてください。
　　PWM0TIMCH　はタイマーとチャンネルを記したdefineです。
　　もしPwm1ならPWM1TIMCHとなります。
　　後ろの TIM12,1は TIM12のCH1を指します。　もしTIM8のCH3を指したければTIM8,3と入力してください。

　４，CANの場合
　　例)
　　#define CAN0_PORT GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8
　　#define CAN0_NUMBER 1
　　CAN0_PORTはCan0という名前のクラスが使うピンを記したdefineです。
　　デジタルと同じです。
　　CAN0_NUMBERはCANモジュールの番号を指します。CAN1なら1を、CAN2なら2を入れて下さい

　５，シリアルの場合
　　例)
　　#define SERIAL0TxRx GPIOB,GPIO_Pin_10,GPIOB,GPIO_Pin_11
　　#define SERIAL0CH USART3
　　SERIAL0TxRxはSerial0という名前のクラスが使うピンを記したdefineです。
　　デジタルと同じです
　　SERIAL0CHはSerial0という名前のクラスが使うシリアルモジュールを記したdefineです。
　　USART3ならUSART3を、UART6ならUSART6を入れて下さい

２，新しいピンを追加したい
　先にpin.hppを読んで下さい

　読んできました？
　既に書かれているものと同じ種類のクラスを見つけてコピーします。
　A0とかPwm0とかにはここからここまでがクラスが書いてあるところという印があります。そこからそこまでをコピーしてください。SerialだけはSerial1を使って下さい。

　それが終わったらコピーして貼り付けた部分のクラス名やdefineを全部新たなクラス名やdefineに書き直してください。全部です。一文字も間違えないで下さい。

３、ピンを消したい
　消したいクラスをまるごと消して下さい。
　クラス名::関数名　という名前で関数が宣言されています。同じクラス名のものを全部消してしまいましよう。
　pin.hppのほうも見て下さい。

４、基板の名前が変わった
　char* board_name()という関数があります。この中に基板の名前を書いてください。
 */


extern "C" {
#include "config_usart.h"
#include "config_encoder.h"
#include "config_systick.h"
#include "config_usart.h"
#include "config_pwm.h"
#include "config_port.h"
#include "config_adc.h"
#include "config_can.h"
#include "config_usb.h"
#include "config_iwdg.h"
}

#include "pin.hpp"

char* board_name() {
	return (char*)"STM32F4 ver.TAKASHIMA 2.0";
}


#define A0_PIN      GPIOC,GPIO_Pin_0
#define A1_PIN      GPIOC,GPIO_Pin_1
#define A2_PIN      GPIOC,GPIO_Pin_2
#define A3_PIN      GPIOC,GPIO_Pin_3
#define A4_PIN      GPIOA,GPIO_Pin_2
#define A5_PIN      GPIOA,GPIO_Pin_3
#define A6_PIN      GPIOA,GPIO_Pin_4
#define A7_PIN      GPIOA,GPIO_Pin_5
/*消しました
#define A8_PIN      GPIOC,GPIO_Pin_4
#define A9_PIN      GPIOC,GPIO_Pin_5
*/
#define D0_PIN      GPIOC,GPIO_Pin_5
#define D1_PIN      GPIOC,GPIO_Pin_4
#define D2_PIN      GPIOB,GPIO_Pin_1
#define D3_PIN      GPIOB,GPIO_Pin_0

/*消しました
#define D4_PIN      GPIOE,GPIO_Pin_1
*/
//#define D3_PIN		  GPIOD,GPIO_Pin_13		//led
//#define D4_PIN		  GPIOD,GPIO_Pin_12		//led

/*アンプPWM*/
#define PWM0        GPIOC,GPIO_Pin_1
#define PWM0SET     TIM8,PWM0
#define PWM0TIMCH   TIM8,4
/*岡本MD_PWM1*/
#define PWM1      GPIOE,GPIO_Pin_6
#define PWM1SET     TIM9,PWM1
#define PWM1TIMCH   TIM9,2
#define CW1PIN        GPIOE,GPIO_Pin_2
#define CCW1PIN       GPIOE,GPIO_Pin_4

#define PWM2        GPIOB,GPIO_Pin_8
#define PWM2SET     TIM10,PWM2
#define PWM2TIMCH   TIM10,1
#define CW2PIN        GPIOE,GPIO_Pin_3
#define CCW2PIN   GPIOE,GPIO_Pin_5
/*岡本MD_PWM2*/
#define PWM3      GPIOB,GPIO_Pin_9
#define PWM3SET     TIM11,PWM3
#define PWM3TIMCH   TIM11,1
#define CW3PIN        GPIOE,GPIO_Pin_6
#define CCW3PIN       GPIOE,GPIO_Pin_4

#define PWM4      GPIOB,GPIO_Pin_7
#define PWM4SET     TIM4,PWM4
#define PWM4TIMCH   TIM4,2
#define CW4PIN        GPIOE,GPIO_Pin_5
#define CCW4PIN       GPIOE,GPIO_Pin_3
#define ENC0TIM TIM5
#define ENC0    GPIOA,GPIO_Pin_0 | GPIO_Pin_1
#define ENC1TIM TIM3
#define ENC1    GPIOA,GPIO_Pin_6 | GPIO_Pin_7
#define ENC2TIM TIM12
#define ENC2    GPIOB,GPIO_Pin_14 | GPIO_Pin_15

#define CAN0_PORT GPIOD,GPIO_Pin_1,GPIOD,GPIO_Pin_0
#define CAN0_NUMBER 1

#define CAN1_PORT GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12
#define CAN1_NUMBER 2




#define SERIAL0TxRx GPIOC,GPIO_Pin_6,GPIOC,GPIO_Pin_7
#define SERIAL0CH USART6

#define SERIAL1TxRx GPIOC,GPIO_Pin_10,GPIOC,GPIO_Pin_11
#define SERIAL1CH UART4

#define SERIAL2TxRx GPIOD,GPIO_Pin_5,GPIOD,GPIO_Pin_6
#define SERIAL2CH USART2

#define SERIAL3TxRx GPIOB,GPIO_Pin_10,GPIOB,GPIO_Pin_11
#define SERIAL3CH USART3

//***A0はじまり***
int A0::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A0_PIN);
}

void A0::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A0_PIN);
  }
  else {
    GPIO_SetBits(A0_PIN);
  }
}

int A0::_digitalRead()
{
  return GPIO_ReadInputDataBit(A0_PIN);
}

int A0::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A0::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A0::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A0_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A0::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, A0_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A0::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A0_PIN);
  return 0;
}

float A0::_analogRead()
{
  return (get_ADC1_value(A0_PIN) / analogResolution());
}
//***A0おわり***

int A1::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A1_PIN);
}

void A1::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A1_PIN);
  }
  else {
    GPIO_SetBits(A1_PIN);
  }
}

int A1::_digitalRead()
{
  return GPIO_ReadInputDataBit(A1_PIN);
}

int A1::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A1::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A1::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A1_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A1::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, A1_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A1::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A1_PIN);
  return 0;
}

float A1::_analogRead()
{
  return (get_ADC1_value(A1_PIN) / analogResolution());
}



int A2::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A2_PIN);
}

void A2::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A2_PIN);
  }
  else {
    GPIO_SetBits(A2_PIN);
  }
}

int A2::_digitalRead()
{
  return GPIO_ReadInputDataBit(A2_PIN);
}

int A2::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A2::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A2::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A2_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A2::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A2_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A2::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A2_PIN);
  return 0;
}

float A2::_analogRead()
{
  return (get_ADC1_value(A2_PIN) / analogResolution());
}



int A3::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A3_PIN);
}

void A3::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A3_PIN);
  }
  else {
    GPIO_SetBits(A3_PIN);
  }
}

int A3::_digitalRead()
{
  return GPIO_ReadInputDataBit(A3_PIN);
}

int A3::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A3::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A3::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A3_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A3::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A3_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A3::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A3_PIN);
  return 0;
}

float A3::_analogRead()
{
  return (get_ADC1_value(A3_PIN) / analogResolution());
}

int A4::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A4_PIN);
}

void A4::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A4_PIN);
  }
  else {
    GPIO_SetBits(A4_PIN);
  }
}

int A4::_digitalRead()
{
  return GPIO_ReadInputDataBit(A4_PIN);
}

int A4::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A4_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A4::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A4_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A4::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A4_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A4::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A4_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A4::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A4_PIN);
  return 0;
}

float A4::_analogRead()
{
  return (get_ADC1_value(A4_PIN) / analogResolution());
}



int A5::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A5_PIN);
}

void A5::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A5_PIN);
  }
  else {
    GPIO_SetBits(A5_PIN);
  }
}

int A5::_digitalRead()
{
  return GPIO_ReadInputDataBit(A5_PIN);
}

int A5::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A5_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A5::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A5_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A5::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A5_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A5::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A5_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A5::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A5_PIN);
  return 0;
}

float A5::_analogRead()
{
  return (get_ADC1_value(A5_PIN) / analogResolution());
}



int A6::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A6_PIN);
}

void A6::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A6_PIN);
  }
  else {
    GPIO_SetBits(A6_PIN);
  }
}

int A6::_digitalRead()
{
  return GPIO_ReadInputDataBit(A6_PIN);
}

int A6::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A6_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A6::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A6_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A6::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A6_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A6::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A6_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A6::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A6_PIN);
  return 0;
}

float A6::_analogRead()
{
  return (get_ADC1_value(A6_PIN) / analogResolution());
}


int A7::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A7_PIN);
}

void A7::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A7_PIN);
  }
  else {
    GPIO_SetBits(A7_PIN);
  }
}

int A7::_digitalRead()
{
  return GPIO_ReadInputDataBit(A7_PIN);
}

int A7::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A7_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A7::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A7_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A7::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A7_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A7::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A7_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A7::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A7_PIN);
  return 0;
}

float A7::_analogRead()
{
  return (get_ADC1_value(A7_PIN) / analogResolution());
}

/*
int A8::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A8_PIN);
}

void A8::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A8_PIN);
  }
  else {
    GPIO_SetBits(A8_PIN);
  }
}

int A8::_digitalRead()
{
  return GPIO_ReadInputDataBit(A8_PIN);
}

int A8::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A8_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A8::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A8_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A8::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A8_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A8::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A8_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A8::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A8_PIN);
  return 0;
}

float A8::_analogRead()
{
  return (get_ADC1_value(A8_PIN) / analogResolution());
}


int A9::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(A9_PIN);
}

void A9::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(A9_PIN);
  }
  else {
    GPIO_SetBits(A9_PIN);
  }
}

int A9::_digitalRead()
{
  return GPIO_ReadInputDataBit(A9_PIN);
}

int A9::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, A9_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A9::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, A9_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int A9::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, A9_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int A9::_setupDigitalInPullDown()
{

  Init_port(GPIO_Mode_IN, A9_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int A9::_setupAnalogIn()
{
  mode = MODE_ANALOG;
  Init_ADC1(A9_PIN);
  return 0;
}

float A9::_analogRead()
{
  return (get_ADC1_value(A9_PIN) / analogResolution());
}
*/
//***D0はじまり***
int D0::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(D0_PIN);
}

void D0::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(D0_PIN);
  }
  else {
    GPIO_SetBits(D0_PIN);
  }
}

int D0::_digitalRead()
{
  return GPIO_ReadInputDataBit(D0_PIN);
}

int D0::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, D0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D0::_setupDigitalOut()
{
  Init_port(GPIO_Mode_OUT, D0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D0::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, D0_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int D0::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, D0_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}
//***D0おわり***
int D1::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(D1_PIN);
}

void D1::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(D1_PIN);
  }
  else {
    GPIO_SetBits(D1_PIN);
  }
}

int D1::_digitalRead()
{
  return GPIO_ReadInputDataBit(D1_PIN);
}

int D1::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, D1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D1::_setupDigitalOut()
{
  Init_port(GPIO_Mode_OUT, D1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D1::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, D1_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int D1::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, D1_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int D2::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(D2_PIN);
}

void D2::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(D2_PIN);
  }
  else {
    GPIO_SetBits(D2_PIN);
  }
}

int D2::_digitalRead()
{
  return GPIO_ReadInputDataBit(D2_PIN);
}

int D2::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, D2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D2::_setupDigitalOut()
{
  Init_port(GPIO_Mode_OUT, D2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D2::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, D2_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int D2::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, D2_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int D3::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(D3_PIN);
}

void D3::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(D3_PIN);
  }
  else {
    GPIO_SetBits(D3_PIN);
  }
}

int D3::_digitalRead()
{
  return GPIO_ReadInputDataBit(D3_PIN);
}

int D3::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, D3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D3::_setupDigitalOut()
{
  Init_port(GPIO_Mode_OUT, D3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D3::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, D3_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int D3::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, D3_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}
/*消しました
int D4::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(D4_PIN);
}

void D4::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(D4_PIN);
  }
  else {
    GPIO_SetBits(D4_PIN);
  }
}

int D4::_digitalRead()
{
  return GPIO_ReadInputDataBit(D4_PIN);
}

int D4::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, D4_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D4::_setupDigitalOut()
{
  Init_port(GPIO_Mode_OUT, D4_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int D4::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, D4_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int D4::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, D4_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}
*/
//***Pwm0はじまり***
int Pwm0::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(PWM0);
}

void Pwm0::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(PWM0);
  }
  else {
    GPIO_SetBits(PWM0);
  }
}

int Pwm0::_digitalRead()
{
  return GPIO_ReadInputDataBit(PWM0);
}

int Pwm0::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, PWM0, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm0::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, PWM0, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm0::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, PWM0, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int Pwm0::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, PWM0, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int Pwm0::_setupPwmOut(float frequency, float duty)
{
  int i;
  mode = MODE_PWM;
  i = Init_PWM(PWM0SET, frequency);
  Set_duty(PWM0TIMCH, duty);
  return i;
}

void Pwm0::_pwmWrite(float duty)
{
  Set_duty(PWM0TIMCH, duty);
}

float Pwm0::_pwmWrite()
{
  return Read_duty(PWM0TIMCH);
}
//***Pwm0おわり***
//***Pwm1はじまり***
int Pwm1::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(PWM1);
}

void Pwm1::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(PWM1);
  }
  else {
    GPIO_SetBits(PWM1);
  }
}

int Pwm1::_digitalRead()
{
  return GPIO_ReadInputDataBit(PWM1);
}

int Pwm1::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, PWM1, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm1::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, PWM1, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm1::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, PWM1, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int Pwm1::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, PWM1, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int Pwm1::_setupPwmOut(float frequency, float duty)
{
  int i;
  mode = MODE_PWM;
  i = Init_PWM(PWM1SET, frequency);
  Set_duty(PWM1TIMCH, duty);
  return i;
}

void Pwm1::_pwmWrite(float duty)
{
  Set_duty(PWM1TIMCH, duty);
}

float Pwm1::_pwmWrite()
{
  return Read_duty(PWM1TIMCH);
}
//***Pwm1おわり***
//***Pwm2はじまり***
int Pwm2::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(PWM1);
}

void Pwm2::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(PWM2);
  }
  else {
    GPIO_SetBits(PWM2);
  }
}

int Pwm2::_digitalRead()
{
  return GPIO_ReadInputDataBit(PWM2);
}

int Pwm2::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, PWM2, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm2::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, PWM2, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm2::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, PWM2, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int Pwm2::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, PWM2, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int Pwm2::_setupPwmOut(float frequency, float duty)
{
  int i;
  mode = MODE_PWM;
  i = Init_PWM(PWM2SET, frequency);
  Set_duty(PWM2TIMCH, duty);
  return i;
}

void Pwm2::_pwmWrite(float duty)
{
  Set_duty(PWM2TIMCH, duty);
}

float Pwm2::_pwmWrite()
{
  return Read_duty(PWM2TIMCH);
}
//***Pwm2おわり***
//***Pwm3はじまり***
int Pwm3::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(PWM3);
}

void Pwm3::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(PWM3);
  }
  else {
    GPIO_SetBits(PWM3);
  }
}

int Pwm3::_digitalRead()
{
  return GPIO_ReadInputDataBit(PWM3);
}

int Pwm3::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, PWM3, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm3::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, PWM3, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm3::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, PWM3, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int Pwm3::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, PWM3, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int Pwm3::_setupPwmOut(float frequency, float duty)
{
  int i;
  mode = MODE_PWM;
  i = Init_PWM(PWM3SET, frequency);
  Set_duty(PWM3TIMCH, duty);
  return i;
}

void Pwm3::_pwmWrite(float duty)
{
  Set_duty(PWM3TIMCH, duty);
}

float Pwm3::_pwmWrite()
{
  return Read_duty(PWM3TIMCH);
}
//***Pwm3おわり***
//***Pwm0はじまり***
int Pwm4::_digitalWrite()
{
  return GPIO_ReadOutputDataBit(PWM4);
}

void Pwm4::_digitalWrite(int value)
{
  if (value == 0) {
    GPIO_ResetBits(PWM4);
  }
  else {
    GPIO_SetBits(PWM4);
  }
}

int Pwm4::_digitalRead()
{
  return GPIO_ReadInputDataBit(PWM4);
}

int Pwm4::_setupDigitalIn()
{
  Init_port(GPIO_Mode_IN, PWM4, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm4::_setupDigitalOut()
{
  mode = MODE_DIGITAL;
  Init_port(GPIO_Mode_OUT, PWM0, GPIO_PuPd_NOPULL, GPIO_OType_PP);
  return 0;
}

int Pwm4::_setupDigitalInPullUp()
{
  Init_port(GPIO_Mode_IN, PWM4, GPIO_PuPd_UP, GPIO_OType_PP);
  return 0;
}

int Pwm4::_setupDigitalInPullDown()
{
  Init_port(GPIO_Mode_IN, PWM4, GPIO_PuPd_DOWN, GPIO_OType_PP);
  return 0;
}

int Pwm4::_setupPwmOut(float frequency, float duty)
{
  int i;
  mode = MODE_PWM;
  i = Init_PWM(PWM4SET, frequency);
  Set_duty(PWM4TIMCH, duty);
  return i;
}

void Pwm4::_pwmWrite(float duty)
{
  Set_duty(PWM4TIMCH, duty);
}

float Pwm4::_pwmWrite()
{
  return Read_duty(PWM4TIMCH);
}
//***Pwm0おわり***
//***Enc0はじまり***
int Enc0::setup()
{
  Init_encoder(ENC0TIM, ENC0);
  return 0;
}

int Enc0::count()
{
  if (revFlag) return -Select_encoder_count(ENC0TIM);
  else return Select_encoder_count(ENC0TIM);
}
//***Enc0おわり***
int Enc1::setup()
{
  Init_encoder(ENC1TIM, ENC1);
  return 0;
}

int Enc1::count()
{
  if (revFlag) return -Select_encoder_count(ENC1TIM);
  else return Select_encoder_count(ENC1TIM);
}

int Enc2::setup()
{
  Init_encoder(ENC2TIM, ENC2);
  return 0;
}

int Enc2::count()
{
  if (revFlag) return -Select_encoder_count(ENC2TIM);
  else return Select_encoder_count(ENC2TIM);
}

SerialInterface *Serial0::interface;
RingBuffer<char, 256> Serial0::txBuf;
int Serial0::isTransmitting = 0;

//#define SERIAL0_TXINT

int Serial0::_setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength)
{
#ifdef SERIAL0_TXINT
	Init_USARTi(SERIAL0CH, baudrate, SERIAL0TxRx, parity, wordLength);
#else
	Init_USART(SERIAL0CH, baudrate, SERIAL0TxRx, parity, wordLength);
#endif
	interface = &interfaceArg;
	interface->serialInterfaceSetup(this);
	return 0;
}

void Serial0::writeChar(char value)
{
	transmit(value);
}

void serial0_rxInterrupt() {
	Serial0::interface->serialReadChar(USART_ReceiveData(SERIAL0CH));
	USART_ClearFlag(SERIAL0CH, USART_FLAG_RXNE);
}

void serial0_txInterrupt() {
#ifdef SERIAL0_TXINT
	if (Serial0::txBuf.isEmpty() == 0) USART_SendData(SERIAL0CH, Serial0::txBuf.read());
	if (Serial0::txBuf.isEmpty() == 1) Serial0::isTransmitting = 0;
	USART_ClearFlag(SERIAL0CH, USART_FLAG_TC);
#endif
}
void Serial0::transmit(char value) {
#ifdef SERIAL0_TXINT
	if (isTransmitting == 0) {
		USART_SendData(SERIAL0CH, value);
		isTransmitting = 1;
	}
	else {
		while (txBuf.write(value));
	}
#else
	while (USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL0CH, value);
#endif
}


extern "C" void USART6_IRQHandler(void) {
	//serial0_rxInterrupt();
	if (USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TC) == SET)serial0_txInterrupt();
	if (USART_GetFlagStatus(SERIAL0CH, USART_IT_RXNE) == SET)serial0_rxInterrupt();
}

void std_char_out(char value) {
	//	while(USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
	//	USART_SendData(SERIAL0CH, value);
	Serial0::transmit(value);
}

int std_char_out_setup() {
	Serial0::interface = new SerialInterface();
#ifdef SERIAL0_TXINT
	Init_USARTi(SERIAL0CH, 9600, SERIAL0TxRx, 0, 8);
#else
	Init_USART(SERIAL0CH, 9600, SERIAL0TxRx, 0, 8);
#endif
	return 0;
}

SerialInterface *Serial1::interface;
RingBuffer<char, 256> Serial1::txBuf;
int Serial1::isTransmitting = 0;
#define SERIAL1_TXINT

//***Serial1はじまり***
int Serial1::_setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength)
{
#ifdef SERIAL1_TXINT
	Init_USARTi(SERIAL1CH, baudrate, SERIAL1TxRx, parity, wordLength);
#else
	Init_USART(SERIAL1CH, baudrate, SERIAL1TxRx, parity, wordLength);
#endif
	interface = &interfaceArg;
	interface->serialInterfaceSetup(this);
	return 0;
}

void Serial1::writeChar(char value)
{
	transmit(value);
}

void serial1_rxInterrupt() {
	Serial1::interface->serialReadChar(USART_ReceiveData(SERIAL1CH));
	USART_ClearFlag(SERIAL1CH, USART_FLAG_RXNE);
}

void serial1_txInterrupt() {
#ifdef SERIAL1_TXINT
	if (Serial1::txBuf.isEmpty() == 0) USART_SendData(SERIAL1CH, Serial1::txBuf.read());
	if (Serial1::txBuf.isEmpty() == 1) Serial1::isTransmitting = 0;
	USART_ClearFlag(SERIAL1CH, USART_FLAG_TC);
#endif
}

void Serial1::transmit(char value) {
#ifdef SERIAL1_TXINT
	if (isTransmitting == 0) {
		USART_SendData(SERIAL1CH, value);
		isTransmitting = 1;
	}
	else {
		while (txBuf.write(value));
	}
#else
	while (USART_GetFlagStatus(SERIAL1CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL1CH, value);
#endif
}


extern "C" void UART4_IRQHandler(void) {
	//serial1_rxInterrupt();
	if (USART_GetFlagStatus(SERIAL1CH, USART_FLAG_TC) == SET)serial1_txInterrupt();
	if (USART_GetFlagStatus(SERIAL1CH, USART_IT_RXNE) == SET)serial1_rxInterrupt();
}
//***Serial1おわり***


SerialInterface *Serial2::interface;
RingBuffer<char, 256> Serial2::txBuf;
int Serial2::isTransmitting = 0;

//#define SERIAL2_TXINT


int Serial2::_setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength)
{
#ifdef SERIAL2_TXINT
	Init_USARTi(SERIAL2CH, baudrate, SERIAL2TxRx, parity, wordLength);
#else
	Init_USART(SERIAL2CH, baudrate, SERIAL2TxRx, parity, wordLength);
#endif
	interface = &interfaceArg;
	interface->serialInterfaceSetup(this);
	return 0;
}

void Serial2::writeChar(char value)
{
	transmit(value);
}

void serial2_rxInterrupt() {
	Serial2::interface->serialReadChar(USART_ReceiveData(SERIAL2CH));
	USART_ClearFlag(SERIAL2CH, USART_FLAG_RXNE);
}

void serial2_txInterrupt() {
#ifdef SERIAL2_TXINT
	if (Serial2::txBuf.isEmpty() == 0) USART_SendData(SERIAL2CH, Serial2::txBuf.read());
	if (Serial2::txBuf.isEmpty() == 1) Serial2::isTransmitting = 0;
	USART_ClearFlag(SERIAL2CH, USART_FLAG_TC);
#endif
}
void Serial2::transmit(char value) {
#ifdef SERIAL2_TXINT
	if (isTransmitting == 0) {
		USART_SendData(SERIAL2CH, value);
		isTransmitting = 1;
	}
	else {
		while (txBuf.write(value));
	}
#else
	while (USART_GetFlagStatus(SERIAL2CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL2CH, value);
#endif
}


extern "C" void USART2_IRQHandler(void) {
	//serial2_rxInterrupt();
	if (USART_GetFlagStatus(SERIAL2CH, USART_FLAG_TC) == SET)serial2_txInterrupt();
	if (USART_GetFlagStatus(SERIAL2CH, USART_IT_RXNE) == SET)serial2_rxInterrupt();
}


SerialInterface *Serial3::interface;
RingBuffer<char, 256> Serial3::txBuf;
int Serial3::isTransmitting = 0;

//#define SERIAL3_TXINT


int Serial3::_setup(int baudrate, SerialInterface &interfaceArg, int parity, int wordLength)
{
#ifdef SERIAL3_TXINT
	Init_USARTi(SERIAL3CH, baudrate, SERIAL3TxRx, parity, wordLength);
#else
	Init_USART(SERIAL3CH, baudrate, SERIAL3TxRx, parity, wordLength);
#endif
	interface = &interfaceArg;
	interface->serialInterfaceSetup(this);
	return 0;
}

void Serial3::writeChar(char value)
{
	transmit(value);
}

void serial3_rxInterrupt() {
	Serial3::interface->serialReadChar(USART_ReceiveData(SERIAL3CH));
	USART_ClearFlag(SERIAL3CH, USART_FLAG_RXNE);
}

void serial3_txInterrupt() {
#ifdef SERIAL2_TXINT
	if (Serial3::txBuf.isEmpty() == 0) USART_SendData(SERIAL3CH, Serial3::txBuf.read());
	if (Serial3::txBuf.isEmpty() == 1) Serial3::isTransmitting = 0;
	USART_ClearFlag(SERIAL3CH, USART_FLAG_TC);
#endif
}
void Serial3::transmit(char value) {
#ifdef SERIAL3_TXINT
	if (isTransmitting == 0) {
		USART_SendData(SERIAL3CH, value);
		isTransmitting = 1;
	}
	else {
		while (txBuf.write(value));
	}
#else
	while (USART_GetFlagStatus(SERIAL3CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL3CH, value);
#endif
}


extern "C" void USART3_IRQHandler(void) {
	//serial3_rxInterrupt();
	if (USART_GetFlagStatus(SERIAL3CH, USART_FLAG_TC) == SET)serial3_txInterrupt();
	if (USART_GetFlagStatus(SERIAL3CH, USART_IT_RXNE) == SET)serial3_rxInterrupt();
}

int WatchDog::setup() {
	return Init_watchdog();
}

void WatchDog::cycle() {
	watchdog_reset();
}
//***Can0はじまり***
CanInterface *Can0::canInterface[30];
int Can0::canInterfaceCursor = 0;

Can0::Can0() {
  setuped = false;
  filter_number = 4;
}

int  Can0::setup() {
  int returnValue = 0;
  if (!setuped) {
    Init_CAN(CAN0_NUMBER, CAN_Mode_Normal, CAN0_PORT); returnValue = 0;
    if (returnValue == 0) setuped = true;
  }
  else {
    returnValue = 0;
  }
  return returnValue;
}

int Can0::setupLoopBack() {
  int returnValue = 0;
  if (!setuped) {
    Init_CAN(CAN0_NUMBER, CAN_Mode_LoopBack, CAN0_PORT); returnValue = 0;
    if (returnValue == 0) setuped = true;
  }
  else {
    returnValue = 0;
  }
  return returnValue;
}

int Can0::addInterface(CanInterface &interfaceArg) {
  if (canInterfaceCursor >= 30) return 1;
  canInterface[canInterfaceCursor++] = &interfaceArg;
  interfaceArg.canInterfaceSetup(this);
  return 0;
}

int Can0::setId(int id) {
  int row = filter_number / 4;
  filter[filter_number] = (short)id;
  //printf("%s,set id:%x\n",__func__,id);
  Init_CANfilter(row, CAN_LIST, filter[row * 4 + 0], CAN_DATA, filter[row * 4 + 1], CAN_DATA, filter[row * 4 + 2], CAN_DATA, filter[row * 4 + 3], CAN_DATA);
  filter_number++;
  return 0;
}
int Can0::setIdAll() {
  Init_CANfilter(10, CAN_MASK, 0, CAN_DATA, 0, CAN_DATA, 0, CAN_DATA, 0, CAN_DATA);
  return 0;
}

int Can0::read(int id, int number, unsigned char data[8]) {
  //  printf("id:%x length:%x data:%x,%x,%x,%x,%x,%x,%x,%x\n",id,number,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
  for (int i = 0; i<canInterfaceCursor; i++) if (canInterface[i]->canId(id)) canInterface[i]->canRead(id, number, data);
  return 0;
}

int Can0::write(int id, int number, unsigned char data[8]) {
  CanTxMsg can_tx_frame;
  can_tx_frame.StdId = id;
  can_tx_frame.ExtId = 0;
  can_tx_frame.IDE = CAN_Id_Standard;
  if (number != 0)can_tx_frame.RTR = CAN_RTR_Data;
  else can_tx_frame.RTR = CAN_RTR_Remote;
  can_tx_frame.DLC = (uint8_t)number;
  can_tx_frame.Data[0] = data[0];
  can_tx_frame.Data[1] = data[1];
  can_tx_frame.Data[2] = data[2];
  can_tx_frame.Data[3] = data[3];
  can_tx_frame.Data[4] = data[4];
  can_tx_frame.Data[5] = data[5];
  can_tx_frame.Data[6] = data[6];
  can_tx_frame.Data[7] = data[7];
  send_can1(&can_tx_frame);
  return 0;
}

CanRxMsg can0_rx_frame;
void Can0_Interrupt() {
  if (CAN_GetITStatus(CAN1, CAN_IT_FMP0)) {
    CAN_Receive(CAN1, CAN_FIFO0, &can0_rx_frame);
    Can0::read((int)can0_rx_frame.StdId, (int)can0_rx_frame.DLC, can0_rx_frame.Data);
  }
}

extern "C" void CAN1_RX0_IRQHandler(void)
{
  Can0_Interrupt();
}

//***Can0おわり***

CanInterface *Can1::canInterface[30];
int Can1::canInterfaceCursor = 0;

Can1::Can1() {
  setuped = false;
  filter_number = 4;
}

int  Can1::setup() {
  int returnValue = 0;
  if (!setuped) {
    Init_CAN(CAN1_NUMBER, CAN_Mode_Normal, CAN1_PORT); returnValue = 0;
    if (returnValue == 0) setuped = true;
  }
  else {
    returnValue = 0;
  }
  return returnValue;
}

int Can1::setupLoopBack() {
  int returnValue = 0;
  if (!setuped) {
    Init_CAN(CAN1_NUMBER, CAN_Mode_LoopBack, CAN1_PORT); returnValue = 0;
    if (returnValue == 0) setuped = true;
  }
  else {
    returnValue = 0;
  }
  return returnValue;
}

int Can1::addInterface(CanInterface &interfaceArg) {
  if (canInterfaceCursor >= 30) return 1;
  canInterface[canInterfaceCursor++] = &interfaceArg;
  interfaceArg.canInterfaceSetup(this);
  return 0;
}

int Can1::setId(int id) {
  int row = filter_number / 4;
  filter[filter_number] = (short)id;
  //printf("%s,set id:%x\n",__func__,id);
  Init_CANfilter(row+15, CAN_LIST, filter[row * 4 + 0], CAN_DATA, filter[row * 4 + 1], CAN_DATA, filter[row * 4 + 2], CAN_DATA, filter[row * 4 + 3], CAN_DATA);
  filter_number++;
  return 0;
}

int Can1::setIdAll() {
  Init_CANfilter(27, CAN_MASK, 0, CAN_DATA, 0, CAN_DATA, 0, CAN_DATA, 0, CAN_DATA);
  return 0;
}

int Can1::read(int id, int number, unsigned char data[8]) {
  printf("id:%x length:%x data:%x,%x,%x,%x,%x,%x,%x,%x\n\r", id, number, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
  for (int i = 0; i<canInterfaceCursor; i++) if (canInterface[i]->canId(id)) canInterface[i]->canRead(id, number, data);
  return 0;
}

int Can1::write(int id, int number, unsigned char data[8]) {
  CanTxMsg can_tx_frame;
  can_tx_frame.StdId = id;
  can_tx_frame.ExtId = 0;
  can_tx_frame.IDE = CAN_Id_Standard;
  if (number != 0)can_tx_frame.RTR = CAN_RTR_Data;
  else can_tx_frame.RTR = CAN_RTR_Remote;
  can_tx_frame.DLC = (uint8_t)number;
  can_tx_frame.Data[0] = data[0];
  can_tx_frame.Data[1] = data[1];
  can_tx_frame.Data[2] = data[2];
  can_tx_frame.Data[3] = data[3];
  can_tx_frame.Data[4] = data[4];
  can_tx_frame.Data[5] = data[5];
  can_tx_frame.Data[6] = data[6];
  can_tx_frame.Data[7] = data[7];
  send_can2(&can_tx_frame);
  return 0;
}

CanRxMsg can1_rx_frame;

void Can1_Interrupt() {
  if (CAN_GetITStatus(CAN2, CAN_IT_FMP0)) {
    CAN_Receive(CAN2, CAN_FIFO0, &can1_rx_frame);
    Can1::read((int)can1_rx_frame.StdId, (int)can1_rx_frame.DLC, can1_rx_frame.Data);
  }
}

extern "C" void CAN2_RX0_IRQHandler(void)
{
  Can1_Interrupt();
}


