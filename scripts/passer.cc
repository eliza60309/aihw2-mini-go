#include <fstream>

using namespace std;

int main()
{
	fstream out;
	out.open("output.txt", fstream::out);
	out << "PASS";
	out.close();
}
