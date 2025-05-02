#include<stdio.h>
#include<conio.h>
#include<tccore\custom.h>
#include<tccore\method.h>

#define DLLAPI _declspec(dllexport)
int status = 0;
extern DLLAPI int Dll_project_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);

extern DLLAPI int Dll_project_register_callbacks()
{
	status = CUSTOM_register_exit("ITK_DLL_Precondition", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	status = CUSTOM_register_exit("ITK_DLL_Precondition", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return status;
}

extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv) {

	*decision = ALL_CUSTOMIZATIONS;
	printf(" Login successfull \n");
	printf(" Welcome to teamcenter \n");
	printf(" Welcome to DLL _project1\n");
	return status;
}
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("lOGOUT Successfull\n");
	return status;
}
