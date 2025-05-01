#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		char *homef_name;
		tag_t *find_list, user_tag, homef_tag;
		SA_find_user2("infodba", &user_tag);
		SA_ask_user_home_folder(user_tag, &homef_tag);
		AOM_ask_name(homef_tag, &homef_name);
		cout << " folder name = " << homef_name << endl;
		FL_ask_references(homef_tag, FL_fsc_by_name, &count, &find_list);
		cout << " count = " << count;
		for (int j = 0; j < count; j++)
		{
			AOM_ask_name(find_list[j], &homef_name);
			cout << " children name = " << homef_name << endl;
		}
	}
	return(0);
}

