#include<stdio.h>
#include<conio.h>
#include<tc/tc_startup.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#define A1 _declspec(dllexport) 
int status = 0;
extern A1 int DLL_register_callbacks();
extern A1 int PLM_execute_callbacks1(int *decision, va_list argv);
extern A1 int PLM_execute_callbacks2(int *decision, va_list argv);
extern A1 int PLM_add_preconditon(METHOD_message_t *msg, va_list args);

extern A1 int DLL_register_callbacks()
{
	status = CUSTOM_register_exit("sample1", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	status = CUSTOM_register_exit("sample1", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return(status);
}
extern A1 int PLM_execute_callback1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("DLL registered and login successfull \n");


}

extern A1 int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("Task done and successfully logged out \n");
	return(status);
}
