#include "sequencer.h"
#include <iostream>

using namespace std;

bool IfOdd(long n)
{
	return n % 2;
}

long ToSquare(long n)
{
	return n * n;
}

int main(void)
{
	long numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	Sequencer::Fetch(numbers)
		.Choose(IfOdd)
		.Transform(ToSquare)
		.Perform([](auto e) {cout << e << endl;});
	
}

