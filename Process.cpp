#include "Declarations.h"

Process::Process(void)
{
}

Process::Process(int processSize, int jobMix, int numRef, int procNum, int pageSize){
	this ->  processSize = processSize;
	this ->  jobMix = jobMix;
	this ->  numRef = numRef;
	this -> residencySum = 0;
	this -> faultCount = 0;
	this ->  pageSize = pageSize;
	this ->  currentNumRef = 1;
	this -> numEvict = 0;
	currentWord = 111 * procNum % processSize;
	finish = false;
}


void Process::goToNextRef(int procNum, FILE *file, bool randVerbose){
	int randomNum = getRand(file, randVerbose);
	int tempRand;
	double y = randomNum / (2147483648 + 1.0);
	if (jobMix == 1)
		currentWord += 1;
	else if (jobMix == 2)
		currentWord += 1;
	else if (jobMix == 3){
		tempRand = getRand(file, randVerbose);
		currentWord = tempRand;
	}
	else{
		if (procNum == 0){
			if (y >= 0.75)
				currentWord -= 5;
			else
				currentWord += 1;	
		}
		else if(procNum == 1){
			if ( y >= 0.75)
				currentWord += 4;
			else
				currentWord += 1;
		}
		else{
			if (y >= 0.5){
				if (y >= 0.625){
					if (y < 0.75)
						currentWord += 4;	
					else{
						tempRand = getRand(file, randVerbose);
						currentWord = tempRand;
					}
				}
				else{
					currentWord -= 5;		
				}
			}
			else
				currentWord += 1;

		}
			
	}
	currentWord = (currentWord + processSize) % processSize;
	currentNumRef++;
	if (currentNumRef > numRef){
		finish = true;
	}
}


int Process::getRand(FILE *file, bool randVerbose){
	char tempStr [15];
	if (fscanf(file, "%s", tempStr) == EOF) {
		rewind(file);
		return getRand(file, randVerbose);
	}
	else{
		if (randVerbose)
			printf("uses random number: %i\n", atoi(tempStr));
			
		return (atoi(tempStr));
	}
}



void Process::displayProcessInfo(int procNum){
	if (numEvict != 0)
		printf("Process %d had %d faults and %f average residency.\n", procNum + 1, this -> faultCount, ((double)residencySum/numEvict));
	else
		printf("Process %d had %d faults\n\tWith no evictions, the average residence is undefined", procNum + 1, this -> faultCount);
		
}