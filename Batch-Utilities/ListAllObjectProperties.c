/*Objective - ITK Utility to get all the properties of business object with given item id*/

/* In this case of auto login with out parameters . testcmi will be the logged in user.*/

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
	char *itemId             =   NULL;
	char *loggedInUser       =   NULL; 
	char *propValue          =   NULL;  
    char *className          =   NULL;	
	char **propName;
	
	int propCount            =  0;
	int i                    =  0;
	
	tag_t loggedInUserTag    =  NULLTAG;
	tag_t itemTag            =  NULLTAG;
	tag_t classTag           =  NULLTAG;
	
	FILE *flogPtr=fopen("ObjAllPropList.txt","w+");
	fprintf(flogPtr,__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,__TIME__);
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	itemId = ITK_ask_cli_argument("-itemid=");
	
	if(itkApiCallStatus==ITK_ok)
        {
		printf("**************************************************************************Login successful to teamcenter \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC \n");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
			{
				fprintf(flogPtr,"\nName of the logged in user = %s",loggedInUser);	
				fprintf(flogPtr,"\n                   ItemId  = %s",itemId);
				fprintf(flogPtr,"\n******************************************************************");
				
				itkApiCallStatus=ITK_APICALL(ITEM_find_item(itemId,&itemTag),flogPtr);
				if(itkApiCallStatus==0)
				{
					fprintf(flogPtr,"\nItem with '%s' itemId found ",itemId);
					fprintf(flogPtr,"\n******************************************************************");
					ITK_APICALL(POM_class_of_instance(itemTag,&classTag),flogPtr);
					ITK_APICALL(POM_name_of_class(classTag,&className),flogPtr);
					fprintf(flogPtr,"\n Class = %s",className);				
					ITK_APICALL(AOM_ask_prop_names(itemTag, &propCount, &propName),flogPtr);
					for(i=0;i<propCount;i++)
					{
					    ITK_APICALL(AOM_UIF_ask_value(itemTag,propName[i],&propValue),flogPtr);	
						fprintf(flogPtr,"\n %s = %s",propName[i],propValue);	
					}
				}
				else
				{
					fprintf(flogPtr,"\nItem with '%s' itemId not found ",itemId);
				}
			}
	    }
	   else{
		  printf("\n************************************************************************************nLogin failed in TC \n");	
		  fprintf(flogPtr,"\n************************************************************************************nLogin failed in TC \n");
	   }
	return(ITK_ok);
}
