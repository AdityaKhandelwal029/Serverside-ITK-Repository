#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		long long int x;
		int iCount;
		char *dvalue = NULL;
		char* cObject_name = NULL;
		tag_t item_t;
		tag_t tOwningUser = NULLTAG;
		tag_t tOwningGroup = NULLTAG;
		tag_t tHomeFolder = NULLTAG;
		tag_t* TValueTag = NULL;
		logical lLock = true;
		cout << ITEM_find_item("1409400", &item_t) << endl;
		cout << AOM_UIF_ask_value(item_t, "object_string", &dvalue) << endl;
		cout << " Name = " << dvalue << endl; 
		cout << AOM_UIF_ask_value(item_t, "item_id", &dvalue) << endl;
		cout << " Item_id = " << dvalue << endl;
		/*POM_ask_owner(item_t, &tOwningUser, &tOwningGroup);
		SA_ask_user_home_folder(tOwningUser, &tHomeFolder);
		AOM_ask_value_tags(tHomeFolder, "contents", &iCount, &TValueTag);
		for (int i = 0; i < iCount; i++)
		{
			AOM_ask_value_string(TValueTag[i], "object_string", &cObject_name);
			int c = tc_strcmp(dvalue, cObject_name);
			if (c == 0)
			{
				FL_remove(tHomeFolder, TValueTag[i]);
				AOM_save(tHomeFolder);
			}
		}
		cout<<" pom-check-status"<<POM_modifiable(item_t, &lLock)<<endl;
		cout << " lock value = " << lLock <<endl;
		if (lLock)
		{
			AOM_refresh(item_t, FALSE);
			AOM_save(item_t);

			cout << " Inside if " << endl;
		}*/
		cout << "Aom-lock " << AOM_lock_for_delete(item_t) << endl;
		x = AOM_delete(item_t);
		cout << " Error code = " << x << endl;
		char *error_code;
		EMH_ask_error_text(x, &error_code);
		cout << " Error message = " << error_code << endl;
		if (x != 0)
		{
			int no_of_reference;
			tag_t *ref_instances;
			int *instance_levels;
			int *instance_where_found;
			int n_classes;
			tag_t *ref_classes;
			int *class_levels;
			int *class_where_found;
			POM_referencers_of_instance(item_t, 1, POM_in_ds_and_db, &no_of_reference, &ref_instances, &instance_levels, &instance_where_found, &n_classes, &ref_classes, &class_levels, &class_where_found);
			cout << " No of referencers = " << no_of_reference << endl;
			for (int i = 0; i < no_of_reference; i++)
			{
				char *dvalue_name, *dvalue_class_name;
				AOM_UIF_ask_value(ref_instances[i], "object_string", &dvalue_name);
				cout << " dvalue instance name = " << dvalue_name;

				AOM_UIF_ask_value(ref_instances[i], "object_name", &dvalue_name);
				cout << " dvalue object name = " << dvalue_name;

				AOM_UIF_ask_value(ref_instances[i], "object_type", &dvalue_name);
				cout << " dvalue object type = " << dvalue_name;

				cout << " instance_levels = " << instance_levels[i];
				cout << "  instance_where_found = " << instance_where_found[i];
				cout << " n_classes = " << n_classes;
				//AOM_UIF_ask_value(ref_classes[i], "object_name", &dvalue_class_name); 
				//cout << " ref_class name = " << dvalue_class_name; 
			//	cout << " class levels = " << class_levels[i]; 
			//	cout << " class_where_found = " << class_where_found[i];

				tag_t class_id;
				char *class_name;
				POM_class_of_instance(ref_instances[i], &class_id);
				POM_name_of_class(class_id, &class_name);
				cout << " Class name = " << class_name;

				tag_t relation_t;
				char *relation_name;
				GRM_find_relation(item_t, ref_instances[i], NULLTAG, &relation_t);
				//AOM_UIF_ask_value(relation_t, "object_string", &relation_name); 
				//cout << " Relation name = " << relation_name;

				cout << " Break relation status = " << GRM_delete_relation(relation_t);
				cout << " AOM Lock status 1 = " << AOM_lock_for_delete(relation_t);
				cout << " AOM delete status = " << AOM_delete(relation_t);
				cout << " AOM lock status 2 " << AOM_lock_for_delete(ref_instances[i]);
				cout << " AOM delete status 2 " << AOM_delete(ref_instances[i]);
				cout << endl;
				cout << endl;
				cout << endl;
				cout << endl;
				/*string cl = class_name;
				if (cl == "Fnd0PartDesignAttrs")
				{
					cout<<" AOM delete 1 = " <<AOM_delete(ref_instances[i]);
				}*/
			}
			x = AOM_delete(item_t);
			cout << " Error code = " << x << endl;
		}
	}
	return(0);
}