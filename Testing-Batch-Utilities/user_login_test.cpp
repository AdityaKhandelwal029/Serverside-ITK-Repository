#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		char *user_id;
		tag_t user_tag;
		POM_get_user(&user_id,&user_tag);
		cout << " User id = " << user_id << endl;
	}
	return(0);
}