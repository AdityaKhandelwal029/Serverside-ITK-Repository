#include<stdio.h>
#include<iostream>
#include<tc/tc_startup.h>
#include<tc/tc_errors.h>
#include<tcinit/tcinit.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	int a=ITK_ok;
	a=ITK_init_module("infodba" ,"infodba", "dba");
	if (a == ITK_ok)
	{
		cout << "Login_successfull";
	}
	else
	{
		cout << "Login aattempt failed ";
	}
	return(0);
}