#include "Header.h"
extern DLLAPI int pre_action_register_callbacks()
{
	Status = CUSTOM_register_exit("pre_action", "USER_init_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks1);
	Status = CUSTOM_register_exit("pre_action", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Custom_execute_callbacks2);
	return Status;
}
extern DLLAPI int Custom_execute_callbacks1(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	METHOD_id_t Method_id;
	printf("\n\n***************************************");
	printf("\n  DLL Register And Login Successfull ");
	printf("\n***************************************\n\n");
	Status = METHOD_find_method("ItemRevision", "IMAN_delete", &Method_id);
	Status = METHOD_add_action(Method_id, METHOD_pre_action_type, (METHOD_function_t)PLM_add_PreAction, NULL);
	return Status;
}
extern DLLAPI int PLM_add_PreAction(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev = NULLTAG;
	source_rev = va_arg(args, tag_t);
	tag_t source_Item = NULLTAG;
	int icount, i, dcount,j,k;
	tag_t* rev_list = NULLTAG;
	tag_t relationType = NULLTAG;
	tag_t relation = NULLTAG;
	tag_t* secondaryObj = NULLTAG;
	tag_t newRelation = NULLTAG;
	char* obj_type = NULL;
	Status = ITEM_ask_item_of_rev(source_rev, &source_Item);
	ITEM_list_all_revs(source_Item, &icount, &rev_list);
		for ( i = 0; i < icount; i++)
		{
			if (source_rev == rev_list[i])
			{
				GRM_find_relation_type(NULL, &relationType);
				GRM_list_secondary_objects_only(rev_list[i], relationType, &dcount, &secondaryObj);
				for (j = 0; j < dcount; j++)
				{
					WSOM_ask_object_type2(secondaryObj[j], &obj_type);
					 k = tc_strcmp(obj_type, "ItemRevision Master");
					 if (k != 0)
					 {
						 GRM_find_relation_type("IMAN_specification", &relation);
						 GRM_create_relation(rev_list[i - 1], secondaryObj[j], relation, NULLTAG, &newRelation);
						 GRM_save_relation(newRelation);
					 }
				}
			}
		}	
	return Status;
}
extern DLLAPI int Custom_execute_callbacks2(int *decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n***************************************");
	printf("\n  DLL Register And Logout Successfull ");
	printf("\n***************************************\n\n");
	return Status;
}