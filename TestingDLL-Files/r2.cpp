#include"Header.h" 
int c = 0;
#define A1 _declspec(dllexport)
logical retain_released_date = TRUE;
extern "C" A1 int dll_register_callbacks();
extern A1 int dll_execute_callbacks1(int *decision, va_list argc);
extern A1 int dll_execute_callbacks2(int *decision, va_list argc);
extern A1 int custom_handler_1(EPM_action_message_t msg);
extern A1 int dll_register_callbacks()
{
	c = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks1);
	c = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks2);
	return(c);
}
extern A1 int dll_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	c=EPM_register_action_handler("Custom-handler-2", "", (EPM_action_handler_t)custom_handler_1);
	cout << " ***********************************************************************" << endl;
	cout << " Welcome to DLL project " << endl;
	cout << " Logging in process successfull " << endl;
	cout << " Custom handler test 1" << endl;
	cout << " Objective - create Action handler such that handler will add release status on item revision but it should not effect the release date"; 
	cout << " ***********************************************************************" << endl;
	afile<< __DATE__ << endl;
	afile << __TIME__ << endl;
	afile << " Welcome to DLL project Logging in to process successfull . "<<endl;
	afile<<" Objective - create Action handler such that handler will add release status on item revision but it should not effect the release date"; 
	return(c);
}
extern A1 int custom_handler_1(EPM_action_message_t msg)
{
	int attachmentcount,noofargu;
	tag_t roottask_t, *attachments_t,class_id; 
	char *argumentname, *argumentvalue,release_status,*release_date,*class_name;
	EPM_ask_root_task(msg.task,&roottask_t); 
	afile << "Roottask_t = "<<roottask_t<<endl;
	EPM_ask_attachments(roottask_t, EPM_target_attachment,&attachmentcount,&attachments_t); 
	afile << "Attachment count = " << attachmentcount<<endl;
	for (int i = 0; i < attachmentcount; i++)
	{
		AOM_UIF_ask_value(attachments_t[i], "Date Released",&release_date); 
		noofargu = TC_number_of_arguments(msg.arguments); 
		afile << " No of arguments = " << noofargu << endl;
		for (int j = 0; j < noofargu; j++)
		{
			ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argumentname, &argumentvalue);
			afile << " Argument name = " << argumentname << endl;
			afile << " Argument value = " << argumentvalue<<endl;
			string p = argumentname;
			if (p == "release_status_name")
			{
				string x = argumentvalue, y, z;
				for (int k = 0; k < x.length(); k++)
				{
					if (x[k] == ',' || k + 1 == x.length())
					{
						afile << " Status Value received = " << y << endl;
						POM_class_of_instance(attachments_t[i],&class_id);
						POM_name_of_class(class_id, &class_name);
						string u = class_name; 
						afile << " Class reveived = " << u<<endl;
						if (u.find("Revision") != string::npos)
						{
							tag_t createdstat;
							const char *release_stat = y.c_str();
							RELSTAT_create_release_status(release_stat,&createdstat); 
							RELSTAT_add_release_status(createdstat, 1, &attachments_t[i],retain_released_date);
						} 
						y = z;
					}
					else
					{
						y = y + x[k];
					}
				}
			} 
		}
		string l = release_date;
			afile << " Release date received = " << l;
			if (l != "")
			{
				AOM_UIF_set_value(attachments_t[i], "Date Released", release_date); 
				afile << " Date successfully settedd what it was before " << endl;
			}
	}
	return(c);
}
extern A1 int dll_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " Logout successfull " << endl;
	cout << " Done with the process " << endl;
	cout << " ***********************************************************************" << endl;
	cout << " ***********************************************************************" << endl; 
	afile << endl;
	afile << " Done with the logout process "<<endl;
	return(c);
}