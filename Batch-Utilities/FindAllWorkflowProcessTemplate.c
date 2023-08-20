/*Objective - To find all available workflow process template in teamcenter using ITK Code. */
/* Api's used - 
                EPM_ask_all_process_templates() 
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
	char *objectName         =   NULL;
	
	int taskTemplatesCount   =   0;
	int i;
	int templateStage        =    2;
	
	
	tag_t loggedInUserTag    =  NULLTAG;
    tag_t *processTemplateTags    =  NULLTAG;
	
	FILE *flogPtr=fopen("FindAllWorkflowProcess.csv","w+");
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
				
				   /*0 = all templates regardless of stage
                     1 = under construction
                     2 = ready for use
                     3 = obsolete*/
	            fprintf(flogPtr,"ObjectString,ObjectType,ObjectName ");	 
				itkApiCallStatus=ITK_APICALL(EPM_ask_all_process_templates(templateStage,&taskTemplatesCount,&processTemplateTags),flogPtr);
				for(i=0;i<taskTemplatesCount;i++)
				{
					ITK_APICALL(AOM_UIF_ask_value(processTemplateTags[i],"object_string",&objectString),flogPtr);
				    ITK_APICALL(AOM_UIF_ask_value(processTemplateTags[i],"object_type",&objectType),flogPtr);
					ITK_APICALL(AOM_UIF_ask_value(processTemplateTags[i],"object_name",&objectName),flogPtr); 				
				    fprintf(flogPtr,"\n %s,%s,%s ",objectString,objectType,objectName);	 
				}	
			}
	}
	else
	{
		printf("\n************************************************************************************nLogin failed in TC \n");	
		fprintf(flogPtr,"\n************************************************************************************nLogin failed in TC \n");
	}
	return(ITK_ok);
}