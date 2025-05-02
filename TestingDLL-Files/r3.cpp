#include"Header.h" 
int c = 0;
#define A1 _declspec(dllexport)
logical retain_released_date = TRUE;
extern "C" A1 int dll_register_callbacks();
extern A1 int dll_execute_callbacks1(int *decision, va_list argc);
extern A1 int dll_execute_callbacks2(int *decision, va_list argc); 
EPM_decision_t custom_rule_handler_3(EPM_rule_message_t msg);

extern A1 int dll_register_callbacks()
{
	c = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks1);
	c = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks2);
	return(c);
}
extern A1 int dll_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	c = EPM_register_rule_handler("custom-handler-3", "", (EPM_rule_handler_t)custom_rule_handler_3);
	cout << " ***********************************************************************" << endl;
	cout << " Welcome to DLL project " << endl;
	cout << " Logging in process successfull " << endl;
	cout << " Custom handler test 1" << endl;
	cout << " Objective - create Rule handler such that handler will check weather there is any release status on item revision or not";
	cout << " ***********************************************************************" << endl;
	bfile << __DATE__ << endl;
	bfile << __TIME__ << endl;
	bfile << " Welcome to DLL project Logging in to process successfull . " << endl;
	bfile << " Objective - create Rule handler such that handler will check weather there is any release status on item revision or not "<<endl;
	return(c);
}
EPM_decision_t custom_rule_handler_3(EPM_rule_message_t msg)
{
	EPM_decision_t decision;
	int attachmentcount;
	tag_t roottask_t,classid_t;
	tag_t *attachment_t;
	char *classname;
	EPM_ask_root_task(msg.task, &roottask_t); 
	EPM_ask_attachments(roottask_t, EPM_target_attachment,&attachmentcount,&attachment_t); 
	for (int i = 0; i < attachmentcount; i++)
	{
		POM_class_of_instance(attachment_t[i],&classid_t);
		POM_name_of_class(classid_t, &classname);
		string x = classname;
		if (x.find("Revision") != string::npos)
		{
			char *propertyvalue;
			AOM_UIF_ask_value(attachment_t[i], "release_status_list", &propertyvalue);
			if (string(propertyvalue).length() > 0)
			{
				decision = EPM_nogo;
			}
			else
			{
				decision = EPM_go;
			}
		}
	}
	return decision;
}
extern A1 int dll_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " Logout successfull " << endl;
	cout << " Done with the process " << endl;
	cout << " ***********************************************************************" << endl;
	cout << " ***********************************************************************" << endl;
	bfile << " Done with the logout process " << endl;
	return(c);
}
