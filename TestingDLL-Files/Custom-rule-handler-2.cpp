/* Custom Rulehandler -When user will initiate a process on Item Revision,
  system will check the previous revision is in the process or not, 
  if yes system will not allow initiate a process on Item Revision.   */

#include"Header2(rulehndler).h"   
#define PLM_error (EMH_USER_error_base +7)
extern "C" DLLAPI int dll_register_callbacks()
{
	int Status = ITK_ok;
	Status = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks1);
	Status = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks2);
	return Status;
}
extern DLLAPI int dll_execute_callbacks1(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	int Status = ITK_ok;
	cout << "\n***************************************" << endl;
	cout << "\n   DLL Register And Login Successfull for ford-custom-rule-handler-2" << endl;
	cout << "\n***************************************\n\n" << endl;
	EPM_register_rule_handler("Ford-Check-custom-workflow-2", " Test-rule-handler", (EPM_rule_handler_t)Check_workflow);
	file << "DLL Register And Login Successfull for ford-custom-rule-handler-2" << endl;
	return Status;
}
EPM_decision_t Check_workflow(EPM_rule_message_t msg)
{
	int Status = ITK_ok;
	EPM_decision_t dec = EPM_go;
	int count, revcount;
	tag_t Roottask = NULLTAG;
	tag_t* attachmentlist = NULLTAG;
	tag_t tItem = NULLTAG;
	tag_t* rev_list = NULLTAG;
	char* valuetag = NULL;
	char* value1 = NULL;
	char* value2 = NULL;
	char* object_name = NULL;
	Status = EPM_ask_root_task(msg.task, &Roottask);
	Status = EPM_ask_attachments(Roottask, EPM_target_attachment, &count, &attachmentlist);
	file << " Inside main function "<<endl; 
	file << " Total Count of revision as attachments  = " << count << endl;
	for (int i = 0; i < count; i++)
	{
		Status = AOM_ask_value_string(attachmentlist[i], "object_name", &object_name);
		Status = AOM_ask_value_string(attachmentlist[i], "item_revision_id", &value1);
		Status = ITEM_ask_item_of_rev(attachmentlist[i], &tItem);
		Status = ITEM_list_all_revs(tItem, &revcount, &rev_list); 
		file << " Total count of revision of an item = " << revcount <<endl;

		if (revcount == 1)
		{
			file << " Inside if condition rev count == 1" << endl;
			dec = EPM_go;
		}
		else
		{
			file << " Inside else condition revcount > 1 " << endl;
			for (int j = 0; j < revcount; j++)
			{
				Status = AOM_ask_value_string(rev_list[j], "item_revision_id", &value2); 
				file << " Item-revision-id = " << value2;
				if (tc_strcmp(value1, value2) == 0 && j > 0)
				{
					Status = AOM_UIF_ask_value(rev_list[j - 1], "process_stage_list", &valuetag); 
					file << " value of process stage list property " << valuetag << endl;
					if (valuetag == 0)
					{
						file << " Inside final decision if " << endl;
						dec = EPM_go;
					}
					else
					{
						file << " Inside final decision else " << endl;
						dec = EPM_nogo;
						EMH_store_error_s1(EMH_severity_error, PLM_error, object_name);
						
						
					}
				}
			}
		}
	}
	file << " End of file " << endl;
	return dec;
}
extern DLLAPI int dll_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	int Status = ITK_ok;
	cout << "\n***************************************" << endl;
	cout << "\n  DLL Logout Successfull for ford-custom-rule-handler-2" << endl;
	cout << "\n***************************************\n\n" << endl;
	file << "DLL Logout Successfull for ford-custom-rule-handler-2";
	return Status;
}