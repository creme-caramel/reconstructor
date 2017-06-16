// count.cpp
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
	ifstream in("TEST2");
	stringstream ss;
	if(in) {
		ss << in.rdbuf();
		in.close();
	}

	size_t num = 0;
	int temp;
	while(ss >> temp)
		num++;
	cout << num << endl;
}

