#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int quan=-1;
		tag_t item_t;
		tag_t create_input;
		tag_t *objectcreated_t;
		vector<tag_t> tagvector;
		cout<<TCTYPE_find_type("Item", "Item",&item_t)<<endl;
		for (int i=1;i<=10;i++)
		{
			cout<<TCTYPE_construct_create_input(item_t, &create_input); 
			cout<<AOM_UIF_set_value(create_input, "object_name", "TCTYPEITEMTEST1-30")<<endl; 
			tagvector.push_back(create_input);
		} 
		int* quantities = new int[tagvector.size()];
		int quant[10] = { 1 };
	   cout<< TCTYPE_create_objects(tagvector.size(), quant,&tagvector[0],&quan,&objectcreated_t);
		cout << " Done " << endl;
		cout << " quan = " << quan << endl;  
	}
	return(0);
}