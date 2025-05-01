/*Objective - To find all the Item of Design Revision type .*/
 
/*Required headers */
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
 
int ITK_APICALL(int x,FILE *Flogfile)
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
                fprintf(Flogfile,"Error code - %d Error message - %s\n",ifails[index],texts[index]);					
            } 
       EMH_clear_errors();
    }
    return status;
}
//Main function implementation.
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	          =   NULL;
    char *sPassword   	      =   NULL;
    char *sGroup              =   NULL;
	char *fromdate            =   NULL;
	char *todate              =   NULL;
	char *sHelp               =   NULL;
	char *loggedInUser        =   NULL;
    char *dataReleased        =   NULL;
	char *mainItemId          =   NULL;
	char *mainObjectName      =   NULL;
	char *makeBuy             =   NULL;  
	char *currentIdValue      =   NULL;
	char *itemRevobjectName   =   NULL;
	char *itemRevId1          =   NULL; 
	char *itemRevId           =   NULL; 
	char *partType            =   NULL; 
	char *releaseStatus       =   NULL; 
	char *partDescription     =   NULL; 
	char *releaseStatusName   =   NULL;
	char *ercDateReleased     =   (char *)MEM_alloc(500);
	char *aplDateReleased     =   (char *)MEM_alloc(500);
	char *tdesc               =   NULL;
	char *treleaseStatues     =   NULL;
	char *erc                 =   "ERC Released";
	char *apl                 =   "APLP Released";


	char *qEntry[4], *qValues[4];
	qEntry[0] = "Type";
	qEntry[1] = "Released After";
	qEntry[2] = "Released Before";
	qEntry[3] = "Release Status";
 
	
    int qEntryParametersCount = 4;	
    int qComponentsCount      = 0; 
	int i                     = 0;
	int releaseStatusCount    = 0;
	int j                     = 0;
	int k                     = 0;
	int l                     = 0;
	int revCount              = 0;
	int latestReleased        = 0;
	int s1=0;
	int present               = 0;
 
	tag_t loggedInUserTag     =  NULLTAG;
	tag_t queryTag            =  NULLTAG;
	tag_t *qComponentsTag     =  NULLTAG;
	tag_t *itemRevTags        =  NULLTAG;
	tag_t  itemTag            =  NULLTAG;
	tag_t *releaseStatusTags  =  NULLTAG;
    //File path details and opening mode.
	FILE *flogPtr=fopen("Incremental_Log.txt","w+");
	FILE *fCsvPtr=fopen("Incremental.csv","w+");
	fprintf(flogPtr,"Date : %s",__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,"Time : %s",__TIME__);
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);

	sUserName     = ITK_ask_cli_argument("-u=");
    sPassword     = ITK_ask_cli_argument("-p=");
    sGroup        = ITK_ask_cli_argument("-g=");
	fromdate      = ITK_ask_cli_argument("-fromdate=");
	todate        = ITK_ask_cli_argument("-todate=");
	char *fromDateUpdate            =   NULL;
	char *toDateUpdated             =   NULL;
	fromDateUpdate = (char *)MEM_alloc(100);
	tc_strcpy(fromDateUpdate,"");
	tc_strcpy(fromDateUpdate,fromdate);
	tc_strcat(fromDateUpdate," 00:00");
	toDateUpdated = (char *)MEM_alloc(100);
	tc_strcpy(toDateUpdated,"");
	tc_strcpy(toDateUpdated,todate);
	tc_strcat(toDateUpdated," 23:59");

	fprintf(flogPtr,"\n -u : %s",sUserName);
	fprintf(flogPtr,"\n -p : %s",sPassword);
	fprintf(flogPtr,"\n -g : %s",sGroup);
	fprintf(flogPtr,"\n -fromdate : %s",fromdate);
	fprintf(flogPtr,"\n -todate   : %s",todate);
	fprintf(flogPtr,"\n -fromDateUpdate  : %s",fromDateUpdate);
	fprintf(flogPtr,"\n -toDateUpdated   : %s",toDateUpdated);
	qValues[0] = "Design Revision";
	qValues[1] = fromDateUpdate;
	qValues[2] = toDateUpdated;
	//qValues[3] = "ERC Released";
	qValues[3] = "T5_LcsErcRlzd";
	itkApiCallStatus=ITK_APICALL(ITK_init_module(sUserName,sPassword,sGroup),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	if(itkApiCallStatus==ITK_ok)
    {
	    printf("*********************************Login successful to teamcenter*****************************************\n");
        printf("%s\n",qValues[0]);
	    printf("%s\n",qValues[1]);
	    printf("%s\n",qValues[2]);
		printf("%s\n",qValues[3]);
        fprintf(flogPtr,"\nLogin Successfull to TC");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
	    {
		    fprintf(flogPtr,"\nLogged in user : %s",loggedInUser);fflush(stdout);
		    itkApiCallStatus=ITK_APICALL(QRY_find2("General...",&queryTag),flogPtr);
	        if(itkApiCallStatus==0)
	            {
		            fprintf(flogPtr,"\nQuery found in TC-Database.");	
					fprintf(flogPtr,"\nTotal search results found before= %d \n ",qComponentsCount);fflush(stdout);
					itkApiCallStatus=ITK_APICALL(QRY_execute(queryTag,qEntryParametersCount,qEntry,qValues,&qComponentsCount,&qComponentsTag),flogPtr);
                    if(itkApiCallStatus==0)
					{
                        fprintf(flogPtr,"\nQuery execution successfull.\n");fflush(stdout);
						fprintf(flogPtr,"\nTotal search results found = %d \n ",qComponentsCount);fflush(stdout);
						printf("Total Object Count = %d\n",qComponentsCount);
						if(qComponentsCount>0)
						{
							fprintf(flogPtr,"\n ****************************************** \n ");
							fprintf(flogPtr,"\n Part no ,Revision Id,Release Status ,Object Description \n");
							fprintf(fCsvPtr,"PART NO,REVISION ID,ERC RELEASED DATE,APLP RELEASED DATE \n");
							for(i=0;i<qComponentsCount;i++)
						    {		
							    ITK_APICALL(AOM_ask_value_string(qComponentsTag[i],"item_id",&itemRevId1),flogPtr);
								ITK_APICALL(AOM_ask_value_string(qComponentsTag[i],"item_revision_id",&itemRevId),flogPtr);
								ITK_APICALL(AOM_ask_value_string(qComponentsTag[i],"release_status_list",&releaseStatus),flogPtr);
								ITK_APICALL(AOM_ask_value_string(qComponentsTag[i],"object_desc",&partDescription),flogPtr);
								ITK_APICALL(WSOM_ask_release_status_list(qComponentsTag[i],&releaseStatusCount,&releaseStatusTags),flogPtr);	
								
								fprintf(flogPtr,"%s,%s,%s,%s,%d",itemRevId1,itemRevId,releaseStatus,partDescription,releaseStatusCount);
								tc_strcpy(ercDateReleased,"");
								tc_strcpy(aplDateReleased,"");
								
								if(releaseStatusCount>0)
								{
									for(j=0;j<releaseStatusCount;j++)
									{
										tc_strcpy(ercDateReleased,"");
										tc_strcpy(aplDateReleased,"");
										
										ITK_APICALL(AOM_ask_value_string(releaseStatusTags[j],"object_name",&releaseStatusName),flogPtr);
									    ITK_APICALL(AOM_ask_value_string(releaseStatusTags[j],"date_released",&dataReleased),flogPtr);
									    if(tc_strcmp(releaseStatusName,"ERC Released")==0||tc_strcmp(releaseStatusName,"T5_LcsErcRlzd")==0)					
									    {
											ITK_APICALL(AOM_ask_value_string(releaseStatusTags[j],"date_released",&ercDateReleased),flogPtr);
									    }
										if(tc_strcmp(releaseStatusName,"APLP Released")==0||tc_strcmp(releaseStatusName,"T5_LcsAplpRlzd")==0)
									    {
											ITK_APICALL(AOM_ask_value_string(releaseStatusTags[j],"date_released",&aplDateReleased),flogPtr);
										}
										
									}
								}
								fprintf(fCsvPtr,"%s,%s,%s,%s\n",itemRevId1,itemRevId,ercDateReleased,aplDateReleased);
							}
						}
						else
						{
							fprintf(flogPtr,"\n Total Query Objects found are less than or equal to zero. "); 
						    printf("\n Total Query Objects found are less than or equal to zero.");
						}
					}
					else{
						fprintf(flogPtr,"\nQuery execution failed."); 
						printf("\n Query execution failed.");
					}
				}
			else
			{
				  fprintf(flogPtr,"\nQuery not found in TC-Database.");
				  printf("\nQuery not found in TC-Database.");
			}
		}
		else
		{
			fprintf(flogPtr,"\nLogged in user not found.");
			printf("\nLogged in user not found.");
		}				
	}
	else
	{
		printf("\n ***************************************Login failed in TC \n");	
		fprintf(flogPtr,"\nLogin failed in TC");
	}
	return(ITK_ok);
}

