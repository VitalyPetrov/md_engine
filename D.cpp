#include <iostream>

int main()
{
	int N, shear, changer;
	std::cin >> N >> shear;
	int array[N], end_array[N];
	for (int i = 0; i < N; i++) {
		std::cin >> array[i];
	}

	int *p_start = array + shear;
	int *p_end = end_array;
	int counter = 1;
	while (counter < N){
		*p_end++ = *p_start++;
		counter++;
	}
	for (int k = 0; k < N; k++) {
		std::cout << end_array[k] << " ";
	}
}
