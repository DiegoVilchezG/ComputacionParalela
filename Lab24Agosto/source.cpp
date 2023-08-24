#include <iostream>
#include <chrono>
#define MAX 1000
//Asumimos que la memoria caché tendrá como máximo 2 líneas

using namespace std;

int main() {
	double A[MAX][MAX], x[MAX], y[MAX];
	//Inicialización de variables
	
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			A[i][j] = i+j;
		}
		x[i] = i;
		y[i] = 0;
	}
	
	auto start = chrono::high_resolution_clock::now();
	
	//Primer Bucle
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			y[i] += A[i][j]*x[j];
		}
	}
	
	auto end = chrono::high_resolution_clock::now();
	
	chrono::duration<double> dur = end-start;
	
	cout << dur.count() << endl;
	
	
	start = chrono::high_resolution_clock::now();
	
	//Segundo Bucle
	for (int j = 0; j < MAX; j++){
		for (int i = 0; i < MAX; i++){
			y[i] += A[i][j]*x[j];
		}
	}
	
	end = chrono::high_resolution_clock::now();
	
	dur = end-start;
	cout << dur.count() << endl;
	
}
