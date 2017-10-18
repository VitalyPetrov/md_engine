#include <iostream>
using namespace std;

int main()
{
	int N, element;
	std::cin >> N;
	int end_array[N];
	for (int i = 1; i <= N; i++) {
		std::cin >> element;
		end_array[N - i] = element;
	}
	for (int k = 0; k < N; k++) {
		std::cout << end_array[k] << " ";
	}

}
