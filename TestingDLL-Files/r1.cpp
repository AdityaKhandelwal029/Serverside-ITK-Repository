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
	cout << " Objective - create Action handler such that handler will apply multiple status on previous item revision of same item provided as an argument";
	cout << " **********************Updated*************************************************" << endl;
	return(c);
}
extern A1 int custom_handler_1(EPM_action_message_t msg)
{
	int count,attacheditemrcount;
	tag_t root_task, *attachment_t, class_id,attacheditem_t,*attacheditemr_list;
	vector<vector<string>> argu;
	vector<string> ax; 
	vector<tag_t> argu_t;
	char *argument_name, *argument_value,*class_name; 
	logical retain_released_date = TRUE;

	EPM_ask_root_task(msg.task,&root_task);
	EPM_ask_attachments(root_task, EPM_target_attachment, &count,&attachment_t);
	if (count)
	{
		for (int i = 0; i < count; i++)
		{
			int nofargument = TC_number_of_arguments(msg.arguments);
			for (int x = 0; x < nofargument; x++)
			{
				ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argument_name, &argument_value);
				if (tc_strcmp(argument_name, "release_status_name") == 0)
				{
					string temp = argument_value, x, y;
					for (int j = 0; j < temp.length(); j++)
					{
						if (temp[j] == ',' || temp.length() == j + 1)
						{
							ax.push_back(x);
							x = y;
						}
						else
						{
							x = x + temp[j];
						}
					}
				}
				argu.push_back(ax);
			}
			argu_t.push_back(attachment_t[i]);
		} 
		for (int k = 0; k < argu_t.size(); k++)
		{
				POM_class_of_instance(argu_t[k],&class_id);
				POM_name_of_class(class_id, &class_name); 
				string z = class_name;
				if (z.find("Revision") != string::npos)
				{
					ITEM_ask_item_of_rev(argu_t[k], &attacheditem_t);
					ITEM_list_all_revs(attacheditem_t,&attacheditemrcount,&attacheditemr_list);
					for (int j = 0; j < attacheditemrcount; j++)
					{
						if (argu_t[k] == attacheditemr_list[j])
						{
							for (int l = 0; l < argu[0].size(); l++)
							{
								tag_t releasestatcreate_t;
								const char *t1 = argu[0][l].c_str();
								RELSTAT_create_release_status(t1, &releasestatcreate_t); 
								if (releasestatcreate_t != NULLTAG)
								{
									RELSTAT_add_release_status(releasestatcreate_t, 1, &attacheditemr_list[j- 1], retain_released_date);
								}
								else
								{
									TC_write_syslog("\n\nWorkflow Process is not initiated on revision object");
								}
							}
							for (int l = 0; l < argu[1].size(); l++)
							{
								tag_t releasestatcreate_t;
								const char *t1 = argu[1][l].c_str();
								RELSTAT_create_release_status(t1, &releasestatcreate_t);
								if (releasestatcreate_t != NULLTAG)
								{
									RELSTAT_add_release_status(releasestatcreate_t, 1, &attacheditemr_list[j - 1], retain_released_date);
								}
								else
								{
									TC_write_syslog("\n\nWorkflow Process is not initiated on revision object");
								}
							}
						}
					}
				}
		}

	}
		else
		{
			TC_write_syslog("\n\nWorkflow Process is not initiated on revision object or there can be any error ");
		}
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