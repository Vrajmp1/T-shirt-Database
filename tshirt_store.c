/*Author -	Vraj Patel U28629231
  Program -	T-Shirt Database
  Description -	This program keeps a database of t-shirt models in stock
                for different club organizations at USF, including their
		prices and quantities. The program also has basic functions
		for adding and searching the t-shirts in the inventory*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//////////////////////
// data definitions //
//////////////////////

#define ORG_NAME_LEN 50
#define SIZE_LEN 3

struct tshirt {
	char org_name[ORG_NAME_LEN+1];
	char size[SIZE_LEN+1];
	double price;
	int quantity;
	struct tshirt *next;
};

/////////////////////////
// function prototypes //
/////////////////////////

struct tshirt * add_to_inventory(struct tshirt *inventory);
void search_by_organization(struct tshirt *inventory);
void search_by_size(struct tshirt *inventory);
void print_inventory(struct tshirt *inventory);
void clear_inventory(struct tshirt *inventory);
int read_line(char str[], int n);
void help();

/**********************************************************
 * main: Prompts the user to enter an operation code,     *
 *       then calls a function to perform the requested   *
 *       action. Repeats until the user enters the        *
 *       command 'q'. Prints an error message if the user *
 *       enters an illegal code.                          *
 **********************************************************/
int main() {
	char code;

	struct tshirt *inventory = NULL;

	help();
	printf("\n");

	for (;;) {
		// read operation code
		printf("Enter operation code: ");
		scanf(" %c", &code);
		while(getchar() != '\n') /* skips to end of line */
			;

		// run operation
		switch (code) {
			case 'a':
				inventory = add_to_inventory(inventory);
				break;
			case 'h':
				help();
				break;
			case 'o':
				search_by_organization(inventory);
				break;
			case 's':
				search_by_size(inventory);
				break;
			case 'p':
				print_inventory(inventory);
				break;
			case 'q':
				clear_inventory(inventory);
				return 0;
			default:
				printf("Illegal operation code!\n");
		}
		printf("\n");
	}
}

//////////////////////////
// function definitions //
//////////////////////////


/********************************************************************
*  Void function help() takes in no inputs and displays to the user *
*  all operations that are supported as well as their codes.        *
********************************************************************/
void help() {
	printf("List of operation codes:\n");
	printf("\t'a' for adding a t-shirt to the inventory;\n");
	printf("\t'h' for help;\n");
	printf("\t'o' for searching t-shirts by organization;\n");
	printf("\t's' for searching t-shirts by size;\n");
	printf("\t'p' for printing the inventory;\n");
	printf("\t'q' for quit.\n");
}


/***************************************************************************
*  Function add_to_inventory() takes in the pointer to the inventory and   *
*  prompts the user to the enter the name of the student club and the size *
*  of the t-shirt. If the specific t-shirt does not exist, the program asks*
*  the price and quantity of the t-shirt and appends the tshirt to the list*
*  The fuction returns a pointer to the first t-shirt in the inventory.    *
***************************************************************************/
struct tshirt * add_to_inventory(struct tshirt *inventory) {
	//ask user to enter organization name and size of t-shirt
	struct tshirt *new_tshirt, *p;
	char name[ORG_NAME_LEN+1],size[SIZE_LEN+1];
	printf("Enter student organization name: ");
	read_line(name,ORG_NAME_LEN);
	printf("Enter t-shirt size: ");
	scanf("%3s",size);

	//If t-shirt exists in inventory, print message and exit function
	for(p=inventory;p!=NULL;p=p->next){
		if(strcmp(name,p->org_name)==0 && strcmp(size,p->size)==0){
			printf("T-shirt already exists in the inventory!\n");
			return inventory;
		}
	}

	//otherwise, allocate memory for the new tshirt and ask save user-inputted price and quantity
	new_tshirt = malloc(sizeof(struct tshirt));
	if (new_tshirt == NULL){
		printf("malloc failed in add_to_inventory\n");
		return inventory;
	}
	printf("Enter price: ");
	scanf("%lf",&new_tshirt->price);
	printf("Enter quantity: ");
	scanf("%d",&new_tshirt->quantity);

	//copy name and size to new_tshirt node just created
	strcpy(new_tshirt->org_name,name);
	strcpy(new_tshirt->size,size);

	//if inventory is empty, make new_tshirt the first node and return a pointer to the first node
	if(inventory==NULL){
		new_tshirt->next=NULL;
		return new_tshirt;
	}

	//if inventory is not empty, move pointer p to the last element, and add new_tshirt to the end of inventory
	for(p=inventory;p->next!=NULL;p=p->next);
	p->next = new_tshirt;
	new_tshirt->next=NULL;

	return inventory;
}


/********************************************************************************
*  Void function search_by_organization() takes in the pointer to the inventory *
*  and prompts the user to the enter the name of the student club. The program      *
*  then prints all t-shirts associated with that organization name, if any.     *
********************************************************************************/
void search_by_organization(struct tshirt *inventory) {
	//ask user to enter organiation name and set up counter to zero
	char name[ORG_NAME_LEN+1];
	struct tshirt *p;
	int count=0;
	printf("Enter student organization name: ");
	read_line(name,ORG_NAME_LEN);

	//print all t-shirts that have the entered organization name. Set count to 1 if a t-shirt by that name is encountered.
	for (p=inventory;p!=NULL;p=p->next){
		if (strcmp(p->org_name,name)==0){
			//print table heading only if count is 0; i.e print header only once
			if(count==0){
				printf("|----------------------------------------------------|-----|-------|------|\n");
				printf("| Student organization                               | Sz. | Price | Qty. |\n");
				printf("|----------------------------------------------------|-----|-------|------|\n");
			}
			printf("| %-50s | %-3s | %5.2f | %4d |\n", p->org_name, p->size, p->price, p->quantity);
			count=1;
		}
	}
	//if no t-shirts were found, print message. Otherwise, print the table's bottom border.
	if (count==0)
		printf("No t-shirt found!\n");
	else
		printf("|----------------------------------------------------|-----|-------|------|\n");
}



/****************************************************************************
*  Void function search_by_size() takes in the pointer to the inventory and *
*  prompts the user to the enter the name of the student club. The program  *
*  then prints all t-shirts associated with that particular size, if any.   *
*****************************************************************************/
void search_by_size(struct tshirt *inventory) {
	//ask user to enter size of t-shirt and set up counter to zero
	char size[SIZE_LEN+1];
	struct tshirt *p;
	int count=0;
	printf("Enter t-shirt size: ");
	read_line(size,SIZE_LEN);

	//print all t-shirts that have the entered size. Set count to 1 if a t-shirt by that size is encountered.
	for (p=inventory;p!=NULL;p=p->next){
		if (strcmp(p->size,size)==0){
			//print table heading only if count is 0; i.e print header file only once
			if(count==0){
				printf("|----------------------------------------------------|-----|-------|------|\n");
				printf("| Student organization                               | Sz. | Price | Qty. |\n");
				printf("|----------------------------------------------------|-----|-------|------|\n");
			}
			printf("| %-50s | %-3s | %5.2f | %4d |\n", p->org_name, p->size, p->price, p->quantity);
			count=1;
		}
	}
	//if no t-shirts were found, print message; otherwise, display the bottom border of the table
	if (count==0)
		printf("No t-shirts found!\n");
	else
			printf("|----------------------------------------------------|-----|-------|------|\n");

}


/*********************************************************************
*  Void function print_inventory() takes in the pointer              *
*  to the inventory and displays all t-shirts stored by their        *
*  organization name, the size, price, and quanity in a table format.*
*********************************************************************/
void print_inventory(struct tshirt *inventory) {

	struct tshirt *p;
	int count=0;
	for(p=inventory;p!=NULL;p=p->next){
		//print header for table if count is 0, meaning only the first time
		if(count==0){
			printf("|----------------------------------------------------|-----|-------|------|\n");
			printf("| Student organization                               | Sz. | Price | Qty. |\n");
			printf("|----------------------------------------------------|-----|-------|------|\n");
		}
		printf("| %-50s | %-3s | %5.2f | %4d |\n", p->org_name, p->size, p->price, p->quantity);
		count=1;
	}
	//if no t-shirts were found, print accordingly. If there were t-shirts, complete the formatted table.
	if(count==0)
		printf("No t-shirt found!\n");
	else
		printf("|----------------------------------------------------|-----|-------|------|\n");
}

/******************************************************************
*  Void function clear_inventory() takes in the pointer to the    *
*  inventory and clears all t-shirts stroed in the inventory list.*
******************************************************************/
void clear_inventory(struct tshirt *inventory) {
	struct tshirt *p;
	//until inventory pointer is NULL, clear memory pointed by p and set inventory to next node. i.e clear entire inventory.
	while(inventory!=NULL){
		p = inventory;
		inventory = inventory->next;
		free(p);
	}
}

/*********************************************************************************
*  Function read_line() takes in the char pointer to a string and an integer n   *
*  representing max characters to be stored in the string. Ignoring leading      *
*  whitespace, the function stores user-inputted characters into the given string*
*  until a newline is encountered or n characters have been stored. Lastly, the  *
*  fuction returns the number of characters stored.                              *
*********************************************************************************/
int read_line(char str[], int n) {
	int ch, i=0;

	while (isspace(ch = getchar()))
		;
	str[i++] = ch;
	while ((ch = getchar()) != '\n') {
		if (i < n)
			str[i++] = ch;
	}
	str[i] = '\0';
	return i;
}

