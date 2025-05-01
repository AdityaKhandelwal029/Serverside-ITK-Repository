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
		int x;
		char *pref_value[2];
		pref_value[0]= "Project101";
		pref_value[1]= "Project201";
		x = PREF_set_char_values("TC_customization_libraries", 2, pref_value);
		printf(" The value of output = %d", x);
	}
	return(0);
}
// This code is also correct 
//int x;
//char *pref_value[90];
//pref_value[0] = "Project101";
//pref_value[1] = "Project201";
//x = PREF_set_char_values("TC_customization_libraries", 2, pref_value);
//printf(" The value of output = %d", x); 
