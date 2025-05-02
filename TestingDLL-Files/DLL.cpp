#include<stdio.h>
#include<conio.h>
#include<tc/tc_startup.h>
#include<tccore\custom.h>
#include<tccore\method.h>

#define A1 _declspec(dllexport)
int status = 0;
extern A1 int DLL_register_callbacks();
extern A1 int DLL_execute_callbacks1(int *decision,va_list argc);
extern A1 int DLL_execute_callbacks2(int *decision,va_list argc);

extern A1 int DLL_register_callbacks()
{
	status = CUSTOM_register_exit("sample_1", "USER_init_module",(CUSTOM_EXIT_ftn_t)DLL_execute_callbacks1);
	status = CUSTOM_register_exit("sample_1", "USER_exit_module", (CUSTOM_EXIT_ftn_t)DLL_execute_callbacks2);
	return(status);
}
extern A1 int DLL_execute_callbacks1(int *decision, va_list argc) {
	*decision = ALL_CUSTOMIZATIONS;
	printf("Login successfull \n");
	printf("Welcome to DLL _ Project \n");
	return(status);
}
extern A1 int DLL_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf(" Logout successful ");
	return(status);
}
