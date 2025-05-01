#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int countofobject, countofdeepcopyobjects;
		tag_t itemt;
		tag_t lstitemrevt;
		tag_t newrevt;
		tag_t *secondaryobjlist;
		tag_t classtag;
		tag_t relationtag;
		char *classname;
		string classtype;
		char operation[50] = "ITEM_revise_operation";
		char *op = &operation[0];

		cout << ITEM_find_item("1412906", &itemt) << endl;
		cout << ITEM_ask_latest_rev(itemt, &lstitemrevt) << endl;
		cout << GRM_find_relation_type("IMAN_specification", &relationtag) << endl;
		cout << GRM_list_secondary_objects_only(lstitemrevt, relationtag, &countofobject, &secondaryobjlist) << endl;
		cout << " Secondary objects found = " << countofobject << endl;
		for (int i = 0; i < countofobject; i++)
		{
			POM_class_of_instance(secondaryobjlist[i], &classtag);
			POM_name_of_class(classtag, &classname);
			classtype = classname;
			if (classtype == "Dataset")
			   {
				cout << " i = 1 " << " class name = " << classname << endl;
				ITEM_deepcopy_info_t structobj;
				structobj.component = secondaryobjlist[i];
				structobj.action = 0;
				//structobj.objName = NULL;
				//structobj.relation = NULL;
				structobj.is_target_primary = false;
				structobj.is_required = false;
				structobj.copy_relations = false;
				cout << " component = " << structobj.component << endl;
				cout << " Action = " << structobj.action << endl;
				//cout << " objname = " << structobj.objName << endl;
				//cout << "  Relation " << structobj.relation << endl;
				cout << " is_target_primary = " << structobj.is_target_primary << endl;
				cout << " is_required = " << structobj.is_required << endl;
				cout << "copy_relations " << structobj.copy_relations << endl;
				int error_code = 0;
				try
				{
					tag_t *deepcopiedobjects;
					cout<<" Revise status = "<<ITEM_copy_rev_using(lstitemrevt, NULL, 1, &structobj, &newrevt, &countofdeepcopyobjects, &deepcopiedobjects);
				    cout << " count of deep copy objects  = " << countofdeepcopyobjects << endl;
				    cout << "Save status = " << AOM_save_with_extensions(newrevt) << endl;
				}
				catch (...)
				{
					cout << " Exception occured " << endl;
					cout << " Error code = " << error_code << endl;
					char *error_text;
					EMH_ask_error_text(error_code, &error_text);
					cout << " Error Text = " << error_text;
				}
			}
		}
	}
	return(0);
}