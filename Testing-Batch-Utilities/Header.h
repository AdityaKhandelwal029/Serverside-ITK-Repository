#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tc/tc_errors.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tc/folder.h>
#include<sa/user.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<tccore/grm.h>
#include<tccore/aom_prop.h>
#include<tc/emh.h>
#include<tccore/workspaceobject.h>
#include<qry/qry.h>
#include<tccore/project.h>
#include<bom/bom.h>
#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<fstream> 
#include<CFM/cfm.h> 
#include<direct.h> 
#include<tccore/tctype.h> 
#include<pom/pom/pom.h> 
#include<tc/preferences.h>
//#define SS_TEXT 010000
using namespace std; 
vector<string> m1, m2;
ofstream i1("C:/Mini_project/sample_report1.csv");
int login()
{
	int a;
	a=ITK_init_module("installuser", "installuser", "dba");
	if (a == 0)
	{
		printf( " Login_successfull\n");
		return(1);
	}
	else
	{
		printf("Login_failed");
		return(2);
	}
} 
void fx1()
{
	ifstream ip("C:/Mini_project/f.txt"); 
	string x;
	while (getline(ip, x))
	{
		int find;
		string y,h1,h2,h3;
		y = x;
		for (int l = 0; l < y.length(); l++)
		{
			if (y[l] == ',')
			{
				h1 = h3;
				h3 = h2;
			}
			else
			{
				h3 = h3 + y[l];
			}
		}
		h2 = h3;
		m1.push_back(h1);
		m2.push_back(h2);
		i1 << h1 << " ";
	}
	i1 << endl;
}