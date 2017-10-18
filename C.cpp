#include <iostream>

int main()
{
	int N, summ_min, summ_max;
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
	summ_min = array[0] + array[1];
	summ_max = array[N - 1] + array[N - 2];
	std::cout << summ_min << " " << summ_max << std::endl;
}
