#include"Header.h"
int ITK_user_main(int argc, char *argv[])
{
	int iFail = 100;
	char *cError = NULLTAG;
	tag_t tItem = NULLTAG;
	tag_t tRevision = NULLTAG;
	iFail = ITK_init_module("infodba", "infodba", "dba");
	if (iFail == ITK_ok)
	{
		cout << " Inside if block " << endl;
		printf(" Login successful .....\n");
		iFail = ITEM_create_item("000453", "Test_1", "lll", "A", &tItem, &tRevision); 
		cout << " ifail = " << iFail << endl;
		if (iFail == 0)
		{
			iFail = ITEM_save_item(tItem);
			printf(" Item created successfully \n");
			if (iFail == ITK_ok)
			{
				printf(" Item saved successfully \n");
			}
			else
			{
				printf(" Process failed \n");
			}
		}
	}
	else
	{
		cout << " Inside else block " << endl;
		EMH_ask_error_text(iFail, &cError);
		printf("The error is %s", cError);
	}
	return 0;
}