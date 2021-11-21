#include <iostream>
#include <mmintrin.h>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <iomanip>

void _m64_i16_print(std::vector<__m64> v, std::string message = ":")
{
	std::cout << std::setw(4) << message;
	for (size_t i = 0; i < v.size(); i++)
		for (size_t j = 0; j < 4; j++)
			std::cout << std::setw(7) << (int)v[i].m64_i16[j] << " ";
	std::cout << std::endl;
}

void _m64_i8_print(__m64 v, std::string message = ":") {
	std::cout << std::setw(4) << message;
	for (size_t i = 0; i < 8; i++)
		std::cout << std::setw(7) << (int)v.m64_i8[i] << " ";
	std::cout << std::endl;
}

template <typename T>
void print_numbers(std::string message, T* v) {
	std::cout << std::setw(4) << message;
	for (char i = 7; i >= 0; i--)
		std::cout << std::setw(7) << (int)v[i] << " ";
	std::cout << std::endl;
}

__m64 __mm_set_8_b_random(__int8* copy) {
	for (size_t i = 0; i < 8; i++)
		copy[i] = rand() % UCHAR_MAX - CHAR_MIN;
	__m64 result = _mm_set_pi8(copy[0], copy[1], copy[2], copy[3], copy[4], copy[5], copy[6], copy[7]);
	return result;
}

std::vector<__m64> __mm_set_16_b_random(__int16* copy) {
	for (size_t i = 0; i < 8; i++)
		copy[i] = rand() % USHRT_MAX - SHRT_MIN;
	std::vector<__m64> result;
	result.push_back(_mm_set_pi16(copy[4], copy[5], copy[6], copy[7]));
	result.push_back(_mm_set_pi16(copy[0], copy[1], copy[2], copy[3]));
	return result;
}

// Ввод восьми восьмибитных числа
__m64 _mm_set_8_b_numbers()
{
	std::string number = "";
	std::getline(std::cin, number, '\n');
	std::stringstream ss(number);
	long* v = new long[8];
	for (size_t i = 0; i < 8; i++)
	{
		std::string s;
		ss >> s;
		v[i] = (long)std::atol(s.c_str());
	}
	return _mm_set_pi8(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
}

// Ввод восьми шестнадцатибитных числа
std::vector<__m64> _mm_set_16_b_numbers()
{
	std::string number = "";
	std::getline(std::cin, number, '\n');
	std::stringstream ss(number);
	long* v = new long[8];
	for (size_t i = 0; i < 8; i++)
	{
		std::string s;
		ss >> s;
		v[i] = (long)std::atol(s.c_str());
	}
	std::vector<__m64> result;
	result.push_back(_mm_set_pi16(v[0], v[1], v[2], v[3]));
	result.push_back(_mm_set_pi16(v[4], v[5], v[6], v[7]));
	return result;
}

// 8 бит * 8 бит = 2 массива 16 бит
std::vector<__m64> _mm_mul_pi8_e(__m64 a, __m64 b)
{
	__m64 zero = _mm_setzero_si64();
	// Строим маску, сравнивая 0 с каждым элементом, если 0 больше, то число хранится в доп коде
	__m64 mask_a = _mm_cmpgt_pi8(zero, a);
	// Чередуем часть массива с элементами маски, там где число отрицательное, будет -1, иначе 0
	__m64 a_lo = _mm_unpacklo_pi8(a, mask_a);
	__m64 a_hi = _mm_unpackhi_pi8(a, mask_a);

	__m64 mask_b = _mm_cmpgt_pi8(zero, b);
	__m64 b_lo = _mm_unpacklo_pi8(b, mask_b);
	__m64 b_hi = _mm_unpackhi_pi8(b, mask_b);

	// Умножаем соответствующие числа
	__m64 a_b_lo = _mm_mullo_pi16(a_lo, b_lo);
	__m64 a_b_hi = _mm_mullo_pi16(a_hi, b_hi);

	std::vector<__m64> result;
	result.push_back(a_b_lo);
	result.push_back(a_b_hi);
	return result;
}

int main()
{
	srand(time(NULL));
	// 22. F[i] = A[i] - (B[i] * C[i]) - D[i]
		__int8* arrayA = new __int8[8];
		__int8* arrayB = new __int8[8];
		__int8* arrayC = new __int8[8];
		__int16* arrayD = new __int16[8];

		//__int16* arraya = new __int16[8];
		__m64 B, C, A_temp; // массив, в котором 8 8-битных элементов
		std::vector<__m64> A, D; // 2 массива, в каждом по 4 16-битных элемента
		std::cout << "\F[i] = A[i] - (B[i] * C[i]) - D[i] \n";

		//std::cout << "A: ";
		//A_temp = _mm_set_8_b_numbers();
		A_temp = __mm_set_8_b_random(arrayA);

		__m64 zero = _mm_setzero_si64();
		// Строим маску, сравнивая 0 с каждым элементом, если 0 больше, то число хранится в доп коде
		__m64 mask_a = _mm_cmpgt_pi8(zero, A_temp);
		// Чередуем часть массива с элементами маски, там где число отрицательное, будет -1, иначе 0
		__m64 a_lo = _mm_unpacklo_pi8(A_temp, mask_a);
		__m64 a_hi = _mm_unpackhi_pi8(A_temp, mask_a);

		A.push_back(a_lo);
		A.push_back(a_hi);

		//std::cout << "B: ";
		//B = _mm_set_8_b_numbers();
		//std::cout << "C: ";
		//C = _mm_set_8_b_numbers();
		//std::cout << "D: ";
		//D = _mm_set_16_b_numbers();


		//A = __mm_set_8_b_random(arraya);
		B = __mm_set_8_b_random(arrayB);
		C = __mm_set_8_b_random(arrayC);
		D = __mm_set_16_b_random(arrayD);
		//_m64_i8_print(B, "B1:  ");
		//print_numbers(arrayB, "B2  ");
		//_m64_i8_print(C, "C: ");
		std::vector<__m64>	B_C = _mm_mul_pi8_e(B, C);
		//_m64_i16_print(A, "A:   ");
		//_m64_i16_print(B_C, "BC:  ");
		std::vector<__m64> A_B_C, F;

		A_B_C.push_back(_mm_sub_pi16(A[0], B_C[0]));
		A_B_C.push_back(_mm_sub_pi16(A[1], B_C[1]));
		//_m64_i16_print(A_B_C, "ABC: ");
		F.push_back(_mm_sub_pi16(A_B_C[0], D[0]));
		F.push_back(_mm_sub_pi16(A_B_C[1], D[1]));

		print_numbers("A:", arrayA);
		//_m64_i16_print(A, "A64:");
		print_numbers("B:", arrayB);
		//_m64_i8_print(B, "B64:");
		print_numbers("C:", arrayC);
		//_m64_i8_print(C, "C64:");
		print_numbers("D:", arrayD);
		//_m64_i16_print(D, "D64:");


		_m64_i16_print(F, "F:");
		std::cout << "\nHere's answer after calculating it manually:\nF:  ";
		for (char i = 7; i >= 0; i--)
		{
			std::cout << std::setw(7) <<
				int(
					(__int16)arrayA[i] -
					((__int8)arrayB[i] * (__int8)arrayC[i]) -
					(__int16)arrayD[i]
					) << ' ';
		}
	return 0;
}