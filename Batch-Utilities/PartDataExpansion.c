/* Objective :- Webservice and ITK program to expand bom and transfer targeted data at particular location. */

// Date - 16_12_2024

/*Required headers */

#include <PartDataExpansion.nsmap>	
#include <soapH.h>	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/custom.h>
#include <cfm/cfm.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <bom/bom.h>
#include <res/res_itk.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <ae/dataset.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>


/*Declarations */

int itkApiCallStatus = -1;
int targetObjCount =1;
char *glbDirectory = NULL;

/* Space Triming functions */

void trimLeading(char* str)
{
    int index, i;
    index = 0;
    /* Find last index of whitespace character */
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }
    if(index != 0)
    {
        /* Shit all trailing characters to its left */
        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0'; // Make sure that string is NULL terminated
    }
}
;
void trimTrailing(char* str)
{
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }
    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
};
/*Custom API error handling function */

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

int FindTrgObj(char *objId,char *objName,tag_t targetRevTag,FILE *Flogfile,FILE* fJtslogPtr)
{
	fprintf(Flogfile,"\n  ############################# Inside FindTrgObj function #############################\n"); 
	
	int itr            = 0;
	int itr2           = 0;
	int jtcount        = 0;
	int txtCount       = 0;
	int prtCount       = 0;
	int catPrtCount    = 0;
    int catPrdCount    = 0;	
	int pdfCount       = 0; 
	int asmCount       = 0;
	int apiStatus      = 0;
	int refListcount   = 0;
	int secObjectCount = 0;
	int jtExported     = 0;
	int txtExported    = 0;
	int prtExported    = 0;
	int catPrtExported = 0;
	int catPrdExported = 0;
    int asmExported    = 0;
    int pdfExported    = 0;	
	int trgObjCount    = 0;
	
	tag_t classObjTag  = NULLTAG;
	tag_t *reflistTags = NULLTAG;
	tag_t *secObjTags  = NULLTAG;
	
	char *secObjName       = NULL;
	char *secObjType       = NULL;
	char *secObjClass      = NULL; 
	char *objString        = NULL;
	char *filePath         = (char*) MEM_alloc(200 * sizeof(char));
	char *tempObjName      = NULL;
	char *nameRefName      = NULL;
	char *fileName         = NULL;
	char *originalFileName = NULL;
		
	fprintf(Flogfile," - objId = %s \n",objId);
	fprintf(Flogfile," - objName = %s \n",objName);
	
	apiStatus=ITK_APICALL(AOM_ask_value_string(targetRevTag,"object_string",&objString),Flogfile);
	fprintf(Flogfile," API Status for AOM_ask_value_string = %d \n",apiStatus);
	fprintf(Flogfile," object_string value found for targetRev = %s \n",objString);
	
	apiStatus=ITK_APICALL(GRM_list_secondary_objects_only(targetRevTag, NULLTAG, &secObjectCount, &secObjTags),Flogfile);
	fprintf(Flogfile," API Status for GRM_list_secondary_objects_only = %d \n",apiStatus);
	fprintf(Flogfile,"Total secObjectCount = %d \n",secObjectCount);
	
	for(itr=0;itr<secObjectCount;itr++)
	{
		apiStatus=ITK_APICALL(AOM_ask_value_string(secObjTags[itr],"object_name",&secObjName),Flogfile);
	    fprintf(Flogfile," API Status for AOM_ask_value_string = %d \n",apiStatus);
		fprintf(Flogfile," object_name in FindTrgObj function = %s \n",secObjName);
		
		apiStatus=ITK_APICALL(AOM_ask_value_string(secObjTags[itr],"object_type",&secObjType),Flogfile);
	    fprintf(Flogfile," API Status for AOM_ask_value_string = %d \n",apiStatus);
		fprintf(Flogfile," object_type in FindTrgObj function = %s \n",secObjType);
		
		apiStatus=ITK_APICALL(POM_class_of_instance(secObjTags[itr],&classObjTag),Flogfile);
	    fprintf(Flogfile," API Status for POM_class_of_instance = %d \n",apiStatus);
		
		apiStatus=ITK_APICALL(POM_name_of_class(classObjTag,&secObjClass),Flogfile);
	    fprintf(Flogfile," API Status for POM_name_of_class = %d \n",apiStatus);		
		fprintf(Flogfile," class_name in FindTrgObj function = %s \n",secObjClass);
		
		if(tc_strcmp(secObjClass,"Dataset")==0)
		{
			fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjClass,Dataset)==0 \n");
			apiStatus=ITK_APICALL(AOM_ask_value_tags(secObjTags[itr],"ref_list",&refListcount,&reflistTags),Flogfile);
	        fprintf(Flogfile," API Status for AOM_ask_value_tags = %d \n",apiStatus);
			fprintf(Flogfile," Total refListcount found  = %d \n",refListcount);
			if(refListcount>0)
			{
				fprintf(Flogfile,"\n ****************** Inside If condition where refListcount > 0 \n");
				for(itr2=0;itr2<refListcount;itr2++)
				{
					apiStatus=ITK_APICALL(AOM_ask_value_string(reflistTags[itr2],"file_name",&fileName),Flogfile);
				    fprintf(Flogfile," API Status for AOM_ask_value_string = %d \n",apiStatus);
				    fprintf(Flogfile," file_name in FindTrgObj function = %s \n",fileName);
					
					apiStatus=ITK_APICALL(AOM_ask_value_string(reflistTags[itr2],"original_file_name",&originalFileName),Flogfile);
					fprintf(Flogfile," API Status for AOM_ask_value_string = %d \n",apiStatus);
					fprintf(Flogfile," original_file_name in FindTrgObj function = %s \n",originalFileName);
					
					if(tc_strcmp(secObjType,"DirectModel")==0&&tc_strstr(originalFileName,".jt")!=NULL)
					{
						fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjType,DirectModel)==0&&tc_strstr(originalFileName,.jt)!=NULL \n");
						jtcount++;
						tc_strcpy(filePath,glbDirectory);
						tc_strcat(filePath,"/");
						tc_strcat(filePath,originalFileName);
						fprintf(Flogfile," -filePath = %s \n",filePath);						
						apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"JTPART",filePath),Flogfile);
						fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);		
						if(apiStatus==0)
						{
							fprintf(Flogfile,"\n ************************* JT Dataset Exported successful ! ************************* \n");
							jtExported++;
						}
						else
						{
							fprintf(Flogfile,"\n ************************* JT Dataset Export Failed !!!!!!! ************************* \n");	
                            fprintf(Flogfile,"\n ************************* Export Process started again by adding object_name property !!!! ************************* \n");
							tc_strcpy(filePath,glbDirectory);
							tc_strcat(filePath,"/");
							tc_strcat(filePath,objId);
							tc_strcat(filePath,"_");
							tc_strcat(filePath,originalFileName);
							fprintf(Flogfile," -filePath (Again Used) = %s \n",filePath);
                            apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"JTPART",filePath),Flogfile);
						    fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);	
                            if(apiStatus==0)
							{
								fprintf(Flogfile,"\n ************************* JT Dataset Exported successful in second try ! ************************* \n");
								jtExported++;
							}			
                            else
							{
								fprintf(Flogfile,"\n ************************* JT Dataset Export Failed in second try even !!!!!!! ************************* \n");	
							}															
						}							
					}
					if(tc_strcmp(secObjType,"Text")==0&&tc_strstr(originalFileName,".txt")!=NULL)
					{
						fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjType,Text)==0&&tc_strstr(originalFileName,.txt)!=NULL \n");
						txtCount++;
						tc_strcpy(filePath,glbDirectory);
						tc_strcat(filePath,"/");
						tc_strcat(filePath,originalFileName);
						fprintf(Flogfile," -filePath = %s \n",filePath);						
						apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"Text",filePath),Flogfile);
						fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);		
						if(apiStatus==0)
						{
							fprintf(Flogfile,"\n ************************* TEXT Dataset Exported successful ! ************************* \n");
							txtExported++;
						}
						else
						{
							fprintf(Flogfile,"\n ************************* TEXT Dataset Export Failed !!!!!!! ************************* \n");	
						    fprintf(Flogfile,"\n ************************* Export Process started again by adding object_name property !!!! ************************* \n");
							tc_strcpy(filePath,glbDirectory);
							tc_strcat(filePath,"/");
							tc_strcat(filePath,objId);
							tc_strcat(filePath,"_");
							tc_strcat(filePath,originalFileName);
							fprintf(Flogfile," -filePath (Again Used) = %s \n",filePath);
						    apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"Text",filePath),Flogfile);
						    fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);	
						    if(apiStatus==0)
							{
								fprintf(Flogfile,"\n ************************* TEXT Dataset Exported successful in second try ! ************************* \n");
							    txtExported++;
							}			
						    else
							{
								fprintf(Flogfile,"\n ************************* TEXT Dataset Export Failed in second try even !!!!!!! ************************* \n");	
							}															
						}									
					}
					if(tc_strcmp(secObjType,"ProPrt")==0&&tc_strstr(originalFileName,".prt")!=NULL)
					{
						fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjType,ProPrt)==0&&tc_strstr(originalFileName,.prt)!=NULL \n");
					    prtCount++;
					    tc_strcpy(filePath,glbDirectory);
					    tc_strcat(filePath,"/");
					    tc_strcat(filePath,originalFileName);
					    fprintf(Flogfile," -filePath = %s \n",filePath);						
					    apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"PrtFile",filePath),Flogfile);
					    fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);		
					    if(apiStatus==0)
					    {
					    	fprintf(Flogfile,"\n ************************* Creo Part Dataset Exported successful ! ************************* \n");
					    	prtExported++;
					    }
					    else
					    {
					    	fprintf(Flogfile,"\n ************************* Creo Part Dataset Export Failed !!!!!!! ************************* \n");	
					        fprintf(Flogfile,"\n ************************* Export Process started again by adding object_name property !!!! ************************* \n");
					    	tc_strcpy(filePath,glbDirectory);
					    	tc_strcat(filePath,"/");
					    	tc_strcat(filePath,objId);
					    	tc_strcat(filePath,"_");
					    	tc_strcat(filePath,originalFileName);
					    	fprintf(Flogfile," -filePath (Again Used) = %s \n",filePath);
					        apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"PrtFile",filePath),Flogfile);
					        fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);	
					        if(apiStatus==0)
					    	{
					    		fprintf(Flogfile,"\n ************************* Creo Part Dataset Exported successful in second try ! ************************* \n");
					    		prtExported++;
					    	}			
					        else
					    	{
					    		fprintf(Flogfile,"\n ************************* Creo Part Dataset Export Failed in second try even !!!!!!! ************************* \n");	
					    	}															
					    }														
					}
					if(tc_strcmp(secObjType,"PDF")==0&&tc_strstr(originalFileName,".pdf")!=NULL)
					{
						fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjType,PDF)==0&&tc_strstr(originalFileName,.pdf)!=NULL \n");
					    pdfCount++;
						tc_strcpy(filePath,glbDirectory);
					    tc_strcat(filePath,"/");
					    tc_strcat(filePath,originalFileName);
					    fprintf(Flogfile," -filePath = %s \n",filePath);						
					    apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"PDF_Reference",filePath),Flogfile);
					    fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);		
					    if(apiStatus==0)
					    {
					    	fprintf(Flogfile,"\n ************************* PDF Dataset Exported successful ! ************************* \n");
					    	pdfExported++;
					    }
					    else
					    {
					    	fprintf(Flogfile,"\n ************************* PDF Dataset Export Failed !!!!!!! ************************* \n");	
					        fprintf(Flogfile,"\n ************************* Export Process started again by adding object_name property !!!! ************************* \n");
					    	tc_strcpy(filePath,glbDirectory);
					    	tc_strcat(filePath,"/");
					    	tc_strcat(filePath,objId);
					    	tc_strcat(filePath,"_");
					    	tc_strcat(filePath,originalFileName);
					    	fprintf(Flogfile," -filePath (Again Used) = %s \n",filePath);
					        apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"PDF_Reference",filePath),Flogfile);
					        fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);	
					        if(apiStatus==0)
					    	{
					    		fprintf(Flogfile,"\n ************************* PDF Dataset Exported successful in second try ! ************************* \n");
					    	    pdfExported++;
							}			
					        else
					    	{
					    		fprintf(Flogfile,"\n ************************* PDF Dataset Export Failed in second try even !!!!!!! ************************* \n");	
					    	}															
					    }					
					}
					if(tc_strcmp(secObjType,"CMI2Part")==0&&tc_strstr(originalFileName,".CATPart")!=NULL)
					{
						fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjType,CMI2Part)==0&&tc_strstr(originalFileName,.CATPart)!=NULL \n");
						catPrtCount++;
						tc_strcpy(filePath,glbDirectory);
						tc_strcat(filePath,"/");
						tc_strcat(filePath,originalFileName);
						fprintf(Flogfile," -filePath = %s \n",filePath);						
						apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"CATPart",filePath),Flogfile);
						fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);		
						if(apiStatus==0)
						{
							fprintf(Flogfile,"\n ************************* Cad Part Dataset Exported successful ! ************************* \n");
							catPrtExported++;
						}
						else
						{
							fprintf(Flogfile,"\n ************************* Cad Part Dataset Export Failed !!!!!!! ************************* \n");	
						    fprintf(Flogfile,"\n ************************* Export Process started again by adding object_name property !!!! ************************* \n");
							tc_strcpy(filePath,glbDirectory);
							tc_strcat(filePath,"/");
							tc_strcat(filePath,objId);
							tc_strcat(filePath,"_");
							tc_strcat(filePath,originalFileName);
							fprintf(Flogfile," -filePath (Again Used) = %s \n",filePath);
						    apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"CATPart",filePath),Flogfile);
						    fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);	
						    if(apiStatus==0)
							{
								fprintf(Flogfile,"\n ************************* Cad Part Dataset Exported successful in second try ! ************************* \n");
							    catPrtExported++;
							}			
						    else
							{
								fprintf(Flogfile,"\n ************************* Cad Part Dataset Export Failed in second try even !!!!!!! ************************* \n");	
							}															
						}											
					}
					if(tc_strcmp(secObjType,"CMI2Product")==0&&tc_strstr(originalFileName,".CATProduct")!=NULL)
					{
						fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjType,CMI2Product)==0&&tc_strstr(originalFileName,.CATProduct)!=NULL \n");
						catPrdCount++;
						tc_strcpy(filePath,glbDirectory);
						tc_strcat(filePath,"/");
						tc_strcat(filePath,originalFileName);
						fprintf(Flogfile," -filePath = %s \n",filePath);						
						apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"CATProduct",filePath),Flogfile);
						fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);		
						if(apiStatus==0)
						{
							fprintf(Flogfile,"\n ************************* Cad Product Dataset Exported successful ! ************************* \n");
							catPrdExported++;
						}
						else
						{
							fprintf(Flogfile,"\n ************************* Cad Product Dataset Export Failed !!!!!!! ************************* \n");	
						    fprintf(Flogfile,"\n ************************* Export Process started again by adding object_name property !!!! ************************* \n");
							tc_strcpy(filePath,glbDirectory);
							tc_strcat(filePath,"/");
							tc_strcat(filePath,objId);
							tc_strcat(filePath,"_");
							tc_strcat(filePath,originalFileName);
							fprintf(Flogfile," -filePath (Again Used) = %s \n",filePath);
						    apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"CATProduct",filePath),Flogfile);
						    fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);	
						    if(apiStatus==0)
							{
								fprintf(Flogfile,"\n ************************* Cad Product Dataset Exported successful in second try ! ************************* \n");
							    catPrdExported++;
							}			
						    else
							{
								fprintf(Flogfile,"\n ************************* Cad Product Dataset Export Failed in second try even !!!!!!! ************************* \n");	
							}															
						}																
					}
					if(tc_strcmp(secObjType,"ProAsm")==0&&tc_strstr(originalFileName,".asm")!=NULL)
					{
						fprintf(Flogfile,"\n ****************** Inside If condition where tc_strcmp(secObjType,ProAsm)==0&&tc_strstr(originalFileName,.asm)!=NULL \n");
						asmCount++;
						tc_strcpy(filePath,glbDirectory);
						tc_strcat(filePath,"/");
						tc_strcat(filePath,originalFileName);
						fprintf(Flogfile," -filePath = %s \n",filePath);						
						apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"AsmFile",filePath),Flogfile);
						fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);		
						if(apiStatus==0)
						{
							fprintf(Flogfile,"\n ************************* Asm File Dataset Exported successful ! ************************* \n");
							asmExported++;
						}
						else
						{
							fprintf(Flogfile,"\n ************************* Asm File Dataset Export Failed !!!!!!! ************************* \n");	
						    fprintf(Flogfile,"\n ************************* Export Process started again by adding object_name property !!!! ************************* \n");
							tc_strcpy(filePath,glbDirectory);
							tc_strcat(filePath,"/");
							tc_strcat(filePath,objId);
							tc_strcat(filePath,"_");
							tc_strcat(filePath,originalFileName);
							fprintf(Flogfile," -filePath (Again Used) = %s \n",filePath);
						    apiStatus=ITK_APICALL(AE_export_named_ref(secObjTags[itr],"AsmFile",filePath),Flogfile);
						    fprintf(Flogfile," API Status for AE_export_named_ref = %d \n",apiStatus);	
						    if(apiStatus==0)
							{
								fprintf(Flogfile,"\n ************************* Asm File Dataset Exported successful in second try ! ************************* \n");
							    asmExported++;
							}			
						    else
							{
								fprintf(Flogfile,"\n ************************* Asm File Dataset Export Failed in second try even !!!!!!! ************************* \n");	
							}															
						}																	
					}					
				}
			}
			else
			{
				fprintf(Flogfile,"\n ****************** Else condition executed where refListcount<=0 nameRef file not found or error \n");
				fprintf(Flogfile,"\n Exporting of dataset with %s objectName and %s objectType and %s className failed because ref file not available \n",secObjName,secObjType,secObjClass);				
			}				
		}		
	}
	if(targetObjCount==1)
	{
	    fprintf(fJtslogPtr,"Srno,Item Id,Object Name,Jt Found,Jt Exported,Txt Found,Txt Exported,Prt Found,Prt Exported,Pdf Found,Pdf Exported,CadPart Found,CatPart Exported,CadPrd Found,CadPrd Exported,Asm Found,Asm Exported \n");
	    fprintf(fJtslogPtr,"%d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",targetObjCount,objId,objName,jtcount,jtExported,txtCount,txtExported,prtCount,prtExported,pdfCount,pdfExported,catPrtCount,catPrtExported,catPrdCount,catPrdExported,asmCount,asmExported);
	    targetObjCount++;
	}
	else
	{
	    fprintf(fJtslogPtr,"%d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",targetObjCount,objId,objName,jtcount,jtExported,txtCount,txtExported,prtCount,prtExported,pdfCount,pdfExported,catPrtCount,catPrtExported,catPrdCount,catPrdExported,asmCount,asmExported);
		targetObjCount++;
	}
	
	trgObjCount=trgObjCount+jtcount+txtCount+prtCount+catPrtCount+catPrdCount+pdfCount+asmCount;
	
	jtcount        = 0;   
	txtCount       = 0;
	prtCount       = 0;
	catPrtCount    = 0;
	catPrdCount    = 0;	
	pdfCount       = 0; 
	asmCount       = 0;
	
	jtExported     = 0;
	txtExported    = 0;
	prtExported    = 0;
	catPrtExported = 0;
	catPrdExported = 0;
	asmExported    = 0;
	pdfExported    = 0;	
	
	fprintf(Flogfile," $$$$$$$$$$$$$$$$$$ Total Target Object found for %s MainItemId and %s MainObjectName is %d in FindTrgObj function $$$$$$$$$$$$$$$$$$\n",objId,objName,trgObjCount);
	fprintf(Flogfile,"\n  ############################# Outside FindTrgObj function #############################\n");
	
    if(secObjTags)
	{
		MEM_free(secObjTags);
	}
	if(reflistTags)
	{
		MEM_free(reflistTags);
	}
	return(trgObjCount);
}
int BOM_ASK_CHILD(int childCount, tag_t *bomchdTags,FILE *Flogfile, FILE *fJtslogPtr)
{	
    fprintf(Flogfile,"\n  ############################# Inside BOM_ASK_CHILD function #############################\n"); 
	int itr;
	int subChildCount;
	char *itemId   = NULL; 
	char *itemName = NULL; 
	int apiStatus = 0;
	tag_t realObjTag = NULLTAG;
	tag_t *subChildTags = NULLTAG;
	
	fprintf(Flogfile,"Inside BOM_ASK_CHILD function \n");
	for(itr=0;itr<childCount;itr++)
	{
		apiStatus=ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_item_item_id",&itemId),Flogfile);
	    fprintf(Flogfile," API Status for AOM_ask_value_string = %d \n",apiStatus);
	    fprintf(Flogfile," $$$$$$$$$$ bl_item_item_id in BOM_ASK_CHILD = %s \n",itemId);
									
		apiStatus=ITK_APICALL(AOM_ask_value_string(bomchdTags[itr],"bl_item_object_name",&itemName),Flogfile);
		fprintf(Flogfile," API Status for AOM_ask_value_string = %d \n",apiStatus);
		fprintf(Flogfile," $$$$$$$$$$ bl_item_object_name in BOM_ASK_CHILD = %s \n",itemName);
		
		apiStatus=ITK_APICALL(AOM_ask_value_tag(bomchdTags[itr],"bl_line_object",&realObjTag),Flogfile);
		fprintf(Flogfile," API Status for AOM_ask_value_tag = %d \n",apiStatus);
		fprintf(Flogfile," $$$$$$$$$$ bl_line_object in BOM_ASK_CHILD $$$$$$$$$$ \n");
			
		apiStatus=ITK_APICALL(BOM_line_ask_all_child_lines(bomchdTags[itr],&subChildCount,&subChildTags),Flogfile);
		fprintf(Flogfile," API Status for BOM_line_ask_all_child_lines = %d \n",apiStatus);									
		fprintf(Flogfile," $$$$$$$$$$ subChildCount in BOM_ASK_CHILD = %d \n",subChildCount);
		
		fprintf(Flogfile," !!!!!!!!!!!!!!!!!!!! Total Target Object Found with %s ItemId and %s ObjName is %d in BOM_ASK_CHILD function\n",itemId,itemName,FindTrgObj(itemId,itemName,realObjTag,Flogfile,fJtslogPtr));
		if(subChildCount>0)
		{
			BOM_ASK_CHILD(subChildCount,subChildTags,Flogfile,fJtslogPtr);
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
int updateDirectory(char* hostName,char* usrName,char* partNo,char* itemRevisionId,char* itemRevisionSequesnce,FILE* Flogfile)
{	
	fprintf(Flogfile,"\n  ############################# Inside updateDirectory function #############################\n");  
	fprintf(Flogfile,"-hostName = %s\n",hostName); 
	fprintf(Flogfile,"-usrName = %s\n",usrName); 
	fprintf(Flogfile,"-partNo = %s\n",partNo); 
	fprintf(Flogfile,"-itemRevisionId = %s\n",itemRevisionId); 
	fprintf(Flogfile,"-itemRevisionSequesnce = %s\n",itemRevisionSequesnce); 
	
	int dirStatus=0;
	int tItr=0;
	
	char timeArray[100];
	time_t mytime   = time(NULL);
	char *timeStr   = ctime(&mytime);
	char *tmpTime   = NULL;
	char *tmpTime2   = NULL;
	char *day       = NULL;
	char *month     = NULL; 
	char *date      = NULL; 
	char *year      = NULL;
	char *lwrMonth  = NULL;	
	char *dirFolder = NULL; 
	char *finalDate = NULL;	
	
	timeStr[strlen(timeStr)-1] = '\0';
	dirFolder = (char*) MEM_alloc(500*sizeof(char));
	finalDate = (char*) MEM_alloc(500*sizeof(char));	
	
	for(tItr=0;timeStr[tItr]!='\0';tItr++)
    {
        timeArray[tItr]=timeStr[tItr];
    }
	timeArray[tItr]='\0';
		
	day     = tc_strtok(timeArray," ");
    month   = tc_strtok(NULL," "); 	
	date    = tc_strtok(NULL," ");
	if(tc_strlen(date)==0&&tc_strcmp(date,"")==0)
	{
	   fprintf(Flogfile,"-date    = %d\n",tc_strlen(date));
	   date = tc_strtok(NULL," ");
	   fprintf(Flogfile," ***** Double toking used for date command ***** \n");
	}
	tmpTime = tc_strtok(NULL," ");
	year    = tc_strtok(NULL," ");
	
	STRNG_replace_str(tmpTime,":","_",&tmpTime2); 	

	fprintf(Flogfile,"-day     = %s\n",day);
	fprintf(Flogfile,"-month   = %s\n",month); 	
	fprintf(Flogfile,"-date    = %s\n",date); 	
	fprintf(Flogfile,"-tmpTime = %s\n",tmpTime); 
    fprintf(Flogfile,"-tmpTime2 = %s\n",tmpTime2);	
	fprintf(Flogfile,"-year    = %s\n",year); 	
	
	tc_strlwr(month,&lwrMonth);
	fprintf(Flogfile,"-lwrMonth = %s\n",lwrMonth);

    tc_strcpy(finalDate,year);	
	fprintf(Flogfile,"-finalDate point 1 = %s\n",finalDate);
	
	if(tc_strstr(lwrMonth,"jan")!=NULL)
	{
		tc_strcat(finalDate,"-01-");
	}
	if(tc_strstr(lwrMonth,"feb")!=NULL)
	{
		tc_strcat(finalDate,"-02-");
	}
	if(tc_strstr(lwrMonth,"mar")!=NULL)
	{
		tc_strcat(finalDate,"-03-");
	}
	if(tc_strstr(lwrMonth,"apr")!=NULL)
	{
		tc_strcat(finalDate,"-04-");
	}
	if(tc_strstr(lwrMonth,"may")!=NULL)
	{
		tc_strcat(finalDate,"-05-");
	}
	if(tc_strstr(lwrMonth,"jun")!=NULL)
	{
		tc_strcat(finalDate,"-06-");
	}
	if(tc_strstr(lwrMonth,"jul")!=NULL)
	{
		tc_strcat(finalDate,"-07-");
	}
	if(tc_strstr(lwrMonth,"aug")!=NULL)
	{
		tc_strcat(finalDate,"-08-");
	}
	if(tc_strstr(lwrMonth,"sep")!=NULL)
	{
		tc_strcat(finalDate,"-09-");
	}
	if(tc_strstr(lwrMonth,"oct")!=NULL)
	{
		tc_strcat(finalDate,"-10-");
	}
	if(tc_strstr(lwrMonth,"nov")!=NULL)
	{
		tc_strcat(finalDate,"-11-");
	}
	if(tc_strstr(lwrMonth,"dec")!=NULL)
	{
		tc_strcat(finalDate,"-12-");
	}
	
	tc_strcat(finalDate,date);
	fprintf(Flogfile,"-finalDate point 2 = %s\n",finalDate);
	strcpy(dirFolder,"/proj/digitalbuck/dmusession/");
	//strcpy(dirFolder,"/tmp/");
	strcat(dirFolder,hostName);
	strcat(dirFolder,"_");
	strcat(dirFolder,usrName);
	strcat(dirFolder,"_");
	strcat(dirFolder,partNo);
	strcat(dirFolder,"_");
	strcat(dirFolder,itemRevisionId);
	strcat(dirFolder,"_");
	strcat(dirFolder,itemRevisionSequesnce);
	strcat(dirFolder,"_");
	strcat(dirFolder,finalDate);	
	strcat(dirFolder,"-");
	strcat(dirFolder,tmpTime2);
	fprintf(Flogfile,"-dirFolder = %s\n",dirFolder); 	
    stpcpy(glbDirectory,dirFolder); 
	fprintf(Flogfile,"-glbDirectory = %s\n",glbDirectory);	
	dirStatus=mkdir(dirFolder,0777);    
	fprintf(Flogfile,"\n Output directory generated = %s \n",glbDirectory);	
	fprintf(Flogfile,"\n  ############################# Outside updateDirectory function #############################\n");  
    return(dirStatus);
}
extern int ITK_user_main (int argc, char *argv[])
{
	return soap_serve(soap_new()); 
};

int ns__PartDataExpansion(struct soap* soap,char *userId,char *password,char *prtNumber,char *prtRevisionId,char *prtSequence,char* hostname1,char **Response)
{	
	char *sUserName	          =   userId;
    char *sPassword	          =   password;
    char *sGroup              =   "";
	char *hostname            =   hostname1;
	char *loggedInUser        =   NULL; 
    char *itemId              =   prtNumber; 
    char *itemRevId           =   prtRevisionId; 
	char *upritemRevId        =   NULL; 
    char *itemRevSequence     =   prtSequence;
    char *upritemRevSequence  =   NULL;
    char *trgObjName          =   NULL;	
	char *trgObjStr           =   NULL; 
	char *trgRevId            =   NULL;
    char *revId	              =   NULL;
	char *blItemId            =   NULL; 
	char *blObjectName        =   NULL; 
	char *tempId              =   NULL; 
	char *tempObjName         =   NULL; 
	char *rlsStsName          =   NULL;
	char *logPath = (char*) MEM_alloc(200*sizeof(char));
	char *csvPath = (char*) MEM_alloc(200*sizeof(char));
	
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
		
	tag_t loggedInUserTag   =  NULLTAG;
	tag_t itemTag           =  NULLTAG;
	tag_t *revTags          =  NULLTAG;
	tag_t windowTag         =  NULLTAG;
	tag_t topBomLineTag     =  NULLTAG;
	tag_t revRule           =  NULLTAG;
	tag_t *bomChildTags     =  NULLTAG;
	tag_t *rlsStsList       =  NULLTAG;
	tag_t latestRevTag      =  NULLTAG;
		
	glbDirectory  = (char*) MEM_alloc(200*sizeof(char));
	*Response     = (char*) MEM_alloc(500*sizeof(char));	
	fpos_t pos;
	fgetpos(stdout, &pos);
	int fd = dup(fileno(stdout));	
	time_t mytime = time(NULL);
	char *timeStr = ctime(&mytime);
	char *timeStrUpdated = NULL;
	timeStr[strlen(timeStr)-1] = '\0';
	
	freopen("/tmp/PDE_ConsoleLog.txt", "w+", stdout);
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated); 	
	tc_strcpy(logPath,"/tmp/");
	tc_strcat(logPath,timeStrUpdated);
	tc_strcat(logPath,"_");
	tc_strcat(logPath,"PDE_ExportLog.txt");
	
	tc_strcpy(csvPath,"/tmp/");
	tc_strcat(csvPath,timeStrUpdated);
	tc_strcat(csvPath,"_");
	tc_strcat(csvPath,"PDE_ExportStatus.csv");
	
	FILE *flogPtr=fopen(logPath,"w+");
	FILE *fJtlogPtr=fopen(csvPath,"w+");
	fprintf(flogPtr,"-logPath = %s \n",logPath);
	fprintf(flogPtr,"-csvPath = %s \n",csvPath);	
	fprintf(flogPtr,"\n ******************************** MultiBomExpansion exe execution stated  ******************************** \n");	      
    fprintf(flogPtr,"Current Time = %s\n",timeStr);		  
    
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	trimLeading(sUserName);
	trimTrailing(sUserName);
	
	trimLeading(sPassword);
	trimTrailing(sPassword);
	
	trimLeading(itemId);
	trimTrailing(itemId);
	
	trimLeading(itemRevId);
	trimTrailing(itemRevId);
	
	trimLeading(itemRevSequence);
	trimTrailing(itemRevSequence);
	
	trimLeading(hostname);
	trimTrailing(hostname);
			
	fprintf(flogPtr,"-u = %s \n",sUserName);
	fprintf(flogPtr,"-p = %s \n",sPassword);
	fprintf(flogPtr,"-g = %s \n",sGroup);	
	fprintf(flogPtr,"-itemId = %s \n",itemId);
	fprintf(flogPtr,"-itemRevId = %s \n",itemRevId);
	fprintf(flogPtr,"-itemRevSequence = %s \n",itemRevSequence);		
	fprintf(flogPtr,"-hostname = %s \n",hostname);	
	
	fprintf(flogPtr,"-u strlen = %d \n",tc_strlen(sUserName));
	fprintf(flogPtr,"-p strlen = %d \n",tc_strlen(sPassword));
	fprintf(flogPtr,"-g strlen = %d \n",tc_strlen(sGroup));	
	fprintf(flogPtr,"-itemId strlen = %d \n",tc_strlen(itemId));
	fprintf(flogPtr,"-itemRevId strlen = %d \n",tc_strlen(itemRevId));
	fprintf(flogPtr,"-itemRevSequence strlen = %d \n",tc_strlen(itemRevSequence));		
	fprintf(flogPtr,"-hostname strlen = %d \n",tc_strlen(hostname));	
	
	if(tc_strlen(sUserName)>0&&tc_strlen(sPassword)>0&&tc_strlen(hostname)>0&&tc_strlen(itemId)>0)
    {
		fprintf(flogPtr," Inside if condition where userId,password,hostname,itemid are available \n");
		itkApiCallStatus=ITK_APICALL(ITK_init_module(sUserName,sPassword,sGroup),flogPtr); 
		fprintf(flogPtr,"itkApiCallStatus = %d \n",itkApiCallStatus);	
		if(itkApiCallStatus==ITK_ok)
		{	
		    fprintf(flogPtr,"Login Successfull to TC \n");
            
            tc_strcpy(*Response,"Login successfull in TC.");			
						
		    itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
		    if(itkApiCallStatus==ITK_ok)
			{
				fprintf(flogPtr,"Name of the logged in user = %s \n",loggedInUser);                
				tempSequenceId=0;
				sequenceId=0;
				fprintf(flogPtr,"\n *********************************************************** PROCESS SUCCESSFULLY STARTED ************************************************************ \n");																								
				fprintf(flogPtr,"File itemId =  %s\n",itemId);
				fprintf(flogPtr,"File itemRevId =  %s\n",itemRevId);
				fprintf(flogPtr,"File itemRevSequence =  %s\n",itemRevSequence);
								
				if(tc_strcmp(itemId,NULL)!=0)
				{
				    trimLeading(itemId);
		            trimTrailing(itemId);
				}
				if(tc_strcmp(itemRevSequence,NULL)!=0)
				{
					trimLeading(itemRevId);
		            trimTrailing(itemRevId);														
				}
				if(tc_strcmp(itemRevId,NULL)!=0)
				{
					trimLeading(itemRevSequence);
		            trimTrailing(itemRevSequence);
				}
																																
				fprintf(flogPtr,"After trim File itemId =  %s \n",itemId);
				fprintf(flogPtr,"After trim File itemRevId =  %s \n",itemRevId);
				fprintf(flogPtr,"After trim File itemRevSequence =  %s \n",itemRevSequence);
				
				tc_strupr(itemRevId,&upritemRevId);
				tc_strupr(itemRevSequence,&upritemRevSequence); 
				
				fprintf(flogPtr," - upritemRevId =  %s \n",upritemRevId);
				fprintf(flogPtr," - upritemRevSequence =  %s \n",upritemRevSequence);
				
				tc_strcpy(itemRevId,upritemRevId);
				tc_strcpy(itemRevSequence,upritemRevSequence);
				
				fprintf(flogPtr,"- UPPER itemRevId =  %s \n",itemRevId);
				fprintf(flogPtr,"- UPPER itemRevSequence =  %s \n",itemRevSequence);
				
				if(tc_strcmp(itemRevId,NULL)==0||tc_strcmp(itemRevId,"")==0)
				{
					fprintf(flogPtr," Inside if condition where itemRevId == null or empty \n");
					itemRevId=(char*) MEM_alloc(10*sizeof(char));
					tc_strcpy(itemRevId,"NA");
					fprintf(flogPtr," - After Condition itemRevId =  %s \n",itemRevId);
					fprintf(flogPtr," - Final itemId =  %s \n",itemId);
					
				}
				if(tc_strcmp(itemRevSequence,NULL)==0||tc_strcmp(itemRevSequence,"\n")==0||tc_strcmp(itemRevSequence,"")==0)
				{
					fprintf(flogPtr," Inside if condition where itemRevSequence == null or empty \n");
					itemRevSequence=(char*) MEM_alloc(10*sizeof(char));
					tc_strcpy(itemRevSequence,"NA");
					fprintf(flogPtr," - After Condition itemRevSequence =  %s \n",itemRevSequence);
					fprintf(flogPtr," - Final itemId =  %s \n",itemId);
				}																						
														
				if(tc_strcmp(itemId,"NA")==0||tc_strlen(itemId)==0||tc_strcmp(itemId,NULL)==0||tc_strcmp(itemId,"")==0)
				{
					fprintf(flogPtr,"If condition with tc_strcmp(itemId,NA)==0||tc_strlen(itemId)==0||tc_strcmp(itemId,NULL)==0||tc_strcmp(itemId,empty)==0 executed \n");
					fprintf(flogPtr,"Itemid %s is not valid as per the requiment please enter a valid Id !!!!!! \n",itemId);				
				}
				else
				{
					fprintf(flogPtr,"Else condition with valid itemId executed \n");
					
					// For latest working rev with ERC release status 
					if(tc_strcmp(itemRevId,NULL)==0||tc_strcmp(itemRevId,"NA")==0||tc_strcmp(itemRevId,"NA")==0&&tc_strcmp(itemRevSequence,"NA")==0||tc_strcmp(itemRevId,"NA")==0&&tc_strcmp(itemRevSequence,NULL)==0&&tc_strlen(itemId)>0)
					{
						fprintf(flogPtr,"If condition with tc_strcmp(itemRevId,NULL)==0||tc_strcmp(itemRevId,NA)==0||tc_strcmp(itemRevId,NA)==0&&tc_strcmp(itemRevSequence,NA)==0||tc_strcmp(itemRevId,NA)==0&&tc_strcmp(itemRevSequence,NULL)==0&&tc_strlen(itemId)>0 \n");
						fprintf(flogPtr," Execution started for latest working revision \n");
						fprintf(flogPtr," - itemId = %s \n",itemId);
						fprintf(flogPtr," - itemRevId = %s \n",itemRevId);
						fprintf(flogPtr," - itemRevSequence = %s\n",itemRevSequence);
						rlsStsFound=0;								
						// Expansion with latest rev 
						itkApiCallStatus=ITK_APICALL(ITEM_find_item(itemId,&itemTag),flogPtr);
						fprintf(flogPtr," API Status for ITEM_find_item = %d \n",itkApiCallStatus);
						if(itkApiCallStatus==0&&itemTag!=NULLTAG)
						{												
							fprintf(flogPtr," Item with %s itemId found  .... \n",itemId);
							itkApiCallStatus=ITK_APICALL(ITEM_list_all_revs(itemTag,&revCount,&revTags),flogPtr);
							fprintf(flogPtr," API Status for ITEM_list_all_revs = %d \n",itkApiCallStatus);
							fprintf(flogPtr,"Total RevCount =  %d \n",revCount);
							if(revCount > 0)
							{
								fprintf(flogPtr,"Executing condition where Rev Count > 0 \n");
								for(i=revCount-1;i>=0;i--)
								{
									rlsStsFound=0;
									revFound=0;
									ITK_APICALL(AOM_ask_value_string(revTags[i],"object_name",&trgObjName),flogPtr);
			                        ITK_APICALL(AOM_ask_value_string(revTags[i],"object_string",&trgObjStr),flogPtr);
							        ITK_APICALL(AOM_ask_value_string(revTags[i],"item_revision_id",&trgRevId),flogPtr);
							        ITK_APICALL(AOM_ask_value_int(revTags[i],"sequence_id",&sequenceId),flogPtr);
									ITK_APICALL(AOM_ask_value_tags(revTags[i],"release_status_list",&rlsStsCount,&rlsStsList),flogPtr);
									
									fprintf(flogPtr,"-trgObjName = %s\n",trgObjName); 
							        fprintf(flogPtr,"-trgObjStr = %s\n",trgObjStr); 
							        fprintf(flogPtr,"-trgRevId = %s\n",trgRevId); 
							        fprintf(flogPtr,"-sequenceId = %d\n",sequenceId);
									fprintf(flogPtr,"-rlsStsCount = %d\n",rlsStsCount);										
									
									if(rlsStsCount > 0)
									{
										fprintf(flogPtr,"Executing condition where rlsStsCount Count > 0 \n");
									    for(j=0;j<rlsStsCount;j++)
								        {
										    ITK_APICALL(AOM_ask_value_string(rlsStsList[j],"object_name",&rlsStsName),flogPtr);
										    fprintf(flogPtr," - ***** rlsStsName = %s \n",rlsStsName);
											if(tc_strcmp(rlsStsName,"ERC Released")==0||tc_strcmp(rlsStsName,"T5_LcsErcRlzd")==0)
											{
												rlsStsFound=1;
												revFound=1;
												break;
											}
											
									    }
										if(rlsStsFound==1)
										{
											fprintf(flogPtr,"Executing condition where rlsStsFound == 1 or status found \n");
											if(tc_strstr(trgRevId,";")==NULL)
											{
												fprintf(flogPtr," (;) not found in revId condition \n"); 
												h1=0;
												h2=0;
												h3=0;
												char *trgsubSequence = (char*)MEM_alloc(10*sizeof(char));
												char *trgSeqAftconv  = (char*)MEM_alloc(10*sizeof(char));																											
												while(sequenceId!=0)
												{
													h1=sequenceId%10; 
													sequenceId=sequenceId/10; 
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
												fprintf(flogPtr,"-Update Directory status = %d \n",updateDirectory(hostname,sUserName,itemId,trgRevId,trgSeqAftconv,flogPtr));
											}
											else
											{
												fprintf(flogPtr," (;) found in revId condition \n");
												char *trgsubrevId = strtok(trgRevId,";");
												char *trgsubSequence = strtok (NULL,"");
												
												fprintf(flogPtr," - trgsubrevId = %s \n",trgsubrevId);
												fprintf(flogPtr," - trgsubSequence = %s \n",trgsubSequence);
												
												fprintf(flogPtr,"-Update Directory status = %d \n",updateDirectory(hostname,sUserName,itemId,trgsubrevId,trgsubSequence,flogPtr));
											}
																		
											fprintf(flogPtr," Bom expansion started with %s trgObjName %s trgObjStr %s trgRevId %d sequenceId \n",trgObjName,trgObjStr,trgRevId,sequenceId);
											
											itkApiCallStatus=ITK_APICALL(BOM_create_window(&windowTag),flogPtr); 		
											fprintf(flogPtr," API Status for BOM_create_window = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(CFM_find("ERC release and above",&revRule),flogPtr);				
											fprintf(flogPtr," API Status for CFM_find = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_config_rule(windowTag,revRule),flogPtr);
											fprintf(flogPtr," API Status for BOM_set_window_config_rule = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_pack_all(windowTag,true),flogPtr);
											fprintf(flogPtr," API Status for BOM_set_window_pack_all = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_top_line(windowTag,itemTag,revTags[i],NULLTAG,&topBomLineTag),flogPtr);
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
											
											fprintf(flogPtr," ################## Total Target Object Found with %s ItemId and %s ObjName is %d in Main function \n",blItemId,blObjectName,FindTrgObj(blItemId,blObjectName,revTags[i],flogPtr,fJtlogPtr));
											
											if(bomChildCount>0)
											{
												BOM_ASK_CHILD(bomChildCount,bomChildTags,flogPtr,fJtlogPtr);
											}	
		                                    break;																										
										}
										else
										{
											fprintf(flogPtr,"Executing condition where rlsStsFound == 0 or releaseStatus not found !!!! \n");
										}												
									}
									else
									{
										fprintf(flogPtr,"Executing condition where rlsStsCount Count == 0 \n ");
									}											
								}
                                if(revFound==1)
								{
									fprintf(flogPtr,"PartRevision found with ERC released status execution completed with %s itemid ...\n ",itemId);									
								}
                                else
								{
									fprintf(flogPtr,"PartRevision not found with ERC released status execution incompleted with %s itemid ...\n ",itemId);
									tc_strcpy(*Response,"PartRevision not found with ERC Released status or no part released.");
								}																	
							}
							else
							{
								fprintf(flogPtr,"Rev Count == 0 \n ");
							}	
							
						}																						
						else
						{
							fprintf(flogPtr," Item with %s itemId not found  .... \n",itemId);
							tc_strcpy(*Response,"Part with given id not found or part don't exists.");
						}															
					}																					
					else
					{
						fprintf(flogPtr,"Else condition with valid itemRevId executed condition will be checked for itemRevSequence \n");
						
						// For condition where (itemid,iteRevid,na);
						
						if(tc_strcmp(itemRevSequence,"NA")==0 && tc_strcmp(itemRevId,"NA")!=0)
						{   
							fprintf(flogPtr,"If condition with if(tc_strcmp(itemRevSequence,NA)==0 && tc_strcmp(itemRevId,NA)!=0\n");
							fprintf(flogPtr," -itemId = %s \n",itemId);
							fprintf(flogPtr," -itemRevId = %s \n",itemRevId);
							fprintf(flogPtr," -itemRevSequence = %s \n",itemRevSequence);	
							
							itkApiCallStatus=ITK_APICALL(ITEM_find_item(itemId,&itemTag),flogPtr);
						    fprintf(flogPtr," API Status for ITEM_find_item = %d \n",itkApiCallStatus);
						    if(itkApiCallStatus==0&&itemTag!=NULLTAG)
						    {
							    fprintf(flogPtr," Item with %s itemId found  .... \n",itemId);
							    itkApiCallStatus=ITK_APICALL(ITEM_list_all_revs(itemTag,&revCount,&revTags),flogPtr);
							    fprintf(flogPtr," API Status for ITEM_list_all_revs = %d \n",itkApiCallStatus);
							    fprintf(flogPtr,"Total RevCount =  %d \n",revCount);
							    if(revCount > 0)
						    	{
									fprintf(flogPtr,"Executing condition where Rev Count > 0 \n");
									for(i=revCount-1;i>=0;i--)
									{
										revFound=0;
										ITK_APICALL(AOM_ask_value_string(revTags[i],"object_name",&trgObjName),flogPtr);
			                            ITK_APICALL(AOM_ask_value_string(revTags[i],"object_string",&trgObjStr),flogPtr);
							            ITK_APICALL(AOM_ask_value_string(revTags[i],"item_revision_id",&trgRevId),flogPtr);
							            ITK_APICALL(AOM_ask_value_int(revTags[i],"sequence_id",&sequenceId),flogPtr);
									    ITK_APICALL(AOM_ask_value_tags(revTags[i],"release_status_list",&rlsStsCount,&rlsStsList),flogPtr);
										
										fprintf(flogPtr,"-trgObjName = %s\n",trgObjName); 
							            fprintf(flogPtr,"-trgObjStr = %s\n",trgObjStr); 
							            fprintf(flogPtr,"-trgRevId = %s\n",trgRevId); 
							            fprintf(flogPtr,"-sequenceId = %d\n",sequenceId);
									    fprintf(flogPtr,"-rlsStsCount = %d\n",rlsStsCount);	
										
										if(tc_strstr(trgRevId,";")==NULL)
										{
											fprintf(flogPtr," (;) not found in revId condition \n"); 
											h1=0;
											h2=0;
											h3=0;
											char *trgsubSequence = (char*)MEM_alloc(10*sizeof(char));
											char *trgSeqAftconv  = (char*)MEM_alloc(10*sizeof(char));																						
											while(sequenceId!=0)
											{
												h1=sequenceId%10; 
												sequenceId=sequenceId/10; 
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
		                                    if(tc_strcmp(itemRevId,trgRevId)==0)
											{
												fprintf(flogPtr,"Matching rev with %s trgRevId found in TC Sequence is not taken care \n",trgRevId); 
												fprintf(flogPtr,"-Update Directory status = %d \n",updateDirectory(hostname,sUserName,itemId,trgRevId,trgSeqAftconv,flogPtr));
											    revFound=1;
											}													
										}
										else
										{
											fprintf(flogPtr," (;) found in revId condition \n");
											char *trgsubrevId = strtok(trgRevId,";");
											char *trgsubSequence = strtok (NULL,"");
											
											fprintf(flogPtr," - trgsubrevId = %s \n",trgsubrevId);
											fprintf(flogPtr," - trgsubSequence = %s \n",trgsubSequence);
											
											if(tc_strcmp(itemRevId,trgsubrevId)==0)
											{
												fprintf(flogPtr,"Matching rev with %s trgsubrevId found in TC Sequence is not taken care \n",trgsubrevId); 
												fprintf(flogPtr,"-Update Directory status = %d \n",updateDirectory(hostname,sUserName,itemId,trgsubrevId,trgsubSequence,flogPtr));
											    revFound=1;
											}													
										}
										if(revFound==1)
										{
											fprintf(flogPtr," Bom expansion started with %s trgObjName %s trgObjStr %s trgRevId %d sequenceId \n",trgObjName,trgObjStr,trgRevId,sequenceId);
											
											itkApiCallStatus=ITK_APICALL(BOM_create_window(&windowTag),flogPtr); 		
											fprintf(flogPtr," API Status for BOM_create_window = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(CFM_find("ERC release and above",&revRule),flogPtr);				
											fprintf(flogPtr," API Status for CFM_find = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_config_rule(windowTag,revRule),flogPtr);
											fprintf(flogPtr," API Status for BOM_set_window_config_rule = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_pack_all(windowTag,true),flogPtr);
											fprintf(flogPtr," API Status for BOM_set_window_pack_all = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_top_line(windowTag,itemTag,revTags[i],NULLTAG,&topBomLineTag),flogPtr);
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
											
											fprintf(flogPtr," ################## Total Target Object Found with %s ItemId and %s ObjName is %d in Main function \n",blItemId,blObjectName,FindTrgObj(blItemId,blObjectName,revTags[i],flogPtr,fJtlogPtr));
											
											if(bomChildCount>0)
											{
												BOM_ASK_CHILD(bomChildCount,bomChildTags,flogPtr,fJtlogPtr);
											}	
											break;													
										}
										else
										{
											fprintf(flogPtr," Bom expansion process not started or failed with %s itemRevId and %s itemRevSequence because rev not found .... \n",itemRevId,itemRevSequence);                                            				
										}											
									}									
									if(revFound==0)
									{   
									    fprintf(flogPtr,"No PartRevision found with %s itemRevId and %s itemRevSequence.\n",itemRevId,itemRevSequence); 			
									    tc_strcpy(*Response,"PartRevision with given rev id and sequence id not found or partrevision don't exists.");	
									}												
								}
								else
								{
									fprintf(flogPtr,"Executing condition where Rev Count == 0 \n ");
								}
							}
							else
							{
								fprintf(flogPtr," Item with %s itemId not found  .... \n",itemId);
								tc_strcpy(*Response,"Part with given id not found or part don't exists.");
							}																		
						}
						else
						{
							// For condition where (itemid,iteRevid,itemRevSequence);
							// General Expansion executed
							
							fprintf(flogPtr,"Executing else condition where All the three value are properly available ..... \n");
																								
							fprintf(flogPtr," -itemId = %s \n",itemId);
							fprintf(flogPtr," -itemRevId = %s \n",itemRevId);
							fprintf(flogPtr," -itemRevSequence = %s \n",itemRevSequence);									
												
							itkApiCallStatus=ITK_APICALL(ITEM_find_item(itemId,&itemTag),flogPtr);
							fprintf(flogPtr," API Status for ITEM_find_item = %d \n",itkApiCallStatus);
							
							if(itkApiCallStatus==0&&itemTag!=NULLTAG)
							{							
								itkApiCallStatus=ITK_APICALL(ITEM_list_all_revs(itemTag,&revCount,&revTags),flogPtr);
								fprintf(flogPtr," API Status for ITEM_list_all_revs = %d \n",itkApiCallStatus);
								
								fprintf(flogPtr,"Total RevCount =  %d \n",revCount);
								for(t=0;itemRevSequence[t]!='\0';t++)
								{
									tempSequenceId=tempSequenceId*10;
									tempSequenceId=tempSequenceId+((int)(itemRevSequence[t])-48);
							    }
								fprintf(flogPtr," ******* tempSequenceId after converting in INT found = %d \n *******",tempSequenceId); 
								if(revCount>0)
								{
									fprintf(flogPtr,"Rev Count > 0 \n");
									for(i=0;i<revCount;i++)
									{
										temp=0;
										revFound=0;
										char *tempRevId     = (char*) malloc(50*sizeof (char));   																
										ITK_APICALL(AOM_ask_value_string(revTags[i],"item_revision_id",&revId),flogPtr);
										ITK_APICALL(AOM_ask_value_int(revTags[i],"sequence_id",&sequenceId),flogPtr);
										fprintf(flogPtr,"TC revId found = %s \n",revId); 
										fprintf(flogPtr,"TC sequenceId found = %d \n",sequenceId); 	
										tsequenceId=sequenceId;
								        if(tc_strstr(revId,";")==NULL)
										{
											fprintf(flogPtr," (;) not found in revId condition \n"); 
											h1=0;
											h2=0;
											h3=0;
											char *trgsubSequence = (char*)MEM_alloc(10*sizeof(char));
											char *trgSeqAftconv  = (char*)MEM_alloc(10*sizeof(char));																							
											while(sequenceId!=0)
											{
												h1=sequenceId%10; 
												sequenceId=sequenceId/10; 
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
											if(tc_strcmp(itemRevId,revId)==0 && tsequenceId == tempSequenceId)
											{
												fprintf(flogPtr,"Matching rev with %s revId and %d sequenceId found in TC \n",revId,tsequenceId); 
												fprintf(flogPtr,"-Update Directory status = %d \n",updateDirectory(hostname,sUserName,itemId,revId,trgSeqAftconv,flogPtr));
											    revFound=1;
											}																																										
										}
										else
										{
											fprintf(flogPtr," (;) found in revId condition \n");
											char *trgsubrevId = strtok(revId,";");
											char *trgsubSequence = strtok (NULL,"");
											
											fprintf(flogPtr," - trgsubrevId = %s \n",trgsubrevId);
											fprintf(flogPtr," - trgsubSequence = %s \n",trgsubSequence);
											
											if(tc_strcmp(itemRevId,trgsubrevId)==0 && tsequenceId == tempSequenceId)
											{
												fprintf(flogPtr,"Matching rev with %s revId and %d sequenceId found in TC \n",revId,tsequenceId); 
												fprintf(flogPtr,"-Update Directory status = %d \n",updateDirectory(hostname,sUserName,itemId,trgsubrevId,trgsubSequence,flogPtr));
											    revFound=1;
											}																																																
								        }																																																										
								        if(revFound==1)
										{
											fprintf(flogPtr," Bom expansion started .... \n",revId,sequenceId); 
											
											itkApiCallStatus=ITK_APICALL(BOM_create_window(&windowTag),flogPtr); 		
											fprintf(flogPtr," API Status for BOM_create_window = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(CFM_find("ERC release and above",&revRule),flogPtr);									
											fprintf(flogPtr," API Status for CFM_find = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_config_rule(windowTag,revRule),flogPtr);
											fprintf(flogPtr," API Status for BOM_set_window_config_rule = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_pack_all(windowTag,true),flogPtr);
											fprintf(flogPtr," API Status for BOM_set_window_pack_all = %d \n",itkApiCallStatus);
											
											itkApiCallStatus=ITK_APICALL(BOM_set_window_top_line(windowTag,itemTag,revTags[i],NULLTAG,&topBomLineTag),flogPtr);
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
											
											fprintf(flogPtr," ################## Total Target Object Found %s ItemId and %s ObjName is %d in Main function\n",blItemId,blObjectName,FindTrgObj(blItemId,blObjectName,revTags[i],flogPtr,fJtlogPtr));
											
											if(bomChildCount>0)
											{
												BOM_ASK_CHILD(bomChildCount,bomChildTags,flogPtr,fJtlogPtr);
											}
											break;												
										}	
								        else
										{
											fprintf(flogPtr," Bom expansion process not started or failed with %s itemRevId and %s itemRevSequence because rev not found .... \n",itemRevId,itemRevSequence);                          						 
										}									
									}
									if(revFound==0)
									{   
									    fprintf(flogPtr,"No PartRevision found with %s itemRevId and %s itemRevSequence.\n",itemRevId,itemRevSequence); 			
									    tc_strcpy(*Response,"PartRevision with given rev id and sequence id not found or partrevision don't exists.");	
									}											
								}
								else
								{
									fprintf(flogPtr,"Rev Count <= 0 \n ");
								}																									
							}
							else
							{
								fprintf(flogPtr," Item with %s itemId not found  .... \n",itemId);
								tc_strcpy(*Response,"Part with given id not found or part don't exists.");
							}									
						}							    							  
					}				
				}						
				fprintf(flogPtr,"\n *********************************************************** PROCESS SUCCESSFULLY ENDED 16_12_2024 ************************************************************ \n ");			
				fprintf(flogPtr,"\n Ouput folder path = %s \n",glbDirectory);
				if(tc_strlen(glbDirectory)>0&&tc_strcmp(glbDirectory,NULL)!=0)
				{	
			        tc_strcat(*Response," Webservice execution completed.");
					tc_strcat(*Response," Output directory = ");
                    tc_strcat(*Response,glbDirectory);					
				}			
			}
		    else
			{
			   fprintf(flogPtr,"\n Logged in User not found or error \n");
		    }			
		}
		else
		{			
			fprintf(flogPtr,"\n ************************************************************************************ Login failed in TC \n");
		    tc_strcpy(*Response,"Login does not exist invalid userid or password.");	
		}
	}
    else
	{
		fprintf(flogPtr," Inside else condition where userId or password or hostname or itemid are not available \n");
	    tc_strcpy(*Response,"Execution failed please fill all required fields.");	
	}
	fprintf(flogPtr,"\n ******************************** MultiBomExpansion exe execution Ended  ********************************\n");  	

    fclose(flogPtr);
	fclose(fJtlogPtr);
	
	dup2(fd, fileno(stdout));
	close(fd);
	clearerr(stdout);
	fsetpos(stdout, &pos);
	return SOAP_OK;
}
;
