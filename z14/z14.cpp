#include "pch.h"
#include <iostream>
#include<string>
#include<vector>
using namespace std;
int main()
{
	
	vector<int> a(5,1);
	
	for (auto it=a.begin();it !=a.end();++it)
	{
		*it *= 2;
	}
	for(auto &y:a)cout << y << endl;
}