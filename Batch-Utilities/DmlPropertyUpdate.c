/* Objective :- Login Test */

/*Required headers */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/custom.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <res/res_itk.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <fclasses/tc_string.h>
#include <qry/qry.h>

int funExeCount  = 1;
int reasonFound  = 0;
int reasonIndex  = 0;  
int successCheck = 0;

const char *reasonDisplayList[43] = {
                    "NEW REQUIREMENTS (NEW VC, NEW AGGREGATE)",
                    "QUALITY & RELIABILITY IMPROVEMENT",
                    "TO FACILITATE MANUFACTURING",
                    "END CUSTOMER/FIELD COMPLAINTS & SERVICE FEEDBACK/CCT",
                    "COST REDUCTION",
                    "SAFETY REQUIREMENT",
                    "WEIGHT REDUCTION",
                    "REGULATORY REQUIREMENT",
                    "DRAFTING ERROR CORRECTION/BOM-CAD QUALITY IMPROVEMENT",
                    "ALTERNATE SOURCE DEVELOPMENT",
                    "COMMONIZATION/STANDARDIZATION/COMPLEXITY REDUCTION",
                    "INTEGRATED COST REDUCTION",
                    "WARRANTY COST REDUCTION",
                    "IMPROVEMENT IN JD POWER INITIAL QUALITY SCORE",
                    "CHANGE IN FEATURE LIST",
                    "MODULE MASTER DATA MANAGEMENT REQUEST",
                    "VC Attribute Change",
                    "QUALITY",
                    "ATTRIBUTE REQUIREMENT",
                    "ENGINEERING MATURITY (CAE, STYLING, VALIDATION, PROTO FEEDBACK ETC.)",
                    "DFA FEEDBACK",
                    "DFS FEEDBACK",
                    "DFC FEEDBACK",
                    "CONDITION OF SUPPLY CHANGE",
                    "SCOPE CHANGES",
                    "RELEASE OF SPARES/KITS (NO NEW PART DEVELOPMENT)",
                    "INFO-FITMENT DRAWING (IFD)",
                    "ECU CALIBRATION FILE RELEASE",
                    "SAMPLES TO BE APPROVED",
                    "DFM",
                    "REGULATORY / SAFETY REQUIREMENTS",
                    "DESIGN MATURATION",
                    "STYLING SURFACES (CAS)",
                    "EVI REPLACEMENTS (BLACK BOX)",
                    "FEATURE ADDITION",
                    "BTS SUPPLIER CHANGE (GREY BOX)",
                    "DVLO ISSUES",
                    "4D CHANGES",
                    "VALIDATION / CAE FEEDBACK",
                    "TO FACILITATE MANUFACTURING/ PROCESS CHANGES FOR QUALITY RELIABILITY IMPROVEMENT",
                    "INTEGRATED COST REDUCTION / WEIGHT REDUCTION / COMPLEXITY REDUCTION",
                    "QUALITY, WARRANTY & RELIABILITY IMPROVEMENT",
                    "NON-ERC DML"};

const char *reasonRealNameList[43] = {
	"NR",
	"QR",
	"FM",
	"CM",
	"CR",           
	"SR",             
	"WR",            
	"RR",            
	"DR",             
	"AD",             
	"CMR",             
	"ICR",             
	"WCR",             
	"PQ",             
	"CFL",             
	"MMDMR",             
	"VAC",             
	"QU",             
	"ATR",             
	"EM",             
	"DFAF",             
	"DFSF",             
	"DFCF",             
	"CSC",             
	"SC",             
	"RSP",             
	"IFD",             
	"ECFR",             
	"SA",             
	"DFM",             
	"RSR",             
	"DM",             
	"SCAS",             
	"EVI",             
	"FA",             
	"BTSC",             
    "DVLO",             
    "4DCHANGE",             
    "VCAE",             
    "FMPCQRI",             
    "ICWCR",             
    "QRI",             
    "NON-ERC DML" };              

/* ITK Error Rapper function */

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

/* PropUpdateFunction Inplementation */

int PropUpdateFunction(char* propNameValue,char* propSetValue, tag_t trgObgTag,FILE* Flogfile)
{
	int status = 2;
	int i = 0;	
	reasonFound = 0;
	char *newReasonUpdated = (char*) MEM_alloc(50*sizeof(char));
	
	fprintf(Flogfile,"\n ################# Inside PropUpdateFunction execution count = %d #################\n",funExeCount);
	fprintf(Flogfile," -propNameValue = %s\n",propNameValue);
	fprintf(Flogfile," -propSetValue  = %s\n",propSetValue);
	
	if(trgObgTag!=NULLTAG)
	{
		if(tc_strcmp(propNameValue,"t5_reason")==0)
		{
			fprintf(Flogfile,"\n \n Execting If condition where Property IS equal == t5_reason \n");
			for(i=0;i<44;i++)
			{
				if(tc_strcmp(reasonDisplayList[i],propSetValue)==0)
				{
					reasonFound = 1;
					reasonIndex = i;
					break; 
				}
			}
			fprintf(Flogfile,"\n -reasonFound = %d",reasonFound);
		    if(reasonFound==1)
			{
				tc_strcpy(newReasonUpdated,reasonRealNameList[i]);				
				fprintf(Flogfile,"\n -newReasonUpdated = %s \n",newReasonUpdated);
				
				fprintf(Flogfile," - API STATUS AOM_lock                  = %d \n",ITK_APICALL(AOM_lock(trgObgTag),Flogfile));
		        fprintf(Flogfile," - API STATUS AOM_refresh               = %d \n",ITK_APICALL(AOM_refresh(trgObgTag,TRUE),Flogfile));		   
		        fprintf(Flogfile," - API STATUS AOM_UIF_set_value         = %d \n",ITK_APICALL(AOM_UIF_set_value(trgObgTag,propNameValue,newReasonUpdated),Flogfile));
		        fprintf(Flogfile," - API STATUS AOM_save_with_extensions  = %d \n",ITK_APICALL(AOM_save_without_extensions(trgObgTag),Flogfile));
		        fprintf(Flogfile," - API STATUS AOM_unlock                = %d \n",ITK_APICALL(AOM_unlock(trgObgTag),Flogfile));
		        fprintf(Flogfile," - API STATUS AOM_refresh               = %d \n",ITK_APICALL(AOM_refresh(trgObgTag,FALSE),Flogfile));
			}						
		}
		else
		{
			fprintf(Flogfile,"\n \n Execting else condition where Property not equal to t5_reason \n");
	        fprintf(Flogfile," - API STATUS AOM_lock                  = %d \n",ITK_APICALL(AOM_lock(trgObgTag),Flogfile));
		    fprintf(Flogfile," - API STATUS AOM_refresh               = %d \n",ITK_APICALL(AOM_refresh(trgObgTag,TRUE),Flogfile));
		    fprintf(Flogfile," - API STATUS AOM_set_value_string      = %d \n",ITK_APICALL(AOM_set_value_string(trgObgTag,propNameValue,propSetValue),Flogfile));		    
		    fprintf(Flogfile," - API STATUS AOM_save_with_extensions  = %d \n",ITK_APICALL(AOM_save_without_extensions(trgObgTag),Flogfile));
		    fprintf(Flogfile," - API STATUS AOM_unlock                = %d \n",ITK_APICALL(AOM_unlock(trgObgTag),Flogfile));
		    fprintf(Flogfile," - API STATUS AOM_refresh               = %d \n",ITK_APICALL(AOM_refresh(trgObgTag,FALSE),Flogfile));
		}
	}
	else
	{
		fprintf(Flogfile,"\n Error : Real Object tag not found in PropUpdateFunction property updation failed \n");
		status=3;
	}
	fprintf(Flogfile,"\n ################################## outside PropUpdateFunction ################### \n \n ");
	funExeCount++;
	return(status);
}

/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	                = NULL;
    char *sPasswordFile	            = NULL;
    char *sGroup                    = NULL;
	char *sParNo                    = NULL;
	char *itemIdInfo                = NULL;
	char *loggedInUser              = NULL;
    char *shellExecute              = (char*) MEM_alloc(200*sizeof(char));
	char *logPath                   = (char*)malloc(200*sizeof(char));
	char *prefValue                 = NULL;	
	char *timeStr                   = NULL;
	char *timeStrUpdated            = NULL;
	char *itemId                    = NULL; 
	char *itemRevId                 = NULL;
	char *objName                   = NULL;
	char *objDesc                   = NULL;
	char *t5TMLManager              = NULL;
	char *t5Role                    = NULL; 
	char *t5ParRequestProjectCode   = NULL;
	char *trgObjId                  = (char*) MEM_alloc(200*sizeof(char));
	char *trgObjRevId               = (char*) MEM_alloc(200*sizeof(char));
	char *trgObjSequesnce           = (char*) MEM_alloc(200*sizeof(char));
	char *temp                      = (char*) MEM_alloc(5000*sizeof(char));
	char *qryValue1                 = (char*) MEM_alloc(200*sizeof(char));
	char *propName                  = (char*) MEM_alloc(5000*sizeof(char));
    char *propValue                 = (char*) MEM_alloc(5000*sizeof(char)); 
	char filedata[5000];
	char *qryInput[1] = {"Item ID"};
    char *qryValues[1];	
	char *tcItemRevId              = NULL;
	char *tcCurrentRevId           = NULL;
	
	int itkApiCallStatus = 0;
	int qryObjectCount   = 0;
	int itr=0;
	int itrq=0;
	int i=0;
	int j=0;
	int k=0;
	int t1=0;
	int t2=0;
	int h1=0;
	int h2=0;
	int h3=0;
	int tcSequence       = 0;
	int temptcSequence   = 0;
	int found1           = 0;

	tag_t loggedInUserTag    =  NULLTAG;
	tag_t qryTag             =  NULLTAG;
		
	tag_t *qryObjectTag      =  NULLTAG;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	
	tc_strcpy(logPath,"/tmp/");
	tc_strcat(logPath,"DML_PROP_UPD_");
	tc_strcat(logPath,timeStrUpdated);
	tc_strcat(logPath,".txt");

    FILE *flogPtr = fopen(logPath,"w+");
	
	
	tag_t itemTag    = NULLTAG;
	tag_t itemRevTag = NULLTAG;
	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	sParNo         = ITK_ask_cli_argument("-parno=");
	itemIdInfo     = ITK_ask_cli_argument("-itemid=");
	
	fprintf(flogPtr,"-sUserName     = %s\n",sUserName     );
	fprintf(flogPtr,"-sPasswordFile = %s\n",sPasswordFile );
	fprintf(flogPtr,"-sGroup        = %s\n",sGroup        );
	fprintf(flogPtr,"-sParNo        = %s\n",sParNo        );
	fprintf(flogPtr,"-itemIdInfo    = %s\n",itemIdInfo    );
	
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
		    fprintf(flogPtr,"\nName of the logged in user = %s",loggedInUser);	
			itkApiCallStatus=ITK_APICALL(ITEM_find_item(sParNo,&itemTag),flogPtr);
			fprintf(flogPtr," API Status for ITEM_find_item = %d \n",itkApiCallStatus);
			if(itkApiCallStatus==0&&itemTag!=NULLTAG)
			{
				itkApiCallStatus=ITK_APICALL(ITEM_ask_latest_rev(itemTag,&itemRevTag),flogPtr);
				fprintf(flogPtr," API Status for ITEM_ask_latest_rev = %d \n",itkApiCallStatus);
				if(itemRevTag!=NULLTAG)
				{
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"item_id",                  &itemId                  ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"item_revision_id",         &itemRevId               ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"object_name",              &objName                 ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"object_desc",              &objDesc                 ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_Role",                  &t5Role                  ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_ParRequestProjectCode", &t5ParRequestProjectCode ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_TML_Manager",           &t5TMLManager            ),flogPtr);
					
					fprintf(flogPtr," - itemId                  = %s \n",itemId                 );
					fprintf(flogPtr," - itemRevId               = %s \n",itemRevId              );
					fprintf(flogPtr," - objName                 = %s \n",objName                );
					fprintf(flogPtr," - objDesc                 = %s \n",objDesc                );
					fprintf(flogPtr," - t5Role                  = %s \n",t5Role                 );
					fprintf(flogPtr," - t5ParRequestProjectCode = %s \n",t5ParRequestProjectCode);
					fprintf(flogPtr," - t5TMLManager            = %s \n",t5TMLManager           );
					
				    for(i=0;t5TMLManager[i]!='\0';i++)
					{
						if(t5TMLManager[i]==',')
						{
							temp[t1]='\0';
							if(t2==0)
							{
								tc_strcpy(trgObjId,temp);
							}
							if(t2==1)
							{
								tc_strcpy(trgObjRevId,temp);
							}
							if(t2==2)
							{
								tc_strcpy(trgObjSequesnce,temp);
							}
							tc_strcpy(temp,"");
							t2++;
							t1=0;
						}
						else
						{
							temp[t1]=t5TMLManager[i];
							t1++;
						}
						
					}
					tc_strcpy(qryValue1,trgObjRevId);
					tc_strcat(qryValue1,";");
					tc_strcat(qryValue1,trgObjSequesnce);
			     
				    fprintf(flogPtr,"\n");
					fprintf(flogPtr,"\n");
					fprintf(flogPtr,"\n *************************************************************************************** \n");
			  
					fprintf(flogPtr," - trgObjId          = %s \n",trgObjId        );
					fprintf(flogPtr," - trgObjRevId       = %s \n",trgObjRevId     );
					fprintf(flogPtr," - trgObjSequesnce   = %s \n",trgObjSequesnce );
					fprintf(flogPtr," - qryValue1         = %s \n",qryValue1 );
						
					qryValues[0] = trgObjId;
					itkApiCallStatus=ITK_APICALL(QRY_find2("Item Revision...",&qryTag),flogPtr);
				    fprintf(flogPtr," API Status for QRY_find2 = %d \n",itkApiCallStatus);
					if(qryTag!=NULLTAG)
					{
						itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag, 1, qryInput, qryValues, &qryObjectCount, &qryObjectTag),flogPtr); 
					    fprintf(flogPtr," Total qryObjectCount =  %d \n",qryObjectCount);
						if(qryObjectCount > 0)
						{
							for(itrq=0;itrq<qryObjectCount;itrq++)
							{
								ITK_APICALL(AOM_ask_value_string(qryObjectTag[itrq],"item_revision_id"   ,&tcItemRevId     ),flogPtr);
								ITK_APICALL(AOM_ask_value_string(qryObjectTag[itrq],"current_revision_id",&tcCurrentRevId  ),flogPtr);
								ITK_APICALL(AOM_ask_value_int(qryObjectTag[itrq]   ,"sequence_id"        ,&tcSequence      ),flogPtr);
								
								fprintf(flogPtr," - tcItemRevId    = %s \n"  ,tcItemRevId    );
								fprintf(flogPtr," - tcCurrentRevId = %s \n"  ,tcCurrentRevId );
								fprintf(flogPtr," - tcSequence     = %d \n"  ,tcSequence     );
								
								temptcSequence=tcSequence;
								
								if(tc_strstr(tcItemRevId,";")==NULL)
								{
									fprintf(flogPtr," (;) not found in revId condition \n"); 
									h1=0;
									h2=0;
									h3=0;
									char *trgsubSequence    = (char*)MEM_alloc(10*sizeof(char));
									char *trgSeqAftconv     = (char*)MEM_alloc(10*sizeof(char));	
                                    char *newtrgSeqAftconv  = (char*)MEM_alloc(10*sizeof(char));									
									while(tcSequence!=0)
									{
										h1=tcSequence%10; 
										tcSequence=tcSequence/10; 
										trgsubSequence[h2]=(char)(48+h1);
										h2++;										
									}
									trgsubSequence[h2]='\0';
									fprintf(flogPtr," -trgsubSequence = %s \n",trgsubSequence);
									h2=h2-1;
									while(h2!=-1)
									{
										trgSeqAftconv[h3]=trgsubSequence[h2];
										h3++;
										h2--;
									}
									trgSeqAftconv[h3]='\0';										
									fprintf(flogPtr," -trgSeqAftconv = %s \n",trgSeqAftconv);
									
									tc_strcpy(newtrgSeqAftconv,"");
									tc_strcpy(newtrgSeqAftconv,tcItemRevId);
									tc_strcat(newtrgSeqAftconv,";");
									tc_strcat(newtrgSeqAftconv,trgSeqAftconv);
									
									fprintf(flogPtr," -newtrgSeqAftconv = %s \n",newtrgSeqAftconv);
									if(tc_strcmp(newtrgSeqAftconv,qryValue1)==0)
									{
										fprintf(flogPtr," ******** Targeted Item Revision successfully found Property Updation started ******** \n");
										t1=0;
										tc_strcpy(temp,"");
										if(tc_strlen(t5Role)>0&&tc_strcmp(t5Role,"")!=0)
										{
											fprintf(flogPtr,"\n ******** If condition executed where tc_strlen(t5Role)>0 and t5Role !=NULL ******** \n");
											for(itr=0;t5Role[itr]!='\0';itr++)
										 	{
										 		if(t5Role[itr]=='^')
										 		{
										 			temp[t1]='\0';
										 			tc_strcpy(propName,temp);
										 			tc_strcpy(temp,"");
										 			t1=0;
										 		}
										 		else if(t5Role[itr]=='~')
										 		{
										 			temp[t1]='\0';
										 			tc_strcpy(propValue,temp);
										 			tc_strcpy(temp,"");
										 			t1=0;
										 	        PropUpdateFunction(propName,propValue,qryObjectTag[itrq],flogPtr);
										 			tc_strcpy(propName,"");
										 			tc_strcpy(propValue,"");
										 		}
										 		else
										 		{
										 			temp[t1]=t5Role[itr];
										 			t1++;
										 		}
										 	}
										}
										if(tc_strlen(t5ParRequestProjectCode)>0&&tc_strcmp(t5ParRequestProjectCode,"")!=0)
										{
											fprintf(flogPtr,"\n ******** If condition executed where tc_strlen(t5ParRequestProjectCode)>0 and t5ParRequestProjectCode !=NULL ******** \n");
											for(itr=0;t5ParRequestProjectCode[itr]!='\0';itr++)
											{
												if(t5ParRequestProjectCode[itr]=='^')
												{
													temp[t1]='\0';
													tc_strcpy(propName,temp);
													tc_strcpy(temp,"");
													t1=0;
												}
												else if(t5ParRequestProjectCode[itr]=='~')
												{
													temp[t1]='\0';
													tc_strcpy(propValue,temp);
													tc_strcpy(temp,"");
													t1=0;
											        PropUpdateFunction(propName,propValue,qryObjectTag[itrq],flogPtr);
													tc_strcpy(propName,"");
													tc_strcpy(propValue,"");
												}
												else
												{
													temp[t1]=t5ParRequestProjectCode[itr];
													t1++;
												}
											}
										}
										found1=1;																
                                        break;
									}																
								}
								else
								{
									fprintf(flogPtr," (;) found in revId condition \n");
									if(tc_strcmp(tcItemRevId,qryValue1)==0)
									{
										fprintf(flogPtr," ******** Targeted Item Revision successfully found Property Updation started ******** \n");
										t1=0;
										tc_strcpy(temp,"");
										if(tc_strlen(t5Role)>0&&tc_strcmp(t5Role,"")!=0)
										{
											fprintf(flogPtr,"\n ******** If condition executed where tc_strlen(t5Role)>0 and t5Role !=NULL ******** \n");
											for(itr=0;t5Role[itr]!='\0';itr++)
									     	{
									     		if(t5Role[itr]=='^')
									     		{
									     			temp[t1]='\0';
									     			tc_strcpy(propName,temp);
									     			tc_strcpy(temp,"");
									     			t1=0;
									     		}
									     		else if(t5Role[itr]=='~')
									     		{
									     			temp[t1]='\0';
									     			tc_strcpy(propValue,temp);
									     			tc_strcpy(temp,"");
									     			t1=0;
									     	        PropUpdateFunction(propName,propValue,qryObjectTag[itrq],flogPtr);
									     			tc_strcpy(propName,"");
									     			tc_strcpy(propValue,"");
									     		}
									     		else
									     		{
									     			temp[t1]=t5Role[itr];
									     			t1++;
									     		}
									     	}
										}
										if(tc_strlen(t5ParRequestProjectCode)>0&&tc_strcmp(t5ParRequestProjectCode,"")!=0)
										{
											fprintf(flogPtr,"\n ******** If condition executed where tc_strlen(t5ParRequestProjectCode)>0 and t5ParRequestProjectCode !=NULL ******** \n");
											for(itr=0;t5ParRequestProjectCode[itr]!='\0';itr++)
									    	{
									    		if(t5ParRequestProjectCode[itr]=='^')
									    		{
									    			temp[t1]='\0';
									    			tc_strcpy(propName,temp);
									    			tc_strcpy(temp,"");
									    			t1=0;
									    		}
									    		else if(t5ParRequestProjectCode[itr]=='~')
									    		{
									    			temp[t1]='\0';
									    			tc_strcpy(propValue,temp);
									    			tc_strcpy(temp,"");
									    			t1=0;
									    	        PropUpdateFunction(propName,propValue,qryObjectTag[itrq],flogPtr);
									    			tc_strcpy(propName,"");
									    			tc_strcpy(propValue,"");
									    		}
									    		else
									    		{
									    			temp[t1]=t5ParRequestProjectCode[itr];
									    			t1++;
									    		}
									    	}
										}
										found1=1;
                                        break;
									}
								}
							}
							fprintf(flogPtr,"\n -found1 = %d \n",found1);
							
							if(found1==0)
							{
							    fprintf(flogPtr,"\n Error : ******************** ItemRev not Found execution not completed or failed !!!!! ******************** \n "); 	
							}
							if(found1==1)
							{
								fprintf(flogPtr,"\n ******************** ItemRev Found execution completed ******************** \n ");
								successCheck=1;
							}
						}
						else
						{
							fprintf(flogPtr,"\n Error : Executing condition where Rev Count <= 0 \n ");
						}
				    }
					else
					{
						fprintf(flogPtr,"\n Error : Query not found   \n");
					}
				}
				else
				{
					fprintf(flogPtr,"\n Error : ItemRevision for with %s itemId not found  .... \n",sParNo);
				}
			}
			else
			{
				fprintf(flogPtr,"\n Error : Item with %s itemId not found  .... \n",sParNo);
			}
	    }
		if(successCheck==1 && itemTag!=NULLTAG)
		{
			fprintf(flogPtr,"\n ************ PAR Object deletion started for Id = %s ************\n",sParNo);
		    itkApiCallStatus=ITK_APICALL(AOM_lock_for_delete(itemTag),flogPtr);
		    fprintf(flogPtr,"API Status for AOM_lock_for_delete = %d \n",itkApiCallStatus);
		    itkApiCallStatus=ITK_APICALL(AOM_delete(itemTag),flogPtr);
		    fprintf(flogPtr,"API Status for AOM_delete = %d \n",itkApiCallStatus);
		    fprintf(flogPtr,"\n ************ PAR Object deletion successful ************ \n ");
		}
		else
		{
			fprintf(flogPtr,"\n ERROR : ************ PAR Object deletion Stopped for Id = %s because successCheck == 0 or Item not found. ************\n",sParNo);
		}
	}
	else
	{
	    printf("\n Login failed in TC \n");	
	    fprintf(flogPtr,"\n Error : Login failed in TC \n");
	}
	return(ITK_ok);
}

