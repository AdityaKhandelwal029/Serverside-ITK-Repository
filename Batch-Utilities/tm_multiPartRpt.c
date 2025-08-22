/* Objective :- Part PO Front end option exe */

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
#include <sa/user.h>
#include <tc/folder.h>
#include <ae/datasettype.h>
#include <ae/dataset.h>

int funExeCount  = 1;
int reasonFound  = 0;
int reasonIndex  = 0;  
int successCheck = 0;

struct queryReturnObj
{
	tag_t* qryStructTags; 
	int qryObjCount; 
};

const char* month_names[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

char* convert_date_format(const char *input) {
    
    char *output_date = (char*)malloc(100*sizeof(char));
    
    int year, month, day, hour, minute, second;
    sscanf(input, "%d/%d/%d-%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    sprintf(output_date, "%02d-%s-%04d %02d:%02d",
            day, month_names[month - 1], year, hour, minute);
            
    return(output_date);
}

// Function to convert month abbreviation to month number
const char* get_month_number(const char* month_abbr) {
    if (strcmp(month_abbr, "Jan") == 0) return "01";
    if (strcmp(month_abbr, "Feb") == 0) return "02";
    if (strcmp(month_abbr, "Mar") == 0) return "03";
    if (strcmp(month_abbr, "Apr") == 0) return "04";
    if (strcmp(month_abbr, "May") == 0) return "05";
    if (strcmp(month_abbr, "Jun") == 0) return "06";
    if (strcmp(month_abbr, "Jul") == 0) return "07";
    if (strcmp(month_abbr, "Aug") == 0) return "08";
    if (strcmp(month_abbr, "Sep") == 0) return "09";
    if (strcmp(month_abbr, "Oct") == 0) return "10";
    if (strcmp(month_abbr, "Nov") == 0) return "11";
    if (strcmp(month_abbr, "Dec") == 0) return "12";
    return "00"; // default/fallback
}

char* convert_reversedate_format(const char *input) {
    
   // char input[] = "09-Nov-2022 00:00";
    char day[3], month[4], year[5], hour[3], minute[3];
    
    // Parse input string
    sscanf(input, "%2s-%3s-%4s %2s:%2s", day, month, year, hour, minute);

    // Convert to desired format
    const char* month_num = get_month_number(month);

    char output[25]; 
    
    char *output1 = (char*)malloc(100*sizeof(char));
    snprintf(output, sizeof(output), "%s/%s/%s-%s:%s:00", year, month_num, day, hour, minute);

    printf("Converted format: %s\n", output);
    
    output1 = output;
    
    return(output1);
}

	
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

/* Query Data function */

struct queryReturnObj queryData(char *queryName,char *qEntry,char* qValues,int queryPrmCount ,FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	int itr  = 0;
	int itr2 = 0;
	int itj  = 0;
	int itk1 = 0;
	int itk2 = 0;
	
	char *queryRealName    = NULL;

	char *tEntryName       = (char*) MEM_alloc(100*sizeof(char));
	char *tValueName       = (char*) MEM_alloc(100*sizeof(char));

	char *queryEntryInput [queryPrmCount];
	char *queryValuesInput[queryPrmCount];
		
	tag_t qryTag = NULLTAG;
	
	struct queryReturnObj structObj;
	structObj.qryStructTags = NULLTAG;
    structObj.qryObjCount   = 0;
	
	fprintf(flogPtr,"\n *********** Execution of queryData function started *********** \n");
	
	fprintf(flogPtr,"\n - queryName        = %s",queryName);
	fprintf(flogPtr,"\n - qEntry           = %s",qEntry);
	fprintf(flogPtr,"\n - qValues          = %s",qValues);
	fprintf(flogPtr,"\n - queryPrmCount    = %d",queryPrmCount);
	
	for(itr = 0;qEntry[itr]!='\0';itr++)
	{
		if(qEntry[itr] == ',')
		{
			tEntryName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tEntryName = %s",tEntryName);
			queryEntryInput[itk1] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryEntryInput[itk1],tEntryName);
			itk1++;
			
		}
		else if(itr+1 == tc_strlen(qEntry))
		{
			tEntryName[itj] = qEntry[itr];
			itj++;
			tEntryName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tEntryName = %s",tEntryName);
			queryEntryInput[itk1] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryEntryInput[itk1],tEntryName);				
		}
		else
		{
			tEntryName[itj] = qEntry[itr];
			itj++;
		}
	}
	for(itr = 0;qValues[itr]!='\0';itr++)
	{
		if(qValues[itr] == ',')
		{
			tValueName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tValueName = %s",tValueName);
			queryValuesInput[itk2] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryValuesInput[itk2],tValueName);
			itk2++;
		}
		else if(itr+1 == tc_strlen(qValues))
		{
			tValueName[itj] = qValues[itr];
			itj++;
			tValueName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tValueName = %s",tValueName);
			queryValuesInput[itk2] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryValuesInput[itk2],tValueName);
		}
		else
		{
			tValueName[itj] = qValues[itr];
			itj++;
		}
	}
	for(itr = 0;itr < queryPrmCount;itr++)
	{
		fprintf(flogPtr,"\n -queryEntryInput[%d] = %s - queryValuesInput[%d] = %s",itr,queryEntryInput[itr],itr,queryValuesInput[itr]);
	}
	itkApiCallStatus=ITK_APICALL(QRY_find2(queryName,&qryTag),flogPtr);
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n Query found .... ");
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryRealName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,queryPrmCount,queryEntryInput,queryValuesInput,&structObj.qryObjCount,&structObj.qryStructTags),flogPtr);
		fprintf(flogPtr,"\n -queryRealName   = %s",queryRealName);
		fprintf(flogPtr,"\n -qryObjCount     = %d",structObj.qryObjCount);
        
		if(structObj.qryObjCount>0)
		{
			fprintf(flogPtr,"\n *************** Execting condition where qryObjCount > 0 ************** ");	
		}
		else
		{
			fprintf(flogPtr,"\n Execting condition where qryObjCount <= 0 ");
		}		
	}
	else
	{
		fprintf(flogPtr,"\n Error : Query not found .... ");
	}
	fprintf(flogPtr," \n *********** Execution of queryData function Ended *********** \n");
	return(structObj);
}


/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	                = NULL;
    char *sPasswordFile	            = NULL;
    char *sGroup                    = NULL;
	char *sParNo                    = NULL;
	char *sLoggedUser               = NULL;
	char *itemIdInfo                = NULL;
	char *loggedInUser              = NULL;
    char *shellExecute              = (char*) MEM_alloc(200*sizeof(char));
	char *logPath                   = (char*)malloc(200*sizeof(char));
	char *csvPath                   = (char*)malloc(200*sizeof(char));
	char *prefValue                 = NULL;	
	char *timeStr                   = NULL;
	char *timeStrUpdated            = NULL;
	char *itemId                    = NULL; 
	char *itemRevId                 = NULL;
	char *objName                   = NULL;
	char *vendor_id                 = NULL;
	char *plant                     = NULL;
	char *POnum                     = NULL;
	char *podate                    = NULL;
	char *porev                     = NULL;
	char *price                     = NULL;
	char *validity                  = NULL;
	char *stock                     = NULL;
	char *tran                      = NULL;
	char *vendor_name               = NULL;
	char *storeloc                  = NULL;
	char *objDesc                   = NULL;
	char *t5TMLManager              = NULL;
	char *DocumentA                 = NULL;
	char *t5Role                    = NULL; 
	char *t5ParRequestProjectCode   = NULL;
	char *trgObjId                  = (char*) MEM_alloc(200*sizeof(char));
	char *trgObjRevId               = (char*) MEM_alloc(200*sizeof(char));
	char *trgObjSequesnce           = (char*) MEM_alloc(200*sizeof(char));
	char *temp                      = (char*) MEM_alloc(5000*sizeof(char));
	char *qryValue1                 = (char*) MEM_alloc(200*sizeof(char));
	char *propName                  = (char*) MEM_alloc(5000*sizeof(char));
    char *propValue                 = (char*) MEM_alloc(5000*sizeof(char)); 
	char *dateFormatString          = (char*) MEM_alloc(5000*sizeof(char)); 
	char filedata[5000];
	char *qryInput[1] = {"Item ID"};
    char *qryValues[1];	
	char *tcItemRevId              = NULL;
	char *tcCurrentRevId           = NULL;
	char itemIdPOItem[100];
	char *fldObjName          =   NULL;
	char *fldObjType          =   NULL;
	char *dataSetName               = NULL;
	char *FileLoc                   = NULL;
	
	char *maxDateStr = NULL;
	char *newReveresedDate = (char*) MEM_alloc(100*sizeof(char)); 
	
	char *nx1Str = (char*) MEM_alloc(100*sizeof(char)); 
	
	int itkApiCallStatus = 0;
	int qryObjectCount   = 0;
	int itr=0;
	int itrq=0;
	int i=0;
	int j=0;
	int k=0;
	int l=0;
	int t1=0;
	int t2=0;
	int h1=0;
	int h2=0;
	int h3=0;
	int tcSequence       = 0;
	int temptcSequence   = 0;
	int found            = 0;
	int idInt            = 0;
	int dataArryItr      = 0;
	int ix = 0;
	int jx = 0;
	int dMaxPosition = 0;
	int cmpResult = 0;
	int noOfReferences = 0;
    int fldFound = 0;
	 
	
    date_t dateOfPOObj;
	
	date_t dateArray[1000];
	
	date_t dateMax;

	tag_t loggedInUserTag    =  NULLTAG;
	tag_t sysUserTag                  = NULLTAG;
	
	tag_t sysHomeTag                  = NULLTAG;
	tag_t newFolderTag                = NULLTAG;
	tag_t *folderReference            = NULLTAG;

	
	tag_t datasetType                 = NULLTAG;
	tag_t datasetTag                  = NULLTAG;
	//tag_t qryTag             =  NULLTAG;
		
	//tag_t *qryObjectTag      =  NULLTAG;

	
	struct queryReturnObj qryObj1,qryObj2,qryObj3;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	
	tc_strcpy(logPath,"/tmp/");
	//tc_strcpy(logPath,"/user/cvprod/Program_Shells/DML_Support/Multi_Part_PO/11/");
	tc_strcat(logPath,"Multi_Part_PO");
	tc_strcat(logPath,timeStrUpdated);
	tc_strcat(logPath,".txt");

    FILE *flogPtr = fopen(logPath,"w+");
	
	//tc_strcpy(csvPath,"/user/cvprod/Program_Shells/DML_Support/Multi_Part_PO/11/");
	tc_strcpy(csvPath,"/tmp/");
	tc_strcat(csvPath,"Multi_Part_PO");
	tc_strcat(csvPath,timeStrUpdated);
	tc_strcat(csvPath,".csv");
//	FileLoc = (char *) MEM_alloc( 100 * sizeof(char) );
//
//	tc_strcpy(FileLoc,"/tmp/");
//	//tc_strcpy(FileLoc,"/user/cvprod/Program_Shells/DML_Support/Multi_Part_PO/11/");
//	tc_strcat(FileLoc,csvPath);
//	printf("\n FileLoc: %s\n", FileLoc);fflush(stdout);
	
	FILE *fPtr = fopen(csvPath,"w+");
	
	
	
	tag_t itemTag    = NULLTAG;
	tag_t itemRevTag = NULLTAG;
	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");	
	sLoggedUser    = ITK_ask_cli_argument("-LoggedUser=");
	sParNo         = ITK_ask_cli_argument("-parno=");
	
	fprintf(flogPtr,"-sUserName     = %s\n",sUserName     );
	fprintf(flogPtr,"-sPasswordFile = %s\n",sPasswordFile );
	fprintf(flogPtr,"-sGroup        = %s\n",sGroup        );
	fprintf(flogPtr,"-sParNo        = %s\n",sParNo        );
	fprintf(flogPtr,"-sLoggedUser    = %s\n",sLoggedUser  );
	
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr);
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	if(itkApiCallStatus==ITK_ok)
    {
		printf(" \n ******************* Login successful to teamcenter ******************* \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC");	
		//fprintf(fPtr,"ID,VendorId,Vendor Name,Store Location \n");
		fprintf(fPtr,"ID,Plant,PO Number,PO Date,PO Part Rev,Net Price,VendorId,Vendor Name,Store Location,Validity Check,Stock,Transaction \n");
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
					
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_ParRequestProjectCode", &t5ParRequestProjectCode ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_Role",                  &t5Role                  ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_TML_Manager",           &t5TMLManager            ),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"DocumentAuthor",           &DocumentA            ),flogPtr);
					
					fprintf(flogPtr," - itemId                  = %s \n",itemId                 );
					fprintf(flogPtr," - itemRevId               = %s \n",itemRevId              );
					fprintf(flogPtr," - objName                 = %s \n",objName                );
					fprintf(flogPtr," - objDesc                 = %s \n",objDesc                );
					fprintf(flogPtr," - t5Role                  = %s \n",t5Role                 );
					fprintf(flogPtr," - t5ParRequestProjectCode = %s \n",t5ParRequestProjectCode);
					fprintf(flogPtr," - t5TMLManager            = %s \n",t5TMLManager           );
					fprintf(flogPtr," - DocumentAuthor          = %s \n",DocumentA              );
					
			     
				    fprintf(flogPtr,"\n");
					fprintf(flogPtr,"\n");
	                idInt = 0;
					
                    for(itr = 0;t5Role[itr]!='\0';itr++)						 	
					{
						//fprintf(flogPtr,"\n char value = %d char = %d",(int)t5Role[itr],t5Role[itr]);
						if((int)t5Role[itr] == 10)
						{
							// Execution of Query 
							itemIdPOItem[idInt] = '\0';
							tc_strcpy(trgObjId,itemIdPOItem);
							
							fprintf(flogPtr,"\n Item Id Found for PO Item itemIdPOItem = %s ",itemIdPOItem);
							fprintf(flogPtr,"\n Item Id Found for PO Item trgObjId = %s ",trgObjId);							
							idInt = 0;
							
							qryObj1 = queryData("Part PO Details","Name",trgObjId,1,flogPtr);							
							fprintf(flogPtr,"\n ***** QUERY ****  found = %d",qryObj1.qryObjCount);
							
							if(tc_strcmp(t5TMLManager,"1")==0)
							{
								dataArryItr = 0;
								fprintf(flogPtr,"\n Latest Date execution started ");
								for(j=0;j<qryObj1.qryObjCount;j++)
                                { 
							        ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Plant1",&plant),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO1",&POnum),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PODate1",&podate),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO_Revision",&porev),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_NetPrice1",&price),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_VendorID1",&vendor_id),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_BulkMaterial11",&validity),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Stock1",&stock),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Transaction1",&tran),flogPtr);
									
									fprintf(flogPtr,"\n -plant     = %s",plant);
									fprintf(flogPtr,"\n -POnum     = %s",POnum);
									fprintf(flogPtr,"\n -podate    = %s",podate);
									fprintf(flogPtr,"\n -porev     = %s",porev);
									fprintf(flogPtr,"\n -price     = %s",price);
									fprintf(flogPtr,"\n -vendor_id = %s",vendor_id);
									fprintf(flogPtr,"\n -validity  = %s",validity);
									fprintf(flogPtr,"\n -stock     = %s",stock);
									fprintf(flogPtr,"\n -tran      = %s",tran);
																		
							    
							    	if (tc_strstr(plant, t5ParRequestProjectCode) != 0)
							    	{
										dateFormatString = convert_date_format(podate);
										fprintf(flogPtr,"\n - dateFormatString = %s",dateFormatString);
										
										ITK_APICALL(ITK_string_to_date(dateFormatString,&dateOfPOObj),flogPtr);
										
										dateArray[dataArryItr] = dateOfPOObj;
										dataArryItr++;																				
									}
							    }
								
								dateMax = dateArray[0];
								fprintf(flogPtr,"\n - dataArryItr = %d ",dataArryItr);
								
								for(ix = 1;ix < dataArryItr;ix++)
								{
									ITK_APICALL(POM_compare_dates(dateMax,dateArray[ix],&cmpResult),flogPtr);
									
									if(cmpResult == -1)
									{
										dateMax = dateArray[ix];
									}
								}
								ITK_APICALL(ITK_date_to_string(dateMax,&maxDateStr),flogPtr);
								fprintf(flogPtr,"\n **************** maxDateStr = %s",maxDateStr);
								
								newReveresedDate = convert_reversedate_format(maxDateStr);
								
								fprintf(flogPtr,"\n *************** Final - newReveresedDate = %s ",newReveresedDate);
								
								tc_strcpy(nx1Str,newReveresedDate);
								
								for(jx = 0;jx<qryObj1.qryObjCount;jx++)
								{
									ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_Plant1",&plant),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_PO1",&POnum),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_PODate1",&podate),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_PO_Revision",&porev),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_NetPrice1",&price),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_VendorID1",&vendor_id),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_BulkMaterial11",&validity),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_Stock1",&stock),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_Transaction1",&tran),flogPtr);
									
									fprintf(flogPtr,"\n -plant     [2] = %s",plant);
									fprintf(flogPtr,"\n -POnum     [2] = %s",POnum);
									fprintf(flogPtr,"\n -podate    [2] = %s",podate);
									fprintf(flogPtr,"\n -porev     [2] = %s",porev);
									fprintf(flogPtr,"\n -price     [2] = %s",price);
									fprintf(flogPtr,"\n -vendor_id [2] = %s",vendor_id);
									fprintf(flogPtr,"\n -validity  [2] = %s",validity);
									fprintf(flogPtr,"\n -stock     [2] = %s",stock);
									fprintf(flogPtr,"\n -tran      [2] = %s",tran);
														
									fprintf(flogPtr,"\n ********************** Testing Data *************************** "); 
									fprintf(flogPtr,"\n ******* -plant                   = %s",plant);
									fprintf(flogPtr,"\n ******* -t5ParRequestProjectCode = %s",t5ParRequestProjectCode);
									fprintf(flogPtr,"\n ******* -podate                  = %s",podate);
									fprintf(flogPtr,"\n ******* -newReveresedDate        = %s",newReveresedDate);
									fprintf(flogPtr,"\n ******* -nx1Str        = %s",nx1Str);
									
									
							    
							    	if (tc_strstr(plant, t5ParRequestProjectCode)!= NULL && tc_strstr(nx1Str,podate)!= NULL)
							    	{
										fprintf(flogPtr,"\n ***************** Target PO Object with latest matching date found ");
										
										fprintf(flogPtr,"\n - Vendor_id = %s \n",vendor_id);
							    		if(tc_strcmp(vendor_id,"NA") == 0)
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is == NA \n");																				
							    		}
							    		else
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is != NA \n");
							    			qryObj2 = queryData("Item...","Item ID",vendor_id,1,flogPtr);
							    			fprintf(flogPtr,"\n ***** QUERY2 ****  found = %d",qryObj2.qryObjCount);
							    			for(k=0; k<qryObj2.qryObjCount;k++)
							    			{
							    				ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[k],"object_name",&vendor_name),flogPtr);
							    				fprintf(flogPtr,"\n - Vendor_name = %s \n",vendor_name);										 
							    			}
							    		}								
							    	
							    	    qryObj3 = queryData("DesRev_ERC_Rlzd","ID",trgObjId,1,flogPtr);
							    	    fprintf(flogPtr,"\n ***** QUERY3 ****  found = %d",qryObj3.qryObjCount);
							    	    //if(tc_strlen(qryObjCount) > 0)
							    	    //{
							    		for(l=0; l<qryObj3.qryObjCount;l++)
							    		{
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU Pune") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU Pune storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU PNR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PnrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU PNR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "DHARWAD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_DwdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - DHARWAD storeloc = %s \n",storeloc);
							    				
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU JSR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_JsrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU JSR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU LKO") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_LkoStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU LKO storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CAR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_CarStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CAR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SMALLCAR AHD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_AhdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SMALLCAR AHD storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SANAND2") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_SndStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SANAND2 storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "PUVBU") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunUVStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - PUVBU storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strlen(storeloc)>0)
							    			{											
							    				break;											
							    			} 
							    			
							    		}
							    		if(tc_strlen(storeloc) == 0) 
							    		{
							    		    fprintf(flogPtr,"\n Store Location is blank \n");
							    		    tc_strcat(storeloc,"NA");									 									 
							    		}
							    		
							    		fprintf(flogPtr,"\n Final Store location == %s \n",storeloc);
							    		fprintf(flogPtr,"\n For An Plant		  ==   %s \n",t5ParRequestProjectCode);
							    	 									
							    		fprintf(fPtr,"`%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",trgObjId,plant,POnum,podate,porev,price,vendor_id,vendor_name,storeloc,validity,stock,tran);
									}
									else
									{
										fprintf(flogPtr,"\n Skipped If Condition ");
									}
								}
								
							}
							else
							{
								fprintf(flogPtr,"\n Without Latest Date execution started ");
								
								for(j=0;j<qryObj1.qryObjCount;j++)
                                {                            	                               	
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Plant1",&plant),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO1",&POnum),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PODate1",&podate),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO_Revision",&porev),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_NetPrice1",&price),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_VendorID1",&vendor_id),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_BulkMaterial11",&validity),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Stock1",&stock),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Transaction1",&tran),flogPtr);
									
									fprintf(flogPtr,"\n -plant     = %s",plant);
									fprintf(flogPtr,"\n -POnum     = %s",POnum);
									fprintf(flogPtr,"\n -podate    = %s",podate);
									fprintf(flogPtr,"\n -porev     = %s",porev);
									fprintf(flogPtr,"\n -price     = %s",price);
									fprintf(flogPtr,"\n -vendor_id = %s",vendor_id);
									fprintf(flogPtr,"\n -validity  = %s",validity);
									fprintf(flogPtr,"\n -stock     = %s",stock);
									fprintf(flogPtr,"\n -tran      = %s",tran);
																		
							    
							    	if (tc_strstr(plant, t5ParRequestProjectCode) != 0)
							    	{									
							    																			
							    		fprintf(flogPtr,"\n - Vendor_id = %s \n",vendor_id);
							    		if(tc_strcmp(vendor_id,"NA") == 0)
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is == NA \n");																				
							    		}
							    		else
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is != NA \n");
							    			qryObj2 = queryData("Item...","Item ID",vendor_id,1,flogPtr);
							    			fprintf(flogPtr,"\n ***** QUERY2 ****  found = %d",qryObj2.qryObjCount);
							    			for(k=0; k<qryObj2.qryObjCount;k++)
							    			{
							    				ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[k],"object_name",&vendor_name),flogPtr);
							    				fprintf(flogPtr,"\n - Vendor_name = %s \n",vendor_name);										 
							    			}
							    		}								
							    	
							    	    qryObj3 = queryData("DesRev_ERC_Rlzd","ID",trgObjId,1,flogPtr);
							    	    fprintf(flogPtr,"\n ***** QUERY3 ****  found = %d",qryObj3.qryObjCount);
							    	    //if(tc_strlen(qryObjCount) > 0)
							    	    //{
							    		for(l=0; l<qryObj3.qryObjCount;l++)
							    		{
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU Pune") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU Pune storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU PNR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PnrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU PNR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "DHARWAD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_DwdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - DHARWAD storeloc = %s \n",storeloc);
							    				
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU JSR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_JsrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU JSR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU LKO") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_LkoStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU LKO storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CAR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_CarStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CAR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SMALLCAR AHD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_AhdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SMALLCAR AHD storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SANAND2") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_SndStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SANAND2 storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "PUVBU") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunUVStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - PUVBU storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strlen(storeloc)>0)
							    			{											
							    				break;											
							    			} 
							    			
							    		}
							    		if(tc_strlen(storeloc) == 0) 
							    		{
							    		    fprintf(flogPtr,"\n Store Location is blank \n");
							    		    tc_strcat(storeloc,"NA");									 									 
							    		}
							    		
							    		fprintf(flogPtr,"\n Final Store location == %s \n",storeloc);
							    		fprintf(flogPtr,"\n For An Plant		  ==   %s \n",t5ParRequestProjectCode);
							    	 									
							    		fprintf(fPtr,"`%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",trgObjId,plant,POnum,podate,porev,price,vendor_id,vendor_name,storeloc,validity,stock,tran);
							        }
                                    else
							        {
							        	fprintf(flogPtr,"\n Plant Name not match \n");
							        
							        }																
							    }
							}							
						}					
						else if(t5Role[itr+1]=='\0')
						{
							// Execution of Query 
					
							itemIdPOItem[idInt] = t5Role[itr];
							idInt++;
							itemIdPOItem[idInt] = '\0';
							tc_strcpy(trgObjId,itemIdPOItem);
							
							fprintf(flogPtr,"\n Item Id Found for PO Item itemIdPOItem = %s ",itemIdPOItem);
							fprintf(flogPtr,"\n Item Id Found for PO Item trgObjId = %s ",trgObjId);							
							idInt = 0;
							
							qryObj1 = queryData("Part PO Details","Name",trgObjId,1,flogPtr);							
							fprintf(flogPtr,"\n ***** QUERY ****  found = %d",qryObj1.qryObjCount);
							
							if(tc_strcmp(t5TMLManager,"1")==0)
							{
								dataArryItr = 0;
								fprintf(flogPtr,"\n Latest Date execution started ");
								for(j=0;j<qryObj1.qryObjCount;j++)
                                { 
							        ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Plant1",&plant),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO1",&POnum),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PODate1",&podate),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO_Revision",&porev),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_NetPrice1",&price),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_VendorID1",&vendor_id),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_BulkMaterial11",&validity),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Stock1",&stock),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Transaction1",&tran),flogPtr);
									
									fprintf(flogPtr,"\n -plant     = %s",plant);
									fprintf(flogPtr,"\n -POnum     = %s",POnum);
									fprintf(flogPtr,"\n -podate    = %s",podate);
									fprintf(flogPtr,"\n -porev     = %s",porev);
									fprintf(flogPtr,"\n -price     = %s",price);
									fprintf(flogPtr,"\n -vendor_id = %s",vendor_id);
									fprintf(flogPtr,"\n -validity  = %s",validity);
									fprintf(flogPtr,"\n -stock     = %s",stock);
									fprintf(flogPtr,"\n -tran      = %s",tran);
																		
							    
							    	if (tc_strstr(plant, t5ParRequestProjectCode) != 0)
							    	{
										dateFormatString = convert_date_format(podate);
										fprintf(flogPtr,"\n - dateFormatString = %s",dateFormatString);
										
										ITK_APICALL(ITK_string_to_date(dateFormatString,&dateOfPOObj),flogPtr);
										
										dateArray[dataArryItr] = dateOfPOObj;
										dataArryItr++;																				
									}
							    }
								
								dateMax = dateArray[0];
								fprintf(flogPtr,"\n - dataArryItr = %d ",dataArryItr);
								
								for(ix = 1;ix < dataArryItr;ix++)
								{
									ITK_APICALL(POM_compare_dates(dateMax,dateArray[ix],&cmpResult),flogPtr);
									
									if(cmpResult == -1)
									{
										dateMax = dateArray[ix];
									}
								}
								ITK_APICALL(ITK_date_to_string(dateMax,&maxDateStr),flogPtr);
								fprintf(flogPtr,"\n **************** maxDateStr = %s",maxDateStr);
								
								newReveresedDate = convert_reversedate_format(maxDateStr);
								
								fprintf(flogPtr,"\n *************** Final - newReveresedDate = %s ",newReveresedDate);
								
								tc_strcpy(nx1Str,newReveresedDate);
								
								for(jx = 0;jx<qryObj1.qryObjCount;jx++)
								{
									ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_Plant1",&plant),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_PO1",&POnum),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_PODate1",&podate),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_PO_Revision",&porev),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_NetPrice1",&price),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_VendorID1",&vendor_id),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_BulkMaterial11",&validity),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_Stock1",&stock),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[jx],"t5_Transaction1",&tran),flogPtr);
									
									fprintf(flogPtr,"\n -plant     [2] = %s",plant);
									fprintf(flogPtr,"\n -POnum     [2] = %s",POnum);
									fprintf(flogPtr,"\n -podate    [2] = %s",podate);
									fprintf(flogPtr,"\n -porev     [2] = %s",porev);
									fprintf(flogPtr,"\n -price     [2] = %s",price);
									fprintf(flogPtr,"\n -vendor_id [2] = %s",vendor_id);
									fprintf(flogPtr,"\n -validity  [2] = %s",validity);
									fprintf(flogPtr,"\n -stock     [2] = %s",stock);
									fprintf(flogPtr,"\n -tran      [2] = %s",tran);
									
									fprintf(flogPtr,"\n ********************** Testing Data *************************** "); 
									fprintf(flogPtr,"\n ******* -plant                   = %s",plant);
									fprintf(flogPtr,"\n ******* -t5ParRequestProjectCode = %s",t5ParRequestProjectCode);
									fprintf(flogPtr,"\n ******* -podate                  = %s",podate);
									fprintf(flogPtr,"\n ******* -newReveresedDate        = %s",newReveresedDate);
									fprintf(flogPtr,"\n ******* -nx1Str        = %s",nx1Str);
									
							    
							    	if (tc_strstr(plant, t5ParRequestProjectCode)!= NULL && tc_strstr(nx1Str,podate)!= NULL)
							    	{
										fprintf(flogPtr,"\n ***************** Target PO Object with latest matching date found ");
										
										fprintf(flogPtr,"\n - Vendor_id = %s \n",vendor_id);
							    		if(tc_strcmp(vendor_id,"NA") == 0)
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is == NA \n");																				
							    		}
							    		else
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is != NA \n");
							    			qryObj2 = queryData("Item...","Item ID",vendor_id,1,flogPtr);
							    			fprintf(flogPtr,"\n ***** QUERY2 ****  found = %d",qryObj2.qryObjCount);
							    			for(k=0; k<qryObj2.qryObjCount;k++)
							    			{
							    				ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[k],"object_name",&vendor_name),flogPtr);
							    				fprintf(flogPtr,"\n - Vendor_name = %s \n",vendor_name);										 
							    			}
							    		}								
							    	
							    	    qryObj3 = queryData("DesRev_ERC_Rlzd","ID",trgObjId,1,flogPtr);
							    	    fprintf(flogPtr,"\n ***** QUERY3 ****  found = %d",qryObj3.qryObjCount);
							    	    //if(tc_strlen(qryObjCount) > 0)
							    	    //{
							    		for(l=0; l<qryObj3.qryObjCount;l++)
							    		{
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU Pune") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU Pune storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU PNR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PnrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU PNR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "DHARWAD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_DwdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - DHARWAD storeloc = %s \n",storeloc);
							    				
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU JSR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_JsrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU JSR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU LKO") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_LkoStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU LKO storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CAR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_CarStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CAR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SMALLCAR AHD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_AhdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SMALLCAR AHD storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SANAND2") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_SndStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SANAND2 storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "PUVBU") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunUVStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - PUVBU storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strlen(storeloc)>0)
							    			{											
							    				break;											
							    			} 
							    			
							    		}
							    		if(tc_strlen(storeloc) == 0) 
							    		{
							    		    fprintf(flogPtr,"\n Store Location is blank \n");
							    		    tc_strcat(storeloc,"NA");									 									 
							    		}
							    		
							    		fprintf(flogPtr,"\n Final Store location == %s \n",storeloc);
							    		fprintf(flogPtr,"\n For An Plant		  ==   %s \n",t5ParRequestProjectCode);
							    	 									
							    		fprintf(fPtr,"`%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",trgObjId,plant,POnum,podate,porev,price,vendor_id,vendor_name,storeloc,validity,stock,tran);
									}
									else
									{
										fprintf(flogPtr,"\n Skipped If Condition ");
									}
								}
							}
							else
							{
								fprintf(flogPtr,"\n Without Latest Date execution started ");
								
								for(j=0;j<qryObj1.qryObjCount;j++)
                                {                            	                               	
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Plant1",&plant),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO1",&POnum),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PODate1",&podate),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_PO_Revision",&porev),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_NetPrice1",&price),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_VendorID1",&vendor_id),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_BulkMaterial11",&validity),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Stock1",&stock),flogPtr);
                                	ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[j],"t5_Transaction1",&tran),flogPtr);
							    
								    fprintf(flogPtr,"\n -plant     = %s",plant);
									fprintf(flogPtr,"\n -POnum     = %s",POnum);
									fprintf(flogPtr,"\n -podate    = %s",podate);
									fprintf(flogPtr,"\n -porev     = %s",porev);
									fprintf(flogPtr,"\n -price     = %s",price);
									fprintf(flogPtr,"\n -vendor_id = %s",vendor_id);
									fprintf(flogPtr,"\n -validity  = %s",validity);
									fprintf(flogPtr,"\n -stock     = %s",stock);
									fprintf(flogPtr,"\n -tran      = %s",tran);
								
								
							    	if (tc_strstr(plant, t5ParRequestProjectCode) != 0)
							    	{									
							    																			
							    		fprintf(flogPtr,"\n - Vendor_id = %s \n",vendor_id);
							    		if(tc_strcmp(vendor_id,"NA") == 0)
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is == NA \n");																				
							    		}
							    		else
							    		{
							    			fprintf(flogPtr,"\n - Vendor_id is != NA \n");
							    			qryObj2 = queryData("Item...","Item ID",vendor_id,1,flogPtr);
							    			fprintf(flogPtr,"\n ***** QUERY2 ****  found = %d",qryObj2.qryObjCount);
							    			for(k=0; k<qryObj2.qryObjCount;k++)
							    			{
							    				ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[k],"object_name",&vendor_name),flogPtr);
							    				fprintf(flogPtr,"\n - Vendor_name = %s \n",vendor_name);										 
							    			}
							    		}								
							    	
							    	    qryObj3 = queryData("DesRev_ERC_Rlzd","ID",trgObjId,1,flogPtr);
							    	    fprintf(flogPtr,"\n ***** QUERY3 ****  found = %d",qryObj3.qryObjCount);
							    	    //if(tc_strlen(qryObjCount) > 0)
							    	    //{
							    		for(l=0; l<qryObj3.qryObjCount;l++)
							    		{
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU Pune") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU Pune storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU PNR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PnrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU PNR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "DHARWAD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_DwdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - DHARWAD storeloc = %s \n",storeloc);
							    				
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU JSR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_JsrStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU JSR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CVBU LKO") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_LkoStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CVBU LKO storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "CAR") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_CarStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - CAR storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SMALLCAR AHD") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_AhdStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SMALLCAR AHD storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "SANAND2") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_SndStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - SANAND2 storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strcmp(t5ParRequestProjectCode, "PUVBU") == 0)
							    			{										
							    				ITK_APICALL(AOM_ask_value_string(qryObj3.qryStructTags[l],"t5_PunUVStoreLocation",&storeloc),flogPtr);
							    				fprintf(flogPtr,"\n - PUVBU storeloc = %s \n",storeloc);											
							    			}
							    			if(tc_strlen(storeloc)>0)
							    			{											
							    				break;											
							    			} 
							    			
							    		}
							    		if(tc_strlen(storeloc) == 0) 
							    		{
							    		    fprintf(flogPtr,"\n Store Location is blank \n");
							    		    tc_strcat(storeloc,"NA");									 									 
							    		}
							    		
							    		fprintf(flogPtr,"\n Final Store location == %s \n",storeloc);
							    		fprintf(flogPtr,"\n For An Plant		  ==   %s \n",t5ParRequestProjectCode);
							    	 									
							    		fprintf(fPtr,"`%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",trgObjId,plant,POnum,podate,porev,price,vendor_id,vendor_name,storeloc,validity,stock,tran);
							        }
                                    else
							        {
							        	fprintf(flogPtr,"\n Plant Name not match \n");
							        
							        }																
							    }
							}
						}																													
						else
						{
							// Concatination of Item id 
							
							itemIdPOItem[idInt] = t5Role[itr];
							idInt++;
						}
					}	
                    // Dataset Attachement logic
					 fclose(fPtr);
					 fprintf(flogPtr," \n ************** Execution  started for Folder creation and dataset Attachment **************** ");
							
							dataSetName = (char *) MEM_alloc( 70 * sizeof(char) );
							tc_strcpy(dataSetName,"");
							tc_strcat(dataSetName,"Multi_Part_PO_Report");
							tc_strcat(dataSetName,timeStrUpdated);
												
							
							ITK_APICALL(SA_find_user2(sLoggedUser,&sysUserTag),flogPtr);
					        if(sysUserTag!=NULLTAG)
					        {
					        	ITK_APICALL(SA_ask_user_home_folder(sysUserTag,&sysHomeTag),flogPtr);
					        	if(sysHomeTag!=NULLTAG)
					        	{
					        		ITK_APICALL(FL_ask_references(sysHomeTag,FL_fsc_as_ordered,&noOfReferences,&folderReference),flogPtr);
					        		fprintf(flogPtr,"\n -noOfReferences = %d",noOfReferences);
					        		for(itr=0;itr<noOfReferences;itr++)
					        		{
					        			ITK_APICALL(AOM_ask_value_string(folderReference[itr],"object_name",&fldObjName   ),flogPtr);
					        			ITK_APICALL(AOM_ask_value_string(folderReference[itr],"object_type",&fldObjType   ),flogPtr);
					        			
					        			fprintf(flogPtr,"\n -fldObjName = %s",fldObjName);
					        			fprintf(flogPtr,"\n -fldObjType = %s",fldObjType);
					        			
					        			
					        			if(tc_strcmp(fldObjType,"Folder")==0 && tc_strcmp(fldObjName,"Multi Part PO Report") == 0)
					        			{
					        				fldFound=1;
					        				newFolderTag=folderReference[itr];
					        				break;
					        			}
					        
					        		}
					        		fprintf(flogPtr,"\n -fldFound = %d",fldFound);
					        		fprintf(flogPtr,"\n- csvPath     = %s\n",csvPath );
					        		if(fldFound==1)
					        		{
					        			fprintf(flogPtr,"\n ************** Dataset Attachment started Folder available **************** ");
					        
					        			itkApiCallStatus=ITK_APICALL(AE_find_datasettype2("MSExcel",&datasetType),flogPtr);
				                        fprintf(flogPtr,"ITK status for AE_find_datasettype2 = %d\n",itkApiCallStatus);
					        
					        			if(datasetType!=NULLTAG)
					        			{

                                            itkApiCallStatus=ITK_APICALL(AE_create_dataset_with_id(datasetType,dataSetName,"", NULL, NULL, &datasetTag),flogPtr);
										
				                            fprintf(flogPtr,"ITK status for AE_create_dataset_with_id = %d\n",itkApiCallStatus);
					        				if(datasetTag!=NULLTAG)
					        				{
                                                itkApiCallStatus=ITK_APICALL(AE_import_named_ref(datasetTag,"excel",csvPath,dataSetName,SS_TEXT),flogPtr);
				                                fprintf(flogPtr,"ITK status for AE_import_named_ref = %d\n",itkApiCallStatus); 
					        
					        				    itkApiCallStatus=ITK_APICALL(AOM_refresh(datasetTag,TRUE),flogPtr);
				                                fprintf(flogPtr,"ITK status for AOM_refresh = %d\n",itkApiCallStatus);
				                                          
                                                itkApiCallStatus=ITK_APICALL(AOM_save_without_extensions(datasetTag),flogPtr);
				                                fprintf(flogPtr,"ITK status for AOM_save = %d\n",itkApiCallStatus);	
					        					
					        					// Attachment of dataset started with folder.
					        					
					        					 fprintf(flogPtr,"\n ************** Dataset Attachment started with folder **************** ");
					        
					        			         ITK_APICALL(FL_insert(newFolderTag,datasetTag,999),flogPtr);
					        			         fprintf(flogPtr,"\n ITK status for FL_insert = %d\n",itkApiCallStatus);
					        			         
					        			         ITK_APICALL(AOM_save_without_extensions(newFolderTag),flogPtr);
					        			         fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);
												 successCheck =1 ;
					        
					        					 fprintf(flogPtr,"\n \n *********************** Process completed successfully. ********************************** ");
					        				}
					        				else
					        				{
					        					fprintf(flogPtr,"\n Error : datasetTag == NULLTAG. Dataset not created.");
					        				}
					        			}
					        			else
					        			{
					        				fprintf(flogPtr,"\n Error : datasetType not found.");
					        			}
					        			
					        		}
					        		else
					        		{
					        			fprintf(flogPtr,"\n ************** Folder creation  started **************** ");
					        			
					        			ITK_APICALL(FL_create2("Multi Part PO Report","Multi Part PO Report",&newFolderTag),flogPtr);
                                        fprintf(flogPtr,"\n ITK status for FL_create2 = %d\n",itkApiCallStatus);
                                        if(newFolderTag!=NULLTAG)
					        			{
                                            ITK_APICALL(AOM_save_without_extensions(newFolderTag),flogPtr);
                                            fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);
					        
					        			    ITK_APICALL(FL_insert(sysHomeTag,newFolderTag,000),flogPtr);
					        				fprintf(flogPtr,"\n ITK status for FL_insert = %d\n",itkApiCallStatus);
					        
					        			    ITK_APICALL(AOM_save_without_extensions(sysHomeTag),flogPtr);
					        				fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);
                                            
					        				fprintf(flogPtr,"\n ************** Dataset Attachment started After folder Creation. **************** ");
                                            itkApiCallStatus=ITK_APICALL(AE_find_datasettype2("MSExcel",&datasetType),flogPtr);
				                            fprintf(flogPtr,"ITK status for AE_find_datasettype2 = %d\n",itkApiCallStatus);
					        
					        			    if(datasetType!=NULLTAG)
					        			    {
                                                itkApiCallStatus=ITK_APICALL(AE_create_dataset_with_id(datasetType,dataSetName,"", NULL, NULL, &datasetTag),flogPtr);
												
				                                fprintf(flogPtr,"ITK status for AE_create_dataset_with_id = %d\n",itkApiCallStatus);
					        			    	if(datasetTag!=NULLTAG)
					        			    	{
                                                    itkApiCallStatus=ITK_APICALL(AE_import_named_ref(datasetTag,"excel",csvPath,dataSetName,SS_TEXT),flogPtr);
					
				                                    fprintf(flogPtr,"ITK status for AE_import_named_ref = %d\n",itkApiCallStatus); 
					        			    
					        			    	    itkApiCallStatus=ITK_APICALL(AOM_refresh(datasetTag,TRUE),flogPtr);
				                                    fprintf(flogPtr,"ITK status for AOM_refresh = %d\n",itkApiCallStatus);
				                                              
                                                    itkApiCallStatus=ITK_APICALL(AOM_save_without_extensions(datasetTag),flogPtr);
				                                    fprintf(flogPtr,"ITK status for AOM_save = %d\n",itkApiCallStatus);	
					        			    		
					        			    		// Attachment of dataset started with folder.
					        			    		
					        			    		 fprintf(flogPtr,"\n ************** Dataset Attachment started with folder **************** ");
					        			    
					        			             ITK_APICALL(FL_insert(newFolderTag,datasetTag,999),flogPtr);
					        			             fprintf(flogPtr,"\n ITK status for FL_insert = %d\n",itkApiCallStatus);
					        			             
					        			             ITK_APICALL(AOM_save_without_extensions(newFolderTag),flogPtr);
					        			             fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);
													 successCheck =1 ;
					        
					        						 fprintf(flogPtr,"\n \n *********************** Process completed successfully. ********************************** ");
					        			    	}
					        			    	else
					        			    	{
					        			    		fprintf(flogPtr,"\n Error : datasetTag == NULLTAG. Dataset not created.");
					        			    	}
					        			    }
					        			    else
					        			    {
					        			    	fprintf(flogPtr,"\n Error : datasetType not found.");
					        			    }                                                                      
					        			}
					        			else
					        			{
                                            fprintf(flogPtr,"\n Error : New Creation failed.");
					        			}																								 
					        		}
					        	}
					        	else
					        	{
					        		fprintf(flogPtr,"Error : Home folder Tag not found.");
					        	}
					        }						
					        else
					        {
					        	fprintf(flogPtr,"Error : System user Tag not found.");
					        }
					
				}
				else
				{
					fprintf(flogPtr,"\n Error : PAR ItemRev Tag not found with %s   .... \n",sParNo);
				}
			}
			else
			{
				fprintf(flogPtr,"\n Error : PAR Item not found for with %s itemId ....\n",sParNo);
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
	//fclose(fPtr);
	fclose(flogPtr);
	return(ITK_ok);
}



