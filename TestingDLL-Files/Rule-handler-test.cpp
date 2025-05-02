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
	cout << "\n  DLL Register And Login Successfull " << endl;
	cout << "\n***************************************\n\n" << endl;
	file << "************************************************************************"<<endl;
	file << "DLL register successfull" << endl;
	EPM_register_rule_handler("Check-Workflow", "", (EPM_rule_handler_t)Check_workflow);
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
	file << "************************************************************************" << endl;
	file << " troottask tag = " << tRootTask << endl;
	file << " ask job tag = " << tJobtag << endl;
	file << " croottaskname = " << cRootTaskName << endl;
	EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachment);
	file << " Attachment count = " << iCount << endl;
	for (int i = 0; i < iCount; i++)
	{
		AOM_UIF_ask_value(tAttachment[i], "fnd0StartedWorkflowTasks", &cstartedwf);
		AOM_UIF_ask_value(tAttachment[i], "fnd0AllWorkflows", &cAllwf);
		file << " cstartedwf = " << cstartedwf << endl;
		file << " allwf = " << cAllwf << endl;
		EPM_ask_active_job_for_target(tAttachment[i], &iCount1, &tJoblist);
		for (int j = 0; j < iCount1; j++)
		{
			EPM_ask_procedure_name2(tJoblist[j], &cJobName);
			int k = tc_strcmp(cJobName, cRootTaskName);
			string started, allworkflow, roottaskname;
			started = cstartedwf;
			allworkflow = cAllwf;
			roottaskname = cRootTaskName;
			file << " started string = " << started << endl;
			file << " allworkflow = " << allworkflow << endl;
			file << " roottaskstring = " << roottaskname << endl;
			int allworkc1 = a(allworkflow, roottaskname);
			int started1 = a(started, roottaskname);
			file << " allworkc1 = " << allworkc1 << endl;
			file << " started1 = " << started1 << endl;
			file << " k = " << k << endl;
			if ((started1 == 1) && (allworkc1 == 1) )
			{
				file << "Inside if 1 " << endl;
				decision = EPM_go;
			}
			if ((started1 == 1) && (allworkc1 == 2) )
			{
				file << "Inside if 2 " << endl;
				decision = EPM_go;
			}
			if ((started1 > 1) && (allworkc1 >= 2) )
			{
				file << "Inside if 3 " << endl;
				decision = EPM_nogo;
				EMH_store_error_s1(EMH_severity_error, DLL_ERROR, cRootTaskName);
			}
		}
	}
	return decision;
}
extern DLLAPI int dll_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	int Status = ITK_ok;
	cout << "\n***************************************" << endl;
	cout << "\n  DLL Register And Logout Successfull " << endl;
	cout << "\n***************************************\n\n" << endl;
	file << "************************************************************************" << endl;
	return Status;
}