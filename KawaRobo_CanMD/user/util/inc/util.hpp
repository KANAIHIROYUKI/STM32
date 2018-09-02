#pragma once
#include "util.h"
#include <complex>
/*******************************************
 * utilplus ver1.1 2015/2/15
 * This is a program made for mathematically calculation.
 * This is the first kitrp's microcomputer program that uses classes!
 * [Dependency]
 * util 1.1+
 *
 * [Note]
 *
 * [Author]
 * Tomoki Nagatani
 *
 * [Change history]
 * ver1.1 2016/ 2/ 15 Add PhysicalObject,Change,UpEdge,DownEdge,quadraticEquationSolver,cubicEquationSolver. RingBuffer fatal bug fix.RingBuffer add class support.
 * ver1.0 2015/ 2/ 6 Add revolution. Add operator overload for Coord. Add sign.Dependency update to util 1.1+.
 * ver0.2 2015/ 1/16 Addition of add functions.
 * ver0.1 2015/ 1/11 The first version.Constuctor ,read and write members available.
 *
 ******************************************/

class Coord{
protected:
	float cartesianXData;
	float cartesianYData;
	float cartesianZData;
	float eularXData;
	float eularYData;
	float eularZData;

public:
	Coord(){
		cartesianXData=0;
		cartesianYData=0;
		cartesianZData=0;
		eularXData=0;
		eularYData=0;
		eularZData=0;
	};
	Coord(Coord const &coord);
	Coord(float cx,float cy,float cz);
	Coord(float cx,float cy,float cz,float ex,float ey,float ez);

	void cartesian(float cx,float cy,float cz);
	float cartesianX();
	float cartesianY();
	float cartesianZ();
	void cartesianX(float x);
	void cartesianY(float y);
	void cartesianZ(float z);
	void cartesianXAdd(float x);
	void cartesianYAdd(float y);
	void cartesianZAdd(float z);

	void polar(float radius,float theta,float phi);
	float polarRadius();
	float polarTheta();
	float polarPhi();
	void polarRadius(float radius);
	void polarTheta(float theta);
	void polarPhi(float phi);
	void polarRadiusAdd(float radius);
	void polarThetaAdd(float theta);
	void polarPhiAdd(float phi);

	void eular(float x,float y,float z);
	float eularX();
	float eularY();
	float eularZ();
	void eularX(float x);
	void eularY(float y);
	void eularZ(float z);
	void eularXAdd(float x);
	void eularYAdd(float y);
	void eularZAdd(float z);

	float x();
	float y();
	float z();
	float radius();
	float theta();
	float phi();

	float ex();
	float ey();
	float ez();

	float Reradius(float deg);

	void revolutionXY(float angle);
	void revolutionYZ(float angle);
	void revolutionZX(float angle);

	void revolutionX(float angle);
	void revolutionY(float angle);
	void revolutionZ(float angle);

    void revolutionXYCartesian(float angle);
    void revolutionYZCartesian(float angle);
    void revolutionZXCartesian(float angle);

    void revolutionXCartesian(float angle);
    void revolutionYCartesian(float angle);
    void revolutionZCartesian(float angle);

	void revolutionEuler(float a, float b, float c);
	void revolutionEulerinverse(float a, float b, float c);

};

Coord operator+(Coord coord1,Coord coord2);
Coord operator-(Coord coord1,Coord coord2);
Coord operator*(float value,Coord coord2);
Coord operator*(Coord coord1,float value);
Coord operator/(Coord coord1,float value);
Coord operator/(float value,Coord coord2);

class PhysicalObject{
public:
	Coord position;
	Coord velocity;
	Coord acceleration;
	Coord jerk;
	void gainTime(float seconds){velocity=velocity+acceleration*seconds;position=position+velocity*seconds;};
};

int sign(int val);
int sign(float val);

template<typename T=char,int bufSize=256> class RingBuffer{

public:
    T data[bufSize+2];
    int readPointer;
    int writePointer;

    RingBuffer(){
        readPointer=0;
        writePointer=0;
    };
    int write(T value){
        int next=(writePointer + 1) % (bufSize+1);
        if(readPointer==next)return 1;
        data[writePointer]=value;
        writePointer = next;
        return 0;
    }
    T& read(){
        if(readPointer==writePointer) return data[readPointer];
        int oldReadPointer=readPointer;
        readPointer = (readPointer + 1) % (bufSize+1);
        return data[oldReadPointer];
    }
    T& peek(){
    	if(readPointer==writePointer) return data[readPointer];
		return data[readPointer];
    }
    T& peek(int pointer){
        if(pointer>=length()||pointer<0) return data[readPointer];
        int tReadPointer = (readPointer + pointer) % (bufSize+1);
        return data[tReadPointer];
    }
    void clear(){ readPointer = 0; writePointer = 0; };
    void remove(){
    	if(isEmpty())return;
    	if(writePointer>0)writePointer--;
    	else writePointer+=bufSize;
    };
    int isEmpty(){return  (readPointer==writePointer)?(1):(0);}
    int isFull(){return (readPointer==((writePointer + 1) % (bufSize+1)))?(1):(0);}
    int size(){return bufSize;};
    int length(){
    	if(writePointer>=readPointer) return writePointer-readPointer;
        else                          return bufSize-(readPointer-writePointer-1);
    }
};

class Change{
public:
    int oldValue;
    int operator()(int value){
        if(oldValue!=value){
            oldValue=value;
            return 1;
        }else{
            oldValue=value;
            return 0;
        }
    };
};
class UpEdge{
public:
    bool oldValue;
    int operator()(bool value){
        if(oldValue==false&&value==true){
            oldValue=value;
            return 1;
        }else{
            oldValue=value;
            return 0;
        }
    };
};
class DownEdge{
public:
    bool oldValue;
    int operator()(bool value){
        if(oldValue==true&&value==false){
            oldValue=value;
            return 1;
        }else{
            oldValue=value;
            return 0;
        }
    };
};

template <typename T>
class Average{
public:
	T valueInt,returnValue;
	int valueCnt;
	T read(){
		if(valueCnt){
			returnValue = valueInt/valueCnt;
			valueCnt = 0;
			valueInt = 0;
		}
		return returnValue;
	}
	void reset(){
		valueCnt = 0;
		valueInt = 0;
		returnValue = 0;
	}
	T peek(){
		if(valueCnt)returnValue = valueInt/valueCnt;
		return returnValue;
	}
	void stack(T value){
		valueInt += value;
		valueCnt++;
	}
};

template <typename T>
class Minimum{
public:
	T valueMin;
	int valueNumber;
	T read(){
		valueNumber = 0;	//ç≈è¨ílÇë„ì¸ÇµÇΩâÒêî
		return valueMin;
	}
	T peek(){
		return valueMin;
	}
	void stack(T value){
		if(valueNumber == 0 || valueMin > value){
			valueMin = value;
			valueNumber++;
		}
	}
};

template <typename T>
class Maximum{
public:
	T valueMax;
	int valueNumber;
	T read(){
		valueNumber = 0;	//ç≈è¨ílÇë„ì¸ÇµÇΩâÒêî
		return valueMax;
	}
	T peek(){
		return valueMax;
	}
	void stack(T value){
		if(valueNumber == 0 || valueMax < value){
			valueMax = value;
			valueNumber++;
		}
	}
};



//a*x^2+b*x^1+c=0
int quadraticEquationSolver(float &x1,float &x1i,float &x2,float &x2i,float a,float b,float c);
int quadraticEquationSolver(std::complex<float> &x1,std::complex<float> &x2,float a,float b,float c);

//a*x^3+b*x^2+c*x^1+d=0
int cubicEquationSolver(std::complex<float> &x1,std::complex<float> &x2,std::complex<float> &x3
                        ,float a,float b,float c,float d);
int cubicEquationSolver(float &x1r,float &x1i,float &x2r,float &x2i,float &x3r,float &x3i
                        ,float a,float b,float c,float d);
