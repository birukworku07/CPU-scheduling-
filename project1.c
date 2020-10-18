#include <stdio.h>
#include <stdlib.h>


// function to find the number of non-voluntary context switches
int nonVolSwitches (int* myArray, int length) {
	int count = 0;
	for (int i = 0; i < length; ++i) {
		//checking if consecutive pids are not equal
		if ( myArray[i] != myArray[i+1]) {
			for (int j = i+1; j < length; ++j) {
				//checking if pid has another burst
				if (myArray[i] == myArray[j]) {
					++count;
					break;
				}
			}
		}
	}
	return count;	
}
// returns the number of times a pid is duplicated
int duplicateTimes(int* p, int num, int lengthC) {
	int count = 0;
	for (int i = 0; i < lengthC; ++i) {
		if (p[i] == num) {
			count++;
		}
	}
	return count;
}

// checks wheather a pid 'has been seen before'
int hasBeenSeenBefore ( int* array, int num, int lengthC) {
	int result = 1; 
	for (int i = 0; i < lengthC; ++i) {
		if (array[i] == num) {
			return 0;
		}
	}

	return 1;
}

//returns the runningTime
int runningTime (int* pidA, int* burstarray, int num, int lengthC) {
	int sum = 0;
	for (int i = 0; i < lengthC; ++i) {
		if (pidA[i] == num) {
			sum += burstarray[i];
		}
	}
	return sum;
}
//returns the average waiting time
double avgWaitingTime (int* burstTimeC, int* pidC, int length, int uniquePids){

	int arrayUnique[uniquePids];
	int sum;
	int h;
	int j;
	int times;
	int total = 0;
	int runningT;
	int k = 0;

       for (int i = 0; i < length; ++i) {
	 if (hasBeenSeenBefore(arrayUnique,pidC[i],k) == 1){
	   arrayUnique[i] = pidC[i];
	   ++k;	   
       	   times = duplicateTimes(pidC, pidC[i], length);
	   j = 0;
	   h = 0;
	   sum = 0;
	   	while ((j < times) && (h < length)) {
			sum = sum + burstTimeC[h];
			if (pidC[i] == pidC[h]) {
				++j;
			}
			++h;
		}
		runningT = runningTime(pidC, burstTimeC, pidC[i], length);
		total = total + (sum - runningT);
	
          } 
      }

	double avg = (double) total / (double) uniquePids;
  	return avg;	
}
//returns the average Turn Around time
double avgTurnAroundTime (int* pidC, int* burstTimeC, int length, int uniquePids) {
	int arrayUnique[uniquePids];
	int sum;
	int h;
	int j;
	int times;
	int total = 0;
	int runningT;
	int k = 0;

	for (int i = 0; i < length; ++i) {
		if(hasBeenSeenBefore(arrayUnique,pidC[i],k) == 1) {
			arrayUnique[i] = pidC[i];
			++k;
			times = duplicateTimes(pidC, pidC[i], length);
			j = 0;
			h = 0;
			sum = 0;

			while ((j < times) && (h < length)) {
				sum = sum + burstTimeC[h];
				if (pidC[i] == pidC[h]) {
					++j;
				}
				++h;
			}
			total = total + sum;
	

		}
	}
	double avg = (double) total / (double) uniquePids;
	return avg;


}
//returns the response time
double responseTime (int* burstTimeC, int* pidC, int length, int uniquePids) {
	
	int responseTime[length];
	responseTime[0] = 0;
	int sum = 0;
	
	for (int i = 1; i < length; ++i) {
		if(hasBeenSeenBefore(pidC, pidC[i],i) == 1) {
			responseTime[i] = responseTime[i-1] + burstTimeC[i-1];
		} else {
			responseTime[i] = 0;
		}
	}

	for (int i = 0; i < length; ++i) {
		sum += responseTime[i];
	}
	double avg = (double) sum / (double) uniquePids;
	return avg;

}
//returns the throughput 
double throughPut(int* burstTimeC, int uniquePids, int length) {
	int total = 0;

	for (int i = 0; i < length; ++i) {
		total = total + burstTimeC[i];
	}
	double through = (double) uniquePids/ (double) total;
	return through;
}

int main (int argc, char** argv) {
	//opens the file from command argument
	FILE* myFile = fopen(argv[1], "r");
	int capitalP = 0;
	int numProcesses = 0;
	int n = 0;
	
	
	//scanning the first three inputs
	fscanf(myFile, "%d", &capitalP);
	fscanf(myFile, "%d", &numProcesses);
	fscanf(myFile, "%d", &n);
	//declaring and initializing arrays to store data
	int pid[n];
	int pidCounter = 0;;
	int burstTime[n];
	int burstTimeCounter = 0;
	int array[n*3];
	
	int i = 0;
	//reading the rest of the data input
	while (!feof(myFile)) {
		fscanf(myFile, "%d", &array[i]);
		++i;
	}
	fclose(myFile);
	
	//populating the pid array
	int j = 0;
	while (j < (n*3)) {
		pid[pidCounter] = array[j];
		j = j + 3;
		pidCounter++;
	}
	//populating the burstTime array
	int h = 1;
	while (h < (n*3)) {
		burstTime[burstTimeCounter] = array[h];
		h = h + 3;
		burstTimeCounter++;
	}
	

	printf("%d\n", numProcesses);
	
	// calling and printing the statistics
	double responseT = responseTime(burstTime, pid, n, numProcesses);
	double avgTurnT = avgTurnAroundTime(pid, burstTime, n, numProcesses);
	double avgWaitingT = avgWaitingTime(burstTime, pid, n, numProcesses);
	double throughP = throughPut(burstTime, numProcesses,n);
	int switches = nonVolSwitches(pid, n);

	printf ("%d\n", switches);
	double cpuUtilization = 100.00;
	printf("%.2f\n", cpuUtilization);
	printf("%.2f\n", throughP);
	printf("%.2f\n",avgTurnT);
	printf("%.2f\n",avgWaitingT);
	printf("%.2f\n",responseT);

}
