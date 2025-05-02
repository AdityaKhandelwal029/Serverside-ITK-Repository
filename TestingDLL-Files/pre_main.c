#include<stdio.h>
#include<conio.h>
#include<tccore\custom.h>
#include<tccore\method.h>

#define DLLAPI _declspec(dllexport)
int status = 0;
#define DLL_error (EMH_USER_error_base +2)
extern DLLAPI int Dll_project_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int PLM_add_Precondition(METHOD_message_t *msg, va_list args);

extern DLLAPI int Dll_project_register_callbacks()
{
	status = CUSTOM_register_exit("DLL_project", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	status = CUSTOM_register_exit("DLL_project", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);

	return status;
}

extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argc) {

	*decision = ALL_CUSTOMIZATIONS;
	METHOD_id_t Method_id;
	printf(" Login successfull \n");
	printf(" Welcome to teamcenter \n");
	printf(" Welcome to DLL _project\n");
	status = METHOD_find_method("ItemRevision", "ITEM_copy_rev", &Method_id);
	status = METHOD_add_pre_condition(Method_id, (METHOD_function_t)PLM_add_Precondition, NULL);
	return status;
}

extern DLLAPI int PLM_add_Precondition(METHOD_message_t *msg, va_list args)
{
	tag_t Source_rev;
	Source_rev = va_arg(args, tag_t);
	char* Rel_value = NULL;
	char* Obj_name = NULL;

	AOM_UIF_ask_value(Source_rev, "release_status_list", &Rel_value);
	printf("\nRelease status= %s", Rel_value);
	AOM_UIF_ask_value(Source_rev, "object_name", &Obj_name);
	printf("\nObject name= %s", Obj_name);
	if (tc_strcmp(Rel_value, "TCM Released") == 0)
	{
		printf("\nRevision Revise Successfull");
	}
	else
	{
		printf("\nRevision is Not Released. Revision Revise Unsuccessfull");
		EMH_store_error_s1(EMH_severity_user_error, DLL_error, Obj_name);
		return DLL_error;
	}
	return status;
}
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("lOGOUT Successfull\n");
	return status;
}