#include "Header.h" 
extern "C" DLLAPI int Post_action_register_callbacks();
extern DLLAPI int Custom_execute_callbacks1(int *decision, va_list argc);
extern DLLAPI int PLM_add_Post_Action(METHOD_message_t *msg, va_list args);
extern DLLAPI int Custom_execute_callbacks2(int *decision, va_list argc);
extern "C" DLLAPI int dll_register_callbacks()
{
	Status = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks1);
	Status = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks2);
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
	Status = METHOD_add_action(Method_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_Post_Action, NULL);
	return Status;
}
extern DLLAPI int PLM_add_Post_Action(METHOD_message_t *msg, va_list args)
{
	tag_t  source_rev = va_arg(args, tag_t);
	char   *rev_id = va_arg(args, char *);
	tag_t  *new_rev = va_arg(args, tag_t *);
	tag_t  item_rev_master_form = va_arg(args, tag_t);

	char* sourecerevname = NULL;
	char* sourecerevtype = NULL;
	char* newrevname = NULL;
	char* newrevtype = NULL;
	char* irmname = NULL;
	char* irmtype = NULL;

	t1<<"\n GTAC_item_copy_rev\n";

	AOM_UIF_ask_value(source_rev, "object_name", &sourecerevname);
	t1 << "source rev name is "<<sourecerevname<<endl;
	AOM_UIF_ask_value(source_rev, "object_type", &sourecerevtype);
	t1 << "source rev type is " << sourecerevtype << endl;
	 
	t1 << "rev_id  is " << rev_id << endl;

	AOM_UIF_ask_value(*new_rev, "object_name", &newrevname);
	t1 << "new rev name is " << newrevname << endl;
	AOM_UIF_ask_value(*new_rev, "object_type", &newrevtype);
	t1 << " new rev type is " << newrevtype << endl;

	AOM_UIF_ask_value(item_rev_master_form, "object_name", &irmname);
	t1 << "IRM name is " << irmname << endl;
	AOM_UIF_ask_value(item_rev_master_form, "object_type", &irmtype);
	t1 << "IRM type is " << irmtype << endl;



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