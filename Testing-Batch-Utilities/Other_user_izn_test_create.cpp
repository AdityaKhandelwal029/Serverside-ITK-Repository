#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	{
		long long int a;
		a = ITK_init_module("izn", "izn", "Engineering");
		if (a == 0)
		{
			cout<<" Login_successfull ";
			tag_t item_tag, itemr_tag;
			cout<<"Item-creation-status = "<<ITEM_create_item("ITEM-3", "TEST-ITEM-1", "Item", "A", &item_tag, &itemr_tag);
			cout<<"Item-save-status = "<<ITEM_save_item(item_tag); 
		}
		else
		{
			cout<<"Login_failed"<<endl;
			char *error_code; 
			EMH_ask_error_text(a, &error_code); 
			cout << " Error code = " << error_code; 
		}
	}
	return(0);
}