#include <iostream>

int main() {
	int N, summ = 0;
	std::cin >> N;
	int array[N];
	for (int i = 0; i < N; i++) {
		std::cin >> array[i];
		if ((i % 3 == 0) && (i % 7 == 0)) {
			summ += array[i];
			}
		}
	std::cout << summ << std::endl;

}
	 	
