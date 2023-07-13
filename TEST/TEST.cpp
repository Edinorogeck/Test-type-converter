// TEST.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <cmath>
#include <bitset>

#include "header.h"
#include "conv_functions.cpp"


using namespace std;

typedef unsigned char BYTE;



typedef unsigned char BYTE;
int main()
{
	unordered_map<string, double> output;//массив для преобразованных данных
	header_actions();

	BYTE msgs[number_msg];//массив для сообщений
	msgs[0] = 0x00; //00000000
	msgs[1] = 0x3f; //00111111

	for (int i = 0; i < parametrs_order.size(); ++i)
	{
		parametr temp_parametr_info = par_struct[parametrs_order[i]];
		data_type_inf* temp_inf = temp_parametr_info.inf;
		int fst_msg_num = par_struct[parametrs_order[i]].first_msg_number;
		int msgs_n = par_struct[parametrs_order[i]].msgs_needed;
		BYTE* x = new BYTE[msgs_n];
		for (int j = fst_msg_num; j < fst_msg_num + msgs_n; j++)
		{
			x[j - fst_msg_num] = msgs[j];
		}
		if (par_struct[parametrs_order[i]].type == data_type::unsigned_int)
		{
			unsigned_int_inf* unsigned_temp_inf = static_cast<unsigned_int_inf*>(temp_inf);
			output[parametrs_order[i]] = unsigned_integer_data_type_conversion(x, msgs_n, unsigned_temp_inf->bit_lenght, unsigned_temp_inf->order);
		}
		if (par_struct[parametrs_order[i]].type == data_type::signed_int)
		{
			signed_int_inf* sig_temp_inf = static_cast<signed_int_inf*>(temp_inf);
			output[parametrs_order[i]] = signed_integer_data_type_conversion(x, msgs_n, sig_temp_inf->bit_lenght, sig_temp_inf->order, sig_temp_inf->negative_coding);
		}
		if (par_struct[parametrs_order[i]].type == data_type::fixed_point)
		{
			fixed_point_inf* fix_temp_inf = static_cast<fixed_point_inf*>(temp_inf);
			output[parametrs_order[i]] = fixed_point_data_type_conversion(x, msgs_n, fix_temp_inf->bit_lenght, fix_temp_inf->order, fix_temp_inf->negative_coding, fix_temp_inf->low_order_cost);
		}
		if (par_struct[parametrs_order[i]].type == data_type::float_point)
		{
			float_point_inf* flt_temp_inf = static_cast<float_point_inf*>(temp_inf);
			output[parametrs_order[i]] = float_point_data_type_conversion(x, msgs_n, flt_temp_inf->bit_lenght, flt_temp_inf->exponent_lenght, flt_temp_inf->base);
		}
	}
	//вывод преобразованных параметров для примера
	for (auto it = output.begin(); it != output.end(); ++it)
	{
		std::cout << it->first << ' ' << it->second << std::endl;
	}
}
















// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
