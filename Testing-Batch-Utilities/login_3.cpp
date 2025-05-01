#include<stdio.h>
#include<iostream>
#include<tc/tc_startup.h>
#include<tc/tc_errors.h>
#include<tcinit/tcinit.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	int a;
	char *name = ITK_ask_cli_argument("-u=");
	char *pass = ITK_ask_cli_argument("-p=");
	char *role = ITK_ask_cli_argument("-g=");
	a = ITK_init_module(name,pass,role);
	if (a == ITK_ok)
	{
		cout << " a = " << a<<endl;
		cout << "Login_successfull";
	}
	else
	{
		cout << " a = " << a<<endl;
		cout << "Login attempt failed ";
	}
	ITK_exit_module(true);
	return(0);
}