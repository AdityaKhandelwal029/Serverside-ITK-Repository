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
#define x = "ITEM_create_from_rev";
extern DLLAPI int Custom_execute_callbacks1(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	METHOD_id_t Method_id;
	printf("\n\n***************************************");
	printf("\n  DLL Register And Login Successfull ");
	printf("\n***************************************\n\n");
	Status = METHOD_find_method("ItemRevision", "ITEM_create_from_rev", &Method_id);
	Status = METHOD_add_action(Method_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_Post_Action, NULL);
	return Status;
}
extern DLLAPI int PLM_add_Post_Action(METHOD_message_t *msg, va_list args)
{  
	t1 << " Welcome to file " << endl;
	char* objname1, *objname2,*objname3, *objname4;
	va_list largs;
	va_copy(largs, args);
	//t1 << args<<endl;
	/*tag_t s1,s2;
	s1 = va_arg(largs, tag_t);
	s2 = va_arg(largs, tag_t);
	va_end(largs);
	AOM_UIF_ask_value(s1, "object_name", &objname1);
	AOM_UIF_ask_value(s1, "object_type", &objname2);
	t1 << objname1 << endl;
	t1 << objname2 << endl;  
	AOM_UIF_ask_value(s2, "object_name", &objname3);
	AOM_UIF_ask_value(s2, "object_type", &objname4);
	t1 << objname3 << endl;
	t1 << objname4 << endl;*/
	tag_t s1, s2;
	char* s3;
	s1 = va_arg(largs, tag_t); 
	s2 = va_arg(largs, tag_t);
	s3 = va_arg(largs, char*);
	va_end(largs);

	AOM_UIF_ask_value(s1, "object_name", &objname1);
	AOM_UIF_ask_value(s1, "object_type", &objname2);
	t1 << objname1 << endl;
	t1 << objname2 << endl;
	AOM_UIF_ask_value(s2, "object_name", &objname3);
	AOM_UIF_ask_value(s2, "object_type", &objname4);
	t1 << objname3 << endl;
	t1 << objname4 << endl;
	s3= va_arg(largs, char*);
	t1 << s3 << endl;
		

	
	
	/*tag_t Rel_type = NULLTAG;
	int No_of_Form, i;
	tag_t *Form = NULLTAG;
	Status = GRM_find_relation_type("IMAN_master_form_rev", &Rel_type);
	Status = GRM_list_secondary_objects_only(Source_rev, Rel_type, &No_of_Form, &Form);
	for (i = 0; i < No_of_Form; i++)
	{
		Status = AOM_UIF_set_value(Form[i], "user_data_1", "Successfull");
		AOM_save(Form[i]);
	}*/
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