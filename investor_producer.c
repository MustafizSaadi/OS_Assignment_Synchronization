#include <types.h>
#include <lib.h>
#include <synch.h>
#include <test.h>
#include <thread.h>

#include "investor_producer.h"
#include "invest_assignment.h"

/*
 * **********************************************************************
 * YOU ARE FREE TO CHANGE THIS FILE BELOW THIS POINT AS YOU SEE FIT
 *
 */

extern struct bankdata bank_account[NBANK];
extern long int customer_spending_amount[NCUSTOMER];
extern long int producer_income[NPRODUCER];
static struct semaphore *mutex,*TO,*CLA;

/*
 * **********************************************************************
 * FUNCTIONS EXECUTED BY CUSTOMER THREADS
 * **********************************************************************
 */


/*
 * order_item()
 *
 * Takes one argument specifying the item produces. The function
 * makes the item order available to producer threads and then blocks until the producers
 * have produced the item with that appropriate for the customers.
 *
 * The item itself contains the number of ordered items.
 */ 

void order_item(void *itm){
	(void)itm; // to avoid warning
//kprintf("I am in order item\n");
  //panic("You need to write some code!!!!");
	int i; 
	P(mutex);

	struct item *head = (struct item*) kmalloc(sizeof(struct item));
	struct item *temp1 = req_serv_item;
	struct item *temp2;
	if(temp1==NULL)
		req_serv_item = head;
	else{
		while(temp1 != NULL){
			temp1 = temp1->next;
		}
		temp1 = head;
	}
	temp2 = itm;
	//req_serv_item = head;
	//kprintf("%d\n", sizeof(req_serv_item));
	//req_serv_item = (*item) malloc(sizeof(item));	
	//struct item *temp = itm;
	//kprintf("%u %u\n",temp->item_quantity,10);
	//unsigned int x = *(unsigned int *)itm;
	for (i = 0;i < N_ITEM_TYPE; i++){
		//memcpy(head,itm,sizeof(struct item));
		head->item_quantity =(temp2->item_quantity);
		//kprintf("%u\n",head->item_quantity);
		//itm += sizeof(unsigned int);
		head->i_price = ITEM_PRICE;
		//kprintf("%u\n",head->i_price);
		//itm += sizeof(unsigned int);
		head->servBy =(temp2->servBy);
		//kprintf("%lu\n",head->servBy);
		//itm += sizeof(unsigned long int); 
		head->requestedBy =(temp2->requestedBy) ;
		//kprintf("%ld\n",head->requestedBy);
		//itm += sizeof(long int);
		head->order_type = REQUEST;
		//kprintf("%u\n",head->order_type);
		//itm += sizeof(unsigned int);*/
		if(i!=N_ITEM_TYPE-1)
			head->next = (struct item*) kmalloc(sizeof(struct item));
		else{
			//panic("You need to write some code!!!!");
			head->next = NULL;
		}
		head = head->next;
		//kprintf("%u\n",(head->next)->item_quantity);
		temp2++;
	}
	kprintf("%u req\n",req_serv_item->item_quantity);
	V(TO);
	V(mutex);
}

/**
 * consume_item() 
 * Customer consume items which were served by the producers.
 * affected variables in the order queue, on item quantity, order type, requested by, served by
 * customer should keep records for his/her spending in shopping
 * and update spending account
 **/
void consume_item(unsigned long customernum){
    (void) customernum; // avoid warning 
    //panic("You need to write some code!!!!");
	
	while( req_serv_item-> order_type == SERVICED){
		customer_spending_amount[customernum] += (req_serv_item->item_quantity) * (req_serv_item->i_price);
		req_serv_item = req_serv_item->next;
	}
	struct item *cur_head = req_serv_item;
	if( req_serv_item == NULL) return;
	while(1){
		if(cur_head->next == NULL) break;
		if((cur_head->next)->order_type == SERVICED){
			customer_spending_amount[customernum] += ((cur_head->next)->item_quantity) * ((cur_head->next)->i_price);
			cur_head->next = (cur_head->next)->next;
		}
		else
			cur_head = cur_head->next;
		
	}
}

/*
 * end_shoping()
 *
 * This function is called by customers when they go home. It could be
 * used to keep track of the number of remaining customers to allow
 * producer threads to exit when no customers remain.
 */
void end_shoping(){
    //panic("You need to write some code!!!!");
	V(TO);
	kfree(req_serv_item);
}


/*
 * **********************************************************************
 * FUNCTIONS EXECUTED BY ITEM PRODUCER THREADS
 * **********************************************************************
 */

/*
 * take_order()
 *
 * This function waits for a new order to be submitted by
 * customers. When submitted, it records the details and returns a
 * pointer to something representing the order.
 *
 * The return pointer type is void * to allow freedom of representation
 * of orders.
 *
 * The function can return NULL to signal the producer thread it can now
 * exit as there are no customers nor orders left.
 */
 
void *take_order(){
	//kprintf("I am in take order\n");
  //panic("You need to write some code!!!!");
  P(TO);
  return req_serv_item; //modify
}


/*
 * produce_item()
 *
 * This function produce an item if the investment is available for the product
 *
 */

void produce_item(void *v){
	(void)v;
    //panic("You need to write some code!!!!");
	struct item *head = v;
	unsigned int i;
	for(i = 0; i < sizeof(v)/sizeof(struct item*) ; i++){
		head->i_price += ((head->i_price)/100)*PRODUCT_PROFIT + ((head->i_price)/100)*BANK_INTEREST ;
		head = head->next;
		}
}


/*
 * serve_order()
 *
 * Takes a produced item and makes it available to the waiting customer.
 */

void serve_order(void *v,unsigned long producernumber){
	(void)v;
	(void)producernumber;
    //panic("You need to write some code!!!!");
	struct item *head = v;
	unsigned int i;
	for(i = 0; i < sizeof(v)/sizeof(struct item*) ; i++){
		head->servBy = producernumber ;
		head->order_type = SERVICED;
		head = head->next;
		}
}

/**
 * calculate_loan_amount()
 * Calculate loan amount
 */
long int calculate_loan_amount(void* itm){
	(void)itm;
	P(CLA);
    //panic("You need to write some code!!!!");
	struct item *temp = itm;
	int n=0;
	kprintf("order type %u\n",temp->order_type);
	if(temp->next != NULL)
		kprintf("order_type2 %u\n",(temp->next)->order_type);
	//kprintf("order_type2 %u\n",(temp->next)->order_type);
	/*while(temp->next != NULL){
		if(temp->order_type == REQUEST)
			n++;
		temp = temp->next;
		//if(temp == NULL)
		//	break;
	}*/
    //kprintf(" queue size %d\n",n);
	V(CLA);
    return ITEM_PRICE*n;
}

/**
 * void loan_request()
 * Request for loan from bank
 */
void loan_request(void *amount,unsigned long producernumber){
	(void)amount;
	(void)producernumber;
    //panic("You need to write some code!!!!");
	long int money = *(long int *) amount;
	int bank = random()%3 + 1;
	//kprintf(" Bank %d loan %ld \n",bank,money);
	bank_account[bank].remaining_cash -= money;
	bank_account[bank].acu_loan_amount += money;
	bank_account[bank].prod_loan[producernumber] += money;
}

/**
 * loan_reimburse()
 * Return loan amount and service charge
 */
void loan_reimburse(void * loan,unsigned long producernumber){
	(void)loan;
	(void)producernumber;
    //panic("You need to write some code!!!!");
	long int repay = *(long int *) loan;
	long int interest = (repay/100)*BANK_INTEREST;
	int i;
	for(i=0; i<NBANK ; i++){
		if(bank_account[i].prod_loan[producernumber] !=0){
			bank_account[i].remaining_cash += repay + interest;
			bank_account[i].interest_amount += interest;
			bank_account[i].prod_loan[producernumber] -= repay;
			break;
		}
	}
	producer_income[producernumber] += (repay/100) * PRODUCT_PROFIT;
}

/*
 * **********************************************************************
 * INITIALISATION AND CLEANUP FUNCTIONS
 * **********************************************************************
 */


/*
 * initialize()
 *
 * Perform any initialization you need before opening the investor-producer process to
 * producers and customers
 */

void initialize(){
	
	//kprintf("Hello World\n");
    //panic("You need to write some code!!!!");

	memset(customer_spending_amount,0,NCUSTOMER*sizeof(long int));
	mutex = sem_create("mutex", 1);
	TO = sem_create("TO",1);
	CLA = sem_create("CLA",1);
}

/*
 * finish()
 *
 * Perform any cleanup investor-producer process after the finish everything and everybody
 * has gone home.
 */

void finish(){
    //panic("You need to write some code!!!!");
}
