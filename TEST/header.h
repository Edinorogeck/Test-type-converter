#pragma once
#include <unordered_map>
#include <string>
#include <vector>


using std::unordered_map;
using std::string;


enum class data_type {
	unsigned_int, signed_int, fixed_point, float_point
};

class data_type_inf
{
public:
	unsigned int byte_lenght;
	unsigned int bit_lenght;
	data_type_inf() {};
	data_type_inf(unsigned int byte_l, unsigned int bit_l) : byte_lenght{ byte_l }, bit_lenght{ bit_l } {}
};

class unsigned_int_inf : public data_type_inf {
public:
	bool order;
	unsigned_int_inf(bool ord) : data_type_inf(byte_lenght, bit_lenght), order{ ord } {}
};

class signed_int_inf : public data_type_inf {
public:
	bool order;
	int negative_coding;
	signed_int_inf(bool ord, int neg_cod) : data_type_inf(byte_lenght, bit_lenght), order{ ord }, negative_coding{ neg_cod } {}
};

class fixed_point_inf : public data_type_inf {
public:
	bool order;
	int negative_coding;
	double low_order_cost;
	fixed_point_inf(unsigned int byte_l, unsigned int bit_l, bool ord, int neg_cod, double loc) : data_type_inf(byte_l, bit_l), order{ ord }, low_order_cost{ loc } {}
};

class float_point_inf : public data_type_inf {
public:
	size_t exponent_lenght;
	int base;
	float_point_inf(size_t exp_len, int bs) : data_type_inf(byte_lenght, bit_lenght), exponent_lenght{ exp_len }, base{ bs } {}
};






























struct parametr {
	//размер параметра(в битах)
	unsigned int lenght;
	//номер первого сообщения
	unsigned int first_msg_number;
	//начало параметра в сообщении
	unsigned int start_bit;
	//в скольких сообщениях передается параметр
	unsigned int msgs_needed;
	//тип параметра
	data_type type;
	//особенности типа параметра (например для целых знаковых
	//чисел - как кодируется отрицательные числа)
	data_type_inf inf;
};














//Колличество сообщений
const size_t number_msg = 2;
const char interfc[] = "RS-232";

const size_t msg_sz = 11;
const size_t msg_byte_sz = 2;
const unsigned int msg_start_bit = 1;
const size_t msg_data_sz = 8;

unordered_map <string, parametr> par_struct;
const string par_nm_1 = "ugol_krena";


//вектор, содержащий порядок передаваемых параметров
std::vector <string> parametrs_order;

void header_actions() {
	par_struct[par_nm_1] = parametr{ 12, 0, 0, 2, data_type::fixed_point, fixed_point_inf(2, 16, 1, 0, 0.16) };
	parametrs_order[0] = par_nm_1;
}
