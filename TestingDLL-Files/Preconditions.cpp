#include"Header.h" 
int c = 0; 
#define A1 _declspec(dllexport)
#define DLL_error (EMH_USER_error_base +2)
extern "C" A1 int dll_register_callbacks();
extern A1 int login_execute_callbacks1(int *decision, va_list argv);
extern A1 int logout_execute_callbacks2(int *decision, va_list argv); 
extern A1 int PLM_add_precondition(METHOD_message_t *msg, va_list args); 

extern "C" A1 int dll_register_callbacks()
{
	c = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)login_execute_callbacks1);
	c = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)logout_execute_callbacks2); 
	return(c);
}
extern A1 int login_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS; 
	METHOD_id_t  Method_id;
	printf(" Login successfull to Dll \n");
	printf(" Welcome to DLL project \n"); 
	c = METHOD_find_method("ItemRevision", "ITEM_copy_rev", &Method_id); 
	c = METHOD_add_pre_condition(Method_id, (METHOD_function_t)PLM_add_precondition, NULL);
	return(c);
} 
extern A1 int PLM_add_precondition(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev;
	source_rev = va_arg(args, tag_t); 
	int num;
	char **rel_val = NULL;
	char *obj_name = NULL; 
	AOM_ask_displayable_values(source_rev, "release_status_list",&num,&rel_val);
	AOM_ask_value_string(source_rev, "object_name", &obj_name);
	printf(" Release status = %s", rel_val);
	printf("\nObject name = %s", obj_name); 
	if ((rel_val) > 0)
	{
		printf(" \nRevision revise successfully ");
	}
	else
	{
		EMH_store_error_s1(EMH_severity_user_error, DLL_error, obj_name);
		return DLL_error;
	}
	return(c);
}
extern A1 int logout_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf(" Logout successfull  \n");
	printf(" Done with the process \n");
	return(c);
}

