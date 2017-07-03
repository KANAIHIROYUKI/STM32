#include "util.h"

template<typename T=char,int bufferSize = 128>
class RingBuffer{
public:
  T buffer[bufferSize + 2],null;
  int bufferFront;
  int bufferBack;
  RingBuffer(){
	  bufferFront = 0;
	  bufferBack = 0;
  }

  T read(){
    if(bufferBack == bufferFront)return null;      //バッファにデータなし
    bufferBack++;
    if(bufferSize <= bufferBack)bufferBack = 0; //アドレスオーバーフロー
    return buffer[bufferBack];
  }
  T peek(){
    return buffer[bufferBack + 1];
  }
  void push(T data){
    if(bufferSize <= bufferFront)bufferFront = 0; //アドレスオーバーフロー
    bufferFront++;
    if(bufferBack == bufferFront)return;  //バッファオーバーフロー
    buffer[bufferFront] = data;
  }
};
