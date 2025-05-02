#include "Header.h"

//Assignment- To attached a dataset to the specific ItemRevision.

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tDatasettype = NULLTAG;
	tag_t tDataset = NULLTAG;
	tag_t tRel_type = NULLTAG;
	tag_t tRelation = NULLTAG;
	
	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	Error_check(ITEM_find_item("000047", &tItem));//API to find Item (We can item id from console also)
	printf("\nItem find successfull ");
	Error_check(ITEM_find_revision(tItem, "A",&tRev)); //API to specific revision of item
	Error_check(AE_find_datasettype2("PDF", &tDatasettype)); //APi to find dataset type (For Creation of dataset)
	Error_check(AE_create_dataset_with_id(tDatasettype, "My_dataset7", "", "", "", &tDataset)); //API to create dataset
	Error_check(AOM_save_with_extensions(tDataset)); //API to save dataset
	Error_check(GRM_find_relation_type("IMAN_specification", &tRel_type)); //API to find relation type
	Error_check(GRM_create_relation(tRev, tDataset, tRel_type, NULLTAG, &tRelation));//API to attach dataset to item revision with relation
	Error_check(GRM_save_relation(tRelation)); // Saving newly created relation
	return Status;
}