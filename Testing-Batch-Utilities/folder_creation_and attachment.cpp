#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t folder_tag,user_tag,home_folder;
		FL_create2("F11", "sample1folder_11", &folder_tag);
		AOM_save(folder_tag);
		SA_find_user2("installuser", &user_tag);
		SA_ask_user_home_folder(user_tag, &home_folder);
		FL_insert(home_folder, folder_tag, 10);
		AOM_save(home_folder);
	}
	return(0);
}