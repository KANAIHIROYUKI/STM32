#include "can_test.hpp"
#include <stdlib.h>

int argvtodata(char *str){
	int i=0,j=0,value=0;
	int size=0;
	while(str[size]!='\0'){
		size++;
	}
	for(i=0;i<size;i++){
		for(j=0;j<6;j++){
			if(str[i]=='f'-j){
				value+=(15-j)*pow(16,size-(i+1));
				j=20;
			}
		}
		if(j==6){
			for(j=0;j<10;j++)
				if(str[i]=='9'-j){
					value+=(9-j)*pow(16,size-(i+1));
					j=20;
				}
		}
	}
	return value;
}

CanTest::CanTest(Console &console):ConsoleCommand(console){
	cans=0;
	dataLoad=false;
	idNum=0;
	can0SetupFlag=false;
	for(int i=0;i<30;i++)
		ids[i]=0;
	setCanStatus(new Can0,'0',(char*)"can0");
	setCanStatus(new Can1,'1',(char*)"can1");
}

void CanTest::setCanStatus(Can *can,char key,char *canName){
	cankey[cans].can=can;
	cankey[cans].key=key;
	cankey[cans].canName=canName;
	cans++;
}

int CanTest::setup(){
	consolePrintf((char*)"  c a n   Test->input  'can'     and push 'ENTER'key\r\n");
	for(int i=0;i<cans;i++){
		cankey[i].can->addInterface(this);
	}
	return 0;
}

int CanTest::canId(int id){
	if(ids[0]!=-1){
		for(int i=0;i<idNum;i++){
			if(ids[i]==id)
				return 1;
		}
	}else
		return 1;
	return 0;
}

int CanTest::commandRequest(int argc,char **argv){
	int canNum=0;
	int id=0;
	int number=0;
	int i=0;
	unsigned char data[8];
	for(i=0;i<8;i++)
		data[i]=0;
	if(!dataLoad){
		for(i=0;i<cans;i++){
			consolePrintf((char*)"'%c'key:%s\r\n",cankey[i].key,cankey[i].canName);
		}
		consolePrintf((char*)"can0 setup normalMode  ->ex) can 0 normal\r\n");
		consolePrintf((char*)"can1 setup loopbackMode->ex) can 1 loopback\r\n");
		consolePrintf((char*)"can0 filter id set     ->ex) can 0 setid ID\r\n");
		consolePrintf((char*)"can1 filter set id all ->ex) can 1 setidall\r\n");
		consolePrintf((char*)"can0 data sent         ->ex) can 0 write ID NUM DATA0 DATA1 DATA2 DATA3...DATA7\r\n");
		consolePrintf((char*)"ID   :Hexadecimal        ex) 400,f40,fff,280\r\n");
		consolePrintf((char*)"Num  :The number of data ex) can 0 write 400 5 af ff 11 45 14\r\n");
		consolePrintf((char*)"DATAn:Hexadecimal        ex) ff,10,11,45,14\r\n");
		dataLoad=true;
	}
	if(argc>=3){
		canNum=atof(argv[1]);
		if(!(strcmp(argv[2],"loopback"))){
			if(canNum==0||can0SetupFlag==true){
				consolePrintf((char*)"can%d	LoopBack MODE\r\n",canNum);
				cankey[canNum].can->setupLoopBack();
				if(canNum==0)
					can0SetupFlag=true;
			}else{
				consolePrintf((char*)"can0&can%d	LoopBack MODE\r\n",canNum);
				cankey[0].can->setupLoopBack();
				can0SetupFlag=true;
				cankey[canNum].can->setupLoopBack();
			}
		}else if(!(strcmp(argv[2],"normal"))){
			if(canNum==0||can0SetupFlag==true){
				consolePrintf((char*)"can%d	Normal MODE\r\n",canNum);
				cankey[canNum].can->setup();
				if(canNum==0)
					can0SetupFlag=true;
			}else{
				consolePrintf((char*)"can0&can%d	Normal MODE\r\n",canNum);
				cankey[0].can->setup();
				can0SetupFlag=true;
				cankey[canNum].can->setup();
			}
		}else if(!(strcmp(argv[2],"setidall"))){
			consolePrintf((char*)"can%d	setIdAll\r\n",canNum);
			cankey[canNum].can->setIdAll();
			ids[0]=-1;
			idNum++;
		}else if(argc>=3){
			if(!(strcmp(argv[2],"setid"))){
				if(argc>=4){
					id=argvtodata(argv[3]);
					if(id>0){
						ids[idNum]=id;
						idNum++;
						cankey[canNum].can->setId(id);
						consolePrintf((char*)"can%d setId:%x\r\n",canNum,id);
					}else {
						consolePrintf((char*)"setId error\r\n");
					}
				}
			}else if(!(strcmp(argv[2],"write"))){
				if(argc>=5){
					id=argvtodata(argv[3]);
					number=atof(argv[4]);
					for(i=0;i<number;i++){
						if(argc>=i+6){
							data[i]=argvtodata(argv[i+5]);
						}
					}
					consolePrintf((char*)"send a data:%x %x %x %x %x %x %x %x %x %x\r\n",id,number,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
					cankey[canNum].can->write(id,number,data);
				}
			}
		}
	}
	return 0;
}
