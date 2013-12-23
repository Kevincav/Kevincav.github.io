#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

void countingSort(unsigned int *array, int size, int range) {
	unsigned int *temp = new unsigned int[size];
	// initialized to zero.
	unsigned int *hist = new unsigned int[range+1]();
	
	// forms the histogram
	for (int i=0;i<size;i++)
		hist[array[i]]++;
		
	// prefix sum
	for (int i=1;i<=range;i++)
		hist[i] += hist[i-1];
		
	// time for the actual sort
	for (int i=size-1;i>=0;i--)
		temp[--hist[array[i]]] = array[i];

	std::copy(temp, temp + size, array);
	delete[] temp;
	delete[] hist;
}

// a.out size range
int main (int argc, char *argv[]) {
	const int size  = atoi(argv[1]);
	const int range = atoi(argv[2]);
	srand(time(NULL));

	unsigned int *array = new unsigned int[size];
	for (int i=0;i<size;i++)
		array[i] = rand() % range;
	
	struct timeval tp;
	int rtn = gettimeofday(&tp, NULL);
	double start = (double) tp.tv_sec+(1.e-6)*tp.tv_usec;
	
	countingSort(array, size, range);
	
	rtn = gettimeofday(&tp, NULL);
	double stop = (double) tp.tv_sec+(1.e-6)*tp.tv_usec;
	
	for (int i=1;i<size;i++)
		if (array[i] < array[i-1]) {
			std::cout << "Array was not successfully sorted!\n";
			break;
		}
	
	std::cout << "Sorted " << size << " elements in " << stop - start << " seconds.\n";

	delete[] array;
	return 0;
}