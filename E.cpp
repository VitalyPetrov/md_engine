#include <iostream>
bool Checker(int current_element){
        bool result_flag;
	result_flag = true;
        for (int k = 2; k < current_element; k++) {
                if (current_element % k == 0) {
                        result_flag = false;
                        break;
                }
        }
        return result_flag;
}

	

int main()
{
	int N;
	int array[N];
	std::cin >> N;
	for (int i = 2; i <= N; i++) {
		if (Checker(i) == true) {
			std::cout << i << " ";
		}
	}
}
