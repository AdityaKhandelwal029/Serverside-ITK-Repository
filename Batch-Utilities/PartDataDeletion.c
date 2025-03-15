/* Objective :- Login Test */

/*Required headers */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/custom.h>
#include <tccore/workspaceobject.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <bom/bom.h>
#include <res/res_itk.h>
#include <ctype.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>
#include <time.h>

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
            fprintf(Flogfile,"Error code - [%d]. Error message - %s\n",ifails[index],texts[index]);	
        } 
        EMH_clear_errors();
    }
    return status;
}

/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =   NULL;
    char *sPasswordFile      =   NULL;
    char *sGroup             =   NULL;
	char *sHelp              =   NULL;
	char *loggedInUser       =   NULL;
	char *fItemId            =   NULL;
	char *fSequence          =   NULL;
	char *fDescription       =   NULL;
	char *fCreationDate      =   NULL;
	char *fMxPlus            =   NULL;
	char *fTopMxplus         =   NULL;
	char *fComment           =   NULL;
	char *TItemId            =   NULL;
	char *TItemRevId         =   NULL;	
	char *refItemId          =   NULL;
	char *refObjStr          =   NULL;
	char *refObjName         =   NULL;
	char *refObjType         =   NULL;
	char **refRelation       =   NULL;
	
	
	char filedata[500];
	
	int  itkApiCallStatus  = 0;
	int  count             = 0;
	int *TSequence         = 0;
	int referencers        = 0;
	int *refLevel          = NULL;
	int i = 0;
	
	
	tag_t loggedInUserTag =  NULLTAG;
	tag_t itemTag         = NULLTAG;
	tag_t itemRevTag      = NULLTAG;
	tag_t *referencersTag = NULLTAG;
	
	
	FILE *fptr=fopen("PartInputs.csv","r");
	FILE *flogPtr=fopen("PartDataDeletionLog_1.txt","w+");
	FILE *fOutPtr=fopen("PartDeletionOutput_1.csv","w+");
	
	fprintf(flogPtr,__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,__TIME__);
	
	sUserName     = ITK_ask_cli_argument("-u=");
	sPasswordFile = ITK_ask_cli_argument("-pf=");
	sGroup        = ITK_ask_cli_argument("-g=");
	
	
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	if(itkApiCallStatus==ITK_ok)
    {
		printf("\n Login successful to teamcenter \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
		{
		    fprintf(flogPtr,"\nName of the logged in user = %s",loggedInUser);	
			if(fptr!=NULL)
		    {
			    fprintf(flogPtr,"\nFile found and successfully opened ...\n");
				fprintf(fOutPtr,"ItemId,Status,ItemRevId,Referencers,Ref Level\n");
				while(fgets(filedata, 500, fptr) != NULL) 
                {
				    fprintf(flogPtr,"*************** FileLine = %s \n",filedata);
					printf("*************** FileLine = %s \n",filedata);
					
					if(count==1)
					{
						fItemId       = strtok(filedata,",");
				        fSequence     = strtok(NULL,",");
				        fDescription  = strtok(NULL,",");
				        fCreationDate = strtok(NULL,",");
				        fMxPlus       = strtok(NULL,",");
				        fTopMxplus    = strtok(NULL,",");
				        fComment      = strtok(NULL,",");
				        
				        fprintf(flogPtr,"fItemId       - %s \n",fItemId      );
				        fprintf(flogPtr,"fSequence     - %s \n",fSequence    );
				        fprintf(flogPtr,"fDescription  - %s \n",fDescription );
				        fprintf(flogPtr,"fCreationDate - %s \n",fCreationDate);
				        fprintf(flogPtr,"fMxPlus       - %s \n",fMxPlus      );
				        fprintf(flogPtr,"fTopMxplus    - %s \n",fTopMxplus   );
				        fprintf(flogPtr,"fComment      - %s \n",fComment     );
						
						itkApiCallStatus=ITK_APICALL(ITEM_find_item(fItemId,&itemTag),flogPtr);
						if(itkApiCallStatus==0 && itemTag!=NULLTAG)
						{
							fprintf(flogPtr,"Item found in TC \n",fItemId);
							itkApiCallStatus=ITK_APICALL(ITEM_ask_latest_rev(itemTag,&itemRevTag),flogPtr);
							if(itkApiCallStatus==0 && itemRevTag!=NULLTAG)
							{
								fprintf(flogPtr,"ItemRev found in TC \n",fItemId);
								ITK_APICALL(AOM_ask_value_string(itemRevTag,"item_id",&TItemId    ),flogPtr);
								ITK_APICALL(AOM_ask_value_string(itemRevTag,"item_revision_id",&TItemRevId ),flogPtr);								
								ITK_APICALL(WSOM_where_referenced(itemRevTag,1,&referencers,&refLevel,&referencersTag,&refRelation ),flogPtr);
								
								fprintf(flogPtr,"TItemId     - %s \n", TItemId    );
								fprintf(flogPtr,"TItemRevId  - %s \n", TItemRevId );
								fprintf(flogPtr,"referencers - %d \n", referencers   );
								fprintf(flogPtr,"refLevel    - %d \n", *refLevel   );	
								for(i=0;i<referencers;i++)
								{
									ITK_APICALL(AOM_ask_value_string(referencersTag[i],"item_id",&refItemId ),flogPtr);
									ITK_APICALL(AOM_ask_value_string(referencersTag[i],"object_string",&refObjStr ),flogPtr);
									ITK_APICALL(AOM_ask_value_string(referencersTag[i],"object_name",&refObjName),flogPtr);
									ITK_APICALL(AOM_ask_value_string(referencersTag[i],"object_type",&refObjType),flogPtr);
									
									
									fprintf(flogPtr,"refItemId  - %s \n",refItemId );
									fprintf(flogPtr,"refObjStr  - %s \n",refObjStr );
									fprintf(flogPtr,"refObjName - %s \n",refObjName);
									fprintf(flogPtr,"refObjType - %s \n",refObjType);
									
								}
						
								fprintf(fOutPtr,"%s,Found,%s,%d,%d\n",TItemId,TItemRevId,referencers,*refLevel);								
							}
							else
							{
								fprintf(flogPtr,"Error : ItemRev not found in TC \n",fItemId);
							}															
						}
						else
						{
							fprintf(flogPtr,"Error : Item with %s id not found in TC \n",fItemId);
							fprintf(fOutPtr,"%s,Not found,,\n",fItemId);
						}											
				    }
					else
					{
						count=1;
					}					
		    	}
			}
			else
			{
				fprintf(flogPtr,"\n File not found or Error \n");
			}
	    }	   
	}
	else
	{
	  printf("\n Login failed in TC \n");	
	  fprintf(flogPtr,"\n Login failed in TC \n");
	}
	return(ITK_ok);
}