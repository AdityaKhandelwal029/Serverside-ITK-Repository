#include"Header.h" 
int Status=ITK_ok;
#define A1 _declspec(dllexport)
extern "C" A1 int dll_register_callbacks();
extern A1 int dll_execute_callbacks1(int *decision, va_list argc);
extern A1 int dll_execute_callbacks2(int *decision, va_list argc);
extern A1 int custom_handler_1(EPM_action_message_t msg);

extern A1 int dll_register_callbacks()
{
	Status = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks1);
	Status = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks2);
	return(Status);
}
extern A1 int dll_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " ***********************************************************************" << endl;
	cout << " Welcome to DLL project " << endl;
	cout << " Logging in process successfull " << endl;
	cout << " Testing action handler for review task 1" << endl;
	cout << " ***********************************************************************" << endl;
	EPM_register_action_handler("Custom-Assign-signoff", "", (EPM_action_handler_t)custom_handler_1);
	return(Status);
}
extern A1 int custom_handler_1(EPM_action_message_t msg)
{
	int iAttachCount, iNoofMem, iNoOfSignoff;
	tag_t tRootTask = NULLTAG;
	tag_t tRTask = NULLTAG;
	tag_t tSignoffTask = NULLTAG;
	tag_t tUsertag = NULLTAG;
	tag_t* tSignoff = NULL;
	tag_t *tAttachment = NULL;
	tag_t *tMember = NULL;
	char* cArgName = NULL;
	char* cArgValue = NULL;
	char* CArgValueCopy = NULL;
	int No_Of_Arguments = TC_number_of_arguments(msg.arguments);
	for (int i = 0; i < No_Of_Arguments; i++)
	{
		Status = ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgName, &cArgValue);
		if (tc_strcmp(cArgName, "userid") == 0)
		{
		 CArgValueCopy = (char*)MEM_alloc(100);
			tc_strcpy(CArgValueCopy, cArgValue);
		}
	}
	Status=SA_find_user2(CArgValueCopy, &tUsertag);
	Status=SA_find_groupmember_by_user(tUsertag, &iNoofMem, &tMember);
	Status=EPM_ask_root_task(msg.task, &tRootTask);
	Status=EPM_ask_sub_task(tRootTask, "Review_task", &tRTask);
	Status=EPM_ask_sub_task(tRTask, "perform-signoffs", &tSignoffTask);
	Status=EPM_create_adhoc_signoff(tSignoffTask, tMember[0],&iNoOfSignoff,&tSignoff);
	Status=EPM_set_adhoc_signoff_selection_done(tRootTask, TRUE);
	return Status;
}
extern A1 int dll_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " Logout successfull " << endl;
	cout << " Done with the process " << endl;
	cout << " Testing action handler for review task 1" << endl;
	cout << " ***********************************************************************" << endl;
	cout << " ***********************************************************************" << endl;
	return(Status);
}