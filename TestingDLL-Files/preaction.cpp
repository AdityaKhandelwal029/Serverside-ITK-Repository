#include"Header.h" 
int c = 0;
#define DLL_error (EMH_USER_error_base +2) 
#define DLL_error (EMH_USER_error_base +3)
extern A1 int dll_register_callbacks();
extern A1 int login_execute_callbacks1(int *decision, va_list argv);
extern A1 int logout_execute_callbacks2(int *decision, va_list argv);
extern A1 int PLM_add_preaction(METHOD_message_t *msg, va_list args);

extern A1 int dll_register_callbacks()
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
	c = METHOD_find_method("ItemRevision", "ITEM_delete", &Method_id);
	c = METHOD_add_pre_condition(Method_id, (METHOD_function_t)PLM_add_preaction, NULL);
	return(c);
}
extern A1 int PLM_add_preaction(METHOD_message_t *msg, va_list args)
{
	int count,i,j;
	tag_t source_rev,item_t,relation_t; 
	tag_t *rev_list;
	source_rev = va_arg(args, tag_t);
	ITEM_ask_item_of_rev(source_rev, &item_t);  
	ITEM_list_all_revs(item_t, &count, &rev_list);
	for ( i = 0; i < count; i++)
	{
		if (source_rev == rev_list[i])
		{
			int count2;
			tag_t *seco_obj;
			GRM_find_relation_type(NULL, &relation_t); 
			GRM_list_secondary_objects_only(rev_list[i],relation_t,&count2,&seco_obj); 
			for (j = 0; j < count2; j++)
			{
				WSOM_ask_object_type2()
			}
		}
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

