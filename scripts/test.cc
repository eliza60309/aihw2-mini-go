#include <bits/stdc++.h>
#include <sys/stat.h>

using namespace std;

bool exists(const string &s);

int main()
{
	string s;
	while(1)
	{
		cin >> s;
		cout << exists(s) << endl;
	}
}

bool exists(const string &s)
{
	struct stat buf;
	return !stat(s.c_str(), &buf);
}

