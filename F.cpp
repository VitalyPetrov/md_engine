#include <iostream>

int main()
{
	int N;
	std::cin >> N;
	int array[N];
	for (int j = 0; j < N; j++) {
		std::cin >> array[j];
	}
	for (int i = 1; i < N; i++) {
		for (int k = 0; k < N - i; k++) {
			if (array[k] > array[k + 1]) {
				int changer = array[k];
				array[k] = array[k + 1];
				array[k + 1] = changer;
			}
		}
	}
	for (int j_1 = 0; j_1 < N; j_1++) {
		std::cout << array[j_1] << " ";
	}
}
