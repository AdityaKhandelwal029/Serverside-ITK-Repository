
// Objective -: Client Code to get all user with group and role of a project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/custom.h>
#include <tccore/tctype.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <res/res_itk.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <tccore/aom_prop.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>
#include <sa/user.h>
#include <tc/folder.h>
#include <ae/datasettype.h>
#include <ae/dataset.h>

#define ITK_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))						

static int report_error( char *file, int line, char *function, int return_code)			
{
	if (return_code != ITK_ok)
	{
		int				index = 0;
		int				n_ifails = 0;
		const int*		severities = 0;
		const int*		ifails = 0;
		const char**	texts = NULL;

		EMH_ask_errors( &n_ifails, &severities, &ifails, &texts);
		printf("%3d error(s)\n", n_ifails);
		for( index=0; index<n_ifails; index++)
		{
			printf("ERROR: %d\tERROR MSG: %s\n", ifails[index], texts[index]);
			TC_write_syslog("ERROR: %d\tERROR MSG: %s\n", ifails[index], texts[index]);
			printf ("FUNCTION: %s\nFILE: %s LINE: %d\n\n", function, file, line);
			TC_write_syslog("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
		}
		EMH_clear_errors();
		
	}
	return return_code;
}				

int ITK_user_main(int argc, char* argv[])
{
	int iStatus = 0;
	tag_t query_tag = NULLTAG;
	//int n_entries = 3;

	int n_entries = 1;
	int j = 0;

	char *usr = ITK_ask_cli_argument("-u=");
	char *pwd = ITK_ask_cli_argument("-p=");
	char *grp = ITK_ask_cli_argument("-g=");
    char *ProjectID = ITK_ask_cli_argument( "-i=");
	char *qry_entries[1] = {"Project ID"}, *values1[1] = {ProjectID};
			
    int num_found ;
    tag_t *tag_query_result;
    
    char *Object_Name  =  NULL;
     
    iStatus = ITK_auto_login();
    if(iStatus == ITK_ok)
    {
        printf("\n\n\t Login Successful\n");
    }
    else
    {
          printf("\n UnSuccessful");
    }
	
	ITK_CALL(QRY_find2("Test_proj1",&query_tag));

	if(query_tag != 0)
	{
	    printf("==================Query Test_proj1 successfully==========\n");
	}
	else
	{
	    printf("\n\n >>>>>>>>>>>Unable to find Query>>>>>>>>>>");
	}
	
	ITK_CALL(QRY_execute(query_tag,n_entries,qry_entries,values1,&num_found,&tag_query_result));

	printf("\n\n number of users under given project  %d",num_found);
	
	for(j==0;j<num_found;j++)
	{
		ITK_CALL(AOM_UIF_ask_value(tag_query_result[j],"object_string", &Object_Name));
		printf("\n %s",Object_Name);
	}
	return 0;

}