#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int ifail=0,saving_value;
		tag_t item_t;
		tag_t itemrev_t;
		ifail= ITEM_create_item("000986", "TEST_ITEM_UC_CreateItem", "Item", "A", &item_t, &itemrev_t); 
		cout << " ifail = " << ifail << endl;
		if (ifail == ITK_ok)
		{
			cout << " Item created successfully " << endl;
			saving_value = ITEM_save_item(item_t); 
			cout << " saving_value = " << saving_value; 
			if (saving_value > 0)
			{
				char* Error_msg = NULL;
				EMH_ask_error_text(saving_value, &Error_msg);
				cout << " Error_msg = " << Error_msg << endl;
			}
		}
		else
		{
			char* Error_msg = NULL;
			cout << " Item creation failed " << endl;
			EMH_ask_error_text(ifail, &Error_msg);
			cout << " Error_msg = " << Error_msg << endl;
		}
	}
	return(0);
}