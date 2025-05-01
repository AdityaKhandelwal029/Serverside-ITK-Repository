#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int n_referencers;
		int *levels;
		tag_t *refereneres;
		char **relations;
		tag_t item_t;
		ITEM_find_item("002096",&item_t);  
		WSOM_where_referenced(item_t, WSO_where_ref_any_depth,&n_referencers,&levels,&refereneres,&relations);
		cout << " n_refereners = " << n_referencers<<endl; 
		for (int i = 0; i < n_referencers; i++)
		{
			char *d_value,*d_value2;
			AOM_UIF_ask_value(refereneres[i], "object_name", &d_value); 
			AOM_UIF_ask_value(refereneres[i], "item_id", &d_value2);
			cout << " i = " << i + 1 <<"  "<< d_value;
			cout<<"  "<< d_value2 << endl;
		}
	}
	return(0);
}