 

#include"Header.h"

extern DLLAPI int post_action_register_callbacks()
{
	Status = CUSTOM_register_exit("post_action", "USER_init_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks1);
	Status = CUSTOM_register_exit("post_action", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks2);
	return Status;
}

extern DLLAPI int Custom_execute_callbacks1(int *decision, va_list argc)
{

	METHOD_id_t Method_id;
	int ifail;

	ifail = METHOD_find_method("ItemRevision", "ITEM_copy_rev", &Method_id);
	ifail = METHOD_add_action(Method_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_PostAction, NULL);

	return ifail;
}
extern DLLAPI int PLM_add_PostAction(METHOD_message_t *msg, va_list args)
{
	int ifail;

	tag_t DATASETTYPE, DATASET, RelTag, RELATION;
	
	tag_t* new_rev;
	new_rev = va_arg(args, tag_t*);


	ifail = AE_find_datasettype2("PDF", &DATASETTYPE);
	if (ifail == ITK_ok)
		printf("\n\t dataset find successfully");
	else
	{
		EMH_ask_error_text(ifail, &msg);
		printf("error%s", msg);
	}

	ifail = AE_create_dataset_with_id(DATASETTYPE, "sample", "", "", "", &DATASET);

	  ifail = AOM_save(DATASET);

	ifail = GRM_find_relation_type("IMAN_specification", &RelTag);
	ifail = GRM_create_relation(new_rev, DATASET, RelTag, NULLTAG, &RELATION);
	ifail = GRM_save_relation(RELATION);

	if (ifail == ITK_ok)
		printf("\n\t Relation saved successfully");
	else
	{
		EMH_ask_error_text(ifail, &msg);
		printf("error%s", msg);
	}


	return 0;
}
extern DLLAPI int Custom_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n***************************************");
	printf("\n  DLL Register And Logout Successfull ");
	printf("\n***************************************\n\n");
	return Status;
}