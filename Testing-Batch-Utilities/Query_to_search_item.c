#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int i,item_count;
		tag_t query_tag, *result_tag;
		char *item_name;
		 char *ent[4] = { "Name","Type","Owning User","Owning Group"};
		 char *value[4] = { "AX1","Item","infodba","dba"};
		QRY_find2("General...",&query_tag);
		QRY_execute(query_tag, 4,ent,value, &item_count, &result_tag);
		printf("Total item count = %d\n", item_count);
		for (i = 0; i < item_count; i++)
		{
			AOM_ask_name(result_tag[i],&item_name);
			printf("Item name = %s\n", item_name);
		}
	}
	return(0);
}

