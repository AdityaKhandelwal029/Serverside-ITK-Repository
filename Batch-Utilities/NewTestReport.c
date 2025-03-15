/* Objective :- Login Test */

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
#include <bom/bom.h>
#include <res/res_itk.h>
#include <ctype.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>
#include <time.h>
int dr3AndAbove = 0; 
int dr4AndAbove = 0;
int reportAttached  = 0;
int gCount = 0 ;
int gConCount = 0 ;
int totalYesCount = 0;
int totalNocount = 0 ;
int withDR3AndDataset = 0;
int withDR4AndDataset = 0; 
int cvprodFound = 0;
int uaprodFound = 0;
int totalContainerCount =0;

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
int BOM_ASK_CHILD(int childCount, tag_t *bomchdTags,FILE *Flogfile,FILE *csvPtrLog)
{	
    fprintf(Flogfile,"\n  ############################# Inside BOM_ASK_CHILD function #############################\n"); 
	
	int itr;
	int subChildCount;
	int secObjCount = 0;
	int i=0;
	int found =0 ;
	int j=0;
	int apiStatus = 0;
	int levelStructure;
	int g_level = 0;
	int bomChildCount=0;
	
	tag_t realObjTag        = NULLTAG;
	tag_t lineObj           = NULLTAG;
	tag_t lineObj2          = NULLTAG;
	tag_t relationTag       = NULLTAG;
	tag_t *secObjTag        = NULLTAG;
	tag_t windowTag         =  NULLTAG;
	tag_t topBomLineTag     =  NULLTAG;
	tag_t revRule           =  NULLTAG;
	tag_t *bomChildTags     = NULLTAG;
	tag_t *subChildTags     = NULLTAG;
	
	char *itemId         = NULL;
	char *objName        = NULL;
	char *partType       = NULL;
	char *partStatus     = NULL;
	char *objType        = NULL;
	char *eePartType     = NULL;
	char *revIdSequesnce = NULL;
	char *objDesc        = NULL;
	char *ecuType        = NULL;
	char *secObjType     = NULL;
	char *reportFound =(char*) MEM_alloc(500*sizeof(char));
	char *itemName     = NULL; 
	
	char *g_itemId         = NULL;
	char *g_objName        = NULL;
	char *g_partType       = NULL;
	char *g_partStatus     = NULL;
	char *g_objType        = NULL;
	char *g_eePartType     = NULL;
	char *g_revIdSequesnce = NULL;
	char *g_objDesc        = NULL;
	char *g_ecuType        = NULL;
	
	char *t_ecuPartType = (char*) MEM_alloc(500*sizeof(char));
	char *t_objDesc     = (char*) MEM_alloc(500*sizeof(char));
	
	
	for(itr=0;itr<childCount;itr++)
	{
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_item_item_id",&itemId),Flogfile);
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_item_object_name",&objName),Flogfile);		
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_T5_EE_PartRevision_t5_SwPartType",&partType),Flogfile);
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_Design Revision_t5_PartStatus",&partStatus),Flogfile);
		ITK_APICALL(AOM_ask_value_int   (bomchdTags[itr],"bl_level_starting_0",&levelStructure),Flogfile);
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"fnd0bl_line_object_type",&objType),Flogfile);
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_T5_EE_PartRevision_t5_EE_PartType",&eePartType),Flogfile);
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"awb0BomLineRevId",&revIdSequesnce),Flogfile);
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_item_object_desc",&objDesc),Flogfile);
		ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_T5_EE_PartRevision_t5_EcuType",&ecuType),Flogfile);
		ITK_APICALL(AOM_ask_value_tag(bomchdTags[itr],"bl_line_object",&lineObj),Flogfile);	
			
		fprintf(Flogfile," - bl_itemId              = %s\n",itemId     );
		fprintf(Flogfile," - bl_objName             = %s\n",objName    );
		fprintf(Flogfile," - bl_partType            = %s\n",partType   );
		fprintf(Flogfile," - bl_partStatus          = %s\n",partStatus );
		fprintf(Flogfile," - bl_level_starting_0    = %d\n",levelStructure);
		fprintf(Flogfile," - bl_objType             = %s\n",objType );
		fprintf(Flogfile," - bl_eePartType          = %s\n",eePartType );
		fprintf(Flogfile," - bl_revIdSequesnce      = %s\n",revIdSequesnce );
		fprintf(Flogfile," - bl_object_desc         = %s\n",objDesc );
		fprintf(Flogfile," - bl_t5_EcuType          = %s\n",ecuType );
			
		ITK_APICALL(BOM_line_ask_all_child_lines(bomchdTags[itr],&subChildCount,&subChildTags),Flogfile);									
		fprintf(Flogfile," - subChildCount in BOM_ASK_CHILD = %d\n",subChildCount);
				
		if(tc_strcmp(eePartType,"G")==0)
		{
			ITK_APICALL(BOM_create_window(&windowTag),Flogfile);
			ITK_APICALL(CFM_find("ERC release and above",&revRule),Flogfile);		
			ITK_APICALL(BOM_set_window_config_rule(windowTag,revRule),Flogfile);
		    ITK_APICALL(BOM_set_window_pack_all(windowTag,true),Flogfile);
			ITK_APICALL(BOM_set_window_top_line(windowTag,NULLTAG,lineObj,NULLTAG,&topBomLineTag),Flogfile);
			ITK_APICALL(BOM_line_ask_all_child_lines(topBomLineTag,&bomChildCount,&bomChildTags),Flogfile);
			
			fprintf(Flogfile,"\n ******************** Executing condition for G ******************** \n"); 
			fprintf(Flogfile," Total bomChildCount in GroupId = %d \n",bomChildCount);
			for(j=0;j<bomChildCount;j++)
			{
				
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"bl_item_item_id",                        &g_itemId          ),Flogfile);
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"bl_item_object_name",                    &g_objName         ),Flogfile);		
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"bl_T5_EE_PartRevision_t5_SwPartType",    &g_partType        ),Flogfile);
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"bl_Design Revision_t5_PartStatus",       &g_partStatus      ),Flogfile);
                ITK_APICALL(AOM_ask_value_int   (bomChildTags[j],"bl_level_starting_0",                    &g_level           ),Flogfile);
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"fnd0bl_line_object_type",                &g_objType         ),Flogfile);
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"bl_T5_EE_PartRevision_t5_EE_PartType",   &g_eePartType      ),Flogfile);
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"awb0BomLineRevId",                       &g_revIdSequesnce  ),Flogfile);
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"bl_item_object_desc",                    &g_objDesc         ),Flogfile);
                ITK_APICALL(AOM_ask_value_string(bomChildTags[j],"bl_T5_EE_PartRevision_t5_EcuType",       &g_ecuType         ),Flogfile);
                ITK_APICALL(AOM_ask_value_tag(bomChildTags[j],"bl_line_object",                            &lineObj2          ),Flogfile);
   
			    fprintf(Flogfile," - bl_g_itemId            = %s\n",g_itemId         );
		        fprintf(Flogfile," - bl_g_objName           = %s\n",g_objName        );
		        fprintf(Flogfile," - bl_g_partType          = %s\n",g_partType       );
		        fprintf(Flogfile," - bl_g_partStatus        = %s\n",g_partStatus     );
		        fprintf(Flogfile," - bl_g_level             = %d\n",g_level          );
		        fprintf(Flogfile," - bl_g_objType           = %s\n",g_objType        );
		        fprintf(Flogfile," - bl_g_eePartType        = %s\n",g_eePartType     );
		        fprintf(Flogfile," - bl_g_revIdSequesnce    = %s\n",g_revIdSequesnce );
		        fprintf(Flogfile," - bl_g_objDesc           = %s\n",g_objDesc        );
		        fprintf(Flogfile," - bl_g_ecuType           = %s\n",g_ecuType        );
				
				if(tc_strcmp(g_objType,"T5_EE_PartRevision")==0 && tc_strcmp(g_partType,"CON")==0)
				{
					fprintf(Flogfile,"\n ################# CONTAINER FOUND IN GroupID ################# \n");
					if(gConCount==0)
					{
						totalContainerCount++;
						gConCount=1;
						gCount++;
						ITK_APICALL(GRM_find_relation_type("IMAN_specification",&relationTag),Flogfile);	
					    ITK_APICALL(GRM_list_secondary_objects_only(lineObj2,relationTag,&secObjCount,&secObjTag),Flogfile);
					    i=0;
					    found =0 ;
					    tc_strcpy(reportFound,"");
					    if(secObjCount>0)
					    {
					    	for(i=0;i<secObjCount;i++)
					    	{
					    		ITK_APICALL(AOM_ask_value_string(secObjTag[i],"object_type",&secObjType),Flogfile);
					    		if(tc_strcmp(secObjType,"PDF")==0||tc_strcmp(secObjType,"DVP")==0)
					    		{
					    			found = 1;
					    		}
					    	}
					    }
						fprintf(Flogfile, "\n Value of found = %d ",found);
						if(found==1)
						{
							fprintf(Flogfile,"\n Inside If condition where found == 1 \n");
							fprintf(Flogfile,"\n cvprodFound = %d ",cvprodFound);
					        fprintf(Flogfile,"\n uaprodFound = %d ",uaprodFound);
							tc_strcpy(reportFound,"Yes");
							if(cvprodFound==1)
							{
								fprintf(Flogfile," \n Executing IF condition where cvprodFound==1 \n ");
								if(tc_strcmp(g_partStatus,"DR3")==0||tc_strcmp(g_partStatus,"DR3P")==0||tc_strcmp(g_partStatus,"DR4")==0||tc_strcmp(g_partStatus,"DR5")==0||tc_strcmp(g_partStatus,"AR3")==0||tc_strcmp(g_partStatus,"AR3P")==0||tc_strcmp(g_partStatus,"AR4")==0||tc_strcmp(g_partStatus,"AR5")==0)
					            {
									fprintf(Flogfile," \n Executing IF condition where Dataset type found with GroupID  - CVPROD\n ");
									//tc_strcpy(reportFound,"Yes");
							        totalYesCount++;
					            	withDR3AndDataset++;
									dr3AndAbove++;
					            }
							}
							if(uaprodFound==1)
							{
								fprintf(Flogfile," \n Executing IF condition where uaprodFound==1 \n ");
								if(tc_strcmp(g_partStatus,"DR4")==0||tc_strcmp(g_partStatus,"DR5")==0||tc_strcmp(g_partStatus,"AR3")==0||tc_strcmp(g_partStatus,"AR3P")==0||tc_strcmp(g_partStatus,"AR4")==0||tc_strcmp(g_partStatus,"AR5")==0)
					            {
									fprintf(Flogfile," \n Executing IF condition where Dataset type found with GroupID - UAPROD\n ");
									//tc_strcpy(reportFound,"Yes");
							        totalYesCount++;
					            	withDR4AndDataset++;
									dr4AndAbove++;
					            }
							}
						}   
						else
						{
							fprintf(Flogfile," \n Executing else condition where Dataset type not found \n ");
							fprintf(Flogfile,"\n cvprodFound = %d ",cvprodFound);
					        fprintf(Flogfile,"\n uaprodFound = %d ",uaprodFound);
							tc_strcpy(reportFound,"No");
							if(cvprodFound==1)
							{
								fprintf(Flogfile," \n Executing IF condition where cvprodFound==1 \n ");
								if(tc_strcmp(g_partStatus,"DR3")==0||tc_strcmp(g_partStatus,"DR3P")==0||tc_strcmp(g_partStatus,"DR4")==0||tc_strcmp(g_partStatus,"DR5")==0||tc_strcmp(g_partStatus,"AR3")==0||tc_strcmp(g_partStatus,"AR3P")==0||tc_strcmp(g_partStatus,"AR4")==0||tc_strcmp(g_partStatus,"AR5")==0)
					            {
									fprintf(Flogfile," \n Executing else condition where Dataset type not found  - CVPROD\n ");
									//tc_strcpy(reportFound,"No");
							        totalNocount++;
					            	dr3AndAbove++;
					            }
							}
						    if(uaprodFound==1)
							{
								fprintf(Flogfile," \n Executing IF condition where uaprodFound==1 \n ");
						        if(tc_strcmp(g_partStatus,"DR4")==0||tc_strcmp(g_partStatus,"DR5")==0||tc_strcmp(g_partStatus,"AR3")==0||tc_strcmp(g_partStatus,"AR3P")==0||tc_strcmp(g_partStatus,"AR4")==0||tc_strcmp(g_partStatus,"AR5")==0)
					            {
									fprintf(Flogfile," \n Executing else condition where Dataset type not found  - UAPROD\n ");
									//tc_strcpy(reportFound,"No");
							        totalNocount++;
					            	dr4AndAbove++;
					            }
							}
						}
						if(gCount==1)
					    {
							STRNG_replace_str(g_ecuType,",","",&t_ecuPartType);
					        STRNG_replace_str(g_objDesc,",","",&t_objDesc);
							
					    	fprintf(csvPtrLog,"Sr.no,ECU Number (Container),Revision and Sequence,Revision Description,ECU Part Type,DR Status,Report Attached,Remarks\n");
					        fprintf(csvPtrLog,"%d,%s,%s,%s,%s,%s,%s\n",gCount,g_itemId,g_revIdSequesnce,t_objDesc,t_ecuPartType,g_partStatus,reportFound);
					    }
					    else
					    {
							STRNG_replace_str(g_ecuType,",","",&t_ecuPartType);
					        STRNG_replace_str(g_objDesc,",","",&t_objDesc);
							
					    	fprintf(csvPtrLog,"%d,%s,%s,%s,%s,%s,%s\n",gCount,g_itemId,g_revIdSequesnce,t_objDesc,t_ecuPartType,g_partStatus,reportFound);
					    }					
					}					
				}		                   			
			}
			if(gConCount==1)
			{
				gConCount=0;
			}
		}
		else
		{
			if(tc_strcmp(objType,"T5_EE_PartRevision")==0 && tc_strcmp(partType,"CON")==0)
			{
				totalContainerCount++;
				fprintf(Flogfile,"\n ################# CONTAINER FOUND  ################# \n");
				gCount++;
				ITK_APICALL(GRM_find_relation_type("IMAN_specification",&relationTag),Flogfile);	
				ITK_APICALL(GRM_list_secondary_objects_only(lineObj,relationTag,&secObjCount,&secObjTag),Flogfile);
				i=0;
				found =0 ;
				tc_strcpy(reportFound,"");
				if(secObjCount>0)
				{
					for(i=0;i<secObjCount;i++)
					{
						ITK_APICALL(AOM_ask_value_string(secObjTag[i],"object_type",&secObjType),Flogfile);
						if(tc_strcmp(secObjType,"PDF")==0||tc_strcmp(secObjType,"DVP")==0)
						{
							found = 1;
						}
					}
				}
				fprintf(Flogfile, "\n Value of found = %d ",found);
				if(found==1)
				{
					fprintf(Flogfile,"\n Inside If condition where found == 1 \n");
					fprintf(Flogfile,"\n cvprodFound = %d ",cvprodFound);
				    fprintf(Flogfile,"\n uaprodFound = %d ",uaprodFound);
					tc_strcpy(reportFound,"Yes");
					if(cvprodFound==1)
					{
						fprintf(Flogfile," \n Executing IF condition where cvprodFound==1 \n ");
						if(tc_strcmp(partStatus,"DR3")==0||tc_strcmp(partStatus,"DR3P")==0||tc_strcmp(partStatus,"DR4")==0||tc_strcmp(partStatus,"DR5")==0||tc_strcmp(partStatus,"AR3")==0||tc_strcmp(partStatus,"AR3P")==0||tc_strcmp(partStatus,"AR4")==0||tc_strcmp(partStatus,"AR5")==0)
				        {
							fprintf(Flogfile," \n Executing IF condition where Dataset type found  - CVPROD\n ");
							//tc_strcpy(reportFound,"Yes");
					        totalYesCount++;
				        	withDR3AndDataset++;
							dr3AndAbove++;
				        }
					}
					if(uaprodFound==1)
					{
						fprintf(Flogfile," \n Executing IF condition where uaprodFound==1 \n ");
						if(tc_strcmp(partStatus,"DR4")==0||tc_strcmp(partStatus,"DR5")==0||tc_strcmp(partStatus,"AR3")==0||tc_strcmp(partStatus,"AR3P")==0||tc_strcmp(partStatus,"AR4")==0||tc_strcmp(partStatus,"AR5")==0)
				        {
							fprintf(Flogfile," \n Executing IF condition where Dataset type found - UAPROD\n ");
							//tc_strcpy(reportFound,"Yes");
					        totalYesCount++;
				        	withDR4AndDataset++;
							dr4AndAbove++;
				        }
					}
				}   
				else
				{
					fprintf(Flogfile," \n Executing else condition where Dataset type not found \n ");
					fprintf(Flogfile,"\n cvprodFound = %d ",cvprodFound);
				    fprintf(Flogfile,"\n uaprodFound = %d ",uaprodFound);
					tc_strcpy(reportFound,"No");
					if(cvprodFound==1)
					{
						fprintf(Flogfile," \n Executing IF condition where cvprodFound==1 \n ");
						if(tc_strcmp(partStatus,"DR3")==0||tc_strcmp(partStatus,"DR3P")==0||tc_strcmp(partStatus,"DR4")==0||tc_strcmp(partStatus,"DR5")==0||tc_strcmp(partStatus,"AR3")==0||tc_strcmp(partStatus,"AR3P")==0||tc_strcmp(partStatus,"AR4")==0||tc_strcmp(partStatus,"AR5")==0)
				        {
							fprintf(Flogfile," \n Executing else condition where Dataset type not found  - CVPROD\n ");
							//tc_strcpy(reportFound,"No");
					        totalNocount++;
				        	dr3AndAbove++;
				        }
					}
				    if(uaprodFound==1)
					{
						fprintf(Flogfile," \n Executing IF condition where uaprodFound==1 \n ");
				        if(tc_strcmp(partStatus,"DR4")==0||tc_strcmp(partStatus,"DR5")==0||tc_strcmp(partStatus,"AR3")==0||tc_strcmp(partStatus,"AR3P")==0||tc_strcmp(partStatus,"AR4")==0||tc_strcmp(partStatus,"AR5")==0)
				        {
							fprintf(Flogfile," \n Executing else condition where Dataset type not found  - UAPROD\n ");
							//tc_strcpy(reportFound,"No");
					        totalNocount++;
				        	dr4AndAbove++;
				        }
					}
				}	
				if(gCount==1)
				{
					STRNG_replace_str(ecuType,",","",&t_ecuPartType);
					STRNG_replace_str(objDesc,",","",&t_objDesc);
					fprintf(csvPtrLog,"Sr.no,ECU Number (Container),Revision and Sequence,Revision Description,ECU Part Type,DR Status,Report Attached,Remarks\n");
                    fprintf(csvPtrLog,"%d,%s,%s,%s,%s,%s,%s\n",gCount,itemId,revIdSequesnce,t_objDesc,t_ecuPartType,partStatus,reportFound);
				}
				else
				{
					STRNG_replace_str(ecuType,",","",&t_ecuPartType);
					STRNG_replace_str(objDesc,",","",&t_objDesc);
					fprintf(csvPtrLog,"%d,%s,%s,%s,%s,%s,%s\n",gCount,itemId,revIdSequesnce,t_objDesc,t_ecuPartType,partStatus,reportFound);
				}
			}
			BOM_ASK_CHILD(subChildCount,subChildTags,Flogfile,csvPtrLog);
		}
		if(itemId)
		{
			MEM_free(itemId);
		}
		if(itemName)
		{
			MEM_free(itemName);
		}
		if(subChildTags)
		{
			MEM_free(subChildTags);
		}
	}
    fprintf(Flogfile,"\n  ############################# Outside BOM_ASK_CHILD function #############################\n");	
}

/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	                     =   NULL;
    char *sPasswordFile	                 =   NULL;
    char *sGroup                         =   NULL;
	char *sPartNo                        =   NULL;
	char *itemId                         =   (char*)malloc(200*sizeof(char));;
	char *revIdInput                     =   NULL;
	char *loggedInUser                   =   NULL;
    char *shellExecute                   =   (char*) MEM_alloc(200*sizeof(char));
	char *logPath                        =  (char*)malloc(200*sizeof(char));
	char *prefValue                      =   NULL;	
	char *timeStr                        =   NULL;
	char *timeStrUpdated                 =   NULL;
	char *sequenceIdInput                =   NULL;
	char *tempRevId                      =   (char*) MEM_alloc(200*sizeof(char));
    char *qryInput[2]                    = {"Item ID","Revision"};
    char *qryValues[2];	                 
	char *qryObjName                     =   NULL;
	char *qryObjDecs                     =   NULL;
	char *qryT5PartStatus                =   NULL;
	char *moduleObjName                  =   NULL;
	char *moduleObjDesc                  =   NULL;
	char *moduleItemId                   =   NULL;						             
	char *modulelstitemId                =   NULL;
	char *modulelstObjectname            =   NULL;
	char *modulelstObjectDesc            =   NULL;
	char *modulelstItemRevId             =   NULL;
    char *blItemId                       =   NULL; 
	char *blObjectName                   =   NULL; 
	char *csvLog                         = (char*) MEM_alloc(200*sizeof(char));
	char *modulelstItemRevIdt5PartStatus = NULL;
	
	char filedata[500];
	
	int itkApiCallStatus;
    int bomChildCount   = 0;
    int tempSequenceId  = 0; 		
	int revCount = 0;
	int i=0;
	int t=0;
	int j=0;
	int temp=0;
	int revFound = 0;
	int sequenceId   =  0;	
	int tsequenceId  =  0;     
	int h1=0;
	int h2=0;
	int h3=0;
	int h4=0;
	int h5=0;
    int	rlsStsCount = 0;
	int rlsStsFound = 0;	
	int tItr=0;
	int qryObjectCount  =0 ;
	int moduleCount = 0;
	int tPercent1=0;
	float tPercent2=0;
	float tPercent3=0;
	
	float x1=0;
	float y1=0;
	float average =0;
	
	tag_t loggedInUserTag   =  NULLTAG;
	tag_t itemTag           =  NULLTAG;
	
	tag_t windowTag         =  NULLTAG;
	tag_t topBomLineTag     =  NULLTAG;
	tag_t revRule           =  NULLTAG;
	tag_t latestRevTag      =  NULLTAG;
	tag_t relationTag       =  NULLTAG;
	tag_t latestModuleRevTag   =  NULLTAG;
	tag_t qryTag = NULLTAG;
	
	tag_t *qryObjectTag     =  NULLTAG;
	tag_t *revTags          =  NULLTAG;
	tag_t *bomChildTags     =  NULLTAG;
	tag_t *rlsStsList       =  NULLTAG;
	tag_t *modulesTag       =  NULLTAG;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	sPartNo        = ITK_ask_cli_argument("-i=");
	revIdInput     = ITK_ask_cli_argument("-r=");
	sequenceIdInput     = ITK_ask_cli_argument("-s=");
	
	tc_strcpy(logPath,"/tmp/");
	tc_strcat(logPath,"TEST_REPORT_");
	tc_strcat(logPath,sPartNo);
	tc_strcat(logPath,"_");
	tc_strcat(logPath,timeStrUpdated);
	tc_strcat(logPath,".txt");
	
	tc_strcpy(csvLog,"/tmp/");
	tc_strcat(csvLog,sPartNo);
	tc_strcat(csvLog,"_");
	tc_strcat(csvLog,revIdInput);
	tc_strcat(csvLog,"_");
	tc_strcat(csvLog,sequenceIdInput);
	tc_strcat(csvLog,".csv");
	

    FILE *flogPtr = fopen(logPath,"w+");
	FILE *fPtr    = fopen(csvLog,"w+");
	
	fprintf(flogPtr,"-sUserName          = %s\n",sUserName     );
	fprintf(flogPtr,"-sPasswordFile      = %s\n",sPasswordFile );
	fprintf(flogPtr,"-sGroup             = %s\n",sGroup        );
	fprintf(flogPtr,"-sPartNo            = %s\n",sPartNo       );
	fprintf(flogPtr,"-revIdInput         = %s\n",revIdInput    );
	fprintf(flogPtr,"-sequenceIdInput    = %s\n",sequenceIdInput    );
	fprintf(flogPtr,"-logPath            = %s\n",logPath);
	fprintf(flogPtr,"-csvLog             = %s\n",csvLog);
	
	
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr);
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	if(itkApiCallStatus==ITK_ok)
    {
		printf(" \n ******************* Login successful to teamcenter ******************* \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
		{
		    fprintf(flogPtr,"\n Name of the logged in user = %s",loggedInUser);
			
			///// ************************************ CVPROD Execution ************************************
			if(tc_strstr(sPasswordFile,"cvprod")!=NULL)
			{
				cvprodFound = 1;
				uaprodFound = 0;
				fprintf(flogPtr,"\n CVPROD Found in password string. So executing condition for DR3 and above. \n ");
				itkApiCallStatus=ITK_APICALL(QRY_find2("Item Revision...",&qryTag),flogPtr); 
			    if(itkApiCallStatus==0 && qryTag!=NULLTAG)
			    {
			    	fprintf(flogPtr,"\n Qry Found successful Point 1 ");
			    	tc_strcpy(tempRevId,revIdInput);
			    	tc_strcat(tempRevId,";");
			    	tc_strcat(tempRevId,sequenceIdInput);
			    	fprintf(flogPtr,"\n - Qry intput tempRevId = %s \n",tempRevId);
			    	
			    	qryValues[0] = sPartNo;
			        qryValues[1] = tempRevId;
			    	
			    	itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag, 2, qryInput, qryValues, &qryObjectCount, &qryObjectTag),flogPtr); 
			    	fprintf(flogPtr," - Qry object count = %d \n",qryObjectCount);
			    	if(qryObjectCount>0)
			    	{
			    		for(i=0;i<qryObjectCount;i++)
			    		{
			    			ITK_APICALL(AOM_ask_value_string(qryObjectTag[i],"object_name",   &qryObjName     ),flogPtr);
			    			ITK_APICALL(AOM_ask_value_string(qryObjectTag[i],"object_desc",   &qryObjDecs     ),flogPtr);
			    	        ITK_APICALL(AOM_ask_value_string(qryObjectTag[i],"t5_PartStatus", &qryT5PartStatus)  ,flogPtr);
			    	        ITK_APICALL(GRM_find_relation_type("T5_CCVCHasECUModule",&relationTag),flogPtr);
			    	        ITK_APICALL(GRM_list_secondary_objects_only(qryObjectTag[i],relationTag,&moduleCount,&modulesTag),flogPtr);
			    		    
			    			fprintf(flogPtr," - qryObjName      [vc] = %s\n",qryObjName     );
			    			fprintf(flogPtr," - qryObjDecs      [vc] = %s\n",qryObjDecs     );
			    			fprintf(flogPtr," - qryT5PartStatus [vc] = %s\n",qryT5PartStatus);
			    			
			    			fprintf(fPtr,",,,,ECU Module (16R) Test Report Status \n ");
			    			fprintf(fPtr,",VC No., VC Description , VC DR Status , ECU Calibration Module , ECU Module Description , ECU Module DR Status \n");
			    			
			    			fprintf(flogPtr," - modulecount object count = %d \n",qryObjectCount);
			    			
			    			for(j=0;j<moduleCount;j++)
			    			{
			    				
			    				ITK_APICALL(AOM_ask_value_string(modulesTag[j],"item_id"    ,&moduleItemId      ),flogPtr);
			    				ITK_APICALL(AOM_ask_value_string(modulesTag[j],"object_name",&moduleObjName     ),flogPtr);
			    				ITK_APICALL(AOM_ask_value_string(modulesTag[j],"object_desc",&moduleObjDesc     ),flogPtr);
			    				
			    				fprintf(flogPtr," - moduleItemId  [module] = %s\n",moduleItemId   );
			    				fprintf(flogPtr," - moduleObjName [module] = %s\n",moduleObjName  );
			    				fprintf(flogPtr," - moduleObjDesc [module] = %s\n",moduleObjDesc  );
			    				
			    				ITK_APICALL(ITEM_ask_latest_rev(modulesTag[j],&latestModuleRevTag),flogPtr);
			    				if(latestModuleRevTag!=NULLTAG)
			    				{
			    					fprintf(flogPtr,"\n ******* Latest module rev tag found ******* \n");
			    					ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"item_id"    ,     &modulelstitemId         ),flogPtr);
			    				    ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"object_name",     &modulelstObjectname     ),flogPtr);
			    				    ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"object_desc",     &modulelstObjectDesc     ),flogPtr);
			    				    ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"item_revision_id",&modulelstItemRevId      ),flogPtr);
			    					ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"t5_PartStatus"   ,&modulelstItemRevIdt5PartStatus      ),flogPtr);
			    					
			    				
			    					fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstitemId       );
			    				    fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstObjectname   );
			    				    fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstObjectDesc   );
			    					fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstItemRevId    );								
			    					
			    			        fprintf(fPtr,",%s, %s , %s , %s , %s , %s \n",sPartNo, qryObjDecs , qryT5PartStatus , modulelstitemId , modulelstObjectDesc , modulelstItemRevIdt5PartStatus );
			    			
			    			        fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
			    				
			    					fprintf(flogPtr," Bom expansion started .... with %s revId and with %s sequesnce \n",revIdInput,sequenceIdInput); 
			    					                                                                                     
			    					itkApiCallStatus=ITK_APICALL(BOM_create_window(&windowTag),flogPtr); 		
			    					fprintf(flogPtr," API Status for BOM_create_window = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(CFM_find("ERC release and above",&revRule),flogPtr);									
			    					fprintf(flogPtr," API Status for CFM_find = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_set_window_config_rule(windowTag,revRule),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_set_window_config_rule = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_set_window_pack_all(windowTag,true),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_set_window_pack_all = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_set_window_top_line(windowTag,modulesTag[j],latestModuleRevTag,NULLTAG,&topBomLineTag),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_set_window_top_line = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(topBomLineTag,"bl_item_item_id",&blItemId),flogPtr);
			    					fprintf(flogPtr," API Status for AOM_ask_value_string = %d \n",itkApiCallStatus);
			    					fprintf(flogPtr," bl_item_item_id = %s \n",blItemId);
			    															
			    					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(topBomLineTag,"bl_item_object_name",&blObjectName),flogPtr);
			    					fprintf(flogPtr," API Status for AOM_ask_value_string = %d \n",itkApiCallStatus);
			    					fprintf(flogPtr," bl_item_object_name = %s \n",blObjectName);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_line_ask_all_child_lines(topBomLineTag,&bomChildCount,&bomChildTags),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_line_ask_all_child_lines = %d \n",itkApiCallStatus);									
			    					fprintf(flogPtr," bomChildCount = %d \n",bomChildCount);
			    					
			    					
			    					if(bomChildCount>0)
			    					{
			    						BOM_ASK_CHILD(bomChildCount,bomChildTags,flogPtr,fPtr);
			    					}
			    					
			    					fprintf(flogPtr," \n Total DR3 and above found = %d \n",dr3AndAbove);
			    					fprintf(flogPtr," \n Total Yes count = %d \n",totalYesCount);
			    					fprintf(flogPtr," \n Total No count = %d \n",totalNocount);	
			    					fprintf(flogPtr," \n withDR3AndDataset = %d \n",withDR3AndDataset);	
			    
			    					tPercent1=totalContainerCount;
			    					
			    					x1=dr3AndAbove;
			    					y1=tPercent1;
			    					
			    					tPercent2=x1/y1;
			    					tPercent2=tPercent2*100;
			    	
			    					fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
			    					
			    					fprintf(fPtr,",,,,ECU Module (16R) Test Report Status,,,\n");
			    					fprintf(fPtr,",,A, Number of ECUs released with >= DR3, ,Total No. of ECUs ,Percentage(%)\n");
			    					fprintf(fPtr,",,,%d,,%d,%f \n",dr3AndAbove, tPercent1,tPercent2);
			    					
			    					x1=withDR3AndDataset;
			    					y1=tPercent1;
			    					
			    					fprintf(flogPtr," \n x1 = %f (p1)\n",x1);
			    					fprintf(flogPtr," \n y1 = %f (p1)\n",y1);
			    					
			    					tPercent3=x1/y1;
			    					
			    					fprintf(flogPtr," \n tPercent3 = %f (p1)\n",tPercent3);	
			    					
			    					tPercent3=tPercent3*100;
			    					
			    					fprintf(flogPtr," \n tPercent3 = %f (p2)\n",tPercent3);	
									
									average=tPercent2+tPercent3;
									average=average/2;
									
									fprintf(flogPtr," \n average = %f \n",average);
			    					
			    					fprintf(fPtr,",,B, Number of Reports released at DR3, ,Total No. of ECUs ,Percentage(%)\n");
			    					fprintf(fPtr,",,,%d,,%d,%f \n",withDR3AndDataset, tPercent1,tPercent3);
									fprintf(fPtr,",,,,,S1,%f\n",average);
			    					
			    					fclose(fPtr);
			    					fclose(flogPtr);																					
			    				}
			    				else
			    				{
			    					fprintf(flogPtr,"\n ******* Latest module rev tag not found ******* \n");
			    				}
			    			}
			    		}
			    	}
			    	else
			    	{
			    		fprintf(flogPtr,"\n Error : qryObjectCount <= 0 \n");
			    	}
			    }
			    else
			    {
			    	fprintf(flogPtr,"\n Error : Qry not Found  \n ");
			    }
			}
		    ///// ************************************ UAPROD Execution ************************************
			else if(tc_strstr(sPasswordFile,"uaprod")!=NULL)
			{
				uaprodFound = 1;
				cvprodFound = 0;
				fprintf(flogPtr,"\n UAPROD Found in password string. So executing condition for DR4 and above. \n ");
			    itkApiCallStatus=ITK_APICALL(QRY_find2("Item Revision...",&qryTag),flogPtr); 
			    if(itkApiCallStatus==0 && qryTag!=NULLTAG)
			    {
			    	fprintf(flogPtr,"\n Qry Found successful Point 1 ");
			    	tc_strcpy(tempRevId,revIdInput);
			    	tc_strcat(tempRevId,";");
			    	tc_strcat(tempRevId,sequenceIdInput);
			    	fprintf(flogPtr,"\n - Qry intput tempRevId = %s \n",tempRevId);
			    	
			    	qryValues[0] = sPartNo;
			        qryValues[1] = tempRevId;
			    	
			    	itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag, 2, qryInput, qryValues, &qryObjectCount, &qryObjectTag),flogPtr); 
			    	fprintf(flogPtr," - Qry object count = %d \n",qryObjectCount);
			    	if(qryObjectCount>0)
			    	{
			    		for(i=0;i<qryObjectCount;i++)
			    		{
			    			ITK_APICALL(AOM_ask_value_string(qryObjectTag[i],"object_name",   &qryObjName     ),flogPtr);
			    			ITK_APICALL(AOM_ask_value_string(qryObjectTag[i],"object_desc",   &qryObjDecs     ),flogPtr);
			    	        ITK_APICALL(AOM_ask_value_string(qryObjectTag[i],"t5_PartStatus", &qryT5PartStatus)  ,flogPtr);
			    	        ITK_APICALL(GRM_find_relation_type("T5_CCVCHasECUModule",&relationTag),flogPtr);
			    	        ITK_APICALL(GRM_list_secondary_objects_only(qryObjectTag[i],relationTag,&moduleCount,&modulesTag),flogPtr);
			    		    
			    			fprintf(flogPtr," - qryObjName      [vc] = %s\n",qryObjName     );
			    			fprintf(flogPtr," - qryObjDecs      [vc] = %s\n",qryObjDecs     );
			    			fprintf(flogPtr," - qryT5PartStatus [vc] = %s\n",qryT5PartStatus);
			    			
			    			fprintf(fPtr,",,,,ECU Module (16R) Test Report Status \n ");
			    			fprintf(fPtr,",VC No., VC Description , VC DR Status , ECU Calibration Module , ECU Module Description , ECU Module DR Status \n");
			    			
			    			fprintf(flogPtr," - modulecount object count = %d \n",qryObjectCount);
			    			
			    			for(j=0;j<moduleCount;j++)
			    			{
			    				
			    				ITK_APICALL(AOM_ask_value_string(modulesTag[j],"item_id"    ,&moduleItemId      ),flogPtr);
			    				ITK_APICALL(AOM_ask_value_string(modulesTag[j],"object_name",&moduleObjName     ),flogPtr);
			    				ITK_APICALL(AOM_ask_value_string(modulesTag[j],"object_desc",&moduleObjDesc     ),flogPtr);
			    				
			    				fprintf(flogPtr," - moduleItemId  [module] = %s\n",moduleItemId   );
			    				fprintf(flogPtr," - moduleObjName [module] = %s\n",moduleObjName  );
			    				fprintf(flogPtr," - moduleObjDesc [module] = %s\n",moduleObjDesc  );
			    				
			    				
			    				ITK_APICALL(ITEM_ask_latest_rev(modulesTag[j],&latestModuleRevTag),flogPtr);
			    				if(latestModuleRevTag!=NULLTAG)
			    				{
			    					fprintf(flogPtr,"\n ******* Latest module rev tag found ******* \n");
			    					ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"item_id"    ,     &modulelstitemId         ),flogPtr);
			    				    ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"object_name",     &modulelstObjectname     ),flogPtr);
			    				    ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"object_desc",     &modulelstObjectDesc     ),flogPtr);
			    				    ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"item_revision_id",&modulelstItemRevId      ),flogPtr);
			    					ITK_APICALL(AOM_ask_value_string(latestModuleRevTag,"t5_PartStatus"   ,&modulelstItemRevIdt5PartStatus      ),flogPtr);
			    					
			    					fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstitemId       );
			    				    fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstObjectname   );
			    				    fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstObjectDesc   );
			    					fprintf(flogPtr," - latestModuleRevTag [moduleRev] = %s \n",modulelstItemRevId    );								
			    					
			    			        fprintf(fPtr,",%s, %s , %s , %s , %s , %s \n",sPartNo, qryObjDecs , qryT5PartStatus , modulelstitemId , modulelstObjectDesc , modulelstItemRevIdt5PartStatus );
			    			
			    			        fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
			    				
			    					fprintf(flogPtr," Bom expansion started .... with %s revId and with %s sequesnce \n",revIdInput,sequenceIdInput); 
			    					                                                                                     
			    					itkApiCallStatus=ITK_APICALL(BOM_create_window(&windowTag),flogPtr); 		
			    					fprintf(flogPtr," API Status for BOM_create_window = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(CFM_find("ERC release and above",&revRule),flogPtr);									
			    					fprintf(flogPtr," API Status for CFM_find = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_set_window_config_rule(windowTag,revRule),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_set_window_config_rule = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_set_window_pack_all(windowTag,true),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_set_window_pack_all = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_set_window_top_line(windowTag,modulesTag[j],latestModuleRevTag,NULLTAG,&topBomLineTag),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_set_window_top_line = %d \n",itkApiCallStatus);
			    					
			    					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(topBomLineTag,"bl_item_item_id",&blItemId),flogPtr);
			    					fprintf(flogPtr," API Status for AOM_ask_value_string = %d \n",itkApiCallStatus);
			    					fprintf(flogPtr," bl_item_item_id = %s \n",blItemId);
			    															
			    					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(topBomLineTag,"bl_item_object_name",&blObjectName),flogPtr);
			    					fprintf(flogPtr," API Status for AOM_ask_value_string = %d \n",itkApiCallStatus);
			    					fprintf(flogPtr," bl_item_object_name = %s \n",blObjectName);
			    					
			    					itkApiCallStatus=ITK_APICALL(BOM_line_ask_all_child_lines(topBomLineTag,&bomChildCount,&bomChildTags),flogPtr);
			    					fprintf(flogPtr," API Status for BOM_line_ask_all_child_lines = %d \n",itkApiCallStatus);									
			    					fprintf(flogPtr," bomChildCount = %d \n",bomChildCount);
			    					
			    					
			    					if(bomChildCount>0)
			    					{
			    						BOM_ASK_CHILD(bomChildCount,bomChildTags,flogPtr,fPtr);
			    					}
			    					
			    					fprintf(flogPtr," \n Total DR4 and above found = %d \n",dr4AndAbove);
			    					fprintf(flogPtr," \n Total Yes count = %d \n",totalYesCount);
			    					fprintf(flogPtr," \n Total No count = %d \n",totalNocount);	
			    					fprintf(flogPtr," \n withDR4AndDataset = %d \n",withDR4AndDataset);	
			    
			    					tPercent1=totalContainerCount;
			    					
			    					x1=dr4AndAbove;
			    					y1=tPercent1;
			    					
			    					tPercent2=x1/y1;
			    					tPercent2=tPercent2*100;
			    					
			    					fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
		                            fprintf(fPtr,"\n");
			    					
			    					fprintf(fPtr,",,,,ECU Module (16R) Test Report Status,,,\n");
			    					fprintf(fPtr,",,A, Number of ECUs released with >= DR4, ,Total No. of ECUs ,Percentage(%)\n");
			    					fprintf(fPtr,",,,%d,,%d,%f \n",dr4AndAbove, tPercent1,tPercent2);
			    					
			    					x1=withDR4AndDataset;
			    					y1=tPercent1;
			    					
			    					fprintf(flogPtr," \n x1 = %f (p1)\n",x1);
			    					fprintf(flogPtr," \n y1 = %f (p1)\n",y1);
			    					
			    					tPercent3=x1/y1;
			    					
			    					fprintf(flogPtr," \n tPercent3 = %f (p1)\n",tPercent3);	
			    					
			    					tPercent3=tPercent3*100;
			    					
			    					fprintf(flogPtr," \n tPercent3 = %f (p2)\n",tPercent3);	
									
									average=tPercent2+tPercent3;
									average=average/2;
									
									fprintf(flogPtr," \n average = %f \n",average);
			    					
			    					fprintf(fPtr,",,B, Number of Reports released at DR4, ,Total No. of ECUs ,Percentage(%)\n");
			    					fprintf(fPtr,",,,%d,,%d,%f \n",withDR4AndDataset, tPercent1,tPercent3);
			    					fprintf(fPtr,",,,,,S1,%f\n",average);
			    					fclose(fPtr);
			    					fclose(flogPtr);																					
			    				}
			    				else
			    				{
			    					fprintf(flogPtr,"\n ******* Latest module rev tag not found ******* \n");
			    				}
			    			}
			    		}
			    	}
			    	else
			    	{
			    		fprintf(flogPtr,"\n Error : qryObjectCount <= 0 \n");
			    	}
			    }
			    else
			    {
			    	fprintf(flogPtr,"\n Error : Qry not Found  \n ");
			    }
			}
			///// ************************************ No Execution ************************************
			else
			{
				cvprodFound = 0;
				uaprodFound = 0;
				fprintf(flogPtr,"\n Error : CVPROD and UAPROD both not found in password string. So execution failed.\n ");
			}
	    }
		else
	    {
	        printf("\n Login user not found  \n");	
	        fprintf(flogPtr,"\n  Error : Login user not found  \n");
	    }
	}
	else
	{
	  printf("\n Login failed in TC \n");	
	  fprintf(flogPtr,"\n  Error : Login failed in TC \n");
	}
	return(ITK_ok);
}
