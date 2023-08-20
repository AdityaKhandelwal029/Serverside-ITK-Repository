/*Objective - To test working of dates in Teamcenter using ITK.
   Api's used -
                DATE_default_date_format()
				DATE_get_internal_date_string_format()
				DATE_convert_format_date_to_date_t ()
				POM_compare_dates ()
 */

/* Req  -u= username,-p=password -g=group details as command line arguments for successfull login in teamcenter. */


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
#include <fclasses/tc_date.h>

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
	char *sUserName	         =   "mukteshg.ttl";
    char *sPassword 	     =   "mkg123";
    char *sGroup             =   "PLM_Support_Group";
	char *loggedInUser       =   NULL; 
	char *lastModifiedDate   =   NULL;
	char *creationDate       =   NULL;
	char *dateStr            =   NULL;
	char *dateFormatStr      =   NULL;
	
	int conversionStatus     = 1000;
	
	tag_t loggedInUserTag    =  NULLTAG;
	tag_t itemTag            =  NULLTAG;
	
	date_t  creationDateVariable;
	date_t  lastModifiedDateVariable;
	
	FILE *flogPtr=fopen("/user/testcmi/Dev/devgroups/Aditya/testfolder/Datetest1.txt","w+");
	
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	//sUserName    = ITK_ask_cli_argument("-u=");
    //sPassword    = ITK_ask_cli_argument("-p=");
    //sPassword    = ITK_ask_cli_argument("-g=");
	
	
	fprintf(flogPtr,__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,__TIME__);
	fprintf(flogPtr,"\n -u : %s",sUserName);
	fprintf(flogPtr,"\n -p : %s",sPassword);
	fprintf(flogPtr,"\n -g : %s",sGroup);
	
	itkApiCallStatus=ITK_APICALL(ITK_init_module(sUserName,sPassword,sGroup),flogPtr); 
	
	if(itkApiCallStatus==ITK_ok)
        {
		printf("**************************************************************************Login successful to teamcenter \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC \n");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	        if(itkApiCallStatus==ITK_ok)
			    {
				    fprintf(flogPtr,"\nName of the logged in user = %s \n",loggedInUser);
					ITK_APICALL(ITEM_find_item("008279",&itemTag),flogPtr);
					fprintf(flogPtr,"\n Item with given id-008279 found. ");
					ITK_APICALL(AOM_UIF_ask_value(itemTag,"last_mod_date",&lastModifiedDate),flogPtr);
					ITK_APICALL(AOM_UIF_ask_value(itemTag,"creation_date",&creationDate),flogPtr);
					fprintf(flogPtr,"\n -Last modified date =  : %s",lastModifiedDate);
					fprintf(flogPtr,"\n -Creation date      = : %s",creationDate);   
					
                    ITK_APICALL(DATE_default_date_format(&dateStr),flogPtr);
                    ITK_APICALL(DATE_get_internal_date_string_format(&dateFormatStr),flogPtr);
					
					fprintf(flogPtr,"\n -dateStr       =  : %s",dateStr);
					fprintf(flogPtr,"\n -dateFormatStr =  : %s",dateFormatStr);
					
					ITK_APICALL(DATE_convert_format_date_to_date_t(creationDate,dateFormatStr,&creationDateVariable),flogPtr);
					ITK_APICALL(DATE_convert_format_date_to_date_t(lastModifiedDate,dateFormatStr,&lastModifiedDateVariable),flogPtr);
					
					
					ITK_APICALL(POM_compare_dates(creationDateVariable,lastModifiedDateVariable,&conversionStatus),flogPtr);
					fprintf(flogPtr,"\n -conversionStatus 1 =  : %d",conversionStatus);
					
					ITK_APICALL(POM_compare_dates(lastModifiedDateVariable,creationDateVariable,&conversionStatus),flogPtr);
					fprintf(flogPtr,"\n -conversionStatus 2 =  : %d",conversionStatus);
					
					ITK_APICALL(POM_compare_dates(lastModifiedDateVariable,lastModifiedDateVariable,&conversionStatus),flogPtr);
					fprintf(flogPtr,"\n -conversionStatus 3 =  : %d",conversionStatus);
					
					ITK_APICALL(POM_compare_dates(creationDateVariable,creationDateVariable,&conversionStatus),flogPtr);
					fprintf(flogPtr,"\n -conversionStatus 4 =  : %d",conversionStatus);
			    }
	    }
	   else{
		  printf("\n************************************************************************************nLogin failed in TC \n");	
		  fprintf(flogPtr,"\n************************************************************************************nLogin failed in TC \n");
	   }
	return(ITK_ok);
}
