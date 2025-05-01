//Item copy rev - Revise operation
//Item create from rev - save as operation 

#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t itemt;
		tag_t lstitemrevt;
		tag_t newrevt;
		int count;
		char operation[] = "ITEM_revise_operation";
		tag_t *deepcopiedobjects;
		cout << ITEM_find_item("1412901", &itemt) << endl;;
		cout << ITEM_ask_latest_rev(itemt, &lstitemrevt) << endl;
		cout << ITEM_copy_rev(lstitemrevt, NULL, &newrevt) << endl;
		AOM_save_with_extensions(newrevt);
		cout << ITEM_perform_deepcopy(newrevt, operation, lstitemrevt, &count, &deepcopiedobjects) << endl;
		AOM_save_with_extensions(newrevt);
		cout << " count = " << count << endl;
		for (int i = 1; i <= count; i++)
		{
			tag_t classtag;
			char *classname, *objectname;
			POM_class_of_instance(deepcopiedobjects[i], &classtag);
			POM_name_of_class(classtag, &classname);
			AOM_ask_value_string(deepcopiedobjects[i], "object_string", &objectname);
			cout << " i = " << i << " class name = " << classname << " object_string = " << objectname << endl;
		}
	}
	return(0);
}

/* ITK - Program to perform revise operation on latest working revision of Item */