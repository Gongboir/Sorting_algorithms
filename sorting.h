#ifndef _SORTING_H_
#define _SORTING_H_

#include <vector>
#include <iostream>
#include <string>

using std::vector; // we do not want the entire std namespace in our cpp file

namespace sorting
{

	//****************
	// insertionSort *
	//****************
	void insertionSort(vector<int> &arr);

	//************
	// QuickSort *
	//************
	void quickSort(vector<int> &arr, int left, int right);

	//************
	// MergeSort *
	//************
	void merge(vector<int> &a, vector<int> &b, int low, int pivot, int high);
	void mergeSort(vector<int> &a, vector<int> &b, int low, int high);

	//************
	// Heapsort  *
	//************
	void heapSort(vector<int> &a, int n);

	//************
	// Shellsort *
	//************
	void shellSort_2n(vector<int> &a, int n);

	//***************
	// CountingSort *
	//***************
	void countingSort(vector<int> &arr, int left, int right);

	//************
	// RadixSort *
	//************
	void radixSort(vector<int> &arr);
	//*******************
	// Helper functions *
	//*******************
	void randomizeVector(vector<int> &array, int n);

}
#endif
