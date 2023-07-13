#include <cmath>



typedef unsigned char BYTE;


/**
 * @brief функция-преобразователь без знакового целочисленного типа
 *
 * @param x вход функции - массив байтов
 * @param byte_lenght длина входного массива в байтах
 * @param bit_lenght длина целого числа в битах
 * @param order порядок следования разрядов (1 - от младших к старшим, 0 - наоборот)
 * @return unsigned int выход - преобразованное без знаковое целое число
 */
unsigned int unsigned_integer_data_type_conversion(BYTE* x, size_t byte_lenght, size_t bit_lenght, bool order) {
	if (order) {
		unsigned int result = 0;
		for (size_t i = bit_lenght - 1, j = byte_lenght - 1; i >= 0; --i) {
			result += (1 << bit_lenght - i - 1) * ((x[j] & (1 << (7 - (i - j * 8)))) == 0 ? 0 : 1);
			if (i - j * 8 == 0)
				--j;
			if (i == 0)
				break;
		}
		return  result;
	}
	else {
		unsigned int result = 0;
		for (size_t i = 0, j = 0; i < bit_lenght; ++i) {
			result += (1 << i) * ((x[j] & (1 << (7 - (i - j * 8)))) == 0 ? 0 : 1);
			if (i == (j + 1) * 8 - 1)
				++j;
		}
		return  result;
	}
	return 0;
}



/**
 * @brief функция-преобразователь знакового целочисленного типа
 *
 * @param x вход функции - массив байтов
 * @param byte_lenght длина входного массива в байтах
 * @param bit_lenght длина целого числа в битах
 * @param order порядок следования разрядов (1 - от младших к старшим, 0 - наоборот)
 * @param negative_coding метод кодировки отрицательных чисел (0 - прямой код, 1 - обратный код, 2 - дополнительный код)
 * @return int выход - преобразованное знаковое целое число
 */
int signed_integer_data_type_conversion(BYTE* x, size_t byte_lenght, size_t bit_lenght, bool order, int negative_coding) {
	if (bit_lenght > sizeof(int) * 8) {
		// ошибка: bit_lenght больше, чем размер типа данных int
		return 0;
	}
	if (order) {
		if ((x[0] & 0x80) == 0) { //сравнение первого бита знака с 0-м, 0х80 = 10000000 в двоичной системе
			return unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order);
		}
		else {
			if (negative_coding == 0) { // прямой код
				return unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order) - (1 << bit_lenght);
			}
			if (negative_coding == 1) { //обратный код
				return ~(unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order) - (1 << bit_lenght));
			}
			if (negative_coding == 2) { //дополнительный код
				return ~(unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order) - (1 << bit_lenght)) - 1;
			}
		}
	}
}




/**
 * @brief функция-преобразователь вещественного типа с фиксированной запятой
 *
 * @param x вход функции - массив байтов
 * @param byte_lenght длина входного массива в байтах
 * @param bit_lenght длина целого числа в битах
 * @param order порядок следования разрядов (1 - от младших к старшим, 0 - наоборот)
 * @param negative_coding метод кодировки отрицательных чисел (0 - прямой код, 1 - обратный код, 2 - дополнительный код)
 * @param low_order_price цена младшего разряда числа с фиксированной точкой
 * @return double выход - преобразованное вещественное число
 */
double fixed_point_data_type_conversion(BYTE* x, size_t byte_lenght, size_t bit_lenght, bool order, int negative_coding, double low_order_price) {
	return signed_integer_data_type_conversion(x, byte_lenght, bit_lenght, order, negative_coding) * low_order_price;
}






/**
 * @brief функция-преобразователь вещественного типа с плавающей запятой
 *
 * @param x вход функции - массив байтов
 * @param byte_lenght длина входного массива в байтах
 * @param bit_lenght длина целого числа в битах
 * @param exponent_length длина экспоненты в битах
 * @param base база числа с плавающей точкой
 * @return double выход - преобразованное вещественное число
 */
double float_point_data_type_conversion(BYTE* x, size_t byte_lenght, size_t bit_lenght, size_t exponent_length, int base) {
	
	int sign, exponent = 0, mantissa = 0;
	size_t mantissa_lenght = bit_lenght - exponent_length - 1;
	sign = (x[0] & 0x80) == 0 ? 1 : -1;
	for (size_t i = exponent_length, j = (exponent_length + 1) / 8 - 1; i > 0; --i) {
		exponent += (1 << exponent_length - i) * ((x[j] & (1 << (7 - (i - j * 8)))) == 0 ? 0 : 1);
		if (i - j * 8 == 0)
			--j;
	}
	for (size_t i = bit_lenght - 1, j = byte_lenght - 1; i > exponent_length; --i) {
		mantissa += (1 << bit_lenght - i - 1) * ((x[j] & (1 << (7 - (i - j * 8)))) == 0 ? 0 : 1);
		if (i - j * 8 == 0)
			--j;
	}
	return sign * pow(base, exponent) * mantissa;
}
