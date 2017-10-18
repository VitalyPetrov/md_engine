#include <iostream>

void qsort(int array[], int lo, int lhi)
{
	int i = lo, j = lhi;
	int changer;
	int middle = array[(lhi + lo) / 2];

	while (i <= j) {
		while (array[i] < middle) {
			i++;
		}
		while (array[j] > middle) {
			j--;
		}
		if (i <= j) {
			changer = array[i];
			array[i] = array[j];
			array[j] = changer;
			i++;
			j--;
		}
	}

	// Рекурсивно повторяем алгоритм
	if (lo < j)
		qsort(array, lo, j);
	if (i < lhi)
		qsort(array, i, lhi);

}

int main()
{
	int N, step;
	std::cin >> N;
	int array[N];
	bool result_flag = true;
	for (int k1 = 0; k1 < N; k1++) {
		std::cin >> array[k1];
	}
	qsort(array, 0, N - 1);
	for (int k2 = 0; k2 < N - 2; k2++) {
		if ((array[k2 + 1] - array[k2] == 0)
		    && (array[k2 + 2] - array[k2 + 1] == 0)) {
			result_flag = false;
			break;
		}
	}
	if (result_flag == true) {
		std::cout << 1 << std::endl;
	} else {
		std::cout << 0 << std::endl;
	}

}
