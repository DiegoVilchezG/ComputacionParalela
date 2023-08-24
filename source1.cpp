#include <iostream>
#include <chrono>
#define MAX 4
//Asumimos que la memoria caché tendrá como máximo 4 líneas

using namespace std;

void printmat(double a[][MAX]){
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
}
	
void printarr(double a[]){
	for (int i = 0; i < MAX; i++){
		cout << a[i] << " ";
	}
	cout << endl;
}
		
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
	
	printmat(A);
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	//Primer Bucle
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			y[i] += A[i][j]*x[j];
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	 
	auto dur = chrono::duration_cast<chrono::microseconds>(end-begin).count();
	
	cout << dur << endl;
	
	printarr(y);
	
	begin = std::chrono::steady_clock::now();
	
	//Segundo Bucle
	for (int j = 0; j < MAX; j++){
		for (int i = 0; i < MAX; i++){
			y[i] += A[i][j]*x[j];
		}
	}
	
	end = std::chrono::steady_clock::now();
	
	dur = chrono::duration_cast<chrono::microseconds>(end-begin).count();
	cout << dur << endl;
	
	printarr(y);
}
