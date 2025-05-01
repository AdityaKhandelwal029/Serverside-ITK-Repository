#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tc/tc_errors.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tc/folder.h>
#include<sa/user.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<tccore/grm.h>
#include<tccore/aom_prop.h>
#include<tc/emh.h>
#include<tccore/workspaceobject.h>
#include<qry/qry.h>
#include<tccore/project.h>
#include<bom/bom.h>
#include<CFM/cfm.h>  
#include<tccore/tctype.h> 
#include<pom/pom/pom.h> 
#include<tc/preferences.h>
int login()
{
	int a;
	a = ITK_init_module("infodba", "infodba", "dba");
	if (a == 0)
	{
		printf(" Login_successfull\n");
		return(1);
	}
	else
	{
		printf("Login_failed");
		return(2);
	}
}
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int total_count,i; 
		char **total_value;
		PREF_ask_char_values("TC_customization_libraries", &total_count, &total_value); 
		printf(" Total count = %d", total_count); 
		for (i = 0; i < total_count; i++)
		{
			printf("\nValue = %s", total_value[i]);
		}
		int x;
		char *pref_value[1000];
		for (i = 0; i < total_count; i++)
		{
			pref_value[i] = total_value[i];
		}
		pref_value[i] = "Project_test_1"; 
		pref_value[i + 1] = "Project_test_2";
		x = PREF_set_char_values("TC_customization_libraries", i+2, pref_value);
		printf(" the value of output = %d", x);
	}
	return(0);
}