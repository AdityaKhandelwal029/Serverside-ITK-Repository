#include"Header.h" 
int status = ITK_ok;
extern "C" DLLAPI int dll_register_callbacks()
{
	status = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)login);
	status = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)logout);
	return status;
}
extern DLLAPI int login(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	METHOD_id_t tmethod_id;
	cout << " *********************Login Successfull ****************************" << endl; 
	status = METHOD_find_method("Item", "ITEM_create", &tmethod_id);
	status = METHOD_add_action(tmethod_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_postaction1, NULL);
	return status;
}
extern DLLAPI int PLM_add_postaction1(METHOD_message_t *msg, va_list args)
{
	tag_t tNew_item;
	tNew_item = msg->object_tag;
	tag_t tRev;
	tag_t dtype, PDF, rel_type, relation;
	status = ITEM_ask_latest_rev(tNew_item, &tRev);
	status = AE_find_datasettype2("PDF", &dtype);
	status = AE_create_dataset_with_id(dtype, "sample", "sample", "", "", &PDF);
	status = AOM_save_with_extensions(PDF);
	status = GRM_find_relation_type("IMAN_specification", &rel_type);
	status = GRM_create_relation(tRev, PDF, rel_type, NULLTAG, &relation);
	status = GRM_save_relation(relation);
	AOM_save(tRev); 
	AOM_save(tNew_item);
	return status;
}
extern DLLAPI int logout(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " **********************Logout Successfull**********************************" << endl;
	return status;
}