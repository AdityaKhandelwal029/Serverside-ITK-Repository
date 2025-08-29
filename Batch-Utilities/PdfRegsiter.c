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
	int LineNo=1;
    int stat = OKAY;  
	int frzstat ;
	int i=0;
	//integer *mfail;
	
    char filedata[500];	
	char *partNo = NULL; 
	char *revisionId = NULL; 
	char *revisionSequence = NULL; 
	SqlPtr getWorkLocSP		=	NULL;
	SetOfObjects	outWorkLocsSO			=	NULL;
	SetOfObjects	FileSO					=	NULL;
	SetOfObjects	extraObj				=	NULL; 
    SetOfStrings	extraStr				=	NULL;
	
	string worklocDup="abb923373_CAD31517";
	ObjectPtr		TranDialogueObjP	=	NULL;
	ObjectPtr		WorklocObjP			=	NULL;
	ObjectPtr		PDFFileP			=	NULL;
	ObjectPtr		PdfFileRegOP		=	NULL;
	string			DIOwnerNameS		=	NULL;
	string			UnRegFileNameDupS		=	NULL;
	string			UnRegFileNameS		=	NULL;
	boolean			ans					=	FALSE;
	
	
   t5MethodInitWMD("userinfo");
   FILE *flogPtr=fopen("PdfRegistryLog.txt","a");
   //FILE *fptr=fopen("input.txt","r");
   
   fprintf(flogPtr,"\n ***********************************************************************************************************************************************************************");
   fprintf(flogPtr,"\n ***********************************************************************************************************************************************************************");
   fprintf(flogPtr,"\n ***********************************************************************************************************************************************************************");
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
		t5CheckDstat(oiSqlCreateSelect(&getWorkLocSP));
		if(dstat=oiSqlWhereEQ(getWorkLocSP,"OwnerDirName",worklocDup))goto EXIT;
		t5CheckMfail(QueryWhere3(WorkSDirClass,getWorkLocSP,0,SC_SCOPE_OF_SESSION,&outWorkLocsSO,mfail));
		oiSqlPrint(getWorkLocSP);
		printf("\n No of worklocation %s present are %d \n",worklocDup,setSize(outWorkLocsSO));fflush(stdout);
		if(setSize(outWorkLocsSO)==0){
				printf("Work location %s is not present",worklocDup);
				fprintf(flogPtr,"\n WorkSpace not found...... \n");
				goto CLEANUP;
		}
		else
		{	
			    fprintf(flogPtr,"\n WorkSpace found...... \n");	
				WorklocObjP=setGet(outWorkLocsSO,0);
				t5CheckMfail(ExpandObject5(OdrContClass,WorklocObjP,"ContentsOfOwnerDirectory",SC_SCOPE_OF_SESSION, &FileSO,mfail));
		        printf("\n Size of the object [%d]",setSize(FileSO)); fflush(stdout);
                fprintf(flogPtr,"\Total content in worklocation found = %d \n",setSize(FileSO));
                for(i=0;i<setSize(FileSO);i++)
				{
					PDFFileP=setGet(FileSO,i);
					t5CheckDstat(objGetAttribute(PDFFileP,RelativePathAttr,&UnRegFileNameDupS));
					UnRegFileNameS=nlsStrDup(UnRegFileNameDupS);
					printf("\n UnRegFileNameS is  <%s> ",UnRegFileNameS); fflush(stdout);
					t5CheckMfail(IsRegistered(PDFFileP,&ans,mfail));
					if((ans==FALSE))
					{                         									
						t5CheckMfail(RegisterItem(PdfFileClass,PDFFileP,&PdfFileRegOP,mfail));
						fprintf(flogPtr,"\n ********* Line  = %d File name = %s Successfully Registered ",i,UnRegFileNameS);
                        printf("\n ********* Line  = %d File name = %s Successfully Registered ",i,UnRegFileNameS);						
					}
					else
					{
						t5CheckDstat(objCopy(&PdfFileRegOP,PDFFileP));
						t5CheckMfail(t0UpdateObject(&PdfFileRegOP,mfail));
						fprintf(flogPtr,"\n ********* Line  = %d File name = %s Is allready registered ",i,UnRegFileNameS);
						printf("\n ********* Line  = %d File name = %s Is allready registered ",i,UnRegFileNameS);
					}
					if(PdfFileRegOP)
					{
						t5CheckDstat(objGetAttribute(PdfFileRegOP,OwnerNameAttr,&DIOwnerNameS));
						fprintf(flogPtr,"\n ********* Owners name = %s ",DIOwnerNameS);
						t5CheckDstat(SetUpDialog(objClass(PdfFileRegOP),PdfFileRegOP,"TransferDialogC",&extraStr,&extraObj,&TranDialogueObjP,mfail));
						t5CheckDstat(objSetAttribute(TranDialogueObjP,DestOwnerNameAttr,"PE Vault"));
					 	t5CheckDstat(objSetAttribute(TranDialogueObjP,DestOwnerDirNameAttr,"PE_Vault_Loc"));					
						t5CheckMfail(TransferObject(PdfFileRegOP,TranDialogueObjP,mfail));
					    fprintf(flogPtr,"\n ********* PDF with file name %s is successfully checked in ",UnRegFileNameS);
					}
                    else{
                     	fprintf(flogPtr,"\n ********* Checked in operation failed on PDF with file name %s",UnRegFileNameS);					
                }					
		    }
		}
		
	}
	if(flogPtr)
	{
		fclose(flogPtr);
	}
	
	CLEANUP:
       printf("\n in CLEANUP...main");fflush(stdout);
    EXIT:
	   printf("\n in exit...main");fflush(stdout); 
}
