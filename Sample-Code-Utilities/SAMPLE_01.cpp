#include "Header.h"
//#define _ITERATOR_DEBUG_LEVEL 0
//
//#include <conio.h>
//#include <string.h>
//#include<ae/dataset.h>
//#include<ae/datasettype.h>
//#include <tcinit/tcinit.h>
//#include <tc/emh.h>
//#include <tc/tc_startup.h>
//#include <tccore/item.h>
//#include <tc/emh.h>
//#include <tccore/aom.h>
//#include<tccore/grm.h>
//#include <tccore/aom_prop.h>
//#include <tccore/tctype.h>
//#include <base_utils/IFail.hxx>
//#include<tc/folder.h>

int ITK_user_main(int argc, char* argv[])
{
	int status;
	char * user_name = NULL;
	char * password = NULL;
	char * user_group = NULL;
	char * message = NULL;
	tag_t query=NULLTAG;
	char *object_name = NULL;
	char *object_type = NULL;
	int No_of_obj;
	char* DValue = NULL;
	char *Tst = NULL;
	char* Tst1 = NULL;
	user_name = ITK_ask_cli_argument("-u=");
	password = ITK_ask_cli_argument("-p=");
	user_group = ITK_ask_cli_argument("-g=");
	Tst = ITK_ask_cli_argument("-k=");
	Tst1 = ITK_ask_cli_argument("-l=");
	status = ITK_init_module(user_name, password, user_group);
	if (status == ITK_ok)
	{
		printf("login is sucessfull with User %s and group is %s", user_name, user_group);
		status = QRY_find2("Item...", &query);
		if (status == ITK_ok)
		{
			printf("Query found");
			
			tag_t* tObject = NULLTAG;
			char *cEntry_Names[2];
			char *cValues[2];
			//cEntry_Names[0] = "Item ID";
			//cValues[0] = "000074";
			strcpy(cEntry_Names[0], Tst);
			strcpy(cValues[0], Tst1);
			//std::vector<std::string> cEntry_Names(100);
			//std::vector<std::string> cValues(100);
			//cEntry_Names[0] = "Item ID";
			//cValues[0] = "000074";
			//const char* cEntry_Names = reinterpret_cast<char*>("Item ID");
			QRY_execute(query, 1, cEntry_Names, cValues, &No_of_obj, &tObject);
			for (int i=0; i < No_of_obj; i++)
			{
				AOM_UIF_ask_value(tObject[i], "object_name", &DValue);
				printf("\n%s", DValue);
			}
		}
		else
		{
			printf("Failed to find query");
		}
	}
	else
	{
		printf("Failed to login");
	}
	return status;
}

