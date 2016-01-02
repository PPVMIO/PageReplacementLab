#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
using namespace std;

enum ALG {LIFO, RAND, LRU};

struct FTVal {
	int procNum, pageNum, time, addedTime;
	bool isUsed;
	
};

class FT
{
	int usedNum;
	std::vector<FTVal> frameTable;
public:
	FT(int size);
	FT(void);
	FTVal handleEvictionLRU(int* evicted);				
	FTVal handleEvictionLIFO(int* evicted);
	FTVal handleEvictionRAND(int numOfPages, int* evicted, FILE *file, bool randVerbose);   

	int isHit(int procNum, int pageNum);
	int addToTable(int procNum, int pageNum, int time);
	int isFull(){
		return usedNum >= (int)frameTable.size();
	};
	bool updateTime(int procNum, int pageNum, int time);
	int getRand(FILE *file, bool randVerbose);
	void setFTVal(int frameNum, FTVal newValue);
};

class Process
{
	int processSize, jobMix, numRef, currentWord, pageSize, currentNumRef, numEvict,
		residencySum, faultCount;
	bool finish;
	
public:
	Process(void);
	Process(int processSize, int jobMix, int numRefProcess, int procNum, int pageSize);
	bool isFinish(){return finish;};
	void goToNextRef(int procNum, FILE *file, bool randVerbose);
	int getRand(FILE *file, bool randVerbose);
	
	int getCurrentWord(){return currentWord;};
	int getCurrentPage(){
		return currentWord/pageSize;
	};
	int getResidencySum(){return this -> residencySum;};
	int getFaltCount(){return this -> faultCount;};
	int getNumEvict(){return this -> numEvict;};

	void evictedUpdate(int residencyTime){
		numEvict++; 
		residencySum += residencyTime;
	};	
	void displayProcessInfo(int procNum);
	void addFaultCount(){this -> faultCount++;};
};


class Pager
{
	FILE *file;
	int machineSize, pageSize, processSize, jobMix,
		numRefProcess, time;
	
	ALG algorithm;
	bool verbose;
	bool randVerbose;
	
	FT frameTable;
	std::vector<Process> processes;
	bool isComplete();
	bool isProcFinish(int procNum){
		return this -> processes.at(procNum).isFinish();
	};
	FTVal replacement(ALG algorithm, int* evicted);				
public:
	Pager(int machineSize, int pageSize, int processSize, int jobMix, int numRefProcess, ALG algorithm, bool verbose, bool randVerbose);
	void displayTopInfo();
	void runPaging();

	void displayBottomInfo();
};



