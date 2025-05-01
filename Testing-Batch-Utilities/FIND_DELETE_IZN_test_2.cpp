#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	{
		long long int a;
		a = ITK_init_module("infodba", "infodba", "dba");
		if (a == 0)
		{
			cout << " Login_successfull " << endl;
			tag_t item_t;
			char *d_value, *error_text;
			long long int x;
			ITEM_find_item("1409275", &item_t);
			AOM_UIF_ask_value(item_t, "object_name", &d_value);
			cout << " Name value = " << d_value << endl;
			x = AOM_delete(item_t);
			cout << " Delete-status = " << x;
			EMH_ask_error_text(x, &error_text);
			cout << " Error text = " << error_text << endl;
		}
		else
		{
			cout << "Login_failed" << endl;
			char *error_code;
			EMH_ask_error_text(a, &error_code);
			cout << " Error code = " << error_code;
		}
	}
	return(0);
}