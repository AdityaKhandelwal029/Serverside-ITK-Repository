#include"Header1.h"
extern DLLAPI int dll_register_callbacks()
{
	Status = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)Flender_execute_callbacks1);
	Status = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Flender_execute_callbacks2);
	return Status;
}

extern DLLAPI int Flender_execute_callbacks1(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n***************************************");
	printf("\n  DLL Register And Login Successfull ");
	printf("\n***************************************\n\n");
	EPM_register_action_handler("Flender_rel_previous_revision", "", (EPM_action_handler_t)Flender_RelPrevRev_ActionH);
	return Status;
}
extern DLLAPI int Flender_RelPrevRev_ActionH(EPM_action_message_t msg)
{
	int iAttachmentCount;
	int iRevCount;

	tag_t tRootTask = NULLTAG;
	tag_t* tAttachments = NULLTAG;
	tag_t tClassID = NULLTAG;
	tag_t tAttachmentItem = NULLTAG;
	tag_t *tRevtag = NULLTAG;
	tag_t tRelStatus = NULLTAG;

	char* cArgumentName = NULL;
	char* cArgumentValue = NULL;
	char* cArgumentValueCopy = NULL;
	char* cClassName = NULL;

	vector<string> vArgumentValue;
	tag_t vAttachments[1000]; int x = 0;

	logical retain_released_date = true;

	EPM_ask_root_task(msg.task, &tRootTask);
	EPM_ask_attachments(tRootTask, EPM_target_attachment, &iAttachmentCount, &tAttachments);
	if (iAttachmentCount > 0)
	{
		for (int i = 0; i < iAttachmentCount; i++)
		{
			int iNo_of_Arguments = TC_number_of_arguments(msg.arguments);
			for (int j = 0; j < iNo_of_Arguments; j++)
			{
				ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgumentName, &cArgumentValue);
				cArgumentValueCopy = (char*)MEM_alloc(100);
				tc_strcpy(cArgumentValueCopy, cArgumentValue);
			}
			vAttachments[x]=tAttachments[i]; 
			x++;
		}
	}
	if (cArgumentValueCopy != NULL)
	{
		string temp;
		stringstream s(cArgumentValueCopy);
		while (getline(s, temp, ','))
		{
			vArgumentValue.push_back(temp);
		}
	}

	for (int k = 0; k < x; k++)
	{
		POM_class_of_instance(vAttachments[k], &tClassID);
		POM_name_of_class(tClassID, &cClassName);
		string check = cClassName;
		if (check.find("Revision") != string::npos)
		{
			ITEM_ask_item_of_rev(vAttachments[k], &tAttachmentItem);
			ITEM_list_all_revs(tAttachmentItem, &iRevCount, &tRevtag);
			for (int l = 0; l < iRevCount; l++)
			{
				if (vAttachments[k] == tRevtag[l])
				{
					for (int m = 0; m < vArgumentValue.size(); m++)
					{
						const char * ccArgumentValue = vArgumentValue.at(m).c_str();
						RELSTAT_create_release_status(ccArgumentValue, &tRelStatus);
						if (tRelStatus != NULLTAG)
						{
							RELSTAT_add_release_status(tRelStatus, 1, &tRevtag[l - 1], retain_released_date);
						}
						else
						{
							TC_write_syslog("\n\nWorkflow Process is not initiated on revision object");
						}
					}
				}

			}
		}
		else
		{
			//cout << "\n\nWorkflow Process is not initiated on revision object" << endl;
			TC_write_syslog("\n\nWorkflow Process is not initiated on revision object");
		}

	}
	if (cArgumentName)
	{
		MEM_free(cArgumentName);
	}
	if (cArgumentValue)
	{
		MEM_free(cArgumentValue);
	}
	if (cArgumentValueCopy)
	{
		MEM_free(cArgumentValueCopy);
	}
	if (cClassName)
	{
		MEM_free(cClassName);
	}
	if (tAttachments)
	{
		MEM_free(tAttachments);
	}
	if (tRevtag)
	{
		MEM_free(tRevtag);
	}
}

extern DLLAPI int Flender_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n***************************************");
	printf("\n  DLL Register And Login Successfull ");
	printf("\n***************************************\n\n");
	return Status;
}