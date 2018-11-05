#ifndef MESSAGING_H
#define MESSAGING_H

#define MSG_SUCCESS         0
#define MSG_USR_DUPLICATE   1
#define MSG_NO_USER         2
#define MSG_PSWD_MISMATCH   3
#define MSG_FAILED          4

#define MSG_SORT_BY_NUMBER_ASCENDING   5  
#define MSG_SORT_BY_NUMBER_DESCENDING  6
#define MSG_SORT_BY_SENDER_ASCENDING   7  /* lexicographic order         */
#define MSG_SORT_BY_SENDER_DESCENDING  8  /* reverse lexicographic order */

/* MESSAGING_check_account: check to see if the userID and password
      are valid.

   Inputs:
      UserId and password.

   Return values:
      MSG_SUCCESS: message sent successfully
      MSG_NO_USER: user is not in the list, either sender or recipient
      MSG_PSWD_MISMATCH: invalid password for account
*/
int MESSAGING_check_account(char *user_ID, char *passwd);

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
int MESSAGING_account_add (char *user_ID, char *passwd);


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
int MESSAGING_account_delete (char *user_ID, char *passwd);


/*
   MESSAGING_account_search: Search for a user in the list.

   Inputs:
      A string as user ID.

   Return values:
      MSG_SUCCESS: user is found in the list
      MSG_NO_USER: user is not in the list
 */
int MESSAGING_account_search (char *user_ID);

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
      MSG_ALLOC_FAILED: memory allocation failed
 */
int MESSAGING_send_message (char *sender, char *passwd, char *recipient, char *message);

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

int MESSAGING_delete_message(char *user_ID, char *passwd, int msg_no);

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

int MESSAGING_delete_head_message(char *user_ID, char *passwd);

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
int MESSAGING_sort_by_keyword (char *user_ID, char *passwd, char *keyword);


/*
  MESSAGING_sort: sort messages of a user based on "from" in alphabetical order.

   Inputs:
      A userID and pasword.  If an account exists, sort its messages according to
      sort_type:
       MSG_SORT_BY_NUMBER:  sort by the message number field
       MSG_SORT_BY_SENDER:  sort by the sender field

   Return values:
      MSG_SUCCESS: user found in the list
      MSG_NO_USER: user is not in the list
      MSG_PSWD_MISMATCH: invalid password for account
 */

int MESSAGING_sort (char *user_ID, char *passwd, int sort_type);

/* 
   MESSAGING_print_accounts: print the userIDs of all accounts.

    Inputs: 
      num, number of accounts to print.

    Return values: 
      none

   This function must call the print_account_list function.
*/

void MESSAGING_print_accounts(int num);


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

int MESSAGING_print_inbox(char *user_ID, char *passwd, int num);

#endif /* MESSAGING_H */
