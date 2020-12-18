#ifndef SUPPORTLIB_H
#define SUPPORTLIB_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include <list>
#include <fstream>
#include <sstream>

struct erlangRegister
{
	int numberOfCalls;
	int periodOfMinutes;
	float averageHandlingTimeMinutes;
	float requiredServiceLevel;
	int targetAnswerTimeSeconds;
	float maximumOccupancy;
	float shrinkage;
};

class SupportLib
{
	public:
		static list<erlangRegister> leArquivo();
		static void printRegister(erlangRegister e);
		static int fatorial(int n);
		static int A(erlangRegister e);
		static int N(erlangRegister e);
		static double erlang(erlangRegister e);
		static double serviceLevel(erlangRegister e);
		static int somaVetorInt(int* vetor, int tam);

	private:
};
#endif //SUPPORTLIB_H