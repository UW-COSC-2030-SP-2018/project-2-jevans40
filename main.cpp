/*References:
https://en.wikipedia.org/wiki/Quicksort
https://en.wikipedia.org/wiki/Merge_sort
https://www.geeksforgeeks.org/binary-search/
P. J. Weinberger Hash Function from lab 10, because it looked cool
*/

#include "BloomFilter.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <time.h>
#include <chrono>


using std::cout;
using std::cin;
using std::endl;

//Change these to change test perameters
int numoftests = 1000;

void swap(int &first, int &second);

void makeRandVector(std::vector<int> &vect, int size);
int randnum();

void quickSort( std::vector<int> &list, int low, int high);
int quickSortPartitioner(std::vector<int> &list, int low, int high);

int binarySearch(std::vector<int> list,int target,int low,int high);

void mergeSort(std::vector<int> &list, int n);
void SplitMerge(std::vector<int> &copy, int iBegin, int  iEnd, std::vector<int> &list);
void Merge(std::vector<int> &list, int iBegin, int iMiddle, int iEnd, std::vector<int> &copy);
void CopyArray(std::vector<int> &list, std::vector<int> &copy);

unsigned int intHash(const std::string& str);
unsigned int intHash(const int &);


int main() {
	bool quicktest = true;
	bool binarytest = true;
	bool mergetest = true;
	bool hashtest = true;
	bloomFilter filter(407,5);
	
	for (int i = 0; i < numoftests; i++) {
		std::vector<int> testvect;
		makeRandVector(testvect,randnum());

		std::vector<int> quickvect = testvect;
		std::vector<int> stdvect = testvect;
		quickSort(quickvect, 0, quickvect.size() - 1);
		std::sort(stdvect.begin(), stdvect.end());
		if (quickvect != stdvect) {
			quicktest = false;
		}



		std::vector<int> mergevect = testvect;
		mergeSort(mergevect, mergevect.size());
		if (mergevect != stdvect) {
			mergetest = false;
		}


		std::vector<int> searchvect = stdvect;
		srand(time(NULL));
		int val = randnum();
		bool stdsearch = std::binary_search(searchvect.begin(), searchvect.end(), val);
		int bins = binarySearch(searchvect, val, 0, searchvect.size()-1);
		if (((bins != -1) != stdsearch)) {
			binarytest = false;
		}

		if (intHash(val) != intHash(std::to_string(val))) {
			hashtest = false;
		}

		//for fun
		uint8_t f[] = "key";
		filter.add(f,3);



	}
		cout << "Does quicksort equal the std::sort? " << quicktest << endl;
		cout << "Does mergesort equal the std::sort? " << mergetest << endl;
		cout << "Does Binary search equal the std::find? " << binarytest << endl;
		cout << "Did the hash of the int equal a string of the int?" << hashtest << endl;

	system("Pause");
}

void makeRandVector(std::vector<int> &vect, int size) {
	if (size < 0) size = 0;
	if (size > 1000) size = 1000;
	for (int i = 0; i < size; i++) {
		vect.push_back(randnum());
	}
}

int randnum() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);
	return distribution(generator);
}

void swap(int &first,int &second) {
	int tmp = first;
	first = second;
	second = tmp;

}

void quickSort( std::vector<int> &list, int low, int high) {
	if (low < high) {
		int p = quickSortPartitioner(list, low, high);
		quickSort(list, low, p - 1);
		quickSort(list, p + 1, high);
	}
}
int quickSortPartitioner( std::vector<int> &list, int low, int high) {
	int pivot = list[high];
	int i = low - 1;
	for (int j = low; j <= high - 1; j++) {
		if (list[j] < pivot){
			i = i + 1;
			swap(list[i], list[j]);
		}
	}
		swap(list[i + 1], list[high]);
	return i + 1;
}


int binarySearch(std::vector<int> list,int target, int low, int high) {
	if (high >= low)
	{
		int mid = low + (high - low) / 2;

		if (list[mid] == target) {
			return mid;
		}

		if (list[mid] > target) {
			return binarySearch(list, target, low, mid - 1);
		}

		return binarySearch(list, target, mid + 1, high);
	}
	return -1;
	
}

 void mergeSort(std::vector<int> &list, int n)
{
	std::vector<int> tmparray;
	CopyArray(list, tmparray);           
	SplitMerge(tmparray, 0, n, list);   
}


 void SplitMerge(std::vector<int> &copy, int iBegin,int  iEnd, std::vector<int> &list)
{
	if (iEnd - iBegin < 2)                       
		return;                               
												
	int iMiddle = (iEnd + iBegin) / 2;           
												
	SplitMerge(list, iBegin, iMiddle, copy); 
	SplitMerge(list, iMiddle, iEnd, copy); 
											 
	Merge(copy, iBegin, iMiddle, iEnd, list);
}


void Merge(std::vector<int> &list,int iBegin,int iMiddle,int iEnd, std::vector<int> &copy)
{
	int i = iBegin;
	int j = iMiddle;


	for (int k = iBegin; k < iEnd; k++) {

		if (i < iMiddle && (j >= iEnd || list[i] <= list[j])) {
			copy[k] = list[i];
			i = i + 1;
		}
		else {
			copy[k] = list[j];
			j = j + 1;
		}
	}
}

void CopyArray(std::vector<int> &list, std::vector<int> &copy)
{
	for (int k = 0; k < list.size() ; k++)
		copy.push_back(list[k]);
}

unsigned int intHash(const std::string& str)
{
	unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		hash = (hash << OneEighth) + str[i];

		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return hash;
}

unsigned int intHash(const int &hashableint)
{
	std::string str = std::to_string(hashableint);
	unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		hash = (hash << OneEighth) + str[i];

		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return hash;
}