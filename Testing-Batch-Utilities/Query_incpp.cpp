[03:10 pm] Mohite, Shubham
#include "Header.h"
#define _ITERATOR_DEBUG_LEVEL 0
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
	int status;    char * user_name = NULL;    char * password = NULL;    char * user_group = NULL;    char * message = NULL;    tag_t query = NULLTAG;    char *object_name = NULL;    char *object_type = NULL;    int No_of_obj;    char* DValue = NULL;    char *Tst = NULL;    char* Tst1 = NULL;    char* Tst2 = NULL;    user_name = ITK_ask_cli_argument("-u=");    password = ITK_ask_cli_argument("-p=");    user_group = ITK_ask_cli_argument("-g=");    Tst = ITK_ask_cli_argument("-name=");    Tst1 = ITK_ask_cli_argument("-type=");    Tst2 = ITK_ask_cli_argument("-owner=");    status = ITK_init_module(user_name, password, user_group);    if (status == ITK_ok) {
		printf("\nlogin is sucessfull with User %s and group is %s", user_name, user_group);        status = QRY_find2("General...", &query);        if (status == ITK_ok) {
			printf("\nQuery found");            char searchentry[3][16] = { "Name","Type","Owning User" };            tag_t* tObject = NULLTAG;            int n_entries = 1;            char **cEntry_Names;            char **cValues;            char* DValue1;

			cEntry_Names = (char **)MEM_alloc(n_entries * sizeof(char *));            cValues = (char **)MEM_alloc(n_entries * sizeof(char *));

			cEntry_Names[0] = (char *)MEM_alloc(strlen(searchentry[0]) + 1);            strcpy(cEntry_Names[0], searchentry[0]);

			cEntry_Names[1] = (char *)MEM_alloc(strlen(searchentry[1]) + 1);            strcpy(cEntry_Names[1], searchentry[1]);

			cEntry_Names[2] = (char *)MEM_alloc(strlen(searchentry[2]) + 1);            strcpy(cEntry_Names[2], searchentry[2]);

			cValues[0] = (char *)MEM_alloc(strlen(Tst) + 1);            strcpy(cValues[0], Tst);

			cValues[1] = (char *)MEM_alloc(strlen(Tst) + 1);            strcpy(cValues[1], Tst1);

			cValues[2] = (char *)MEM_alloc(strlen(Tst) + 1);            strcpy(cValues[2], Tst2);

			QRY_execute(query, 3, cEntry_Names, cValues, &No_of_obj, &tObject);            printf("\nNumber of objects found = %d", No_of_obj);            for (int i = 0; i < No_of_obj; i++) { AOM_UIF_ask_value(tObject[i], "object_name", &DValue);                AOM_UIF_ask_value(tObject[i], "object_type", &DValue1);                printf("\nname = %s type=%s", DValue, DValue1); }
		}        else        { printf("Failed to find query"); }
	}    else    { printf("Failed to login"); }    return status;
}

[03:34 pm] Khandelwal, Aditya


