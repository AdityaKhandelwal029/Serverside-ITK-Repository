#include"Header.h"
#include<fstream>
using namespace std;
int ITK_user_main(int argc, char *argv[])
{
	fstream a("ITK1.txt");
	int ifail;
	char *cError = NULL;
	ifail = ITK_init_module("infodba", "infodba", "dba");
	if (ifail == ITK_ok)
	{
		cout << " Login successful Aditya khandelwal " << endl;
		a << "Login successfull ";
		a << endl;
		tag_t item_tag, Rev_tag;
		tag_t *rev_list = NULLTAG, *secondary_objects = NULLTAG;
		int rev_count, sec_count;
		char *Rev_name = NULL, *datasettype_name = NULL, *d_value = NULL, *datasettype = NULL;
		ifail = ITEM_find_item("000136", &item_tag);
		if (ifail == 0)
		{
			cout << "Item found successully " << endl;
			a << " Item found successfully ";
			a << endl;
			ifail = ITEM_list_all_revs(item_tag, &rev_count, &rev_list);
			if (ifail == 0)
			{
				cout << " rev count = " << rev_count << endl;
				a << "Total Revision count = ";
				a << rev_count;
				a << endl;
				for (int i = 0; i < rev_count; i++)
				{
					ITEM_ask_rev_name2(rev_list[i], &Rev_name);
					cout << Rev_name << endl;
					GRM_list_secondary_objects_only(rev_list[i], NULLTAG, &sec_count, &secondary_objects);
					cout << " Sec count = " << sec_count;
					a << " Total secondary objects of ";
					a << i + 1;
					a << " = ";
					a << sec_count;
					a << endl;
					cout << endl;
					for (int j = 0; j < sec_count; j++)
					{
						AOM_UIF_ask_value(secondary_objects[j], "object_type", &datasettype);
						AOM_UIF_ask_value(secondary_objects[j], "object_name", &datasettype_name);
						cout << datasettype << endl;
						cout << datasettype_name << endl;
						a << datasettype;
						a << endl;
						a << datasettype_name;
						a << endl;
					}
				}
			}
		}
	}
	else
	{
		EMH_ask_error_text(ifail, &cError);
		printf("The error is %s", cError);
	}
	return(0);
}