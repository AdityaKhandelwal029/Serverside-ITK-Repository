#include "Header.h"
extern DLLAPI int ITK_DLL_Precondition_register_callbacks()
{
	Status = CUSTOM_register_exit("ITK_DLL_Precondition", "USER_init_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks1);
	Status = CUSTOM_register_exit("ITK_DLL_Precondition", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks2);
	return Status;
}

extern DLLAPI int Custom_execute_callbacks1(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	METHOD_id_t Method_id;
	printf("\n\n***************************************");
	printf("\n  DLL Register And Login Successfull ");
	printf("\n***************************************\n\n");
	Status = METHOD_find_method("ItemRevision", "ITEM_copy_rev", &Method_id);
	Status = METHOD_add_pre_condition(Method_id, (METHOD_function_t)PLM_add_Precondition, NULL);
	return Status;
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
	return Status;
}

extern DLLAPI int Custom_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n***************************************");
	printf("\n  DLL Register And Logout Successfull ");
	printf("\n***************************************\n\n");
	return Status;
}