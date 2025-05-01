#include"Header.h" 
ofstream data1("D:/Dataoffolder.txt");
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count, secobjcount;
		char *classname, *owninguser, *contents;
		/*string input;
		cout << " Please enter the name of the folder = ";
		cin >> input;
		const char *inpu1 = input.c_str();*/
		tag_t *itemlist_t, classid_t, *secobj_t;
		WSOM_find2("f1", &count, &itemlist_t);
		cout << " ************************************Login successfull************************************ " << endl;
		data1 << " ************************************Login successfull************************************ " << endl;
		data1 << "Total objects found with the given name = " << count;
		for (int i = 0; i < count; i++)
		{
			POM_class_of_instance(itemlist_t[i], &classid_t);
			POM_name_of_class(classid_t, &classname);
			AOM_UIF_ask_value(itemlist_t[i], "owning_user", &owninguser);
			cout << " count = " << i;
			cout << " owning user = " << owninguser;
			cout << " class name = " << classname;
			data1 << " count = " << i;
			data1 << " owning user = " << owninguser;
			data1 << " class name = " << classname << endl;
			AOM_UIF_ask_value(itemlist_t[i], "contents", &contents);
			cout << " secondary object = " << contents;
			FL_ask_references(itemlist_t[i], FL_fsc_as_ordered, &secobjcount, &secobj_t);
			data1 << " secondary object = " << contents;
			cout << " secondary obeject = " << secobjcount << endl;
		}
	}
	return(0);
}