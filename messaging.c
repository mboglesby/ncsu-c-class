/*	ECE 209 - Program 5
	Michael Oglesby
	Prob Session sect. 402	*/

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "messaging.h"

#define KEY(x) (x)  /* get key for item */
#define EQUAL(x,y) ( KEY(x)==KEY(y) ) /* compare equivalence of items */
#define LESS(x,y) (KEY(x) < KEY(y))

/*
  This file describes the interface for Messaging database system implemented 
  as a singly linked-list.  

  You may not change the protoype for any function in this file, but
  you should provide a definition for all functions.  Also, you
  may define any additional functions you need within this file. But, they
  can only be used within this file (i.e. you cannot modify main).

  Your implementation will be tested with a different main function than the
  one provided to fully evaluate your entire interface.  Make sure that your
  function definitions work properly even if the main I provided you doesn't test 
  them.
 */

/* YOU MAY NOT CHANGE THIS STRUCT DEFINITION */
struct message_list {
   char                 from [10];	/* sender ID */
   int                  no;		/* message number (gets the value from cnt) */
   char                 text [80];	/* the message */

   struct message_list* next;
};

typedef struct message_list Message_list;

/* YOU MAY NOT CHANGE THIS STRUCT DEFINITION */
struct account_list {
   char                 id[10];         /* user ID */
   char                 pswd[10];       /* password of the user */
   int                  cnt;            /* counter to label the messages */
   Message_list         *inbox;		/* a list of messages for the user */

   struct account_list* next;
};

typedef struct account_list Account_list;

Account_list *Head = NULL;

/*======================================================================*/
/*======================================================================*/

/* DEFINE EXTRA FUNCTIONS IN HERE */


/* print_account: prints the information for account a 
      -a: pointer to an account_list
      -Returns no value

   DO NOT MODIFY.
*/
void print_account(Account_list *a)
{
  printf("%-10s\t\t%d\n",a->id,a->cnt);
}

/* print_account_list:
      -Inputs: the number of accounts to print
      -Returns no value

   DO NOT MODIFY.
*/
void print_account_list(int num)
{
  Account_list *tmp = Head;
  int i=0;

  if (tmp==NULL)
    return;

  printf("Account   \t\t#Msgs\n");

  while ( tmp && (i++ < num) )
    {
      print_account(tmp);
      tmp = tmp->next;
    }

}

/* print_message: prints the information for message m 
      -m: pointer to a messsage_list
      -Returns no value

   DO NOT MODIFY.
*/
void print_message(Message_list *m)
{
  printf("%-10s %-8d [%-80s]\n",m->from,m->no,m->text);
}

/* print_message_list:
      -Inputs: a pointer to an inbox, and number of messages to print
      -Returns no value

   DO NOT MODIFY.
*/
void print_message_list(Message_list *inbox, int num)
{
  Message_list *tmp = inbox;
  int i=0;

  if (inbox==NULL)
    return;

  printf("Sender     Msg.No.  Message\n");

  while ( tmp && (i++ < num) )
    {
      print_message(tmp);
      tmp = tmp->next;
    }

  printf("\n");

}


/*======================================================================*/
/*======================================================================*/

/* IMPLEMENT MESSAGING INTERFACE */

/* MESSAGING_check_account: check to see if the userID and password
      are valid.

   Inputs:
      UserId and password.

   Return values:
      MSG_SUCCESS: message sent successfully
      MSG_NO_USER: user is not in the list, either sender or recipient
      MSG_PSWD_MISMATCH: invalid password for account
*/
int MESSAGING_check_account(char *user_ID, char *passwd)
{
	Account_list *tmp;	/* temporary holding variable */
	int i;			/* temporary holding variable */

	tmp = Head;

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}

	/* search account database for user_ID and passwd and return success or failure */
	while (tmp != NULL)
	{
		if ( strcmp(tmp->id, user_ID) == 0 )
		{
			if ( strcmp(tmp->pswd, passwd) == 0 )
				return MSG_SUCCESS;
			else
				return MSG_PSWD_MISMATCH;
		}

		tmp = tmp->next;
	}

	return MSG_NO_USER;
}


/* 
   MESSAGING_account_add: Add a new user to the message list. If user
      is not already in the list, allocate memory to the user as a
      message-list structure, initialize the fields and add it to a
      global variable. You may want to initialize the global list
      structure at this point.
   
   Inputs:
      A string with the new userId and new passwd.

   Return values:
      MSG_SUCCESS: user added successfully
      MSG_USR_DUPLICATE: user already in the list
      MSG_FAILED: memory allocation failed
   Hint:
      Global list should remain sorted after this change. Meaning, user 
      should get added to the proper place to keep the list alphabetically 
      sorted.
 */
int MESSAGING_account_add (char *user_ID, char *passwd)
{
	Account_list *tmp;	/* temporary holding variable */
	Account_list *tmp1;	/* temporary holding variable */
	int i;			/* temporary holding variable */

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}

	/* create new account entry */
	tmp = (Account_list*) malloc( sizeof(Account_list) );
	if (!tmp)
		return MSG_FAILED;
	strcpy(tmp->id, user_ID);
	strcpy(tmp->pswd, passwd);
	tmp->cnt = 0;
	tmp->inbox = NULL;
	tmp->next = NULL;

	/* if there are no accounts, add new account as head */
	if (Head == NULL)
	{
		Head = tmp;
		return MSG_SUCCESS;
	}

	/* add account into appropriate spot (alphabetical order with respect to user_ID)
		if no duplicates occur and return success or failure */
	tmp1 = Head;
	while (tmp1 != NULL)
	{
		if ( strcmp(user_ID, tmp1->id) > 0 )
		{
			if ( tmp1->next == NULL )
			{
				tmp1->next = tmp;
				return MSG_SUCCESS;
			}

			if ( strcmp(user_ID, (tmp1->next)->id) < 0 )
			{
				tmp->next = tmp1->next;
				tmp1->next = tmp;
				return MSG_SUCCESS;
			}
		}
		else if (tmp1 == Head)
		{
			tmp->next = tmp1;
			Head = tmp;
			return MSG_SUCCESS;
		}

		if ( strcmp(user_ID, tmp1->id) == 0 )
			return MSG_USR_DUPLICATE;

		tmp1 = tmp1->next;
	}			
	
  	return MSG_FAILED;
}


/*
   MESSAGING_account_delete: Delete a user from the message list. If user is not in the 
      list, return error. Otherwise, remove the user account along with all 
      the fields. Also, any memory allocated to this user should be deallocated 
      (freed).
   
   Inputs:
      The userID and password for the account.

   Return values:
      MSG_SUCCESS: user deleted successfully
      MSG_NO_USER: user is not in the list
      MSG_PSWD_MISMATCH: invalid password for account
      MSG_FAILED: could not delete account
 */
int MESSAGING_account_delete (char *user_ID, char *passwd)
{
	Account_list *tmp;	/* temporary holding variable */
	Account_list *prev;	/* temporary holding variable */
	int i;			/* temporary holding variable */
	
	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}

	/* search for account to delete, delete if possible, and return success or failure */
	tmp = Head;
	prev = NULL;
	while(tmp != NULL)
	{
		if ( strcmp(user_ID, tmp->id) == 0 )
		{
			if ( strcmp(passwd, tmp->pswd) != 0 )
				return MSG_PSWD_MISMATCH;
		
			if (prev == NULL)
			{
				Head = tmp->next;
				free(tmp);
				return MSG_SUCCESS;
			}

			prev->next = tmp->next;
			free(tmp);
			return MSG_SUCCESS;
		}

		prev = tmp;
		tmp = tmp->next;		
	}
	
 	return MSG_NO_USER;
}


/*
   MESSAGING_account_search: Search for a user in the list.

   Inputs:
      A string as user ID.

   Return values:
      MSG_SUCCESS: user is found in the list
      MSG_NO_USER: user is not in the list
 */
int MESSAGING_account_search (char *user_ID)
{
	Account_list *tmp;	/* temporary holding variable */
	int i;			/* temporary holding variable */

	tmp = Head;

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* search for account and return success or failure */
	while(tmp != NULL)
	{
		if ( strcmp(user_ID, tmp->id) == 0 )
			return MSG_SUCCESS;

		tmp = tmp->next;	
	}

 	return MSG_NO_USER;
}

/*
   MESSAGING_send_message: Send a message from user to another user. 
      This function should find the user, allocate memory for the message 
      structure, add the message to the inbox of that user, assign the message the next
      message number for that user, and update message counter.

   Inputs:
      Sender ID and pasword, the message itself, and the recipient ID.  Validate the user
      and password, and if the account is valid, send the message to recipient.

   Return values:
      MSG_SUCCESS: message sent successfully
      MSG_NO_USER: user is not in the list, either sender or recipient
      MSG_PSWD_MISMATCH: invalid password for account
      MSG_FAILED: memory allocation failed
 */
int MESSAGING_send_message (char *sender, char *passwd, char *recipient, char *message)
{
	Account_list *tmp;	/* temporary holding variable */
	Message_list *tmp1;	/* temporary holding variable */
	Message_list *tmp2;	/* temporary holding variable */
	int i;			/* temporary holding variable */

	tmp = Head;

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(sender) ; i++)
	{
		if ( sender[i] >= 65 && sender[i] <= 90 )
			sender[i]+=32;

		if (i == 9)
			sender[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(recipient) ; i++)
	{
		if ( recipient[i] >= 65 && recipient[i] <= 90 )
			recipient[i]+=32;

		if (i == 9)
			recipient[i] = '\0';
	}

	/* convert message to length 80 */
	for (i = 0; i < strlen(message) ; i++)
	{
		if (i == 79)
			message[i] = '\0';
	}
	
	/* check for sender password mismatch */
	while(tmp != NULL)
	{
		if ( strcmp(sender, tmp->id) == 0 )
		{
			if ( strcmp(passwd, tmp->pswd) != 0 )
				return MSG_PSWD_MISMATCH;
		}

		tmp = tmp->next;	
	}

	/* search for recipient and place message into recipients message list and
		return success or failure */
	tmp = Head;
	while(tmp != NULL)
	{
		if ( strcmp(recipient, tmp->id) == 0 )
		{
			(tmp->cnt)++;
			tmp1 = tmp->inbox;

			if (tmp1 == NULL)
			{
				tmp1 = (Message_list*) malloc( sizeof(Message_list) );

				strcpy(tmp1->from, sender);
				tmp1->no = (tmp->cnt) - 1;
				strcpy(tmp1->text, message);
				tmp1->next = NULL;

				tmp->inbox = tmp1;
				return MSG_SUCCESS;
			}			

			while (tmp1->next != NULL)
				tmp1 = tmp1->next;

			if ( !( tmp2 = (Message_list*) malloc( sizeof(Message_list) ) ) )
				return MSG_FAILED;

			tmp1->next = tmp2;

			strcpy(tmp2->from, sender);
			tmp2->no = (tmp->cnt) - 1;
			strcpy(tmp2->text, message);
			tmp2->next = NULL;

			return MSG_SUCCESS;
		}

		tmp = tmp->next;	
	}
	
 	return MSG_NO_USER;
}

/* 
   MESSAGING_delete_message:  delete a message with corresponding number
      from the user's inbox.

   Inputs: userId and password for account, and the number for the message.

   Return values:
      MSG_SUCCESS: message deleted successfully
      MSG_NO_USER: user is not in the list, either sender or recipient
      MSG_PSWD_MISMATCH: invalid password for account
      MSG_FAILED: no such message number
*/

int MESSAGING_delete_message(char *user_ID, char *passwd, int msg_no)
{
	Account_list *tmp;	/* temporary holding variable */
	Message_list *tmp1;	/* temporary holding variable */
	Message_list *prev;	/* temporary holding variable */
	int i;			/* temporary holding variable */

	tmp = Head;
	prev = NULL;

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}
	
	/* search for user_ID, check password, then search for message to delete and return
		success or failure */
	while(tmp != NULL)
	{
		if ( strcmp(user_ID, tmp->id) == 0 )
		{
			if ( strcmp(passwd, tmp->pswd) != 0 )
				return MSG_PSWD_MISMATCH;

			tmp1 = tmp->inbox;

			while (tmp1 != NULL)
			{
				if (tmp1->no == msg_no)
				{
					if (prev == NULL)
					{
						tmp->inbox = tmp1->next;
						free(tmp1);
						return MSG_SUCCESS;
					}
		
					prev->next = tmp1->next;
					free(tmp1);
					return MSG_SUCCESS;
				}
				
				prev = tmp1;
				tmp1 = tmp1->next;	
			}
				
			return MSG_FAILED;
		}

		tmp = tmp->next;	
	}
	
  	return MSG_NO_USER;
}

/* 
   MESSAGING_delete_head_message:  deletes message at the head of a user's
      inbox.

   Inputs: userId and password for account.

   Return values:
      MSG_SUCCESS: message deleted successfully
      MSG_NO_USER: user is not in the list, either sender or recipient
      MSG_PSWD_MISMATCH: invalid password for account
      MSG_FAILED: empty inbox
*/

int MESSAGING_delete_head_message(char *user_ID, char *passwd)
{
	Account_list *tmp;	/* temporary holding variable */
	Message_list *tmp0;	/* temporary holding variable */
	int i;			/* temporary holding variable */

	tmp = Head;

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}
	
	/* search for user_ID, check password, then delete head message and return
		success or failure */
	while(tmp != NULL)
	{
		if ( strcmp(user_ID, tmp->id) == 0 )
		{
			if ( strcmp(passwd, tmp->pswd) != 0 )
				return MSG_PSWD_MISMATCH;

			if (tmp->inbox == NULL)
				return MSG_FAILED;

			tmp0 = tmp->inbox;
			tmp->inbox = (tmp->inbox)->next;
			free(tmp0);

			return MSG_SUCCESS;
		}

		tmp = tmp->next;	
	}	

	return MSG_NO_USER;
}

/* 
   MESSAGING_sort_by_keyword: Search the message list of the user and
      sort those messages using a keyword.

   Inputs:
      The userID and password for an account.  If such an account exists and the
      password matches, sort the inbox using the keyword.  Messages that include
      the keyword must come first in the list.  Of those that match, messages
      with a lower message number should come first.

   Return values:
      MSG_SUCCESS:  messages sorted successfully
      MSG_NO_USER: user is not in the list
      MSG_PSWD_MISMATCH: invalid password for account
 */
int MESSAGING_sort_by_keyword (char *user_ID, char *passwd, char *keyword)
{
	Account_list *tmp0;		/* temporary holding variable */
	Message_list *tmp1;		/* temporary holding variable */
	Message_list *keyword_head;	/* temporary holding variable */
	Message_list *keyword_tmp;	/* temporary holding variable */
	Message_list *no_keyword_head;	/* temporary holding variable */
	Message_list *no_keyword_tmp;	/* temporary holding variable */
	int i;				/* temporary holding variable */

	Message_list *sorted;		/* temporary holding variable (sorting algorithm) */
	Message_list *n;		/* temporary holding variable (sorting algorithm) */
	Message_list *tmp,*prev;	/* temporary holding variable (sorting algorithm) */

	tmp0 = Head;
	keyword_head = NULL;
	no_keyword_head = NULL;

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}
	
	/* find user_ID, check password, and then sort message list and return
		success or failure */
	while(tmp0 != NULL)
	{
		if ( strcmp(user_ID, tmp0->id) == 0 )
		{
			if ( strcmp(passwd, tmp0->pswd) != 0 )
				return MSG_PSWD_MISMATCH;

			if ( (tmp1 = tmp0->inbox) == NULL )
				return MSG_SUCCESS;

			/* create a list of messages that containe the keyword
				and a list of messages that don't */
			while (tmp1 != NULL)
			{
				if ( strstr(tmp1->text, keyword) )
				{
					if (keyword_head == NULL)
					{
						keyword_head = tmp1;
						keyword_tmp = tmp1;
					}
					else
					{
						keyword_tmp->next = tmp1;
						keyword_tmp = keyword_tmp->next;
					}
				}
				else
				{
					if (no_keyword_head == NULL)
					{
						no_keyword_head = tmp1;
						no_keyword_tmp = tmp1;
					}
					else
					{
						no_keyword_tmp->next = tmp1;
						no_keyword_tmp = no_keyword_tmp->next;
					}
				}

				tmp1 = tmp1->next;					
			}

			keyword_tmp->next = NULL;
			no_keyword_tmp->next = NULL;

			/* sort the list of messages containing the keyword.
			sorting algorithm taken from class lecture   */	
			sorted = NULL;	
			n = keyword_head;
			while (keyword_head && n)   
			{
				keyword_head = keyword_head->next;
		      		tmp = sorted;
		      		prev = NULL;

		      		/* iterate until we find the right place to insert */
		      		while(tmp && LESS(tmp->no,n->no))
				{
			  		prev = tmp;
			  		tmp = tmp->next;
				}
		      
		      		if (sorted==NULL)
				{
			  		/* sorted is empty */
			  		sorted = n;
			  		n->next=NULL;
				}
		      		else if (prev==NULL)
				{
			  		/* insert at the beginning */
			  		n->next = sorted;
			  		sorted = n;
				}
		      		else
				{
			  		/* in the middle of the list, or at the end */
			  		n->next = tmp;
			  		prev->next = n;
				}
				
				n = keyword_head;
		    	}
		  	keyword_head = sorted;

			/* arrange inbox by combining lists (containing keyword, and
				not containint keyword) */
			tmp0->inbox = keyword_head;
			while (keyword_head->next != NULL)
				keyword_head = keyword_head->next;
			keyword_head->next = no_keyword_head;

			return MSG_SUCCESS;			
		}

		tmp0 = tmp0->next;	
	}
	
	return MSG_NO_USER;
}


/*
  MESSAGING_sort: sort messages of a user based on "from" in alphabetical order.

   Inputs:
      A userID and pasword.  If an account exists, sort its messages according to
      sort_type:
		MSG_SORT_BY_NUMBER_ASCENDING: sort with the message number field increasing
		MSG_SORT_BY_NUMBER_DESCENDING: sort with the message number field decreasing
		MSG_SORT_BY_SENDER_ASCENDING: sort by the sender field in alphabetical order
		MSG_SORT_BY_SENDER_DESCENDING: sort by the sender field in reverse alphabeticl

   Return values:
      MSG_SUCCESS: user found in the list
      MSG_NO_USER: user is not in the list
      MSG_PSWD_MISMATCH: invalid password for account
 */

int MESSAGING_sort (char *user_ID, char *passwd, int sort_type)
{
	Account_list *tmp0;	/* temporary holding variable */
	Message_list *tmp1;	/* temporary holding variable */
	int i;			/* temporary holding variable */

	Message_list *sorted;		/* temporary holding variable (sorting algorithm) */
	Message_list *n;		/* temporary holding variable (sorting algorithm) */
	Message_list *tmp,*prev;	/* temporary holding variable (sorting algorithm) */

	tmp0 = Head;

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}
	
	/* find user_ID, check password, then sort message list and return 
		success or failure */
	while(tmp0 != NULL)
	{
		if ( strcmp(user_ID, tmp0->id) == 0 )
		{
			if ( strcmp(passwd, tmp0->pswd) != 0 )
				return MSG_PSWD_MISMATCH;

			if ( (tmp1 = tmp0->inbox) == NULL )
				return MSG_SUCCESS;

			if (sort_type == MSG_SORT_BY_NUMBER_ASCENDING)
			{
				/* sort the list of messages by number.
				sorting algorithm taken from class lecture   */	
				sorted = NULL;		
				n = tmp1;
				while (n)   
				{
					tmp1 = tmp1->next;
			      		tmp = sorted;
			      		prev = NULL;
	
			      		/* iterate until we find the right place to insert */
			      		while(tmp && LESS(tmp->no,n->no))
					{
				  		prev = tmp;
				  		tmp = tmp->next;
					}
			      
			      		if (sorted==NULL)
					{
				  		/* sorted is empty */
				  		sorted = n;
				  		n->next=NULL;
					}
			      		else if (prev==NULL)
					{
				  		/* insert at the beginning */
				  		n->next = sorted;
				  		sorted = n;
					}
			      		else
					{
				  		/* in the middle of the list, or at the end */
				  		n->next = tmp;
				  		prev->next = n;
					}
					
					n = tmp1;
			    	}
			  	tmp0->inbox = sorted;

				return MSG_SUCCESS;
			}
			else if (sort_type == MSG_SORT_BY_NUMBER_DESCENDING)
			{
				/* sort the list of messages by number.
				sorting algorithm taken from class lecture   */	
				sorted = NULL;		
				n = tmp1;
				while (n)   
				{
					tmp1 = tmp1->next;
			      		tmp = sorted;
			      		prev = NULL;
	
			      		/* iterate until we find the right place to insert */
			      		while(tmp && LESS(n->no,tmp->no))
					{
				  		prev = tmp;
				  		tmp = tmp->next;
					}
			      
			      		if (sorted==NULL)
					{
				  		/* sorted is empty */
				  		sorted = n;
				  		n->next=NULL;
					}
			      		else if (prev==NULL)
					{
				  		/* insert at the beginning */
				  		n->next = sorted;
				  		sorted = n;
					}
			      		else
					{
				  		/* in the middle of the list, or at the end */
				  		n->next = tmp;
				  		prev->next = n;
					}
					
					n = tmp1;
			    	}
			  	tmp0->inbox = sorted;

				return MSG_SUCCESS;
			}
			else if (sort_type == MSG_SORT_BY_SENDER_ASCENDING)
			{
				/* sort the list of messages by sender.
				sorting algorithm taken from class lecture   */	
				sorted = NULL;		
				n = tmp1;
				while (n)   
				{
					tmp1 = tmp1->next;
			      		tmp = sorted;
			      		prev = NULL;
	
			      		/* iterate until we find the right place to insert */
			      		while( tmp && ( strcmp(tmp->from,n->from) < 0 ) )
					{
				  		prev = tmp;
				  		tmp = tmp->next;
					}
			      
			      		if (sorted==NULL)
					{
				  		/* sorted is empty */
				  		sorted = n;
				  		n->next=NULL;
					}
			      		else if (prev==NULL)
					{
				  		/* insert at the beginning */
				  		n->next = sorted;
				  		sorted = n;
					}
			      		else
					{
				  		/* in the middle of the list, or at the end */
				  		n->next = tmp;
				  		prev->next = n;
					}
					
					n = tmp1;
			    	}
			  	tmp0->inbox = sorted;

				return MSG_SUCCESS;				
			}
			else if (sort_type == MSG_SORT_BY_SENDER_DESCENDING)
			{
				/* sort the list of messages by sender.
				sorting algorithm taken from class lecture   */	
				sorted = NULL;		
				n = tmp1;
				while (n)   
				{
					tmp1 = tmp1->next;
			      		tmp = sorted;
			      		prev = NULL;
	
			      		/* iterate until we find the right place to insert */
			      		while( tmp && ( strcmp(tmp->from,n->from) > 0 ) )
					{
				  		prev = tmp;
				  		tmp = tmp->next;
					}
			      
			      		if (sorted==NULL)
					{
				  		/* sorted is empty */
				  		sorted = n;
				  		n->next=NULL;
					}
			      		else if (prev==NULL)
					{
				  		/* insert at the beginning */
				  		n->next = sorted;
				  		sorted = n;
					}
			      		else
					{
				  		/* in the middle of the list, or at the end */
				  		n->next = tmp;
				  		prev->next = n;
					}
					
					n = tmp1;
			    	}
			  	tmp0->inbox = sorted;

				return MSG_SUCCESS;				
			}						
		}

		tmp0 = tmp0->next;	
	}

	return MSG_NO_USER;
}

/* 
   MESSAGING_print_accounts: print the userIDs of all accounts.

    Inputs: 
      num, number of accounts to print.

    Return values: 
      none

   This function must call the print_account_list function.
*/

void MESSAGING_print_accounts(int num)
{
	print_account_list(num);
}


/* 
   MESSAGING_print_inbox: print the first num messages of the account's
     inbox.
     
   Inputs: userID and password to vaidate the account. 
           num, number of accounts to print.

   Return values: 
      MSG_SUCCESS: printed inbox
      MSG_NO_USER: user is not in the list
      MSG_PSWD_MISMATCH: invalid password for account

   This function must call the print_message_list function.
*/

int MESSAGING_print_inbox(char *user_ID, char *passwd, int num)
{
	Account_list *tmp = Head;	/* temporary holding variable */
	int i;				/* temporary holding variable */

	/* convert user_ID to lower case & length 9 */
	for (i = 0; i < strlen(user_ID) ; i++)
	{
		if ( user_ID[i] >= 65 && user_ID[i] <= 90 )
			user_ID[i]+=32;

		if (i == 9)
			user_ID[i] = '\0';
	}

	/* convert passwd to length 9 */
	for (i = 0; i < strlen(passwd) ; i++)
	{
		if (i == 9)
			passwd[i] = '\0';
	}
	
	/* search for user_ID, check password, then print message list and return
		success or failure */
	while(tmp != NULL)
	{
		if ( strcmp(user_ID, tmp->id) == 0 )
		{
			if ( strcmp(passwd, tmp->pswd) != 0 )
				return MSG_PSWD_MISMATCH;

			if (tmp->inbox == NULL)
				return MSG_SUCCESS;

			print_message_list(tmp->inbox, num);
			return MSG_SUCCESS;
		}

		tmp = tmp->next;	
	}	

	return MSG_NO_USER;
}
