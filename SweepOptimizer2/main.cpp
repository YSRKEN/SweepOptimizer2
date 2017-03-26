#include <cstdlib>
#include <iostream>
#include "problem.cpp"

int main(int argc, char *argv[]) {
	using std::cout;
	using std::endl;
	if (argc < 2)
		return EXIT_FAILURE;
	try {
		Problem problem(argv[1]);
		problem.put();
	}
	catch (char *str) {
		cout << str << endl;
	}
}
