
#include "Declarations.h"

FT::FT(void)
{
}

FT::FT(int size)
{
	FTVal temp;
	temp.isUsed = false;
	temp.time = -1;
	temp.addedTime = -1;
	temp.procNum = -1;
	temp.pageNum = -1;
	frameTable.resize(size, temp);
	usedNum = 0;
}



int FT::isHit(int procNum, int pageNum){
	for (int i = 0 ; i < (int)frameTable.size(); i++){
		if (frameTable.at(i).procNum == procNum && frameTable.at(i).pageNum == pageNum){
			return i;
		}
	}
	return -1;
}

int FT::addToTable(int procNum, int pageNum, int time){
	if (isFull())
		return -1;
	else{
		FTVal temp;
		temp.procNum = procNum;
		temp.pageNum = pageNum;
		temp.isUsed = true;
		temp.time = time;
		temp.addedTime = time;
		for (int i = (int)frameTable.size()-1; i >= 0; i--){
			if (!frameTable.at(i).isUsed){
				frameTable.at(i) = temp;
				usedNum++;
				return i;
			}
		}
		return -1;
	}
}

FTVal FT::handleEvictionLRU(int* evicted){
	if (!isFull()){
		throw "ERROR NOT FULL";
	}
	int tempMinVal = -1;
	FTVal resultValue;
	int result;
	for (int i =0; i< (int)frameTable.size(); i++){
		if (frameTable.at(i).time < tempMinVal || tempMinVal == -1){
			tempMinVal = frameTable.at(i).time;
			result = i;
		}
	}
	resultValue = frameTable.at(result);
	frameTable.at(result).isUsed = false;
	frameTable.at(result).pageNum = -1;
	frameTable.at(result).procNum = -1;
	frameTable.at(result).time = -1;
	frameTable.at(result).addedTime = -1;
	*evicted = result;
	return resultValue;
}


FTVal FT::handleEvictionLIFO(int* evicted){
	if (!isFull()){
		throw "ERROR NOT FULL";
	}
	int tempMaxValue = 30000;
	FTVal resultValue;
	int result;
	for (int i =0; i< (int)frameTable.size(); i++){
		if (frameTable.at(i).time > tempMaxValue || tempMaxValue == 30000){
			tempMaxValue = frameTable.at(i).time;
			result = i;
		}
	}
	resultValue = frameTable.at(result);
	frameTable.at(result).isUsed = false;
	frameTable.at(result).pageNum = -1;
	frameTable.at(result).procNum = -1;
	frameTable.at(result).time = -1;
	frameTable.at(result).addedTime = -1;
	*evicted = result;
	return resultValue;
}


FTVal FT::handleEvictionRAND(int numOfPages, int* evicted, FILE *file, bool randVerbose){
	FTVal resultValue;
	if (!isFull()){
		throw "ERROR NOT FULL";
	}
	int r = this -> getRand(file, randVerbose);
	if (randVerbose)
		printf("\tuses random number: %d\n", r);
	
	int result = r % numOfPages ;
	resultValue = frameTable.at(result);
	frameTable.at(result).isUsed = false;
	frameTable.at(result).pageNum = -1;
	frameTable.at(result).procNum = -1;
	frameTable.at(result).time = -1;
	frameTable.at(result).addedTime = -1;
	*evicted = result;
	return resultValue;
}

bool FT::updateTime(int procNum, int pageNum, int time){
	for (int i = 0 ; i < (int)frameTable.size(); i++){
		if (frameTable.at(i).procNum == procNum && frameTable.at(i).pageNum == pageNum){
			frameTable.at(i).time = time;
			return true;
		}
	}
	return false;
}

int FT::getRand(FILE *file, bool randVerbose){
	char tempStr [15];
	if (fscanf(file, "%s", tempStr) == EOF) {
		rewind(file);
		return getRand(file, randVerbose);
		
	}
	else{
		if (randVerbose)
			printf("\tuses random number: %d\n", atoi(tempStr));
		
		return (atoi(tempStr));
		
	}
}

void FT::setFTVal(int frameNum, FTVal newValue){
	frameTable.at(frameNum) = newValue;
}