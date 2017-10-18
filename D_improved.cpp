#include <iostream>
using namespace std;

int main()
{
	int N, shear, changer;
	std::cin >> N >> shear;
	int array[N + shear];
	// считываем массив
	for (int i = 0; i < N; i++) {
		std::cin >> array[i];
	}
	//
	for (int j = 0; j < shear; j++) {
		array[N + j] = array[j];
	}
	for (int k = shear; k < N + shear; k++) {
		std::cout << array[k] << " ";
	}
}
