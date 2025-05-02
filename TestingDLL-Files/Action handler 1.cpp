#include"Header.h" 
int c = 0;
#define A1 _declspec(dllexport)
extern "C" A1 int dll_register_callbacks();
extern A1 int dll_execute_callbacks1(int *decision, va_list argc);
extern A1 int dll_execute_callbacks2(int *decision, va_list argc); 
extern A1 int custom_handler_1(EPM_action_message_t msg);

extern A1 int dll_register_callbacks()
{
	c = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks1);
	c = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks2);
	EPM_register_action_handler("Custom-handler-1", "", (EPM_action_handler_t)custom_handler_1);
	return(c);
}
extern A1 int dll_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " ***********************************************************************" << endl;
	cout << " Welcome to DLL project " << endl;
	cout << " Logging in process successfull " << endl;
	cout << " Custom handler test 1" << endl;
	cout << " Objective - Sample to check release data";
	cout << " ***********************************************************************" << endl;
	return(c);
} 
extern A1 int custom_handler_1(EPM_action_message_t msg)
{
	tag_t tRoot_task = NULLTAG;
	int No_of_attachment, i;
	tag_t* tAttachment = NULLTAG;
	tag_t tClass_id = NULLTAG;
	tag_t tRel_sts = NULLTAG;
	char* cClass_name = NULL;
	logical retain_released_date = true;
	EPM_ask_root_task(msg.task, &tRoot_task);
	EPM_ask_attachments(tRoot_task, EPM_target_attachment, &No_of_attachment, &tAttachment);
	for (i = 0; i < No_of_attachment; i++)
	{
		if (tAttachment[i] != NULLTAG)
		{
			POM_class_of_instance(tAttachment[i], &tClass_id);
			POM_name_of_class(tClass_id, &cClass_name);
			if (tc_strcmp(cClass_name, "ItemRevision") == 0)
			{
				RELSTAT_create_release_status("TCM Released", &tRel_sts);
				if (tRel_sts != NULLTAG)
				{
					RELSTAT_add_release_status(tRel_sts, 1, &tAttachment[i], retain_released_date);
				}
			}
		}
	}
	return c;
	/*int count;
	tag_t root_task,*attachment_t;
	EPM_ask_root_task(msg.task,&root_task); 
	EPM_ask_attachments(root_task, EPM_target_attachment, &count,&attachment_t);
	if (count)
	{
		for (int i = 0; i < count < i++)
		{
			vector<vector<string>> argu;
			vector<string> ax;
			char *argument_name, *argument_value;
			int nofargument = TC_number_of_arguments(msg.arguments);
			ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argument_name, &argument_value); 
			string temp = argument_value,x,y;
			for (int j = 0; j < temp.length(); j++)
			{
				if (temp[j] == ',')
				{
					ax.push_back(x);
					x = y;
				}
				else
				{
					x = x + temp[j];
				}
			} 
			argu.push_back(ax);
		}*/
	/*}
	else
	{
		TC_write_syslog("\n\nWorkflow Process is not initiated on revision object");
	}*/
}
extern A1 int dll_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " Logout successfull " << endl;
	cout << " Done with the process " << endl;
	cout << " ***********************************************************************" << endl;
	cout << " ***********************************************************************" << endl;
	return(c);
}