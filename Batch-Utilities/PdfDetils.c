/* Description : C Progam to create a part and document in TCE and attaching a pdf file with it*/

#include <cl.h>
#include <msg.h>
#include <msgapc.h>
#include <Mtimsgh.h>
#include <mutproto.h>
#include <nls.h>
#include <pdmc.h>
#include <pdmroot.h>
#include <relation.h>
#include <status.h>  
#include <usc.h>
#define _CLMAIN_DEFNS
#include <cfg.h>    
#include <ft.h>
#include <ft.h>
#include <meta.h>
#include <mserv.h>
#include <msglcm.h>
#include <msgomf.h>
#include <msgtel.h>
#include <Mtimsg.h>
#include <pdmitem.h>
#include <pdmsessn.h>
#include <sc.h>
#include <serv.h>
#include <sys.h>
#include <tel.h>
#include <ui.h>    
#include<string.h>
char* subString(char* mainStringf,  int fromCharf,  int toCharf)
{
        int i;
        char* retStringf;
        retStringf =  (char*) malloc(toCharf + 1);
        for(i = 0; i < toCharf; i++ )
                *(retStringf + i) =  *(mainStringf + i + fromCharf);
        *(retStringf + i) =  '\0';
        return retStringf;
}
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

int main(int argc, char *argv[])
{
	int ii=0;
    int cntt=0;
    int partFound=0;
	int documentFound = 0;
	int pdfFound=0;
	int i=0;
	int j=0;
	int LineNo=1;
    int stat = OKAY;  
	int count=0;
	int frzstat=0;
	
    string docobjst               =          NULL;
    SetOfStrings dbScp            =          NULL;  
    char filedata[500];	
	char *partNo = NULL;
    char *partNoTemp=(char *)malloc(20 * sizeof(char));	
	char *revisionId = NULL; 
	char *revisionSequence = NULL; 
	
	SqlPtr a_sql          = NULL;
    SqlPtr Sql_ptr1       = NULL;
	SqlPtr Sql_ptr2       = NULL;
	SqlPtr part_ptr1       = NULL;
	SqlPtr part_ptr2       = NULL;
    SetOfObjects PartSet  = NULL;
	SetOfObjects DocumentSet = NULL;
	SetOfObjects PDFSet = NULL;
	SetOfObjects	PdfObj				= NULL;
	
	ObjectPtr PartObjPtr= NULL;
	ObjectPtr DocumentObjPtr= NULL;
	ObjectPtr PDFObjPtr= NULL;
	ObjectPtr PDFAttachedObjPtr= NULL;
	ObjectPtr CldPrtObj= NULL;
	ObjectPtr CldPrtObj2= NULL;
	ObjectPtr outputObj = NULL;
	ObjectPtr outputObj2 = NULL;
	ObjectPtr relationPtr=NULL;
	
	ObjectPtr DocCreateDia = NULL;
	ObjectPtr oNewObj = NULL ;
	ObjectPtr oDocObj = NULL ;
	
	SetOfObjects notFrozenDepObjs = NULL;
	SetOfObjects allObjsToFreeze = NULL;
	SetOfObjects dataItems = NULL;
	SetOfObjects    PrtDocSO1				= NULL;
	SetOfObjects    PrtDocRelSO1		    = NULL;
	SetOfObjects	PdfObjectSet			=	NULL;
	
	string tempstat =NULL ;
	string rev = NULL ;
	string PdfFileDup=NULL;
	PdfFileDup=nlsStrAlloc(500);
	
	string PdfFileQueryname = NULL; 
	string PdfFileAttachedName= NULL; 
	PdfFileQueryname = nlsStrAlloc(500);
	PdfFileAttachedName = nlsStrAlloc(500);
	
	
   t5MethodInitWMD("userinfo");
   FILE *flogPtr=fopen("PdfDetilslogFile.txt","a");
   FILE *fptr=fopen("input.txt","r");
   fprintf(flogPtr,"\n ***********************************************************************************************************************************************************************");
   fprintf(flogPtr,"\n ***********************************************************************************************************************************************************************");   
   fprintf(flogPtr,"\n Date  - %s ",__DATE__);
   fprintf(flogPtr,"\n Time  - %s ",__TIME__);
   
    if (dstat = clInitMB2 (argc, &argv, NULL)) goto CLEANUP;
    if (dstat = clTestNetwork ()) goto CLEANUP;
    if (dstat = clInitialize2 (FALSE)) goto CLEANUP;
    if (dstat = clLogin2("super user","abc123",&stat)) goto CLEANUP;
    if (stat != OKAY)
    {
		fprintf(flogPtr,"\nLogin Failed in TCE !");
        printf("\nInvalid User Name or Password !!!!!!!!!!!!\n");
		fflush(stdout);
    }
	else 
    {	
		fprintf(flogPtr,"\nLogin successfull in TCE !");
        printf("\nLogin Successful In TCE..... !!!!\n");
		fflush(stdout);
		if(fptr!=NULL)
		{
			fprintf(flogPtr,"\nFile found and successfully opened ... \n");
			printf("\nFile found and successfully opened ... ");
			while(fgets(filedata, 500, fptr) != NULL) 
            {
				int PdfFoundArray[1000]={0};
				fprintf(flogPtr,"\n ************************ File Line %d - %s ************************",LineNo,filedata);
				printf("\n ************************ File Line %d - %s ************************",LineNo,filedata);
				fprintf(flogPtr,"\nLength of data = %d \n",strlen(filedata));	
				trimLeading(filedata);
				trimTrailing(filedata);	
				fprintf(flogPtr,"Length of data after trimming= %d \n",strlen(filedata));
				
				//partNo       = strtok(filedata,"-");
				//revisionId   = strtok(NULL,"-");
			    //revisionSequence = strtok(NULL,".");
				if(nlsStrStr(filedata,"-")!=0)
				{
					partNo       = strtok(filedata,"-");
					revisionId   = strtok(NULL,"-");
					revisionSequence = strtok(NULL,".");
				}
				else{
					partNo       = strtok(filedata,".");
					//revisionId   = strtok(NULL,"-");
					//revisionSequence = strtok(NULL,".");					
				}
						   
			    fprintf(flogPtr,"partNo       - %s \n", partNo);
			    fprintf(flogPtr,"revisionId    - %s\n", revisionId);
			    fprintf(flogPtr,"revisionSequence - %s\n", revisionSequence);
				fprintf(flogPtr,"partNoTemp       - %s \n", partNoTemp);				
				printf("partNo       - %s \n", partNo);
				printf("revisionId    - %s\n", revisionId);
				printf("revisionSequence - %s\n", revisionSequence);
				printf("partNoTemp       - %s \n", partNoTemp);
								
				
				if(strcmp(partNoTemp,partNo)==0)
				{
					printf("Inside if condition with partNoTemp == partNo ");
					fprintf(flogPtr,"Inside if condition with partNoTemp == partNo ");
					fprintf(flogPtr," \n ************ %s partno is repeated again process stoped for %s partno ", partNoTemp,partNoTemp);
				}
				else
				{
					printf("Inside else with partNoTemp != partNo condition ");
					fprintf(flogPtr,"Inside if condition with partNoTemp != partNo ");
					fprintf(flogPtr," \n ************ Query process started for %s part no !!!! ",partNo);
					//Query to search part in TCE .
					
					if ((dstat = oiSqlCreateSelect(&a_sql)) ||
						(dstat = oiSqlWhereEQ(a_sql,PartNumberAttr,partNo)) ||
						(dstat =oiSqlWhereAND(a_sql))||
						(dstat = oiSqlWhereEQ(a_sql,SupersededAttr,"-")) ||
						(dstat = QueryWhere(PartClass,a_sql,&PartSet,mfail)) )	goto EXIT;
						if (*mfail) goto CLEANUP;
						printf("\n No of PartSet t5Create_PP-Part:%d for NewPartNo :%s \n",setSize(PartSet),partNo);
						partFound=setSize(PartSet);
						printf("\npartFound: [%d]",partFound);
					    fprintf(flogPtr," \n ************************************************************ Total part's Found  = %d ",partFound);
						if(partFound==0)
						{
							printf(" Part with %s Id not found.. .",partNo);
							fprintf(flogPtr,"\n Part with %s Id not found Creation logic started in TCE ...",partNo);
						}
						else
						{
							printf(" Part with %s Id is already present in TCE .",partNo);
							fprintf(flogPtr,"\n *********** Part with %s Id is already present in TCE .**********",partNo);
							PartObjPtr=setGet(PartSet,0);
						}
											
					//Query to search Document in TCE . 					
					if ((dstat = oiSqlCreateSelect(&Sql_ptr1)) ||
						(dstat = oiSqlWhereEQ(Sql_ptr1,DocumentNameAttr,partNo)) ||
						(dstat =oiSqlWhereAND(Sql_ptr1))||
						(dstat = oiSqlWhereEQ(Sql_ptr1,SupersededAttr,"-")) ||
						(dstat = QueryWhere(t5DrwDocClass,Sql_ptr1,&DocumentSet,mfail))) goto EXIT;
						if (*mfail) goto CLEANUP;
						printf("\n No of DocumentSet t5Create_PP-Part:%d for NewPartNo :%s \n",setSize(DocumentSet),partNo);
						documentFound=setSize(DocumentSet);
						printf("\documentFound: [%d]",documentFound);
					    fprintf(flogPtr,"\n ************************************************************ Total document's Found  = %d ",documentFound);
						
						if(documentFound==0)
						{
							printf(" Document with %s Id not found Creation logic started in TCE .",partNo);
							fprintf(flogPtr,"\n Document with %s Id not found Creation logic started in TCE ...",partNo);
						}
						else
						{
							printf(" Document with %s Id is already present in TCE .",partNo);
							fprintf(flogPtr,"\n *********** Document with %s Id is already present in TCE .**********",partNo);
							DocumentObjPtr=setGet(DocumentSet,0);
						}	  						
						//Query to search PDF in TCE . 					
					   printf(" \n Query to serach pdf started ...... \n");					
						    nlsStrCpy(PdfFileDup,"");
						    nlsStrCat(PdfFileDup,"*");
						    nlsStrCat(PdfFileDup,partNo);
							nlsStrCat(PdfFileDup,"*");
							nlsStrCat(PdfFileDup,"pdf");
							printf("\nPDF FILE NAME [%s]",PdfFileDup);					
						    PdfObj = NULL;
							if(Sql_ptr2) oiSqlDispose(Sql_ptr2); Sql_ptr2 = NULL;							
					        if (dstat = oiSqlCreateSelect(&Sql_ptr2)) goto CLEANUP;							
							if(dstat=(oiSqlWhereBegParen(Sql_ptr2)))goto CLEANUP;	
							if(dstat = oiSqlWhereLike(Sql_ptr2,WorkingRelativePathAttr,PdfFileDup)) goto CLEANUP;
							if(dstat=(oiSqlWhereEndParen(Sql_ptr2))) goto CLEANUP;
							if(dstat = QueryDbObject(PdfFileClass,Sql_ptr2,0,TRUE,SC_SCOPE_OF_SESSION,&PdfObj,mfail)) goto CLEANUP;
							if(Sql_ptr2) oiSqlDispose(Sql_ptr2); Sql_ptr2 = NULL;
							pdfFound=setSize(PdfObj);								
					        fprintf(flogPtr,"\n ************************************************************ Total pdf's Found value = %d ",pdfFound);												
							printf("\n Total of pdf Found %d\n",setSize(PdfObj));fflush(stdout);								
							if(setSize(PdfObj)==0)
								{
									printf(" PDF with %s Id not found Creation logic started in TCE .",partNo);
							        fprintf(flogPtr,"\n PDF with %s Id not found Creation logic started in TCE ...",partNo);
								}
								else{
									printf(" PDF with %s Id is already present in TCE .",partNo);
							        fprintf(flogPtr,"\n *********** PDF with %s Id is already present in TCE .**********",partNo);							
								}
					
					    if(partFound==0 && documentFound==0)
					    {
					    	// Creating Part
					    	// Creating Document
					    	// Creating Pdf and attaching
					    	fprintf(flogPtr,"\n If condition with partFound==0 && documentFound==0 executed ...");
					    	printf("\n If condition with partFound==0 && documentFound==0 executed ..."); 
					    	  // Part Creation logic
					    	  fprintf(flogPtr,"\n *********** Part creation started in TCE .**********");
					    	  if(dstat=ConstructArgsForCreate("t5PeAsm",&outputObj,mfail)) goto EXIT; 
					    	  if(dstat=objSetAttribute(outputObj,PartNumberAttr,partNo)) goto EXIT;  
					    	  if(dstat=objSetAttribute(outputObj,RevisionAttr,"NR")) goto EXIT;    
					    	  if(dstat=objSetAttribute(outputObj,SequenceAttr,"1")) goto EXIT;
					    	  if(dstat=objSetAttribute(outputObj,t5PEDesignGrpAttr,"CTED")) goto EXIT;
					    	  if(dstat=objSetAttribute(outputObj,PPartTypeAttr,"Other")) goto EXIT;   
					    	  if(dstat=objSetAttribute(outputObj,NomenclatureAttr,partNo)) goto EXIT; 
					    	  if(dstat=objSetAttribute(outputObj,t5MRPControllerAttr,"DES")) goto EXIT; 
					       //   if (dstat = PerformFreeze(outputObj,mfail));				// Freezing logic Added 09_12_2023
					    	   if(CldPrtObj)
					    	   {
					    	  	objDispose(CldPrtObj);  
					    	  	CldPrtObj = NULL;
					    	   }
					    	   if(dstat=CreateObject("t5PeAsm",outputObj,&CldPrtObj,mfail)) goto EXIT;
					    	   printf(" \nAfter Create Object(post left).....NewColourPart: \n");fflush(stdout);
					    	   if(dstat = oiSqlCreateSelect(&part_ptr1)) goto CLEANUP;
					    	   if(dstat = oiSqlWhereEQ(part_ptr1,t5SyscdAttr,"8888")) goto CLEANUP;
					    	   if(dstat = oiSqlWhereAND(part_ptr1)) goto CLEANUP;
					    	   if(dstat = oiSqlWhereEQ(part_ptr1,t5SubSyscdAttr,"Live")) goto CLEANUP;
					    	   if(dstat = QueryDbCount("t5Cntrol",part_ptr1,SC_SCOPE_OF_SESSION,&count,mfail)) goto CLEANUP;
					    	   printf("\n No of Control Object found for CE Vault live check is %d \n",count);fflush(stdout);						  
					    	  if(CldPrtObj)
					    	  {					  	
					    	  	if(count > 0)
					          	{
					    	  			t5CheckMfail(CheckInItem(CldPrtObj,"PE Vault",mfail)); 
					    	  	}
					    	  	else
					    	  	{
					    	  			t5CheckMfail(CheckInItem(CldPrtObj,"PE Vault",mfail));
					    	  	}						  
					    	    t5CheckMfail(ChangeStateItem(CldPrtObj,"LcsReview",mfail)) ;						  
					    	  	t5CheckMfail(t0UpdateObject(&CldPrtObj,mfail));
					    		if (dstat=FreezeObject(CldPrtObj,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
					    	  	printf("\n PE Part with %s id created sussfully \n",partNo);fflush(stdout);
					    		fprintf(flogPtr,"\n *********** Part with %s id created in TCE .**********",partNo);
					    		
					    	}
					    	else{
					    		printf("\n PE Part with %s id creation failed !!!! \n",partNo);fflush(stdout);
					    		fprintf(flogPtr,"\n *********** PE Part with %s id creation failed !!!! .**********",partNo);
					    	}                        						  
					    	  // Drawing document Creation logic						  
					    	 fprintf(flogPtr,"\n *********** Drawing creation started in TCE .**********");
					    	 if(dstat=ConstructArgsForCreate(t5DrwDocClass,&DocCreateDia,mfail)) goto EXIT;
					    	 if(dstat=objSetAttribute(DocCreateDia,DocumentNameAttr,partNo)) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,SequenceAttr,"1")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,t5SheetSizeAttr,"A4")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,t5DesignGroupAttr,"00")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,ProjectNameAttr,"0009")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,DocumentTypeAttr,"DRG")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,DocumentDescriptionAttr,partNo)) goto EXIT;  
					    	 if(dstat=CreateObject(t5DrwDocClass,DocCreateDia,&oNewObj,mfail)) goto EXIT; //check for dialog  
					    	 if(dstat=objCopy(&oDocObj,oNewObj)) goto EXIT; 	
					    	 if(oDocObj)
					    	 {
					    		  t5CheckMfail(CheckInItem(oDocObj,"PE Vault",mfail)); 
					    		  t5CheckMfail(ChangeStateItem(oDocObj,"LcsWorking",mfail)) ;
					    		  t5CheckMfail(t0UpdateObject(&oDocObj,mfail));
					    		  if (dstat=FreezeObject(oDocObj,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
					    		 fprintf(flogPtr,"\n *********** Drawing document with %s id created successfully .**********",partNo);  
					    	     printf("*********** Drawing Docuement with %s Id created in TCE .**********",partNo);
					    		 // Creating relation between PE Part and Drawing document
					    	        if (dstat = CreateRelation2(PartDocClass,NULL,CldPrtObj,oDocObj,&relationPtr,mfail)) goto EXIT;  
					                printf("\n Relation created between PE Part and Document ...\n"); fflush(stdout);
					    	        fprintf(flogPtr,"\n *********** Relation between part and document created successfully  .**********");  
					    	 		if(pdfFound==0)
					    	            {
					    		            fprintf(flogPtr,"\n PDF found count is zero ...");
					    	            }
					    	        else{
					    		            // Creating relation between Drawing Document and PDF
					    		           fprintf(flogPtr,"\n PDF found count is  ...%d ",pdfFound);
					    		           for(i=0;i<pdfFound;i++)
					    		            {
					    			            PDFObjPtr=setGet(PdfObj,i);
					    						if (dstat=FreezeObject(PDFObjPtr,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
					    			            fprintf(flogPtr,"\n ********* Creating Relation for i = %d PDF",i+1); 
					    			            if (dstat = CreateRelation2(AttachClass,NULL,oDocObj,PDFObjPtr,&relationPtr,mfail)) goto EXIT;  
					                            printf("\n Relation created between Document and PDF...\n"); fflush(stdout);
					    	                   fprintf(flogPtr,"\n *********** Relation between Document and PDF created successfully  .**********");  
					    		            }
					    		           fprintf(flogPtr,"\n *********** Relation for all PDF's created successfully  .**********");
					      	           }
					    	 }
					        else{
					    		fprintf(flogPtr,"\n *********** Drawing creation failed with %s id .**********",partNo);  
					    	    printf("*********** Drawing creation failed with %s id .**********",partNo);
					    	}																			
					    }
						if(partFound==1 && documentFound==1)
					    {
					    	// Attaching PDF
					    	fprintf(flogPtr,"\n If condition with partFound==1 && documentFound==1 executed ...");
							printf("\n If condition with partFound==1 && documentFound==1 executed ...");
					    	if(pdfFound==0)
					    	{
					    		fprintf(flogPtr,"\n PDF found count is zero ...");
					    	}
					    	else
							{              
                                t5CheckMfail(ExpandObject2(PartDocClass,PartObjPtr,"DocumentsDescribingPart",SC_SCOPE_OF_SESSION,&PrtDocSO1,&PrtDocRelSO1,mfail));
                                if(setSize(PrtDocSO1)>0)
								{
									printf("\n Relation exists between PE Part and Drawing document");
									fprintf(flogPtr,"\n Relation exists between PE Part and Drawing document....");													
								}
								else{
									printf("\n Creating relation between PE Part and Drawing document");
									fprintf(flogPtr,"\n Creating relation between PE Part and Drawing document since relation not found ...");
									if (dstat = CreateRelation2(PartDocClass,NULL,PartObjPtr,DocumentObjPtr,&relationPtr,mfail)) goto EXIT;  
									printf("\n Relation created between PE Part and Document ...\n"); fflush(stdout);
									fprintf(flogPtr,"\n *********** Relation created between PE Part and Document ...  .**********");  																																					
								}
								t5CheckMfail(ExpandObject5(AttachClass,DocumentObjPtr,"DataItemsAttachedToBusItem",SC_SCOPE_OF_SESSION ,&PdfObjectSet,mfail));
								fprintf(flogPtr,"\n PDF files already attached to document = %d",setSize(PdfObjectSet));
								fprintf(flogPtr,"\n PDF found in query = %d ",pdfFound);							
								if(setSize(PdfObjectSet)==pdfFound)
								{
									printf("\n Total pdf found count == setSize(PdfObjectSet) \n"); fflush(stdout);
									fprintf(flogPtr,"\n *********** Total pdf found count == setSize(PdfObjectSet)**********"); 
 									fprintf(flogPtr,"\n *********** Executing if condition ... **********");
								}
								else
								{
									fprintf(flogPtr,"\n *********** Executing else condition ... **********");
									for(i=0;i<pdfFound;i++)
									{
										PDFObjPtr=setGet(PdfObj,i);
										if (dstat =objGetAttribute(PDFObjPtr,WorkingRelativePathAttr,&PdfFileQueryname));
										fprintf(flogPtr,"\n PdfFileQueryname = %s ",PdfFileQueryname);
										for(j=0;j<setSize(PdfObjectSet);j++)
										{
											PDFAttachedObjPtr=setGet(PdfObjectSet,j);
											if (dstat=objGetAttribute(PDFAttachedObjPtr,WorkingRelativePathAttr,&PdfFileAttachedName));
											fprintf(flogPtr,"\n ************************* PdfFileAttachedName = %s ",PdfFileAttachedName);
											if(strcmp(PdfFileQueryname,PdfFileAttachedName)==0)
											{
												fprintf(flogPtr,"File with name %s name already attached .....",PdfFileAttachedName);
												PdfFoundArray[i]=1;
												break;
											}											
										}									
									}
									for(i=0;i<pdfFound;i++)
									{
										if(PdfFoundArray[i]==0)
										{
											PDFObjPtr=setGet(PdfObj,i);
											if (dstat=FreezeObject(PDFObjPtr,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
											fprintf(flogPtr,"\n ********* Creating Relation for i = %d PDF",i+1); 
											if (dstat = CreateRelation2(AttachClass,NULL,DocumentObjPtr,PDFObjPtr,&relationPtr,mfail)) goto EXIT;  
											printf("\n Relation created between Document and PDF...\n"); fflush(stdout);
											fprintf(flogPtr,"\n *********** Relation between Document and PDF created successfully  .**********");  																																	
										}
									}
									fprintf(flogPtr,"\n *********** Relation for all PDF's created successfully  .**********");
									printf("\n *********** Relation for all PDF's created successfully  .**********");																						
								}																	
							}
					    }
						if(partFound==1 && documentFound==0)
						{
					    	// Creating Document 
					    	// And Attaching PDF
					    	fprintf(flogPtr,"\n If condition with partFound==1 && documentFound==0 executed ...");
					    	printf("\n If condition with partFound==1 && documentFound==0 executed ...");
					    	// Drawing document Creation logic
					    	  
					    	 fprintf(flogPtr,"\n *********** Drawing creation started in TCE .**********");
					    	 if(dstat=ConstructArgsForCreate(t5DrwDocClass,&DocCreateDia,mfail)) goto EXIT;
					    	 if(dstat=objSetAttribute(DocCreateDia,DocumentNameAttr,partNo)) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,SequenceAttr,"1")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,t5SheetSizeAttr,"A4")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,t5DesignGroupAttr,"00")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,ProjectNameAttr,"0009")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,DocumentTypeAttr,"DRG")) goto EXIT; 
					    	 if(dstat=objSetAttribute(DocCreateDia,DocumentDescriptionAttr,partNo)) goto EXIT;  
					    	 if(dstat=CreateObject(t5DrwDocClass,DocCreateDia,&oNewObj,mfail)) goto EXIT; //check for dialog  
					    	 if(dstat=objCopy(&oDocObj,oNewObj)) goto EXIT; 	
					    	 if(oDocObj)
					    	 {
					    		  t5CheckMfail(CheckInItem(oDocObj,"PE Vault",mfail)); 
					    		  t5CheckMfail(ChangeStateItem(oDocObj,"LcsWorking",mfail)) ;
					    		  t5CheckMfail(t0UpdateObject(&oDocObj,mfail));
					    		  if (dstat=FreezeObject(oDocObj,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
					    		 fprintf(flogPtr,"\n*********** Drawing Docuement with %s Id created in TCE .**********",partNo);  
					    	     printf("*********** Drawing Docuement with %s Id created in TCE .**********",partNo);
					    		 //Creating relation between part and document.					 
					    	      if(dstat = CreateRelation2(PartDocClass,NULL,PartObjPtr,oDocObj,&relationPtr,mfail)) goto EXIT;  
					               printf("\n Relation created between PE Part and Document ...\n"); fflush(stdout);
					    		   fprintf(flogPtr,"\n *********** Relation between part and document created successfully  .**********"); 
					    		   if(pdfFound==0)
					    		   {
					    		   	fprintf(flogPtr,"\n PDF found count is zero ...");
					    		   }
					    		   else{
					    		   	fprintf(flogPtr,"\n PDF found count is  ...%d ",pdfFound);
					    		   	for(i=0;i<pdfFound;i++)
					    		   	{
					    		   		PDFObjPtr=setGet(PdfObj,i);
					    				if (dstat=FreezeObject(PDFObjPtr,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
					    		   		fprintf(flogPtr,"\n ********* Creating Relation for i = %d PDF",i+1); 
					    		   		if (dstat = CreateRelation2(AttachClass,NULL,oDocObj,PDFObjPtr,&relationPtr,mfail)) goto EXIT;  
					    		           printf("\n Relation created between Document and PDF...\n"); fflush(stdout);
					    		           fprintf(flogPtr,"\n *********** Relation between Document and PDF created successfully  .**********");  
					    		   	}
					    		   	fprintf(flogPtr,"\n *********** Relation for all PDF's created successfully  .**********");										   
					    	    }
					    	 }
					    	 else{
					    		 fprintf(flogPtr,"\n *********** Drawing document %s id creation Failed !!!! .**********",partNo);  
					    	     printf("*********** Drawing document creation Failed !!!! .**********",partNo); 
					    	 }							              						
					    }
						
						if(partFound==0 && documentFound==1)
						{
							//Creating part
							// Creating pdf and attaching
							fprintf(flogPtr,"\n If condition with partFound==0 && documentFound==1 executed ...");
							printf("\n If condition with partFound==0 && documentFound==1 executed ...");	
							  if(dstat=ConstructArgsForCreate("t5PeAsm",&outputObj,mfail)) goto EXIT; 
							  if(dstat=objSetAttribute(outputObj,PartNumberAttr,partNo)) goto EXIT;  
							  if(dstat=objSetAttribute(outputObj,RevisionAttr,"NR")) goto EXIT;    
							  if(dstat=objSetAttribute(outputObj,SequenceAttr,"1")) goto EXIT;
							  if(dstat=objSetAttribute(outputObj,t5PEDesignGrpAttr,"CTED")) goto EXIT;
							  if(dstat=objSetAttribute(outputObj,PPartTypeAttr,"Other")) goto EXIT;   
							  if(dstat=objSetAttribute(outputObj,NomenclatureAttr,partNo)) goto EXIT; 
							  if(dstat=objSetAttribute(outputObj,t5MRPControllerAttr,"DES")) goto EXIT; 
							  if (dstat = PerformFreeze(outputObj,mfail));				// Freezing logic Added 09_12_2023
							  						  					   
							   if(CldPrtObj)
							   {
							  	objDispose(CldPrtObj);  
							  	CldPrtObj = NULL;
							   }
							   if(dstat=CreateObject("t5PeAsm",outputObj,&CldPrtObj,mfail)) goto EXIT;
							   printf(" \nAfter Create Object(post left).....NewColourPart: \n");fflush(stdout);
							   if(dstat = oiSqlCreateSelect(&part_ptr1)) goto CLEANUP;
							   if(dstat = oiSqlWhereEQ(part_ptr1,t5SyscdAttr,"8888")) goto CLEANUP;
							   if(dstat = oiSqlWhereAND(part_ptr1)) goto CLEANUP;
							   if(dstat = oiSqlWhereEQ(part_ptr1,t5SubSyscdAttr,"Live")) goto CLEANUP;
							   if(dstat = QueryDbCount("t5Cntrol",part_ptr1,SC_SCOPE_OF_SESSION,&count,mfail)) goto CLEANUP;
							   printf("\n No of Control Object found for CE Vault live check is %d \n",count);fflush(stdout);						
							  if(CldPrtObj)
							  {					  	
							  	if(count > 0)
						      	{
							  			t5CheckMfail(CheckInItem(CldPrtObj,"PE Vault",mfail)); 
							  	}
							  	else
							  	{
							  			t5CheckMfail(CheckInItem(CldPrtObj,"PE Vault",mfail));
							  	}						  
							    t5CheckMfail(ChangeStateItem(CldPrtObj,"LcsReview",mfail)) ;							
							  	t5CheckMfail(t0UpdateObject(&CldPrtObj,mfail));
								if (dstat=FreezeObject(CldPrtObj,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
							  	printf("\n PE Part with %s id created sussfully \n",partNo);fflush(stdout);
								fprintf(flogPtr,"\n *********** Part with %s id created in TCE .**********",partNo);
								
								if (dstat = CreateRelation2(PartDocClass,NULL,CldPrtObj,DocumentObjPtr,&relationPtr,mfail)) goto EXIT;  
						        printf("\n Relation created between PE Part and Document ...\n"); fflush(stdout);
							    fprintf(flogPtr,"\n *********** Relation between part and document created successfully  .**********");
								if(pdfFound==0)
								{
									fprintf(flogPtr,"\n PDF found count is zero ...");
								}
								else{
									fprintf(flogPtr,"\n PDF found count is  ...%d ",pdfFound);
									for(i=0;i<pdfFound;i++)
									{
										PDFObjPtr=setGet(PdfObj,i);
										if (dstat=FreezeObject(PDFObjPtr,&notFrozenDepObjs,&allObjsToFreeze,&frzstat,mfail)) goto EXIT ;
										fprintf(flogPtr,"\n ********* Creating Relation for i = %d PDF",i+1); 
										if (dstat = CreateRelation2(AttachClass,NULL,DocumentObjPtr,PDFObjPtr,&relationPtr,mfail)) goto EXIT;  
								        printf("\n Relation created between Document and PDF...\n"); fflush(stdout);
								        fprintf(flogPtr,"\n *********** Relation between Document and PDF created successfully  .**********"); 
									}
									fprintf(flogPtr,"\n *********** Relation for all PDF's created successfully  .**********");
									
								  }			
							  }
						    else{
								printf("\n PE Part with %s id creation failed !!!! \n",partNo);fflush(stdout);
								fprintf(flogPtr,"\n *********** PE Part with %s id creation failed !!!!**********",partNo);				
						    }
						}			
				}								
				LineNo=LineNo+1;
				strcpy(partNoTemp, partNo);			
			}
			fprintf(flogPtr,"\n \n ************************ Successful end of main funtion ************************");
	    }
		else{
			fprintf(flogPtr,"\n \n ************************ File not found or error occured !!!!  ************************");
		}
	}
	if(flogPtr)
	{
		fclose(flogPtr);
	}
	if(fptr)
	{
		fclose(fptr);
	}	
	CLEANUP:
       printf("\n in CLEANUP...main");fflush(stdout);
    EXIT:
	   printf("\n in exit...main");fflush(stdout); 
} 
