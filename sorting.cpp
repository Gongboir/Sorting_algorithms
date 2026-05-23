// ADS Praktikum 2.1 Sortieren Musterl�sung
// Stand April 2025
#include "sorting.h"
#include <algorithm>

namespace sorting
{

	//****************
	// insertionSort *
	//****************
	void insertionSort(vector<int> &arr)
	{
		for (int i = 1; i < arr.size(); i++) // this must start from 1, cause we consider the first element to be trivially sorted
		{
			int key = arr[i]; // store the element, cause this is the key element we will use to compare the elements in the left side
			int j = i - 1;
			while (j >= 0 && arr[j] > key) // check if the element in left is bigger till we reach the end of all elements
			{
				arr[j + 1] = arr[j];
				j--;
			}
			arr[j + 1] = key; // once all the elemnts are shifted towards the right, then simply place the key in the right place
							  // here we do j+1 cause the last element decremented the array one step before getting out of the loop
		}
	}

	//************
	// QuickSort *
	//************
	// static void swap(int &a, int &b)
	// {
	// 	int temp = a;
	// 	a = b;
	// 	b = temp;
	// }

	void quickSort(vector<int> &arr, int left, int right)
	{
		if (left >= right) // Base Case:- in case there is only element left in both the sub-arrays
			return;

		int pivot = right; // we are choosing the right most element as the pivot here
		int i = left - 1;  // this is the i pointer we will use though the loop for swaps

		// loop starts from left till we reach pivot(our case last element in the array)
		for (int j = left; j < pivot; j++)
		{
			if (arr[j] < arr[pivot]) // if j is smaller than then we swap j with i+1
			{
				i++;
				std::swap(arr[i], arr[j]);
			}
		}
		i++;						   // after the swaps are done
		std::swap(arr[i], arr[pivot]); // we would find the right position of the pivot, which is -> i++

		// then we simply divide the array into 2 halves and reccursively call both the halves
		quickSort(arr, left, i - 1);
		quickSort(arr, i + 1, right); // this i+1 might lead to left > right, in case there are only 2 elements left in the array,
									  // if this comes up then we treat it as a base case and stop the reccursion
	}

	//************
	// MergeSort *
	//************
	void merge(vector<int> &a, vector<int> &b, int low, int pivot, int high)
	{
		// copy elements to b with what we will be working with
		for (int i = low; i <= high; i++)
		{
			b[i] = a[i];
		}

		int left = low;
		int right = pivot + 1;
		int a_writer = low;

		while (left <= pivot && right <= high)
		{
			if (b[left] <= b[right])
				a[a_writer] = b[left++]; // basically use the value of left, and then increment it
			else
				a[a_writer] = b[right++];
			a_writer++; // the a pointer needs to increment in both cases
		}

		// Clean up, in case there are any elements left out:-
		while (left <= pivot)
			a[a_writer++] = b[left++];
		while (right <= high)
			a[a_writer++] = b[right++];
	}

	void mergeSort(vector<int> &a, vector<int> &b, int low, int high)
	{
		// std::sort(a.begin(), a.end());
		// here the array b is simply a buffer array to store the working range

		// Base case; mostly
		if (low >= high) // the >= is important here, to be defensive if the user passes an empty array
			return;

		int pivot = low + (high - low) / 2; // this is a good practice that helps to stop INT_MAX overflow
		mergeSort(a, b, low, pivot);		// keep splitting the first half
		mergeSort(a, b, pivot + 1, high);	// now start splitting the second half
		merge(a, b, low, pivot, high);		// now merge the splitted halves
	}

	//************
	// Heapsort:- in-place array-based version, time:- o(nlogn) , space :- 0(1)*
	//************
	void Heapify(vector<int> &a, int n, int i)
	{
		// First we compute the left and right child in valid bounds of array a
		int largest = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		// Since we are creating a max heap, we find the largest element among a[i], a[left] and a[right]
		if (left < n && a[left] > a[largest])
			largest = left;
		if (right < n && a[right] > a[largest])
			largest = right;

		if (largest != i) // once we find the largest value, we swap it with a[i]
		{
			std::swap(a[i], a[largest]);
			Heapify(a, n, largest); // we reccusively continue shifting down from the new position(largest)
		}
	}

	void heapSort(vector<int> &a, int n)
	{
		// phase 1:- build the max heap
		// here we call heapify for all "non-leaf" nodes
		for (int i = n / 2 - 1; i >= 0; --i) // Anything past n/2 - 1(half) is a leaf and is already a trivially valid heap.
		{
			Heapify(a, n, i);
		}

		// Phase 2: extract the max element repeatedly
		for (int x = n - 1; x > 0; x--)
		{
			std::swap(a[0], a[x]); // we swap the first element(max heap) with the last element
			Heapify(a, x, 0);	   // now we shrink the heap by 1(x=n-1), and restore the heap property at the new a[0]
		}
	}

	//************
	// Shellsort *
	//************
	// Hier soll Hibbard implementiert werden
	void shellSort_2n(vector<int> &a, int n)
	{
		// Find largest Hibbard gap < n
		int gap = 1;
		while (gap < n)
			gap = 2 * gap + 1; // overshoots
		gap = (gap - 1) / 2;   // step back one

		for (; gap > 0; gap = (gap - 1) / 2)
		{
			for (int i = gap; i < n; i++)
			{										 // from here on we are basically doing a insertion sort:-
				int key = a[i];						 // element at index gap
				int j = i;							 // here j starts at gap(the half array index)
				while (j >= gap && a[j - gap] > key) // 1st iteration :- a[0] > a[7] if gap = 7
				{
					a[j] = a[j - gap]; // here we swap the element at gap with the element at j-gap
					j -= gap;		   // we also shift j(index) to the index of the element we swapped above
				}
				a[j] = key; // now we simply put the key in the right place
			}
		}
	}

	//***************
	// CountingSort *
	//***************
	void countingSort(vector<int> &arr, int left, int right)
	{
		// for last element simply return
		if (left >= right)
			return;

		// 1. Find min and max in the sub-array :-
		int minVal = arr[left], maxVal = arr[left];
		for (int i = left + 1; i <= right; ++i) // we are starting here from left+1 cause min and macVal are already initialised to left
		{
			minVal = std::min(minVal, arr[i]);
			maxVal = std::max(maxVal, arr[i]);
		}

		int range = maxVal - minVal + 1;
		int n = right - left + 1;

		// 2. Count occurrences
		vector<int> count(range, 0);
		for (int i = left; i <= right; ++i)
		{
			count[arr[i] - minVal]++; // this increments the index creating a frequency map
		}

		// 3. Prefix sum: count[i] becomes the end position (exclusive)
		//    in the sorted output for value (i + minVal)
		for (int i = 1; i < range; ++i)
		{
			count[i] += count[i - 1]; // simply adding i-1 to i for the entire arry
		}

		// 4. Place elements into output, walking right-to-left for stability
		vector<int> output(n);
		for (int i = right; i >= left; --i)
		{
			int idx = --count[arr[i] - minVal]; // this would give the index, same logic as frequency map but in reverse
			output[idx] = arr[i];
		}

		// Copy back into the original subrange
		for (int i = 0; i < n; ++i)
		{
			arr[left + i] = output[i];
		}
	}

	//************
	// RadixSort *
	//************
	// Stable sort of arr by the digit at place value `exp` (1, 10, 100, ...)
	static void countingSortByDigit(vector<int> &arr, int exp)
	{
		int n = arr.size();
		vector<int> output(n);
		int count[10] = {0};

		// 1. Count digit occurrences
		for (int i = 0; i < n; i++)
		{
			int digit = (arr[i] / exp) % 10; // here we extract the digit in each number=arr[i]
			count[digit]++;					 // for that digit we simply increment the index corresponding to that digit
		}

		// 2. Cumulative counts → end-positions for each digit bucket
		for (int i = 1; i < 10; i++)
		{
			count[i] += count[i - 1]; // same präfix sum login like count sort
		}

		// 3. Walk backwards to preserve stability
		for (int i = n - 1; i >= 0; i--)
		{
			int digit = (arr[i] / exp) % 10;
			count[digit]--;
			output[count[digit]] = arr[i]; // again the same extracting login like counting sort, but for each digit in the arr[i]
		}

		// 4. Copy back
		for (int i = 0; i < n; i++)
		{
			arr[i] = output[i];
		}
	}

	void radixSort(vector<int> &arr)
	{
		if (arr.empty())
			return;

		int maxVal = *std::max_element(arr.begin(), arr.end());

		// One counting-sort pass per digit position
		for (int exp = 1; maxVal / exp > 0; exp *= 10)
		{
			countingSortByDigit(arr, exp);
		}
	}

	void randomizeVector(vector<int> &array, int n)
	{
		array.resize(n);

		for (unsigned int i = 0; i < array.size(); i++)
			array[i] = rand() % 10;
		// array[i]=rand() % 1000000;
	}

}
