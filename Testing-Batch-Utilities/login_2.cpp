#include<stdio.h>
#include<iostream>
#include<tc/tc_startup.h>
#include<tc/tc_errors.h>
#include<tcinit/tcinit.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	int a=100;
	string uname, upass, urole;
	cout << " Please enter name = ";
	scanf("%s", &uname);
	cout << " Please enter password = ";
	scanf("%s", &upass);
	cout << " Please enter role = ";
	scanf("%s", &urole);
	const char *name = uname.c_str();
	const char *pass = upass.c_str();
	const char *role = urole.c_str();
	a = ITK_init_module(name,pass,role);
	if (a == ITK_ok)
	{
		cout << "Login_successfull";
	}
	else
	{
		cout << "Login attempt failed ";
	}
	return(0);
}