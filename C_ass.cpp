#include <iostream>

void SortArray(int array[], const int size)
{
	const int N = size ;
	for (int i = 1; i < N; i++) {
		for (int k = 0; k < N - i; k++) {
			if (array[k] > array[k + 1]) {
				int changer = array[k];
				array[k] = array[k + 1];
				array[k + 1] = changer;
			}
		}
	}
}
