//gsoap ns service name:				 MailWorkFlowDecision
//gsoap ns service style:				 rpc
//gsoap ns service namespace:			 urn:MailWorkFlowDecision
//gsoap ns service location:			 http://172.22.97.106:9080/cgi/MailWorkFlowDecision.cgi
//gsoap ns service encoding:			 encoded
//gsoap ns schema namespace:			 urn:MailWorkFlowDecision
//gsoap ns service method-documentation: Module name web service


int ns__MailWorkFlowDecision(char *uniqueId, char* ObjectName,char* businessUnit,char* workFlowName,char *targetUserId,char *taskName,char* action,char *approveComment,char **Response);