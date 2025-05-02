/* Write a pre- condition which will evaluate role of user before creating Item, revising Item and Saving Item.*/
#include"Header.h" 
int c = ITK_ok;
#define A1 _declspec(dllexport)
#define DLL_error (EMH_USER_error_base +6)
#define DLLERROR (EMH_USER_error_base +5)
extern "C" A1 int dll_register_callbacks();
extern A1 int login_execute_callbacks1(int *decision, va_list argv);
extern A1 int logout_execute_callbacks2(int *decision, va_list argv);
extern A1 int PLM_add_precondition1(METHOD_message_t *msg, va_list args);
extern A1 int PLM_add_precondition2(METHOD_message_t *msg, va_list args);

extern "C" A1 int dll_register_callbacks()
{
	c = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)login_execute_callbacks1);
	c = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)logout_execute_callbacks2);
	return(c);
}
extern A1 int login_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	METHOD_id_t  Method_id, Method_id2;
	printf(" Login successfull to Dll \n");
	printf(" Welcome to DLL project \n");
	c = METHOD_find_method("A5Ford_F3", ITEM_create_msg, &Method_id);
	c = METHOD_find_method("A5Ford_F3Revision", ITEM_copy_rev_msg, &Method_id2);
	c = METHOD_add_pre_condition(Method_id, (METHOD_function_t)PLM_add_precondition1, NULL);
	c = METHOD_add_pre_condition(Method_id2, (METHOD_function_t)PLM_add_precondition2, NULL);

	return(c);
}

extern A1 int PLM_add_precondition1(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev;
	source_rev = va_arg(args, tag_t);
	tag_t Usertag = NULLTAG;
	tag_t Roletag = NULLTAG;
	tag_t Defaultrole = NULLTAG;
	tag_t grouptag = NULLTAG;
	char* Username = NULL;
	char* Rolename = NULL;
	char* DefRolename = NULL;
	const char* s1 = "Login role & default role does not match";
	c=POM_get_user(&Username, &Usertag);
	c=SA_ask_current_role( &Roletag);
	c=SA_ask_role_name2(Roletag, &Rolename);
	c=POM_ask_default_group(&grouptag);
	c=SA_ask_default_role(Usertag, grouptag, &Defaultrole);
	c=SA_ask_role_name2(Defaultrole, &DefRolename);
	if (tc_strcmp(DefRolename, Rolename) == 0)
	{
		printf("Item Created successfully");
	}
	else
	{
		EMH_store_error_s1(EMH_severity_error, DLLERROR, s1);
		return DLLERROR;
	}
	return(c);
}

extern A1 int PLM_add_precondition2(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev;
	source_rev = va_arg(args, tag_t);
	tag_t Usertag = NULLTAG;
	tag_t Roletag = NULLTAG;
	tag_t Defaultrole = NULLTAG;
	tag_t grouptag = NULLTAG;
	char* Username = NULL;
	char* Rolename = NULL;
	char* DefRolename = NULL;
	char* revname = NULL;

	c=AOM_ask_value_string(source_rev, "object_name", &revname);
	c=POM_get_user(&Username, &Usertag);
	c=SA_ask_current_role(&Roletag);
	c=SA_ask_role_name2(Roletag, &Rolename);
	c=POM_ask_default_group(&grouptag);
	c=SA_ask_default_role(Usertag, grouptag, &Defaultrole);
	c=SA_ask_role_name2(Defaultrole, &DefRolename);
	if (tc_strcmp(DefRolename, Rolename) == 0)
	{
		printf("Item revision revised successfully");
	}
	else
	{
		EMH_store_error_s1(EMH_severity_error, DLL_error, revname);
		return DLLERROR;
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

