#include<iostream>
#include<tccore/item.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/item.h>
#include<tc/emh.h>
#include<lov/lov.h>
#include<fclasses/tc_string.h>

using namespace std;
//void login(const char* userid, const char*pswd, const char*grp);
//int check(int x);
int ifail = ITK_ok;
char * Error = NULL;
int ITK_user_main(int argc, char* argv[])
{
	tag_t titem = NULLTAG;
	tag_t tlov = NULLTAG;
	char** values = NULL;
	int nvalues = 0;



	ifail = ITK_init_module("infodba", "infodba", "dba");
	cout << " Logging to lov project  " << endl;
	if (ifail == ITK_ok)
	{
	    cout<<" ITEM found = "<<ITEM_find_item("000487", &titem)<<endl;
		cout<<" Ask Lov = "<< AOM_ask_lov(titem, "ip_classification", &tlov)<<endl;
		cout<<" Lov values = "<<LOV_ask_values_string(tlov, &nvalues, &values)<<endl;
		for (int i = 0; i < nvalues; i++)
		{
			cout << " i = " << i << " value = " << values[i] << endl;
		}
		cout<<" Lov = "<<AOM_set_value_string(titem, "ip_classification", "secret")<<endl;
		/*for (int i = 0; i < nvalues; i++)
		{
			ifail = tc_strcmp(values[i], "car");
			if (ifail == 0)
			{
				ifail = AOM_set_value_string(titem, "ip_classification", "secret");
				if (ifail == ITK_ok)
				{
					AOM_save_without_extensions(titem);
					cout << "property updates";
					break;
				}
				else
				{
					EMH_ask_error_text(ifail, &Error);
					cout << "Error is %s" << Error << endl;
				}
			}
			else
			{
				EMH_ask_error_text(ifail, &Error);
				cout << "Error is %s" << Error << endl;
			}
		}

*/
	}
	return ITK_ok;
}