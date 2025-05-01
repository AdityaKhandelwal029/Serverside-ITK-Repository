#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int x;
		POM_site_id(&x); 
		cout << x << endl; 
	}
	return(0);
}