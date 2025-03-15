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
#include <tc/folder.h>

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
            fprintf(Flogfile,"Error code - [%d]. Error message - %s \n",ifails[index],texts[index]);	
        } 
        EMH_clear_errors();
    }
    return status;
}

/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	          = NULL;
    char *sPasswordFile	      = NULL;
    char *sGroup              = NULL;
	char *sHelp               = NULL;
	char *loggedInUser        = NULL; 
	char *dmlNo               = NULL;
	time_t mytime             = time(NULL);
	char *timeStr             = ctime(&mytime);
	char *timeStrUpdated      = NULL;
	char *csvFileName         = (char*)malloc( 100 * sizeof(char));
	char *itemId              = NULL;
	char *itemRevId           = NULL;
	char *creationData        = NULL;
	char *t5BusinessUnit      = NULL;
	char *t5Reason            = NULL;
	char *t5ErcWBS            = NULL;
	char *t5CProjectCode      = NULL;
	char *t5ReleaseType       = NULL;
	char *t5CdrStatus         = NULL;
	char *t5UReason           = NULL;
	char *taskItemId          = NULL; 
	char *taskItemRevId       = NULL;
	char *secItemId           = NULL; 
	char *secItemRevId        = NULL; 
	char *sect5BomCmpl        = NULL; 
	char *sect5RolledUpWt     = NULL; 
	char *sect5TargetWeight   = NULL;
	char *sect5PartStatus     = NULL; 
	char *sect5DesignGroup    = NULL; 
	char *secObjecttype       = NULL;
	char *sect5ProjectCode    = NULL; 
	char *secObjName          = NULL;
	char *subsecItemId        = NULL; 
	char *subsecItemRevId     = NULL; 
	char *subsect5BomCmpl     = NULL; 
	char *subsect5RolledUpWt  = NULL; 
	char *subsect5PartStatus  = NULL; 
	char *subsect5DesignGroup = NULL; 
	char *subsecObjecttype    = NULL;
	char *subsect5ProjectCode = NULL; 
	char *subsecObjName       = NULL;
	char *subSecTagWt         = NULL;
	char *dataSetName         = (char*)malloc( 50 * sizeof(char));
	char *datasetDesc         = (char*)malloc( 50 * sizeof(char));
	char *datasetId           = (char*)malloc( 50 * sizeof(char));
	char *revSecObjtype       = NULL; 
	char *revSecObjName       = NULL;
	char *relationName        = NULL; 
	
	
	int itkApiCallStatus=0;
	int taskCount       = 0;
	int i=0,j=0,k=0,l=0;
	int secObjCount     = 0;
	int folderSize      = 0;
	int fldObjCount     = 0;
	int revSecObj       = 0;
	
	
	tag_t loggedInUserTag = NULLTAG;
	tag_t itemTag         = NULLTAG;
	tag_t itemRevTag      = NULLTAG;
	tag_t relationType    = NULLTAG;
	tag_t datasetType     = NULLTAG;
	tag_t datasetTag      = NULLTAG;
	tag_t relationTag     = NULLTAG;
	tag_t nRelationTag    = NULLTAG;
	tag_t priRelationTag  = NULLTAG;
	
	tag_t *taskTags       = NULLTAG;
	tag_t *secObjTags     = NULLTAG;
	tag_t *fldObjTags     = NULLTAG;
	tag_t *revSecObjTags   = NULLTAG;
	
	FILE *flogPtr=fopen("DmlInfoLog.txt","w+");
	
	
	sUserName     = ITK_ask_cli_argument("-u=");
    sPasswordFile = ITK_ask_cli_argument("-pf=");
    sGroup        = ITK_ask_cli_argument("-g=");
	dmlNo         = ITK_ask_cli_argument("-dmlno=");
	
	fprintf(flogPtr,__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,__TIME__);
	fprintf(flogPtr,"\n");
	
	fprintf(flogPtr,"sUserName      - %s \n", sUserName);
	fprintf(flogPtr,"sPasswordFile  - %s \n", sPasswordFile);
	fprintf(flogPtr,"sGroup         - %s \n", sGroup);
	fprintf(flogPtr,"dmlNo          - %s \n", dmlNo);
	fprintf(flogPtr,"csvFileName    - %s \n", csvFileName);
			
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated); 
	
	tc_strcpy(csvFileName,"/tmp/ADmlInfo");
	tc_strcat(csvFileName,"_");
	tc_strcat(csvFileName,dmlNo);
	tc_strcat(csvFileName,"_");
	tc_strcat(csvFileName,timeStrUpdated);
	tc_strcat(csvFileName,"_");
	tc_strcat(csvFileName,".csv");
    	
    fprintf(flogPtr,"csvFileName    - %s \n", csvFileName);
	FILE *fPtr=fopen(csvFileName,"w+");
	
	tc_strcpy(dataSetName,"ADmlInfo");
	tc_strcat(dataSetName,"_");
	tc_strcat(dataSetName,dmlNo);
	tc_strcat(dataSetName,"_");
	tc_strcat(dataSetName,timeStrUpdated);
	
	fprintf(flogPtr,"dataSetName    - %s \n", dataSetName);
	
	tc_strcpy(datasetDesc,"ADmlInfo");
	tc_strcat(datasetDesc,"_");
	tc_strcat(datasetDesc,dmlNo);
	tc_strcat(datasetDesc,"_");
	tc_strcat(datasetDesc,timeStrUpdated);
	tc_strcat(datasetDesc,"_Desc");
	
	fprintf(flogPtr,"datasetDesc    - %s \n", datasetDesc);
	
	tc_strcpy(datasetId,"ADmlInfo");
	tc_strcat(datasetId,"_");
	tc_strcat(datasetId,dmlNo);
	tc_strcat(datasetId,"_");
	tc_strcat(datasetId,timeStrUpdated);
	tc_strcat(datasetId,"_Id");
	
	fprintf(flogPtr,"datasetId      - %s \n", datasetId);			
	if(itkApiCallStatus==ITK_ok)
    {
		printf("************************************************************************** \n Login successful to teamcenter \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
		{
		    fprintf(flogPtr,"Name of the logged in user = %s\n",loggedInUser);
			if(fPtr!=NULL)
			{
			    fprintf(flogPtr,"Output File successfully opened ...\n");
				itkApiCallStatus=ITK_APICALL(ITEM_find_item(dmlNo,&itemTag),flogPtr);
				if(itkApiCallStatus==0 && itemTag!=NULLTAG)
				{
					fprintf(flogPtr,"Item with %s id found in TC \n",dmlNo);
					itkApiCallStatus=ITK_APICALL(ITEM_ask_latest_rev(itemTag,&itemRevTag),flogPtr);
					if(itkApiCallStatus==0 && itemRevTag!=NULLTAG)
					{
						fprintf(flogPtr,"ItemRev found in TC \n",dmlNo);
						ITK_APICALL(AOM_ask_value_string(itemRevTag,"item_id",&itemId),flogPtr);
						ITK_APICALL(AOM_ask_value_string(itemRevTag,"item_revision_id",&itemRevId),flogPtr);
						ITK_APICALL(AOM_UIF_ask_value(itemRevTag,"creation_date",&creationData),flogPtr);
						ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_ERCBusiUt",&t5BusinessUnit),flogPtr);
						ITK_APICALL(AOM_UIF_ask_value(itemRevTag,"t5_reason",&t5Reason),flogPtr);
						ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_ERCWBS",&t5ErcWBS),flogPtr);
						ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_cprojectcode",&t5CProjectCode),flogPtr);
						ITK_APICALL(AOM_UIF_ask_value(itemRevTag,"t5_rlstype",&t5ReleaseType),flogPtr);
						ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_cDRstatus",&t5CdrStatus),flogPtr);
						ITK_APICALL(AOM_ask_value_tags(itemRevTag,"T5_DMLTaskRelation",&taskCount,&taskTags),flogPtr);
												
						fprintf(flogPtr,"itemId              - %s \n", itemId);
						fprintf(flogPtr,"itemRevId           - %s \n", itemRevId);
						fprintf(flogPtr,"creationData        - %s \n", creationData);
						fprintf(flogPtr,"t5BusinessUnit      - %s \n", t5BusinessUnit);
						fprintf(flogPtr,"t5Reason            - %s \n", t5Reason);
						fprintf(flogPtr,"t5ErcWBS            - %s \n", t5ErcWBS);
						fprintf(flogPtr,"t5CProjectCode      - %s \n", t5CProjectCode);
						fprintf(flogPtr,"t5ReleaseType       - %s \n", t5ReleaseType);
						fprintf(flogPtr,"t5CdrStatus         - %s \n", t5CdrStatus);
						fprintf(flogPtr,"taskCount           - %d \n", taskCount);
						
						ITK_APICALL(STRNG_replace_str(t5Reason,","," ",&t5UReason),flogPtr);
						
						fprintf(flogPtr,"t5UReason           - %s \n", t5UReason);
						
						fprintf(fPtr,"DMLNo,Revision,Creation Date,Business Unit,Dml Reason,ERC Wbs Des,Project Code,Release Type,DR Status\n");
						fprintf(fPtr,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",itemId,itemRevId,creationData,t5BusinessUnit,t5UReason,t5ErcWBS,t5CProjectCode,t5ReleaseType,t5CdrStatus);        
						fprintf(fPtr,"\n PartNo,Revision,BOM Completeness,Rolled up weight,Target Weight,DR status,Design Group,Object Type,Project Code \n");
						if(taskCount>0)
						{
							fprintf(flogPtr,"taskCount found greater than zero - %d \n", taskCount);
							for(i=0;i<taskCount;i++)
							{
								ITK_APICALL(AOM_ask_value_string(taskTags[i],"item_id",&taskItemId),flogPtr);
						        ITK_APICALL(AOM_ask_value_string(taskTags[i],"item_revision_id",&taskItemRevId),flogPtr);
								fprintf(flogPtr,"taskItemId          - %s \n", taskItemId);
								fprintf(flogPtr,"taskItemRevId       - %s \n", taskItemRevId);
								
								//For Vehicle release
								if(tc_strcmp(t5ReleaseType,"Vehicle Release")==0 && tc_strstr(taskItemRevId,"_00")!=NULL)
								{
									fprintf(flogPtr,"If condition passed with t5ReleaseType = Vehicle Release \n");
									ITK_APICALL(GRM_find_relation_type("CMHasSolutionItem",&relationType),flogPtr);
									ITK_APICALL(GRM_list_secondary_objects_only(taskTags[i],relationType,&secObjCount,&secObjTags),flogPtr);
									fprintf(flogPtr,"secObjCount       - %d \n", secObjCount);
									
									if(secObjCount>0)
									{
										for(j=0;j<secObjCount;j++)
										{
											ITK_APICALL(AOM_ask_value_string(secObjTags[j],"item_id",&secItemId),flogPtr);
											ITK_APICALL(AOM_ask_value_string(secObjTags[j],"item_revision_id",&secItemRevId),flogPtr);
											ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_BOMCmpl",&sect5BomCmpl),flogPtr);
											ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_RolledupWt",&sect5RolledUpWt),flogPtr);
											ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_PartStatus",&sect5PartStatus),flogPtr);
											ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_DesignGrp",&sect5DesignGroup),flogPtr);
											ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_TargetWt",&sect5TargetWeight),flogPtr);
											ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"object_type",&secObjecttype),flogPtr);
											ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_ProjectCode",&sect5ProjectCode),flogPtr);
											ITK_APICALL(AOM_ask_value_string(secObjTags[j],"object_name",&secObjName),flogPtr);
											
											fprintf(flogPtr,"************************************************* \n");
											fprintf(flogPtr,"	secItemId              - %s \n", secItemId       );
											fprintf(flogPtr,"	secItemRevId           - %s \n", secItemRevId    );
											fprintf(flogPtr,"	sect5BomCmpl           - %s \n", sect5BomCmpl    );
											fprintf(flogPtr,"	sect5RolledUpWt        - %s \n", sect5RolledUpWt );
											fprintf(flogPtr,"	sect5PartStatus        - %s \n", sect5PartStatus );
											fprintf(flogPtr,"	sect5DesignGroup       - %s \n", sect5DesignGroup);
											fprintf(flogPtr,"	secObjecttype          - %s \n", secObjecttype   );
											fprintf(flogPtr,"	sect5ProjectCode       - %s \n", sect5ProjectCode);
											fprintf(flogPtr,"	secObjName             - %s \n", secObjName);
											fprintf(flogPtr,"	sect5TargetWeight      - %s \n", sect5TargetWeight);
																						
											fprintf(fPtr,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",secItemId,secItemRevId,sect5BomCmpl,sect5RolledUpWt,sect5TargetWeight,sect5PartStatus,sect5DesignGroup,secObjecttype,sect5ProjectCode);															
										}
									}
									else
									{
										fprintf(flogPtr," Else condition execute where secObjCount less than zero \n");
									}									
								}
								
								// For Module release 
								else if(tc_strcmp(t5ReleaseType,"Module Release")==0)
								{
									fprintf(flogPtr,"Else If condition passed with t5ReleaseType = Module Release \n");
									if(tc_strstr(taskItemRevId,"_CL")==NULL && tc_strstr(taskItemRevId,"_APL")==NULL && tc_strstr(taskItemRevId,"_0E")==NULL && tc_strstr(taskItemRevId,"_MBOM")==NULL)
									{
										fprintf(flogPtr,"Valid condition passed where _CL,_APL,_0E,_MBOM are not present \n");
										ITK_APICALL(GRM_find_relation_type("CMHasSolutionItem",&relationType),flogPtr);
										ITK_APICALL(GRM_list_secondary_objects_only(taskTags[i],relationType,&secObjCount,&secObjTags),flogPtr);
										fprintf(flogPtr,"secObjCount       - %d \n", secObjCount);
										
										if(secObjCount>0)
										{
											for(j=0;j<secObjCount;j++)
											{
												ITK_APICALL(AOM_ask_value_string(secObjTags[j],"item_id",&secItemId),flogPtr);
												ITK_APICALL(AOM_ask_value_string(secObjTags[j],"item_revision_id",&secItemRevId),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_BOMCmpl",&sect5BomCmpl),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_RolledupWt",&sect5RolledUpWt),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_PartStatus",&sect5PartStatus),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_DesignGrp",&sect5DesignGroup),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"object_type",&secObjecttype),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_ProjectCode",&sect5ProjectCode),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_TargetWt",&sect5TargetWeight),flogPtr);
												ITK_APICALL(AOM_ask_value_string(secObjTags[j],"object_name",&secObjName),flogPtr);
												
												fprintf(flogPtr,"************************************************* \n");
												fprintf(flogPtr,"	secItemId              - %s \n", secItemId       );
												fprintf(flogPtr,"	secItemRevId           - %s \n", secItemRevId    );
												fprintf(flogPtr,"	sect5BomCmpl           - %s \n", sect5BomCmpl    );
												fprintf(flogPtr,"	sect5RolledUpWt        - %s \n", sect5RolledUpWt );
												fprintf(flogPtr,"	sect5PartStatus        - %s \n", sect5PartStatus );
												fprintf(flogPtr,"	sect5DesignGroup       - %s \n", sect5DesignGroup);
												fprintf(flogPtr,"	secObjecttype          - %s \n", secObjecttype   );
												fprintf(flogPtr,"	sect5ProjectCode       - %s \n", sect5ProjectCode);
												fprintf(flogPtr,"	secObjName             - %s \n", secObjName);
												fprintf(flogPtr,"	sect5TargetWeight      - %s \n", sect5TargetWeight);
												
												fprintf(fPtr,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",secItemId,secItemRevId,sect5BomCmpl,sect5RolledUpWt,sect5TargetWeight,sect5PartStatus,sect5DesignGroup,secObjecttype,sect5ProjectCode);
											}
										}
										else
										{
											fprintf(flogPtr," Else condition execute where secObjCount less than zero \n");
										}																													
									}
									else
									{
										fprintf(flogPtr," Skipping for condition where _CL,_APL,_0E,_MBOM are present \n");
									}
								}
								// For Gate Maturation
								else if(tc_strcmp(t5ReleaseType,"Gate Maturation")==0)
								{
									fprintf(flogPtr,"Else If condition passed with t5ReleaseType = Gate Maturation \n");
									if(tc_strstr(taskItemRevId,"_CL")==NULL && tc_strstr(taskItemRevId,"_APL")==NULL && tc_strstr(taskItemRevId,"_0E")==NULL && tc_strstr(taskItemRevId,"_MBOM")==NULL)
									{
										fprintf(flogPtr,"Valid condition passed where _CL,_APL,_0E,_MBOM are not present \n");
										ITK_APICALL(GRM_find_relation_type("CMHasSolutionItem",&relationType),flogPtr);
										ITK_APICALL(GRM_list_secondary_objects_only(taskTags[i],relationType,&secObjCount,&secObjTags),flogPtr);
										fprintf(flogPtr,"secObjCount       - %d \n", secObjCount);
										
										if(secObjCount>0)
										{
											for(j=0;j<secObjCount;j++)
											{
												ITK_APICALL(AOM_ask_value_string(secObjTags[j],"item_id",&secItemId),flogPtr);
												ITK_APICALL(AOM_ask_value_string(secObjTags[j],"item_revision_id",&secItemRevId),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_BOMCmpl",&sect5BomCmpl),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_RolledupWt",&sect5RolledUpWt),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_PartStatus",&sect5PartStatus),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_DesignGrp",&sect5DesignGroup),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"object_type",&secObjecttype),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_ProjectCode",&sect5ProjectCode),flogPtr);
												ITK_APICALL(AOM_UIF_ask_value(secObjTags[j],"t5_TargetWt",&sect5TargetWeight),flogPtr);
												ITK_APICALL(AOM_ask_value_string(secObjTags[j],"object_name",&secObjName),flogPtr);
												
												fprintf(flogPtr,"************************************************* \n");
												fprintf(flogPtr,"	secItemId              - %s \n", secItemId       );
												fprintf(flogPtr,"	secItemRevId           - %s \n", secItemRevId    );
												fprintf(flogPtr,"	sect5BomCmpl           - %s \n", sect5BomCmpl    );
												fprintf(flogPtr,"	sect5RolledUpWt        - %s \n", sect5RolledUpWt );
												fprintf(flogPtr,"	sect5PartStatus        - %s \n", sect5PartStatus );
												fprintf(flogPtr,"	sect5DesignGroup       - %s \n", sect5DesignGroup);
												fprintf(flogPtr,"	secObjecttype          - %s \n", secObjecttype   );
												fprintf(flogPtr,"	sect5ProjectCode       - %s \n", sect5ProjectCode);
												fprintf(flogPtr,"	secObjName             - %s \n", secObjName);
												fprintf(flogPtr,"	sect5TargetWeight             - %s \n", sect5TargetWeight);
												
												fprintf(fPtr,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",secItemId,secItemRevId,sect5BomCmpl,sect5RolledUpWt,sect5TargetWeight,sect5PartStatus,sect5DesignGroup,secObjecttype,sect5ProjectCode);
												if(tc_strcmp(secObjecttype,"Folder")==0 && tc_strcmp(secObjName,"Parts For Gate Maturation")==0)
												{
													fprintf(flogPtr,"If Condition passed where secObjecttype = Folder and secObjName = Parts For Gate Maturation \n");
												    ITK_APICALL(FL_ask_size(secObjTags[j],&folderSize),flogPtr);			
													ITK_APICALL(FL_ask_references(secObjTags[j],FL_fsc_by_date_modified,&fldObjCount,&fldObjTags),flogPtr);
													
													fprintf(flogPtr,"folderSize           - %d \n", folderSize);
													fprintf(flogPtr,"fldObjCount          - %d \n", fldObjCount);
													
													for(k=0;k<fldObjCount;k++)
													{
														ITK_APICALL(AOM_ask_value_string(fldObjTags[k],"item_id",&subsecItemId),flogPtr);
														ITK_APICALL(AOM_ask_value_string(fldObjTags[k],"item_revision_id",&subsecItemRevId),flogPtr);
														ITK_APICALL(AOM_UIF_ask_value(fldObjTags[k],"t5_BOMCmpl",&subsect5BomCmpl),flogPtr);
														ITK_APICALL(AOM_UIF_ask_value(fldObjTags[k],"t5_RolledupWt",&subsect5RolledUpWt),flogPtr);
														ITK_APICALL(AOM_UIF_ask_value(fldObjTags[k],"t5_TargetWt",&subSecTagWt),flogPtr);
														ITK_APICALL(AOM_UIF_ask_value(fldObjTags[k],"t5_PartStatus",&subsect5PartStatus),flogPtr);
														ITK_APICALL(AOM_UIF_ask_value(fldObjTags[k],"t5_DesignGrp",&subsect5DesignGroup),flogPtr);
														ITK_APICALL(AOM_UIF_ask_value(fldObjTags[k],"object_type",&subsecObjecttype),flogPtr);
														ITK_APICALL(AOM_UIF_ask_value(fldObjTags[k],"t5_ProjectCode",&subsect5ProjectCode),flogPtr);
														ITK_APICALL(AOM_ask_value_string(fldObjTags[k],"object_name",&subsecObjName),flogPtr);    

														fprintf(flogPtr,"************************************************* \n");
														fprintf(flogPtr,"		subsecItemId        - %s \n", subsecItemId        );
														fprintf(flogPtr,"		subsecItemRevId     - %s \n", subsecItemRevId     );
														fprintf(flogPtr,"		subsect5BomCmpl     - %s \n", subsect5BomCmpl     );
														fprintf(flogPtr,"		subsect5RolledUpWt  - %s \n", subsect5RolledUpWt  );
														fprintf(flogPtr,"		subsect5PartStatus  - %s \n", subsect5PartStatus  );
														fprintf(flogPtr,"		subsect5DesignGroup - %s \n", subsect5DesignGroup );
														fprintf(flogPtr,"		subsecObjecttype    - %s \n", subsecObjecttype    );
														fprintf(flogPtr,"		subsect5ProjectCode - %s \n", subsect5ProjectCode );
														fprintf(flogPtr,"		subsecObjName       - %s \n", subsecObjName       );
														fprintf(flogPtr,"		subSecTagWt         - %s \n", subSecTagWt       );
																										
														fprintf(fPtr,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",subsecItemId,subsecItemRevId,subsect5BomCmpl,subsect5RolledUpWt,subSecTagWt,subsect5PartStatus,subsect5DesignGroup,subsecObjecttype,subsect5ProjectCode); 																						
													}												
												}																
											}
										}
										else
										{
											fprintf(flogPtr," Else condition execute where secObjCount less than zero \n");
										}																			
									}
									else
									{
										fprintf(flogPtr," Skipping for condition where _CL,_APL,_0E,_MBOM are present \n");
									}										
								}
								else
								{
									fprintf(flogPtr,"Else condition passed with invalid t5ReleaseType\n");
								}																
							}
							fclose(fPtr);
							itkApiCallStatus=ITK_APICALL(AE_find_datasettype2("MSExcel",&datasetType),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for AE_find_datasettype2 = %d \n",itkApiCallStatus);
							
							itkApiCallStatus=ITK_APICALL(AE_create_dataset_with_id(datasetType,dataSetName,datasetDesc,datasetId,"A",&datasetTag),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for AE_create_dataset_with_id = %d \n",itkApiCallStatus);
							
							fprintf(flogPtr,"csvFileName before API call   - %s \n", csvFileName);
													
							itkApiCallStatus=ITK_APICALL(AE_import_named_ref(datasetTag,"excel",csvFileName,dataSetName,SS_TEXT),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for AE_import_named_ref = %d \n",itkApiCallStatus);
							
							itkApiCallStatus=ITK_APICALL(AOM_refresh(datasetTag,TRUE),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for AOM_refresh = %d \n",itkApiCallStatus);
							
							itkApiCallStatus=ITK_APICALL(AOM_save(datasetTag),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for AOM_save = %d \n",itkApiCallStatus); 
							
							itkApiCallStatus=ITK_APICALL(GRM_find_relation_type("IMAN_reference", &relationTag),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for GRM_find_relation_type = %d \n",itkApiCallStatus);
							
							itkApiCallStatus=ITK_APICALL(GRM_create_relation(itemRevTag,datasetTag,relationTag,NULLTAG,&nRelationTag),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for GRM_create_relation = %d \n",itkApiCallStatus);
							
							itkApiCallStatus=ITK_APICALL(GRM_save_relation(nRelationTag),flogPtr);
							fprintf(flogPtr,"itkApiCallStatus for GRM_save_relation = %d \n",itkApiCallStatus);
							
							fprintf(flogPtr," \n");
							fprintf(flogPtr,"\n Exectution successfull.\n");
							
							fclose(flogPtr);
						}
						else
						{
							fprintf(flogPtr,"taskCount Less than zero - %d \n", taskCount);
						}						 						
					}
					else
					{
						fprintf(flogPtr,"Error : ItemRev not found in TC \n",dmlNo);
					}						
				}
				else
				{
					fprintf(flogPtr,"Error : Item with %s id not found in TC \n",dmlNo);
				}											
			}
			else
			{
				fprintf(flogPtr,"\n File not found or Error \n");
			}						
	    }
	   else
	   {
		  printf("\n Login failed in TC \n");	
		  fprintf(flogPtr,"\n Login failed in TC \n");
	   }
	}
	return(ITK_ok);
}