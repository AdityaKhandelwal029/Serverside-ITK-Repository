#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t itemtag1, itemtag2,itemrevtag1,itemrevtag2;
		tag_t *revised_objects;
		TCTYPE_bulk_copy_input_t*  pBulkCopyInput = 0;
		vector<tag_t> source_objects;


		cout << ITEM_find_item("1420245", &itemtag1) << endl;
		cout << ITEM_find_item("1420246", &itemtag2) << endl;
		cout<<ITEM_ask_latest_rev(itemtag1, &itemrevtag1) << endl;
		cout << ITEM_ask_latest_rev(itemtag2, &itemrevtag2) << endl;
		source_objects.push_back(itemrevtag1);
		source_objects.push_back(itemrevtag2);

		cout << TCTYPE_construct_bulk_copy_inputs(2, &source_objects[0], TCTYPE_OPERATIONINPUT_REVISE, &pBulkCopyInput) << endl;

		cout << TCTYPE_bulk_revise_objects(2, pBulkCopyInput, &revised_objects) << endl;
	}
	return(0);
}
