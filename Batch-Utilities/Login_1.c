/*Objective - Login test in teamcenter . With the data provided by the user through command line arguments.*/

/*Required headers files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/custom.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <res/res_itk.h>
#include <ctype.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>
#include <time.h>

/*Declarations */

int itkApiCallStatus = -1;

/*Custom API error handling function */

int ITK_APICALL(int x)
{      
	    int status=x;
        if(status!=ITK_ok)
        {
            int index = 0;                      
            int n_ifails = 0;           
            const int*  severities = 0;         
            const int*  ifails = 0;                 
            const char** texts = NULL;           
            EMH_ask_errors( &n_ifails, &severities, &ifails, &texts);               
            for( index=0; index<n_ifails; index++)                                     
                {                                                                          
                    printf("Error code - %d Error message - %s\n",ifails[index],texts[index]);					
                } 
           EMH_clear_errors();
        }
        return status;
}
/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =   NULL;
    char *sPasswordFile	     =   NULL;
    char *sGroup             =   NULL;
	char *sHelp              =   NULL;
	
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ));
	sUserName     = ITK_ask_cli_argument("-u=");
    sPasswordFile = ITK_ask_cli_argument("-pf=");
    sGroup        = ITK_ask_cli_argument("-g=");
	sHelp         = ITK_ask_cli_argument("-h=");
    itkApiCallStatus=ITK_APICALL(ITK_auto_login()); 
    ITK_APICALL(ITK_set_journalling(TRUE));
	if(sUserName==NULL && sPasswordFile==NULL && sGroup==NULL && sHelp!=NULL )
	{
		printf("\n  ************************************************************************************ -u  = username \n");
		printf("\n  ************************************************************************************ -pf = password \n");
		printf("\n  ************************************************************************************ -g  = group    \n");
	}
	else if(sUserName==NULL && sPasswordFile==NULL && sGroup==NULL && sHelp==NULL)
	{
		printf("\n************************************************************************************Logging failed please enter login id, password and group \n");	
	}
	else if(sUserName!=NULL && sPasswordFile!=NULL && sGroup!=NULL && sHelp==NULL)
	{
		if(itkApiCallStatus==ITK_ok)
        {
		    printf("\n************************************************************************************Login Successfull in TC \n");	
	    }
	   else{
		  printf("\n************************************************************************************nLogin failed in TC \n");	
	   }
	}
	else{
		printf("\n************************************************************************************n Required argument missing or logging failed \n");
	}
	return(ITK_ok);
}

