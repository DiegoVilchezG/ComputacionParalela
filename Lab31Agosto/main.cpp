#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

const int MAX = 1000, block = MAX/2;
vector<vector<double>> a, b, res, res2;

void printmat(vector<vector<double>> l) {
	if (MAX > 4) return;
	for (int i = 0; i < l.size(); i++) {
		for (int j = 0; j < l[i].size(); j++) {
			cout << l[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {

	//Inicializacion de Matrices
	auto start = chrono::high_resolution_clock::now();

	for (int i = 0; i < MAX; i++) {
		vector<double> t1, t2, t3, t4;
		for (int j = 0; j < MAX; j++) {
			t1.push_back(2*i*j-1);
			t2.push_back(2*(i+j)-j);
			t3.push_back(0);
			t4.push_back(0);
		}
		a.push_back(t1);
		b.push_back(t2);
		res.push_back(t3);
		res2.push_back(t4);
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> dur = end - start;

	cout << "Matrices inicializadas despues de: " << dur.count() << " segundos." << endl;

	start = chrono::high_resolution_clock::now();
	//Multiplicacion Tradicional
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			for (int k = 0; k < MAX; k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	end = chrono::high_resolution_clock::now();

	dur = end - start;

	cout << "Multiplicacion Tradicional: " << dur.count() << " segundos." << endl;
	cout << endl;

	start = chrono::high_resolution_clock::now();
	//Multiplicacion por Bloques
	for (int bi = 0; bi < MAX; bi += block) {

		for (int bj = 0; bj < MAX; bj += block) {

			for (int bk = 0; bk < MAX; bk += block) {

				for (int i = 0; i < bi; i++) {

					for (int j = 0; j < bj; j++) {

						for (int k = 0; k < bk; k++) {

							res2[i][j] += a[i][k] * b[k][j];
						}
					}
				}
			}
		}
	}
	end = chrono::high_resolution_clock::now();

	dur = end - start;
	cout << "Multiplicacion Por Bloques: " << dur.count() << " segundos." << endl;

}
