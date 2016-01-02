#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "Declarations.h"

int main(int argc, char* argv[]) {
	
	int machineSize = 0;
	int pageSize = 0;
	int processSize = 0;
	int jobMix = 0;
	int numRefProcess = 0;
	
	bool verbose = false;
	bool randVerbose = false;
	int verboseLvl = 0;

	ALG algorithm;
	
	string algorithmName;
	machineSize = atoi(argv[1]);
	pageSize = atoi(argv[2]);
	processSize = atoi(argv[3]);
	jobMix = atoi(argv[4]);
	numRefProcess = atoi(argv[5]);
	algorithmName.assign(argv[6]);
	if (!algorithmName.compare(string("lifo")))
		algorithm = LIFO;
	if (!algorithmName.compare(string("random")))
		algorithm = RAND;
	if (!algorithmName.compare(string("lru")))
		algorithm = LRU;
	verboseLvl = atoi(argv[7]);
	if (verboseLvl == 1)
		verbose = true;
	if (verboseLvl == 2){
		verbose = true;
		randVerbose = true;
	}

	printf("\n*********************\n*Starting Simulation*\n*********************\n\n");

	Pager p(machineSize, pageSize, processSize, jobMix, numRefProcess, algorithm, verbose, randVerbose);
	p.displayTopInfo();
	p.runPaging();
	p.displayBottomInfo();

	return 0;
}  