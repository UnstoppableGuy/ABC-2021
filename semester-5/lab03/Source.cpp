#include <chrono>
#include <iostream>
#include <omp.h>
#include <cassert>

const int length = 8192;
//const int chunk = 16;

double matrix[length * length];
double vector[length];
double matrix_parallel[length * length];
double matrix_series[length * length];

void createMatrix() {
	for (int i = 0; i < length; i++)
		for (int j = 0; j < length; j++)
			matrix[i * length + j] = rand();
}

void createVector() {
	for (int i = 0; i < length; i++)
		vector[i] = rand();
}

int main() {
	//for (size_t temp = 0; temp < 20; temp++)
	//{
		int i = 0, j = 0, k = 0;
		createMatrix();
		createVector();

		auto start_time = std::chrono::steady_clock::now();

#pragma omp parallel for private(i,k) schedule(static)
		for (i = 0; i < length; i++) {
			for (k = 0; k < length; k++) {
				matrix_parallel[i * length + k] = matrix[i * length + k] * vector[k];
			}
		}
		auto end_time = std::chrono::steady_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		std::cout << "Parallel algorithm running time: " << (double)time.count() / 1000 << std::endl;


		start_time = std::chrono::steady_clock::now();
		for (i = 0; i < length; i++) {
			for (k = 0; k < length; k++) {
				matrix_series[i * length + k] = matrix[i * length + k] * vector[k];
			}
		}

		end_time = std::chrono::steady_clock::now();
		time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		std::cout << "Running time of the sequential algorithm: " << (double)time.count() / 1000 << std::endl;

		for (i = 0; i < length; i++)
			assert(matrix_parallel[i] == matrix_series[i]);
	//}
}
