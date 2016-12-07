#include "console.hpp"
#include <string.h>
#include <stdio.h>
void ConsoleCommand::consolePrintf(char *pFormat, ...){
	va_list ap;
	va_start(ap, pFormat);
	if(console!=0)console->serialVprintf(pFormat,ap);
	va_end(ap);
}

int ConsoleCommand::commandSetup(Console *console){
	this->console=console;
	return this->console->addCommand(this);
}

int ConsoleCommand::newLine(char value){
	if(console!=0)return console->newLine(value);
	return 0;
}

Console::Console(Serial &serial){
	serialInterfaceSetup(&serial);
	prompt=new char[4];
	strcpy(prompt,">>>");
	echoAvailable=true;
	outputAvailable=true;
	newLineData=DEFAULT_NEWLINE;
	newLineCount=0;

	commandCursor=0;
	commandRunning=-1;

	rxBuffer.clear();

	printPrompt=1;
}

int Console::setup(int baudrate){
	//return
	serialSetup(baudrate);
	return 0;
}

int Console::addCommand(ConsoleCommand &commandClass){
	if(commandCursor>NUMBER_OF_CONSOLE_COMMANDS)return 1;
	commandClass.console=this;
	command[commandCursor++]=&commandClass;
	return 0;
}

int Console::addCommand(ConsoleCommand *commandClass){
	if(commandCursor>NUMBER_OF_CONSOLE_COMMANDS)return 1;
	commandClass->console=this;
	command[commandCursor++]=commandClass;
	return 0;
}

void Console::serialReadChar(char value)
{
	if(commandRunning!=-1) command[commandRunning]->consoleReadChar(value);
	if(rxBuffer.isFull()) return;
	if(value=='\b'){
		if(rxBuffer.length()>0)	if(echoAvailable)serialWriteString((char*)"\b \b");
		rxBuffer.remove();
	}else{
		if(echoAvailable)serialWriteChar(value);
		rxBuffer.write(value);
	}
	if(newLineData==NEWLINE_CR){
		newLineCount+=cr(value);
	}else if(newLineData==NEWLINE_CRLF){
		newLineCount+=crlf(value);
	}else{
		newLineCount+=lf(value);
	}
}

int Console::getString(char *value){
	char buf;
	int i;
//	printf((char*)"[Console::getString]");
	if(newLineData==NEWLINE_CRLF) crlf(0);
	for(i=0;rxBuffer.length()>0;i++){
		buf=rxBuffer.read();
//		serialPrintf((char*)"%c",buf);
		if(newLineData==NEWLINE_CR){
			if(cr(buf)){
				value[i]='\0';
				newLineCount--;
				return i-1;
			}
		}else if(newLineData==NEWLINE_CRLF){
			if(crlf(buf)){
				value[i-1]='\0';
				newLineCount--;
				return i-2;
			}
		}else{
			if(lf(buf)){
				value[i]='\0';
				newLineCount--;
				return i-1;
			}
		}
		value[i]=buf;
	}
	//serialPrintf((char*)"[Console::getString] no data\n");
	newLineCount=0;
	return i-1;
}

void Console::cycle(){
	if(printPrompt){serialPrintf(prompt);printPrompt=0;}
	while(newLineCount){
//	while(0){

		char str[SERIAL_BUFFER_LENGTH];
//		printf("[Console::cycle] getString");
		getString(str);

		char **argv;
		int argc=0;
		int exeFlag=0;
//		printf("[Console::cycle] setCommand...");
		argc=setCommand(&argv,str);
//		printf("done.\n");

//		printf("[Console::cycle] commandCursor:%d\n",commandCursor);

		for(int i=0;i<commandCursor;i++){
//			printf("[Console::cycle] command[%d]->commandCompare(argv[0])\n",i);
			if( command[i]->commandCompare(argv[0]) ==1 ){
				if(command[i]->commandRequest(argc,argv)){
					commandRunning=i;
				}else{
					serialPrintf(prompt);
				}
				exeFlag=1;
				break;
			}
		}
//		printf("[Console::cycle] for(int i=0;i<commandCursor;i++)\n");
		if(exeFlag==0){
			if(argc)serialPrintf((char*)"%s: command not found.\r",argv[0]);
			serialPrintf(prompt);
		}
//		printf("[Console::cycle] deleteCommand...");
		deleteCommand(&argv);
//		printf("done.\n");
	}
	if(commandRunning!=-1){
		if(command[commandRunning]->commandCycle()==0){
			commandRunning=-1;
			serialPrintf(prompt);
		}
	}
}

int Console::setNewLine(int newLineData){
	this->newLineData=newLineData;
	if(this->newLineData!=NEWLINE_CRLF&&this->newLineData!=NEWLINE_CR&&this->newLineData!=NEWLINE_LF){
		this->newLineData=NEWLINE_LF;
		return 1;
	}
	return 0;
}
void Console::echo(bool value){
	echoAvailable=value;
	if(echoAvailable)outputAvailable=true;
}
void Console::output(bool value){
	outputAvailable=value;
	if(!outputAvailable)echoAvailable=false;
}
void Console::setPrompt(char *value){
	delete[] prompt;
	prompt=new char[strlen(value)+1];
	strcpy(prompt,value);
}

int Console::newLine(char value){
	if(newLineData==NEWLINE_CR){
		return cr(value);
	}else if(newLineData==NEWLINE_CRLF){
		return crlf(value);
	}else{
		return lf(value);
	}
}

void Console::startRelative(){
	clear();
	serialWriteString((char*)"\033[1;1H");
	relativeCursorX=0;
	relativeCursorY=0;
	serialWriteString((char*)"\0337");
}

void Console::relative(int x,int y){
	serialWriteString((char*)"\0338");
	if(x-relativeCursorX>0) serialPrintf((char*)"\033[%dC",x-relativeCursorX);
	else if(x-relativeCursorX<0) serialPrintf((char*)"\033[%dD",relativeCursorX-x);
	if(y-relativeCursorY>0) serialPrintf((char*)"\033[%dB",y-relativeCursorY);
	else if(y-relativeCursorY<0) serialPrintf((char*)"\033[%dA",relativeCursorY-y);
	relativeCursorX=x;
	relativeCursorY=y;
	serialWriteString((char*)"\0337");
}

void Console::clear(){
	serialWriteString((char*)"\033[2J");
}

void Console::absolute(int x,int y){
	serialPrintf((char*)"\033[%d;%dH",y,x);
}
void Console::serialVprintf(char *format,va_list arg){

	char buffer[SERIAL_BUFFER_LENGTH];
    vsprintf(buffer,format,arg);
	if(newLineData==NEWLINE_CR) {toCR(buffer);}
	else if(newLineData==NEWLINE_CRLF) toCRLF(buffer);
	else toLF(buffer);
    serialWriteString(buffer);
}

void Console::serialWriteChar(char value){
	if(outputAvailable)serial->writeChar(value);
}

void Console::serialWriteString(char *value){
	if(outputAvailable)serial->writeString(value);
}

#define LF '\n'
#define CR '\r'

int Console::toLF(char *value){
	int crFlag=0;
	int i=0;
	for(i=0;value[i]!='\0';i++){
		if(value[i]==CR){
			crFlag=1;
			value[i]=LF;
		}else if(value[i]==LF){
			if(crFlag)delOneChar(value,i);
		}else{
			crFlag=0;
		}
	}
	return i;
}

int Console::toCR(char *value){
	int crFlag=0;
	int i=0;
	for(i=0;value[i]!='\0';i++){
		if(value[i]==CR){
			crFlag=1;
		}else if(value[i]==LF){
			if(crFlag) delOneChar(value,i);
			else value[i]=CR;
		}else{
			crFlag=0;
		}
	}
	return i;
}

int Console::toCRLF(char *value){
	int crFlag=0,i;
	for(i=0;value[i]!='\0';i++){
		//        printf("value[%d]:%c\n",i,value[i]);
		if(value[i]==CR){
			if(crFlag){
				addOneChar(value,i,LF);
				i++;
			}
			crFlag=1;
		}else if(value[i]==LF){
			if(crFlag){
				//CRLF. good
				crFlag=0;
			}else{
				//LF only
				value[i]=CR;
				addOneChar(value,i+1,LF);
				i++;
			}
			crFlag=0;
		}else{
			if(crFlag){
				//CR only
				addOneChar(value,i,LF);
				i++;
			}
			crFlag=0;
		}
	}
	return i;
}


int Console::delOneChar(char *value,int num){
	if(num<0) return 1;
	for(int i=num;value[i]!='\0';i++){
		value[i]=value[i+1];
	}
	return 0;
}

int Console::addOneChar(char *value,int num,char add){
	char bufferA=add;
	char bufferB;

	if(num<0) return 1;

	for(int i=num;value[i-1]!='\0';i++){
		bufferB=value[i];
		value[i]=bufferA;
		bufferA=bufferB;
		bufferB=value[i+1];
	}
	return 0;
}

int Console::setCommand(char ***argv,char *text){
	int numberOfCommands;
	int textPointer=0;
	int commandPointer=0;

	numberOfCommands=noc(text);

	try{
		*argv = new char*[numberOfCommands+1];
		//        printf("*argv = new char*[%d];\n",numberOfCommand+1);
	}catch(...){
		return -1;
	}

	for(int i=0;i<numberOfCommands;i++){

		//ignore space
		while(text[textPointer]==' '){
			if(text[textPointer]=='\0') break;
			textPointer++;
		}

		//check first character isn't null
		if(text[textPointer]=='\0') break;

		//count length of the command
		while(text[textPointer+commandPointer]!=' '&&text[textPointer+commandPointer]!='\0'){
			commandPointer++;
		}
		try{
			//get new memory area
			(*argv)[i]=new char[commandPointer+1];
			//            printf("*argv[%d]=new char[%d];\n",i,commandPointer+1);
		}catch(...){
			return -1;
		}

		//set null character
		(*argv)[i][commandPointer]='\0';
		//        printf("*argv[%d][%d]=%c\n",i,commandPointer,(*argv)[i][commandPointer]);
		//copy text
		for(int j=0;j<commandPointer;j++){
			(*argv)[i][j]=text[textPointer+j];
			//            printf("*argv[%d][%d]=%c\n",i,j,text[textPointer+j]);
		}
		//add text pointer
		textPointer+=commandPointer;
		//reset command pointer
		commandPointer=0;
	}

	//set sentinel
	try{
		(*argv)[numberOfCommands]=new char[2];
		//        printf("*argv[%d]=new char[2];\n",numberOfCommand);
	}catch(...){
		return -1;
	}
	(*argv)[numberOfCommands][0]='\n';
	//    printf("*argv[%d][%d]=%c\n",numberOfCommand,0,(*argv)[numberOfCommand][0]);
	(*argv)[numberOfCommands][1]='\0';
	//    printf("*argv[%d][%d]=%c\n",numberOfCommand,1,(*argv)[numberOfCommand][1]);

	return numberOfCommands;
}

int Console::deleteCommand(char ***argv){
	int i=0;
	for(i=0;(*argv)[i][0]!='\n';i++){//search sentinel
		delete[] (*argv)[i];
		//        printf("delete argv[%d]\n",i);
	}
	delete[] (*argv)[i];//delete sentinel area
	//    printf("delete argv[%d]\n",i);
	delete[] *argv;
	return i;
}

int Console::noc(char *text){
	int i=0;
	int inCommand=0;
	int count=0;

	while(text[i]!='\0'){
		if(text[i]==' '){
			if(inCommand==1){
				inCommand=0;
				count++;
			}
		}else{
			if(inCommand==0){
				inCommand=1;
			}
		}
		i++;
	}
	if(inCommand){
		count++;
	}
	return count;
}
