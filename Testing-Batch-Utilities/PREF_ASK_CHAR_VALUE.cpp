//PREF_ask_char_value - It is used to find value of a preference at the given location
//PREF_ask_char_values - It is used to find list and count of all  the values of a preference.
#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		char **total_values;
		int total_count;
		int status = ITK_ok;
		char *value;
		 cout<<PREF_ask_char_value("3DMarkup_relation_primary", 1, &value)<<endl; 
		cout << " Pref name found = " << value<<endl; 

		cout << " ************************************************************************ " << endl;
		PREF_ask_char_values("EC_affected_item_rel_relation_primary", &total_count, &total_values); 
		cout << " Total count of the values = " << total_count<<endl;  
		for (int i = 0; i < total_count; i++)
		{
			cout << " PREF name = " << total_values[i] << endl; 
		}
		cout << "**************************************************************************** " << endl;
	}
	return(0);
}