/*Objective - Creation of Requirement type (Item) in teamcenter database through ITK Utility by reading information from a give file input.*/

/*File input format */
/*ID	TYPE	ITEMREVISIONID	NAME
R1-ABCD	Requirement	AK	T1-IT1
R2-ABCD	Requirement	AK	T2-IT2
R3-ABCD	Requirement	AK	T3-IT3
R4-ABCD	Requirement	AK	T4-IT4
R5-ABCD	Requirement	AK	T5-IT5
*/

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

#define ITK_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
#define CALLAPI(expr)ITKCALL(ifail = expr); if(ifail != ITK_ok) {  return ifail;}

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
		printf("%3d error(s)\n", n_ifails);fflush(stdout);
		for( index=0; index<n_ifails; index++)
		{
			printf("ERROR: %d\tERROR MSG: %s\n", ifails[index], texts[index]);fflush(stdout);
			TC_write_syslog("ERROR: %d\tERROR MSG: %s\n", ifails[index], texts[index]);
			printf ("FUNCTION: %s\nFILE: %s LINE: %d\n\n", function, file, line);fflush(stdout);
			TC_write_syslog("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
		}
		EMH_clear_errors();

	}
	return return_code;
}

int ITK_user_main (int argc, char ** argv )
{
  
    char *sUserName	=   NULL;
    char *sPasswordFile	=   NULL;
    char *sGroup      =   NULL;
    char *sFilepath   =   NULL; 
    char *itemId=NULL; 
    char *itemType=NULL; 
    char *itemRevId=NULL; 
    char *itemName=NULL;
    char filedata[500];
    int apiStatus;
    int count=0;
    
    
    FILE *fPtr;
    tag_t item_t;
    tag_t itemRev_t;
    printf("\n Auto login1 ");fflush(stdout);
    ITK_CALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ));
    printf("\n Auto login2 ");
    fflush(stdout);
	apiStatus = ITK_auto_login();
	printf("\n Auto login ");fflush(stdout);
	ITK_CALL(ITK_set_journalling( TRUE ));
	printf("\n Auto login .......\n");fflush(stdout);

	sUserName     = ITK_ask_cli_argument("-u=");
    sPasswordFile = ITK_ask_cli_argument("-pf=");
    sGroup        = ITK_ask_cli_argument("-g=");
   if(apiStatus==ITK_ok)
   {
    printf("Login Successfull...\n");  
    fPtr=fopen("/user/testcmi/Dev/devgroups/Aditya/ReqObjCreation.csv","r"); 
    if(fPtr!=NULL)
    {
        printf("Data file opened successfully...\n");
        while (fgets(filedata, 500, fPtr) != NULL) 
        {
            printf("%s\n", filedata);
            if(count>0)
            {
                itemId=strtok(filedata,",");
                itemType=strtok(NULL,",");
                itemRevId=strtok(NULL,",");
                itemName=strtok(NULL,",");
                
                printf("itemId - %s \n",itemId); 
                printf("itemType - %s\n",itemType); 
                printf("itemRevId - %s\n",itemRevId); 
                printf("itemName - %s\n",itemName); 
                printf(" ******************** ");
                apiStatus=ITEM_create_item(itemId,itemName,itemType,itemRevId,&item_t,&itemRev_t); 
                if(apiStatus==ITK_ok)
                {
                    printf(" #################################Item with %s name created successfully###########\n",itemName);
                    apiStatus=ITEM_save_item(item_t);
                    if(apiStatus==ITK_ok)
                    {
                        printf("*********Item save successfully***********\n"); 
                    }
                    else
					{
                        printf(" Item saving failed\n");
                    }
                }
                else
                {
                    printf("Item creation failed...\n");   
                    char* Error_msg = NULL;                              
                    printf("\napiStatus: %d",apiStatus);                      
	                if( apiStatus != ITK_ok)                                 
	                {                                                  
	 	                 EMH_ask_error_text(apiStatus, &Error_msg);            
		                 printf("\nError: %s", Error_msg);                  
                    }
                } 
            }
            count++;
        }
    }
    else
    {
        printf("Data file opening failed...\n");
    }
  }
  else
  {
    printf("Login failed...\n");                                            
    char* Error_msg = NULL;                              
    printf("\napiStatus: %d",apiStatus);                      
	 if( apiStatus != ITK_ok)                                 
		{                                                  
	 	     EMH_ask_error_text(apiStatus, &Error_msg);            
		     printf("\nError: %s", Error_msg);                  
        }
  }
 return ITK_ok;
}

       



