#include <cmath>



typedef unsigned char BYTE;


/**
 * @brief �������-��������������� ��� ��������� �������������� ����
 *
 * @param x ���� ������� - ������ ������
 * @param byte_lenght ����� �������� ������� � ������
 * @param bit_lenght ����� ������ ����� � �����
 * @param order ������� ���������� �������� (1 - �� ������� � �������, 0 - ��������)
 * @return unsigned int ����� - ��������������� ��� �������� ����� �����
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
 * @brief �������-��������������� ��������� �������������� ����
 *
 * @param x ���� ������� - ������ ������
 * @param byte_lenght ����� �������� ������� � ������
 * @param bit_lenght ����� ������ ����� � �����
 * @param order ������� ���������� �������� (1 - �� ������� � �������, 0 - ��������)
 * @param negative_coding ����� ��������� ������������� ����� (0 - ������ ���, 1 - �������� ���, 2 - �������������� ���)
 * @return int ����� - ��������������� �������� ����� �����
 */
int signed_integer_data_type_conversion(BYTE* x, size_t byte_lenght, size_t bit_lenght, bool order, int negative_coding) {
	if (bit_lenght > sizeof(int) * 8) {
		// ������: bit_lenght ������, ��� ������ ���� ������ int
		return 0;
	}
	if (order) {
		if ((x[0] & 0x80) == 0) { //��������� ������� ���� ����� � 0-�, 0�80 = 10000000 � �������� �������
			return unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order);
		}
		else {
			if (negative_coding == 0) { // ������ ���
				return unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order) - (1 << bit_lenght);
			}
			if (negative_coding == 1) { //�������� ���
				return ~(unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order) - (1 << bit_lenght));
			}
			if (negative_coding == 2) { //�������������� ���
				return ~(unsigned_integer_data_type_conversion(x, byte_lenght, bit_lenght, order) - (1 << bit_lenght)) - 1;
			}
		}
	}
}




/**
 * @brief �������-��������������� ������������� ���� � ������������� �������
 *
 * @param x ���� ������� - ������ ������
 * @param byte_lenght ����� �������� ������� � ������
 * @param bit_lenght ����� ������ ����� � �����
 * @param order ������� ���������� �������� (1 - �� ������� � �������, 0 - ��������)
 * @param negative_coding ����� ��������� ������������� ����� (0 - ������ ���, 1 - �������� ���, 2 - �������������� ���)
 * @param low_order_price ���� �������� ������� ����� � ������������� ������
 * @return double ����� - ��������������� ������������ �����
 */
double fixed_point_data_type_conversion(BYTE* x, size_t byte_lenght, size_t bit_lenght, bool order, int negative_coding, double low_order_price) {
	return signed_integer_data_type_conversion(x, byte_lenght, bit_lenght, order, negative_coding) * low_order_price;
}






/**
 * @brief �������-��������������� ������������� ���� � ��������� �������
 *
 * @param x ���� ������� - ������ ������
 * @param byte_lenght ����� �������� ������� � ������
 * @param bit_lenght ����� ������ ����� � �����
 * @param exponent_length ����� ���������� � �����
 * @param base ���� ����� � ��������� ������
 * @return double ����� - ��������������� ������������ �����
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
