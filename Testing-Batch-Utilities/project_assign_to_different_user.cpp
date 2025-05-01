#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int member_count,administrator_count,privuser_count;
		char *proj_name;
		tag_t pro_tag,user_tag,*members,*administrator_tag,*priuser_tag;
		SA_find_user2("installuser", &user_tag);
		PROJ_find("000111", &pro_tag);
		PROJ_ask_name2(pro_tag, &proj_name);
		cout << " Project name = " << proj_name << endl;
		PROJ_ask_team(pro_tag, &member_count, &members, &administrator_count, &administrator_tag, &privuser_count, &priuser_tag);
		cout << " Total members = " << member_count << endl;
		cout << " administrator count = " << administrator_count << endl;
		cout << " Priviliged user = " << privuser_count << endl;
	}
	return(0);
}

