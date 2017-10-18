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
	int initial_number, probab_element;
	int array_of_numbers[4], probab_numbers[1000];
	std::cin >> initial_number;
	int iterator = 0;
	while (initial_number != 0) {
		array_of_numbers[iterator] = initial_number % 10;
		initial_number = initial_number / 10;
		iterator += 1;
		
	}
	iterator = 0; // обнуляем счетчик, используем его позднее
	// делаем массив из натуральных 4значных чисел, составленных из этих цифр
	for (int i1 = 0; i1 < 4; i1++){
		for (int i2 = 0; i2 < 4; i2++){
			for (int i3 = 0; i3 < 4; i3++){
				for (int i4 = 0; i4 < 4; i4++){
					probab_element = array_of_numbers[i1] * 1000 + array_of_numbers[i2] * 100 + array_of_numbers[i3] * 10 + array_of_numbers[i4];
					if ((Checker(probab_element) == true) && (probab_element >= 1000)){
						probab_numbers[iterator] = probab_element;
						iterator += 1;
					}
				}
			}
		}
	}
	int min = 10001; // ищем минимум в получившемся массиве натуральных чисел
	for (int k = 0; k < iterator; k++) {
		if (probab_numbers[k] < min) {
			min = probab_numbers[k]; 
		}
	}
	std::cout << min << std::endl;
}
