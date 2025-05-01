#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_tag, itemr_tag,dataset_tag,dataset_type,relation,c_relation;
		ITEM_find_item("000135", &item_tag);
		ITEM_ask_latest_rev(item_tag, &itemr_tag);
		char *itemr_name;
		AOM_ask_name(itemr_tag, &itemr_name);
		cout << " item revision name = " << itemr_name << endl;
		AE_find_datasettype2("HTML", &dataset_type);
		AE_create_dataset_with_id(dataset_type, "sampledataset1", "test dataset", "01", "A", &dataset_tag);
		AOM_save(dataset_tag);
		GRM_find_relation_type("IMAN_reference", &relation);
		GRM_create_relation(itemr_tag,dataset_tag,relation,NULLTAG,&c_relation);
		AOM_save(c_relation);
		GRM_save_relation(c_relation);
		cout << " Done " << endl;
	}
	return(0);
}