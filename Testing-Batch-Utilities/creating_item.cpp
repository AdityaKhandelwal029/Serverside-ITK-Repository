#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int c;
		tag_t item_tag = NULL;
		tag_t rev_tag = NULL;
		c = ITEM_create_item("001000", "item1000", "Item", "A", &item_tag, &rev_tag);
		AOM_save(item_tag);//saves the create object
		if (c == ITK_ok)
		{
			cout << " c = " << c << endl;
			cout << "Item create successfully "<<endl;
		}
		else
		{
			cout << " Item creation failed " << endl;
		}
	}
	else
	{
		cout << " Task not complete " << endl;
	}
	ITK_exit_module(true);
	return(0);
}