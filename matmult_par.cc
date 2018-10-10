// matmult.cc: implements integer matrix multiplication C = A x B
// for simplication, assumes A and B are both n x n matrices
//
// gw

#include <iostream>
#include <fstream>
#include <cstdlib>
#include<chrono>
#include<omp.h>
using namespace std;

void read_matrix(int*, int, char*);
void matmult (int*, int*, int*, int);
void print_matrix (int*, int);

int main (int argc, char *argv[])
{
	int dim;	// matrix is dim x dim

	// setup/initialize
	if (argc != 5) {
		cerr<< "usage: matmult dim(NxN) inputA inputB" << endl;
		exit (-1);
	}
//	cout << chrono::high_resolution_clock::period::den << " ticks/sec" << endl;
    auto programstart_time = chrono::high_resolution_clock::now();

	dim = atoi (argv[1]);
	int *A = new int[dim * dim];
	int *B = new int[dim * dim];
	int *C = new int[dim * dim];

	// input A and B matrices

//    cout << chrono::high_resolution_clock::period::den << " ticks/sec" << endl;
    auto start_time = chrono::high_resolution_clock::now();



	read_matrix(A, dim, argv[2]);
	read_matrix(B, dim, argv[3]);


	auto end_time = chrono::high_resolution_clock::now();
	cout << "Execution Serial " << chrono::duration_cast<chrono::microseconds>(end_time - start_time).count() << " us" << endl;


//	// compute product using straightforward algorithm
//	cout << chrono::high_resolution_clock::period::den << " ticks/sec" << endl;
    auto start_timeop = chrono::high_resolution_clock::now();

    #pragma omp parallel
    {

    omp_set_num_threads(atoi(argv[4]));
	matmult (C, A, B, dim);


    }

	auto end_timeop = chrono::high_resolution_clock::now();

	cout <<"Compute matrix " <<chrono::duration_cast<chrono::microseconds>(end_timeop - start_timeop).count() << " us" << endl;


    auto programend_time = chrono::high_resolution_clock::now();
	cout << "Total time " << chrono::duration_cast<chrono::microseconds>(programend_time - programstart_time).count() << " us" << endl;

    cout<<"\n";
    cout<<"\n";
	// check results
	print_matrix (C, dim);

	delete [] A;
	delete [] B;
	delete [] C;

	return 0;
}


void read_matrix(int *data, int size, char *fileName)
{
	int i,j;
	ifstream inputFile;

	inputFile.open (fileName);
	// read random matrix of size NxN
	for (i=0; i < size; i++)
		for (j=0; j < size; j++)
			inputFile >> data[i*size+j];
}


void matmult (int *dataC, int *dataA, int *dataB, int size)
{

	int i, j, k, sum;

    #pragma omp for collapse(2)
	for (i=0; i < size; i++ ) {
		for (j=0; j < size; j++ ) {
			sum = 0;
			for (k=0; k < size; k++ ) {
				sum += dataA[i *size+k] * dataB[k*size+j];
			}
			dataC[i*size+j] = sum;
		}
	}
}


void print_matrix (int *data, int size)
{
	int i,j;
	ofstream outputFile;

	outputFile.open ("C.dat");
	for (i=0; i < size; i++)
		for (j=0; j < size; j++)
			outputFile << data[i*size+j] << "\t";
	outputFile.close();
}

