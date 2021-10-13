#include <bits/stdc++.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <sys/param.h>
using namespace std;

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		cout << "cd to " << argv[1] << endl;
		chdir(argv[1]);
	}
	mkdir("data/", 0700);
	mkdir("data/black", 0700);
	mkdir("data/white", 0700);
	for(int i = 1; i <= 12; i++)
	{
		mkdir((string("data/black/step") + to_string(i)).c_str(), 0700);
		mkdir((string("data/white/step") + to_string(i)).c_str(), 0700);
	}
}
