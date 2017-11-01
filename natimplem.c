/* NAT table implementation in C
 * Uma Shankar - ushankar@usc.edu
 *
 */
#include "natimplem.h"
/* tables for NAT datastructure */
struct natmap* head = NULL;
struct natmap* link_head = NULL;

/* new NAT element addition */
struct natmap* newnatobj(struct natentry* entry)
{
	struct natmap* newelem = NULL;
	newelem= (struct natmap*) malloc(sizeof(struct natmap));
	memset(&(newelem->newnatentry),'\0',sizeof(newelem->newnatentry));
	newelem->left = newelem->right = NULL;
	(newelem->newnatentry).int_ip = entry->int_ip;
	(newelem->newnatentry).int_port = entry->int_port;
	(newelem->newnatentry).int_flag = entry->int_flag;
	(newelem->newnatentry).ext_ip = entry->ext_ip;
	(newelem->newnatentry).ext_port = entry->ext_port;
	return(newelem);
}

/* NAT special entry link list addition*/
struct natmap* linkinsert(struct natmap* map,struct natentry* entry)
{
	struct natmap* temp = map;
	if(map == NULL)
	{
		map = newnatobj(entry);
	}
	else
	{
		while(temp->right != NULL)
		{
			temp = temp->right;
		}
		temp->right = newnatobj(entry);
	}
}

/* Addition of NAT element into NAT map */
struct natmap* insert(struct natmap* map,struct natentry* entry)
{
	if(map == NULL)
	{
		map = newnatobj(entry);
	}
	else if((map->newnatentry).ext_port >entry->ext_port)
	{
		map->left = insert(map->left,entry);
	}
	else if((map->newnatentry).ext_port <entry->ext_port)
	{
		map->right = insert(map->right,entry);
	}
	else if(((map->newnatentry).ext_port == entry->ext_port) && ((map->newnatentry).ext_ip > entry->ext_ip))
	{
		map->left = insert(map->left,entry);
	}
	else if(((map->newnatentry).ext_port == entry->ext_port) && ((map->newnatentry).ext_ip < entry->ext_ip))
	{
		map->right = insert(map->right,entry);
	}
}
/* search nat in normal treestructure */
struct natmap* searchtree(struct natmap* head_ref,struct natentry opentry)
{
	if(head_ref == NULL)
	{
		return(NULL);
	}
	if(((head_ref->newnatentry).ext_port == opentry.ext_port) && ((head_ref->newnatentry).ext_ip == opentry.ext_ip))
	{
		return(head_ref);
	}
	else if((((head_ref->newnatentry).ext_port == opentry.ext_port) && ((head_ref->newnatentry).ext_ip < opentry.ext_ip)) ||
			((head_ref->newnatentry).ext_port < opentry.ext_port))
	{
		return(searchtree(head_ref->right,opentry));
	}
	else if((((head_ref->newnatentry).ext_port == opentry.ext_port) && ((head_ref->newnatentry).ext_ip > opentry.ext_ip)) ||
			((head_ref->newnatentry).ext_port > opentry.ext_port))
	{
		return(searchtree(head_ref->left,opentry));
	}
}
/* NAT file ip file to data structure addition */
struct natmap* addnatinfo(char* linebuf,struct natmap* head_ref,int lineno)
{
	struct natentry newentry;
	memset(&newentry,'\0',sizeof(newentry));
	int index_flag = 0;
	char tempbuf[MAXLINEBUF],tempcolbuf[MAXLINEBUF];
	memset(&tempbuf,'\0',sizeof(tempbuf));
	memset(&tempcolbuf,'\0',sizeof(tempcolbuf));
	strcpy(tempbuf,linebuf);
	//struct natmap* temp = head_ref;
	const char comma[2] =",";
	const char colon[2] =":";
	char* strtok_com = NULL;
	char* strtok_col = NULL;
	strtok_com = strtok(tempbuf,comma);
	/* Spilting the ip line into ip and port number */
	while(strtok_com != NULL)
	{
		strcpy(tempcolbuf,strtok_com);
		strtok_col = strtok(tempcolbuf,colon);
		while(strtok_col != NULL)
		{
			if(index_flag == 0)
			{
				if(strcmp(strtok_col,"*")!= 0)
				{
					(void)inet_pton(AF_INET,strtok_col,&(newentry.ext_ip));
				}
				else 
				{
					newentry.int_flag = 1;
				}
				index_flag++;
			}
			else if(index_flag == 1)
			{
				if(strcmp(strtok_col,"*")!= 0)
				{
					newentry.ext_port = atoi(strtok_col);
				}
				else
				{
					newentry.int_flag +=2;
				}
				index_flag++;
			}
			else if(index_flag == 2)
			{
				if(strcmp(strtok_col,"*")!= 0)
					(void)inet_pton(AF_INET,strtok_col,&(newentry.int_ip));
				index_flag++;
			}
			else if(index_flag == 3)
			{
				if(strcmp(strtok_col,"*")!= 0)
					newentry.int_port = atoi(strtok_col);
				index_flag = 0;
			}
			strtok_col = strtok(NULL,colon);
		}
		strtok_com = strtok(linebuf,comma);
		strtok_com = strtok(NULL,comma);
	}
	if(newentry.int_flag == 3)
	{
		printf("In line No %d both the external ip and port number are *-Not adding info\n",lineno );
		return NULL;
	}
	else
	{
		if(newentry.int_flag == 0)
		{
			insert(head_ref,&newentry); /* normal NAT entry addition*/
		}
		else
		{
			if(link_head == NULL)
			{
				link_head = linkinsert(link_head,&newentry); /* special NAT-containing * entry addition*/
			}
			else
			{
				linkinsert(link_head,&newentry);
			}
		}
	}
}
/* Process FLOW txt nat entries */
struct natmap* processnat(char *linebuf)
{
	struct natmap* temp = NULL;
	if(head == NULL)
	{
		return(NULL);
	}
	struct natentry opentry;
	memset(&opentry,'\0',sizeof(opentry));
	int index_flag = 0;
	char tempbuf[MAXLINEBUF];
	const char colon[2] =":";
	strcpy(tempbuf,linebuf);
	char* strtok_col = NULL;
	strtok_col = strtok(tempbuf,colon);
	while(strtok_col != NULL)
	{
		if(index_flag == 0)
		{
			if(strcmp(strtok_col,"*")!= 0)
			{
				(void)inet_pton(AF_INET,strtok_col,&(opentry.ext_ip));
			}
			else 
			{
				opentry.int_flag = 1;
			}
			index_flag++;
		}
		else if(index_flag == 1)
		{
			if(strcmp(strtok_col,"*")!= 0)
			{
				opentry.ext_port = atoi(strtok_col);
			}
			else
			{
				opentry.int_flag +=2;
			}
			index_flag = 0;
		}
		strtok_col = strtok(NULL,colon);
	}
	temp = searchtree(head,opentry);
	if(temp)
	{
		return(temp);
	}
	else
	{
		temp = link_head;
		while(temp != NULL)
		{
			if(((temp->newnatentry).int_flag == 1) && ((temp->newnatentry).ext_port == opentry.ext_port))
			{
				return(temp);
			}
			else if(((temp->newnatentry).int_flag == 2) && ((temp->newnatentry).ext_ip == opentry.ext_port))
			{
				return(temp);
			}
			temp = temp->right;
		}
		return(temp);
	}
}
/* Process flow txt file */
void process_flow(char* argv[])
{
	char int_ip[INET_ADDRSTRLEN],int_port[6];
	FILE* flowfp = fopen(argv[2],"r");
	FILE* opfp = fopen(argv[3],"w+");
	struct natmap* opentry = NULL;
	char flowbuf[MAXLINEBUF],opbuf[MAXLINEBUF];
	memset(&flowbuf,'\0',sizeof(flowbuf));
	memset(&opbuf,'\0',sizeof(opbuf));
	int lineno =1;
	if(flowfp == NULL)
	{
		perror("The File pointer error happened:");
		exit(1);
	}
	while(fgets(flowbuf,sizeof(flowbuf),flowfp)!= NULL)
	{
		opentry = NULL;
		if(flowbuf[strlen(flowbuf)-1] ='\n')
			flowbuf[strlen(flowbuf)-1] ='\0'; /* Removing new line character */
		opentry = processnat(flowbuf);
		if(opentry == NULL)
		{
			strcpy(opbuf,"No nat match for ");
			strcat(opbuf,flowbuf);
			strcat(opbuf,"\n");
			fputs(opbuf,opfp);
		}
		if(opentry != NULL)
		{
			/* writing into the OP file */
			strcpy(opbuf,flowbuf);
			strcat(opbuf,"->");
			inet_ntop(AF_INET,&((opentry->newnatentry).int_ip),int_ip,INET_ADDRSTRLEN);
			strcat(opbuf,int_ip);
			strcat(opbuf,":");
			sprintf(int_port,"%d",((opentry->newnatentry).int_port));
			strcat(int_port,"\n");
			strcat(opbuf,int_port);
			fputs(opbuf,opfp);
		}
	}
	fclose(flowfp);
	fclose(opfp);
}
/* File handling for NAT file */
void opennpro_nat(char* argv[])
{
	FILE* natfp = fopen(argv[1],"r");
	char linebuf[MAXLINEBUF];
	memset(&linebuf,'\0',sizeof(linebuf));
	
	int lineno =1;
	if(natfp == NULL)
	{
		perror("The File pointer error happened:");
		exit(1);
	}
	while(fgets(linebuf,sizeof(linebuf),natfp)!= NULL)
	{
		if(linebuf[strlen(linebuf)-1] ='\n')
			linebuf[strlen(linebuf)-1] ='\0'; /* Removing new line character */
		if( head == NULL)
		{
			head = addnatinfo(linebuf,head,lineno);
		}
		else
		{
			addnatinfo(linebuf,head,lineno);
		}
		memset(&linebuf,'\0',sizeof(linebuf));
		lineno++;
	}
	fclose(natfp);
}

void main(int argc,char* argv[])
{
	opennpro_nat(argv);
	process_flow(argv);
	printf("All the flow processing is done\n");
}