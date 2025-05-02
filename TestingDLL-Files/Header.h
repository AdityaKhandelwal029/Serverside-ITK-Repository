#include<stdio.h> 
#include<conio.h>
#include<tc/tc_startup.h>
#include<tccore/method.h>
#include<tccore/custom.h>
#include<tccore/aom_prop.h> 
#include<tccore\item.h>
#include<tccore/aom.h>
#include<tc/emh.h>
#include<tccore/grm.h>
#include<tccore\workspaceobject.h> 
#include<epm/epm.h>
#include<iostream>
#include<string>
#include<string.h> 
#include<unidefs.h>
#include<vector> 
#include<fstream> 
#include<ae/dataset.h> 
#include<ae/datasettype.h> 
#include<tccore/item_msg.h>
#include<sa\user.h>
#include<pom\pom\pom.h>
#include<fclasses\tc_string.h>
#include<fclasses\tc_stdio.h>
#include<fclasses\tc_stdlib.h>
#include<epm\signoff.h>
using namespace std;




#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<epm/epm.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<tc/emh.h>

#define DLLAPI __declspec(dllexport)
#define PLM_Error (EMH_USER_error_base +100)

int iFail = ITK_ok;
extern "C" DLLAPI int dll_register_callbacks();

extern DLLAPI int PLM_execute_callbacks1(int*, va_list);
extern DLLAPI int PLM_execute_callbacks2(int*, va_list);

extern DLLAPI int Workflow_Rule(EPM_rule_message_t msg);

//ofstream afile("D:/dllfilehandling/a.txt",ios::app); 
//ofstream bfile("D:/dllfilehandling/a.txt", ios::app);
//ofstream cfile("D:/dllfilehandling/b.txt", ios::app); 
//ofstream t1("D:/va_list.txt");
//#define A1 _declspec(dllexport) 
//#define DLL_error (EMH_USER_error_base +2)


//extern A1 int dll_register_callbacks();
//extern A1 int dll_execute_callbacks1(int *decision, va_list argc);
//extern A1 int dll_execute_callbacks2(int *decision, va_list argc);
//extern "C" DLLAPI int dll_register_callbacks();
//extern DLLAPI int login(int *decision, va_list argc);
//extern DLLAPI int PLM_add_postaction1(METHOD_message_t *msg, va_list args);
//extern DLLAPI int logout(int *decision, va_list argc);