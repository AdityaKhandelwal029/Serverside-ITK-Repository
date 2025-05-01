#include"Header.h"
int get_all_child_lines(tag_t);
static int LEVEL = 0;
int pro6()
{
	int status = 0;
	tag_t bomWindow, revision_tag = NULLTAG;
	tag_t
	itemRevTag = NULLTAG,
	topLine = NULLTAG,
	itemTag = NULLTAG;
	printf("\n");
	status = BOM_create_window(&bomWindow);
	status = ITEM_find_rev("000023", "A", &revision_tag);
	status = BOM_set_window_top_line(bomWindow, NULLTAG, revision_tag, NULLTAG, &topLine);
	status = get_all_child_lines(topLine);
	status = ITK_exit_module(true);
	_getch();
	return status;
}
int get_all_child_lines(tag_t currentLine)
{
	int noOfChildLines = 0;
	tag_t* childLines = NULL;
	char* currentItemID = NULL;
	int status = 0;
	int    l = 0;
	int    j = 0;
	status = BOM_line_ask_child_lines(currentLine, &noOfChildLines, &childLines);
	status = AOM_ask_value_string(currentLine, bomAttr_itemId, &currentItemID);
	printf("%s\n", currentItemID);
	if (currentItemID)
		MEM_free(currentItemID);
	printf("The number of child = %d", noOfChildLines);
	printf("\n");
	for (int i = 0; i < noOfChildLines; i++)
	{
		LEVEL++;
		for (l = 0; l < 2; l++)
		{
			for (j = 0; j < LEVEL * 3; j++)
				printf(" ");
			printf("|\n");
		}
		for (j = 0; j < LEVEL * 3; j++)
			printf(" ");
		printf(" ---");
		status = get_all_child_lines(childLines[i]);
	}
	if (status == ITK_ok)
	{
		printf("ok");
	}
	if (childLines)
		MEM_free(childLines);
	LEVEL--;
	_getch();
	return status;
}