/*Objective - To find workflow process template using ITK Code. */
/*  
  Api's used -
               EPM_find_process_template()  
*/

/*Required headers */
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
#include <epm/epm_task_template_itk.h>

/*Declarations */

int itkApiCallStatus = -1;

/*Custom API error handling function */

int ITK_APICALL(int x,FILE *Flogfile){      
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
                    fprintf(Flogfile,"Error code - %d Error message - %s\n",ifails[index],texts[index]);					
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
	char *loggedInUser       =   NULL; 
	char *objectType         =   NULL; 
	char *objectString       =   NULL; 
	
	tag_t loggedInUserTag    =  NULLTAG;
    tag_t processTemplate    =  NULLTAG;
	
	FILE *flogPtr=fopen("/user/testcmi/Dev/devgroups/Aditya/testfolder/FindWorkflowProcess.txt","w+");
	fprintf(flogPtr,__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,__TIME__);
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);

	if(itkApiCallStatus==ITK_ok){
			
            fprintf(flogPtr,"\nLogin Successfull to TC \n");	
            itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
			if(itkApiCallStatus==ITK_ok)
			{
				fprintf(flogPtr,"\nName of the logged in user = %s \n",loggedInUser);	
				itkApiCallStatus=ITK_APICALL(EPM_find_process_template("APL Working",&processTemplate),flogPtr);
				fprintf(flogPtr,"\n  itkApiCallStatus = %d \n",itkApiCallStatus);	
				ITK_APICALL(AOM_UIF_ask_value(processTemplate,"object_string",&objectString),flogPtr);
				ITK_APICALL(AOM_UIF_ask_value(processTemplate,"object_type",&objectType),flogPtr);
				
				fprintf(flogPtr,"\n  objectString = %s \n",objectString);	 
				fprintf(flogPtr,"\n    objectType = %s \n",objectType);	
			}
	}
	else
	{
		printf("\n************************************************************************************nLogin failed in TC \n");	
		fprintf(flogPtr,"\n************************************************************************************nLogin failed in TC \n");
	}
	return(ITK_ok);
}