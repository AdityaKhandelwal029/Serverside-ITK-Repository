#include<stdio.h>
#include<conio.h>
#include<tc\tc_startup.h>
#include<tccore\method.h>
#include<tccore\custom.h>
#include<tccore\aom_prop.h>
#include<epm\epm.h>
#include<pom\pom\pom.h>
#include<tccore\releasestatus.h>
#include<tccore/item.h>
#include<iostream>
#include <string>
#include<string.h>
#include <vector>
#include<sstream>
#include <algorithm>
#include<tc\emh.h>
using namespace std;
#define DLLAPI _declspec(dllexport)
#define DLL_ERROR (EMH_USER_error_base +3)

#include<fstream>

ofstream file("D:/rulehandler.txt");

extern "C" DLLAPI int dll_register_callbacks();
extern DLLAPI int dll_execute_callbacks1(int *decision, va_list args);
extern DLLAPI int dll_execute_callbacks2(int *decision, va_list args);
EPM_decision_t Check_workflow(EPM_rule_message_t msg); 
 
int a(string x, string y)
{
	int count = 0;
	for (long long int i = 0; i < x.length(); i++)
	{
		int c = i, temp = 0;
		for (long long int j = 0; j < y.length(); j++)
		{
			if (x[c] == y[j])
			{

			}
			else
			{
				temp = 1;
				break;
			}
			c++;
		}
		if (temp == 0)
		{
			count = count + 1;
		}
	}
	return(count);
}