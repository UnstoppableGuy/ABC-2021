#include <chrono>
#include <iostream>
#include <omp.h>
#include <cassert>

const long long length = 25000;
const long long chunk = 16;

int8_t matrix[length * length];
int8_t vector[length];
int8_t matrix_parallel[length * length];
int8_t matrix_series[length * length];


void createMatrix() {
	for (long long i = 0; i < length; i++)
		for (long long j = 0; j < length; j++)
			matrix[i * length + j] = rand();
}

void createVector() {
	for (long long i = 0; i < length; i++)
		vector[i] = rand();
}

int main() {

	//for (size_t temp = 0; temp < 20; temp++)
	//{
		long long i = 0, j = 0, k = 0;
		createMatrix();
		createVector();

		auto start_time = std::chrono::steady_clock::now();

#pragma omp parallel for private(i,k) schedule(static)
		for (i = 0; i < length; i++) {
			for (k = 0; k < length; k++) {
				matrix_parallel[i * length + k] += matrix[i * length + k] * vector[k];
			}
		}
		auto end_time = std::chrono::steady_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		std::cout << "Parallel algorithm running time: " << (double)time.count() / 1000 << std::endl;


		start_time = std::chrono::steady_clock::now();
		for (i = 0; i < length; i++) {
			for (k = 0; k < length; k++) {
				matrix_series[i * length + k] += matrix[i * length + k] * vector[k];
			}
		}

		end_time = std::chrono::steady_clock::now();
		time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		std::cout << "Running time of the sequential algorithm: " << (double)time.count() / 1000 << std::endl;

		for (i = 0; i < length; i++)
			if (matrix_parallel[i] != matrix_series[i]) {
				std::cout << "Error in calculating" << std::endl;
				return 404;
			}
				
	//}
}
