/* Custom Rulehandler to stop over use of workflows on itemrevision . Basically if one workflow is started on an item 
revision then it must be completed be starting second */

#include"Header2(rulehndler).h"  
#include<string> 
#include<string.h>  
extern DLLAPI int dll_register_callbacks()
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
	cout << "\n   DLL Register And Login Successfull for Check-custom-workflow" << endl;
	cout << "\n***************************************\n\n" << endl;
	EPM_register_rule_handler("Check-custom-workflow", "", (EPM_rule_handler_t)Check_workflow);
	file << "DLL Register And Login Successfull for Check-custom-workflow" << endl;
	return Status;
}
EPM_decision_t Check_workflow(EPM_rule_message_t msg)
{
	EPM_decision_t decision;
	int iCount, iCount1;
	char* cRootTaskName = NULL;
	char* cJobName = NULL;
	char* cstartedwf = NULL;
	char* cAllwf = NULL;
	tag_t tRootTask = NULLTAG;
	tag_t* tAttachment = NULLTAG;
	tag_t* tJoblist = NULLTAG;
	tag_t tJobtag = NULLTAG;
	EPM_ask_root_task(msg.task, &tRootTask); 
	EPM_ask_job(tRootTask, &tJobtag);
	EPM_ask_procedure_name2(tJobtag, &cRootTaskName);
	EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachment);
	for (int i = 0; i < iCount; i++)
	{
		int count;
		char *obj_name,*obj_id,*process;
		string x,y;
		AOM_UIF_ask_value(tAttachment[i], "object_name", &obj_name);
		AOM_UIF_ask_value(tAttachment[i], "item_id", &obj_id);
		AOM_UIF_ask_value(tAttachment[i], "process_stage_list", &process); 
		file << " object name = " << obj_name << endl;
		file << " object id = " << obj_id << endl;
		file << " Process_stage_list = " << process << endl;
		x = process; 
		y = cRootTaskName;
		count = a(x, y);
		file << " x = " << x << endl;
		file << " y = " << y << endl;
		if (x.find(y) != string::npos && count <=1)
		{
			file << " Inside if condition " << endl;
			decision = EPM_go;
		}
		else
		{
			file << " Inside else condition " << endl;
			decision = EPM_nogo;
		}
	}
	return decision;
}
extern DLLAPI int dll_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	int Status = ITK_ok;
	cout << "\n***************************************" << endl;
	cout << "\n  DLL Register And Logout Successfull custom-rule-handler-1-logout" << endl;
	cout << "\n***************************************\n\n" << endl;
	file << "DLL Register And Logout Successfull for Check-custom-workflow";
	return Status;
}