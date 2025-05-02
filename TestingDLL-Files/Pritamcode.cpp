#include"Header.h"
extern "C" DLLAPI int dll_register_callbacks()
{
	iFail = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	iFail = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n***The Rule handler dll is registered***\n\n");
	TC_write_syslog("$$The Rule handler dll is registered$$");
	iFail = EPM_register_rule_handler("EPM-Restrict-DuplicateWorkflow2", "This will restrict duplicate WF", (EPM_rule_handler_t)Workflow_Rule);
	return iFail;
}

//Custom Logic
extern DLLAPI int Workflow_Rule(EPM_rule_message_t msg)
{
	EPM_decision_t decision = EPM_nogo;
	int iCount = 0;
	int i = 0;
	//int *iAttachment_type    = 0;
	tag_t *iAttachment = NULL;
	char* obj_type = NULLTAG;
	tag_t tclassID = NULLTAG;
	char* class_name = NULL;
	char* WValue = NULL;
	tag_t tRootTask, tJobtag = NULLTAG;
	char *cRootTaskName = NULL;
	//iFail = EPM_ask_all_attachments(msg.task, &iCount, &iAttachment, &iAttachment_type);    //Retrieves all the object attached to given task

	EPM_ask_root_task(msg.task, &tRootTask);
	EPM_ask_job(tRootTask, &tJobtag);
	EPM_ask_procedure_name2(tJobtag, &cRootTaskName);
	iFail = EPM_ask_attachments(msg.task, EPM_target_attachment, &iCount, &iAttachment);
	for (i = 0; i < iCount; i++)
	{
		iFail = WSOM_ask_object_type2(iAttachment[i], &obj_type);
		//iFail = POM_class_of_instance(iAttachment[i], &tclassID);                         //Finds out the class to which instance is belongs
		//iFail = POM_name_of_class(tclassID, &class_name);                                 // Return name of class

		if (tc_strcmp(obj_type, "ItemRevision") == 0)
		{
			iFail = AOM_ask_value_string(iAttachment[i], "process_stage_list", &WValue);
			//if (tc_strlen(WValue)== 0)
			//if (tc_strcmp(WValue, "TCM Released") != 0)

				if (strlen(WValue) <= 1)
			    {
				if (strcmp(WValue, cRootTaskName) != 0)
				{
				decision = EPM_go;
				}
			}
			else
			{
				EMH_store_error(EMH_severity_error, PLM_Error);
			}
		}
	}
	return decision;
}

extern DLLAPI int PLM_execute_callbacks2(int* decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n***Thank You !!!***\n\n");
	return iFail;
}