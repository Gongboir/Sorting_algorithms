#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include "sorting.h"
#include <iomanip>

using namespace std;

//benchmark functions

void benchmarkAll();


// Helper function to print an array
void printArray(const std::vector<int>& arr) {
	for (int num : arr)
		std::cout << num << " ";
	std::cout << std::endl;
}


int main(int argc, char** argv) {

	Catch::Session().run();  

	std::cout << "\nPress Enter to run measurement" << std::endl;
	std::cin.get();

  
	benchmarkAll();
  
	return 0;
}

// executes benchmark for all algorithms and writes the avg. runtime of 10 runs into a file
void benchmarkAll()
{
	//file stream
	ofstream quicksortMeasurement;
	quicksortMeasurement.open("quicksort.txt", ios::out | ios::app);

	ofstream mergesortMeasurement;
	mergesortMeasurement.open("mergesort.txt", ios::out | ios::app);

	ofstream heapsortMeasurement;
	heapsortMeasurement.open("heapsort.txt", ios::out | ios::app);

	ofstream shellsortMeasurement;
	shellsortMeasurement.open("shellsort_2n.txt", ios::out | ios::app);

	ofstream countingMeasurement;
	countingMeasurement.open("counting.txt", ios::out | ios::app);

	ofstream radixMeasurement;
	radixMeasurement.open("radix.txt", ios::out | ios::app);

	//benchmark parameters / variables
	double dtime;
	int nStart = 1000;
	int nStep = 10000;
	int nEnd = 1000000;

	vector<int> V;


	//actual benchmark loop
	for (int n = nStart; n <= nEnd; n += nStep) {

		//"progress bar"
		std::cout << "Running Benchmarks with n: " << n << std::endl;

		double timeSumQuick = 0;
		double timeSumMerge = 0;
		double timeSumHeap = 0;
		double timeSumShell = 0;
		double timeSumInsert = 0;
		double timeSumCounting = 0;
		double timeSumRadix = 0;

		for (int i = 0; i < 10; i++)
		{
			//generate n random integers
			sorting::randomizeVector(V, n);
			vector<int> V2(V);
			vector<int> V3(V);
			vector<int> V4(V);
			vector<int> V5(V);
			vector<int> V6(V);
			vector<int> V7(V);

			//start measurement
			//dtime = omp_get_wtime();
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

			//execute QuickSort
			sorting::quickSort(V, 0, V.size() - 1);

			//stop time
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			auto dtime = end - begin;
			//dtime = omp_get_wtime() - dtime;
			timeSumQuick += dtime.count();

			begin = std::chrono::steady_clock::now();

			//execute MergeSort
			vector<int> tempArray;
			tempArray.resize(V2.size());
			sorting::mergeSort(V2, tempArray, 0, V2.size() - 1);

			//stop time
			end = std::chrono::steady_clock::now();
			dtime = end - begin;
			timeSumMerge += dtime.count();

			begin = std::chrono::steady_clock::now();

			//execute HeapSort
			sorting::heapSort(V3, V3.size());

			//stop time
			end = std::chrono::steady_clock::now();
			dtime = end - begin;
			timeSumHeap += dtime.count();

			begin = std::chrono::steady_clock::now();

			//execute ShellSort_2n
			sorting::shellSort_2n(V4, V4.size());

			//stop time
			end = std::chrono::steady_clock::now();
			dtime = end - begin;
			timeSumShell += dtime.count();

			begin = std::chrono::steady_clock::now();

			//stop time
			end = std::chrono::steady_clock::now();
			dtime = end - begin;
			timeSumInsert += dtime.count();

			begin = std::chrono::steady_clock::now();

			//execute CountingSort
			sorting::countingSort(V6, 0, V6.size() - 1);

			//stop time
			end = std::chrono::steady_clock::now();
			dtime = end - begin;
			timeSumCounting += dtime.count();

			begin = std::chrono::steady_clock::now();

			//execute RadixSort
			sorting::radixSort(V7);

			//stop time
			end = std::chrono::steady_clock::now();
			dtime = end - begin;
			timeSumRadix += dtime.count();

		}



		//write to file
		quicksortMeasurement << n << "\t" << setprecision(10) << scientific << timeSumQuick / 10 << endl;
		mergesortMeasurement << n << "\t" << setprecision(10) << scientific << timeSumMerge / 10 << endl;
		heapsortMeasurement << n << "\t" << setprecision(10) << scientific << timeSumHeap / 10 << endl;
		shellsortMeasurement << n << "\t" << setprecision(10) << scientific << timeSumShell / 10 << endl;
		countingMeasurement << n << "\t" << setprecision(10) << scientific << timeSumCounting / 10 << endl;
		radixMeasurement << n << "\t" << setprecision(10) << scientific << timeSumRadix / 10 << endl;
	}

	//close file handle
	quicksortMeasurement.close();
	mergesortMeasurement.close();
	heapsortMeasurement.close();
	shellsortMeasurement.close();
	countingMeasurement.close();
	radixMeasurement.close();
}