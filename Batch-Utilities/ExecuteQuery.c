/*Objective - ITK Utility to query business Object of "Design" type from given date to given date and stored the query result in a file with csv format .*/

//Required headers 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
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
#include <qry/qry.h>
/*Global variable Declarations */

int itkApiCallStatus = -1;

//Custom API error handling function. 

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
//Main function implementation.
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	                =   NULL;
    char *sPassword   	            =   NULL;
    char *sGroup                    =   NULL;
	char *fromdate                  =   NULL;
	char *fromDateUpdate            =   NULL;
	char *todate                    =   NULL;
	char *toDateUpdated             =   NULL;
	char *sHelp                     =   NULL;
	char *loggedInUser              =   NULL; 
	char *itemIdValue               =   NULL;
	char *itemobjectNameValue       =   NULL;
	char *itemCreationDateValue     =   NULL;
	char *itemLastModifiedDateValue =   NULL;
	
	char *qEntry[3], *qValues[3];
	qEntry[0] = "Type";
	qEntry[1] = "Created After";
	qEntry[2] = "Created Before";
	
	
    
    int qEntryParametersCount = 3;	
    int qComponentsCount      = 0; 
	int i                     = 0;
	int itemRevCount          = 0;

	tag_t loggedInUserTag    =  NULLTAG;
	tag_t queryTag           =  NULLTAG;
	tag_t *qComponentsTag    =  NULLTAG;
	tag_t *itemRevTags       =  NULLTAG;
	
//File path details and opening mode.
	
	FILE *flogPtr=fopen("LWKQueryInfoDesignRevdetails.csv","w+");
	fprintf(flogPtr,"Date : %s",__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,"Time : %s",__TIME__);
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    
	
	sUserName     = ITK_ask_cli_argument("-u=");
    sPassword     = ITK_ask_cli_argument("-p=");
    sGroup        = ITK_ask_cli_argument("-g=");
	fromdate      = ITK_ask_cli_argument("-fromdate=");
	todate        = ITK_ask_cli_argument("-todate=");
	
	fromDateUpdate = (char *)MEM_alloc(20);
	tc_strcpy(fromDateUpdate,"");
	tc_strcpy(fromDateUpdate,fromdate);
	tc_strcat(fromDateUpdate," 13:00");
	
	toDateUpdated = (char *)MEM_alloc(20);
	tc_strcpy(toDateUpdated,"");
	tc_strcpy(toDateUpdated,todate);
	tc_strcat(toDateUpdated," 23:59");
	
	fprintf(flogPtr,"\n -u : %s",sUserName);
	fprintf(flogPtr,"\n -p : %s",sPassword);
	fprintf(flogPtr,"\n -g : %s",sGroup);
	fprintf(flogPtr,"\n -fromdate : %s",fromdate);
	fprintf(flogPtr,"\n -fromDateUpdate   : %s",fromDateUpdate);
	fprintf(flogPtr,"\n -todate   : %s",todate);
	fprintf(flogPtr,"\n -toDateUpdated   : %s",toDateUpdated);
	
	
	
	qValues[0] = "Design";
	qValues[1] = fromDateUpdate;
	qValues[2] = toDateUpdated;
	
	
	itkApiCallStatus=ITK_APICALL(ITK_init_module(sUserName,sPassword,sGroup),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	if(itkApiCallStatus==ITK_ok)
        {
		    printf("*********************************Login successful to teamcenter*****************************************\n");	
            fprintf(flogPtr,"\nLogin Successfull to TC");	
            itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	        if(itkApiCallStatus==ITK_ok)
		        {
				    fprintf(flogPtr,"\nLogged in user : %s",loggedInUser);fflush(stdout);
				    itkApiCallStatus=ITK_APICALL(QRY_find2("General...",&queryTag),flogPtr);
		            if(itkApiCallStatus==0)
		                {
			                fprintf(flogPtr,"\nQuery found in TC-Database.");	
							itkApiCallStatus=ITK_APICALL(QRY_execute(queryTag,qEntryParametersCount,qEntry,qValues,&qComponentsCount,&qComponentsTag),flogPtr);
                            if(itkApiCallStatus==0)
							{
                                fprintf(flogPtr,"\nQuery execution successfull.\n");fflush(stdout);
								fprintf(flogPtr,"\n Total search results found = %d \n ",qComponentsCount);fflush(stdout);
								if(qComponentsCount>0)
								{
									fprintf(flogPtr,"ItemId,ItemName,Revcount");fflush(stdout);
									for(i=0;i<qComponentsCount;i++)
								    {											
								  	    ITK_APICALL(AOM_UIF_ask_value(qComponentsTag[i],"item_id",&itemIdValue),flogPtr);
									    ITK_APICALL(AOM_UIF_ask_value(qComponentsTag[i],"object_name",&itemobjectNameValue),flogPtr);
									    ITK_APICALL(AOM_UIF_ask_value(qComponentsTag[i],"creation_date",&itemCreationDateValue),flogPtr);
										ITK_APICALL(AOM_UIF_ask_value(qComponentsTag[i],"last_mod_date",&itemLastModifiedDateValue),flogPtr);									
									    ITK_APICALL(ITEM_list_all_revs(qComponentsTag[i],&itemRevCount,&itemRevTags),flogPtr);
										fprintf(flogPtr,"\n %s, %s, %d ,%s ,%s ",itemIdValue,itemobjectNameValue,itemRevCount,itemCreationDateValue,itemLastModifiedDateValue);								
								    }
								}	
							}							
                            else
 							{
								fprintf(flogPtr,"\nQuery execution failed."); 
							}								
		                }
		            else{
			            fprintf(flogPtr,"\nQuery not found in TC-Database.");
		            }
			    }
	 	    else
			    {
			        fprintf(flogPtr,"\nLogged in user not found.");
		        } 
	    }
	   else{
		  printf("\n************************************************************************************nLogin failed in TC \n");	
		  fprintf(flogPtr,"\nLogin failed in TC");
	   }
	return(ITK_ok);
}
