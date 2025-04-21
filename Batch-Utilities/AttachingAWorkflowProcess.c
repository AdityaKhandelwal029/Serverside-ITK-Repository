static void new_process(void)
 
{
 
   
 
  int n_items = 0;
 
  tag_t *items = NULL;
 
  find_items(<some input to fetch item revisions tag>, &n_items, &items);
 
  ECHO("\n n_items: %d \n", n_items);
 
 
 
  if (n_items > 0)
 
  {
 
    // Must have an attachment type for each object
 
    int *attach_types = NULL;
 
    attach_types = (int *) MEM_alloc (n_items * sizeof(int));
 
    for (int ii = 0; ii < n_items; ii++)
 
    {
 
      attach_types[ii] = EPM_target_attachment;
 
    }
 
 
 
    tag_t process_template = NULLTAG;
 
    IFERR_REPORT(EPM_find_process_template("<Your WF Template name>", 
 
      &process_template));
 
 
 
    tag_t process = NULLTAG;
 
    IFERR_REPORT(EPM_create_process("Any WF Process Name", "", process_template,n_items, 
 
      items, attach_types, &process));   
 
 
 
    MEM_free(attach_types);
 
  }
 
  else ECHO("\n No items found! \n");
 
  MEM_free(items);
 
}