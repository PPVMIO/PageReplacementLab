#include <cmath>
#include <fstream>
#include "Declarations.h"



Pager::Pager(int machineSize, int pageSize, int processSize, int jobMix, int numRefProcess, ALG algorithm, bool verbose, bool randVerbose)
{
	this ->  machineSize = machineSize;
	this ->  pageSize = pageSize;
	this ->  processSize = processSize;
	this ->  jobMix = jobMix;
	this ->  numRefProcess = numRefProcess;
	this ->  algorithm = algorithm; 
	this ->  verbose = verbose; 
	this ->  randVerbose = randVerbose;
	
	file = fopen("random.txt", "r");
	this ->  frameTable = FT(machineSize/pageSize);
	if (jobMix != 1)
		for (int i = 0 ; i < 4; i++)
			processes.push_back(Process(processSize, jobMix, numRefProcess, i+1, pageSize));
	else
		processes.push_back(Process(processSize, jobMix, numRefProcess, 1, pageSize));
	
	this ->  time = 1;

}

void Pager::runPaging(){
	while(!isComplete()){
		for (int i = 0 ; i< (int)processes.size(); i++){
			for (int ref = 0; ref<3; ref++){
				if (processes.at(i).isFinish()){
					break;
				}
				if (verbose){
					printf("%d reference word %d (page %d) at time %d: ", i + 1, processes.at(i).getCurrentWord(), processes.at(i).getCurrentPage(), time);
					
				}
				if (frameTable.isHit(i, processes.at(i).getCurrentPage()) != -1){		//hit
					if (verbose){
						printf("Hit in frame, %d\n", frameTable.isHit(i, processes.at(i).getCurrentPage()));
						
					}
					if (algorithm == LRU){
						if (!frameTable.updateTime(i, processes.at(i).getCurrentPage(), time)){
							throw "ERROR DOESN'T EXIST IN FRAME TABLE";
						}
					}
				}
				else{															//page fault
					if (verbose){
						printf("Fault, ");
					}
					processes.at(i).addFaultCount();
					if (frameTable.isFull()){									//need to evict
						int tempEvictedFrom = -1;
						FTVal tempEvicted = replacement(algorithm, &tempEvictedFrom);
						//compute residency time
						this -> processes.at(tempEvicted.procNum).evictedUpdate(time - tempEvicted.addedTime);
						//set new value
						FTVal temp;
						temp.procNum = i;
						temp.pageNum = processes.at(i).getCurrentPage();
						temp.isUsed = true;
						temp.time = time;
						temp.addedTime = time;
						this -> frameTable.setFTVal(tempEvictedFrom, temp);
						if (verbose)
							printf("evicting page %d of %d from frame %d.\n", tempEvicted.pageNum, tempEvicted.procNum + 1, tempEvictedFrom);
						
						
					}
					else{														//use free frame
						int frame = frameTable.addToTable(i, processes.at(i).getCurrentPage(), time);
						if (frame == -1){
							throw "should not be full but it is";
						}
						else{
							if (verbose){
								cout<<"using free frame "<<frame<<".\n";
							}
						}
					}
				}
				processes.at(i).goToNextRef(i, file, randVerbose);
				time++;
			}
		}
	}
}

bool Pager::isComplete(){
	for (int i = 0 ; i < (int)processes.size(); i++){
		if (!processes.at(i).isFinish()){
			return false;
		}
	}
	return true;
}





FTVal Pager::replacement(ALG algorithm, int* evicted){
	FTVal result;
	switch (algorithm){
	case LIFO:	
		result = frameTable.handleEvictionLIFO(evicted);
		break;
	case RAND:
		result = frameTable.handleEvictionRAND(machineSize/pageSize, evicted, file, randVerbose);
		break;
	case LRU:
		result = frameTable.handleEvictionLRU(evicted);
		break;
	}
	return result;
}

void Pager::displayTopInfo(){

	printf("The machine size is %d\nThe page size is %d\nThe process size is %d\nThe job mix number is %d\nThe number of references per process is %d. \n", machineSize, pageSize, processSize, jobMix, numRefProcess);
	
	switch (algorithm){
		case LIFO:
			printf("The replacement aglorithm is LIFO.\n");
			break;
		case RAND:
			printf("The replacement algorithm is random.\n");
			break;
		case LRU:
			printf("The replacement algorithm is LRU.\n");
			break;
	}
	printf("The level of debugging output is %d.\n", verbose);
}

void Pager::displayBottomInfo(){
	int totalFaults = 0;
	int residencySum = 0;
	int EvictedSum = 0;
	cout<<"\n";
	for (int i = 0; i< (int)processes.size(); i++){
		processes.at(i).displayProcessInfo(i);
		totalFaults += processes.at(i).getFaltCount();
		residencySum += processes.at(i).getResidencySum();
		EvictedSum += processes.at(i).getNumEvict();
	}
	if (EvictedSum!= 0)
		printf("\nThe total number of faults is %d and the overall average residency is %f.\n\n", totalFaults, (double)residencySum/EvictedSum);
	else
		printf("\nThe total number of faults is %d.\n\tWith no evictions, the overall average residence is undefined.", totalFaults);
	
}