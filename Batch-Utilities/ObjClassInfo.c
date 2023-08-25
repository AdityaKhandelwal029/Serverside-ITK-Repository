/*Objective - ITK Utility to get information of class and superclasses of a business object with given itemid as command line argument as input. */

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
	char *objType            =   NULL;
	char **propName;
	
	int i                    =  0;
	int superClasscount      =  -1;
	
	tag_t loggedInUserTag    =  NULLTAG;
	tag_t itemTag            =  NULLTAG;
	tag_t classTag           =  NULLTAG;
	tag_t *superClassIds;
	
	FILE *flogPtr=fopen("ObjClassInfo.txt","w+");
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
				if(itemTag!=NULLTAG)
				{
					fprintf(flogPtr,"\nItem with '%s' itemId found. ",itemId);
					printf("\nItem with '%s' itemId found \n",itemId);
					fprintf(flogPtr,"\n******************************************************************");
					ITK_APICALL(POM_class_of_instance(itemTag,&classTag),flogPtr);
					ITK_APICALL(POM_name_of_class(classTag,&className),flogPtr);
					fprintf(flogPtr,"\n          Class = %s",className);			
 
				    while(superClasscount!=0)
					{
						itkApiCallStatus=ITK_APICALL(POM_superclasses_of_class(classTag,&superClasscount,&superClassIds),flogPtr);
					    for(i=0;i<superClasscount;i++)
					    {
						    ITK_APICALL(POM_name_of_class(superClassIds[i],&className),flogPtr);
					     	fprintf(flogPtr,"\n %d.) SuperClass = %s",i+1,className);	
							classTag=superClassIds[i];
				    	}
					}
					ITK_APICALL(AOM_UIF_ask_value(itemTag,"object_type",&propValue),flogPtr);	
					fprintf(flogPtr,"\n \n Object_type = %s",propValue);
				}
				else
				{
					fprintf(flogPtr,"\nItem with '%s' itemId not found. ",itemId);
					printf("\nItem with '%s' itemId not found \n",itemId);
					fprintf(flogPtr,"\n******************************************************************");
				}
			}
	    }
	   else{
		  printf("\n************************************************************************************nLogin failed in TC \n");	
		  fprintf(flogPtr,"\n************************************************************************************nLogin failed in TC \n");
	   }
	return(ITK_ok);
}
