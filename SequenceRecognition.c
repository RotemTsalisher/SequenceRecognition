/* --------------------------------------------------- 
 * 8-bit sequence recognition state machine
 * the state machine will recognize the sequence 1010
 * will return an 8 bit vector with 1 
 * in places that the sequence was found,
 * 0 otherwise.
 * READING VECTORS RIGHT_TO_LEFT! {lsb to mbs}
 * example: v = [10110101] is being read as: {1,0,1,0,1,1,0,1}
 * and will result in '1' as the sequence 1010 is found.
   --------------------------------------------------- */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE !FALSE
#define FOREVER TRUE 

#define BIT0 (0x01<<0)
#define BIT1 (0x01<<1)
#define BIT2 (0x01<<2)
#define BIT3 (0x01<<3)
#define BIT4 (0x01<<4)
#define BIT5 (0x01<<5)
#define BIT6 (0x01<<6)
#define BIT7 (0x01<<7)

typedef enum
{
	STATE_ONE = 1 << 0,
	STATE_TWO = 1 << 1,
	STATE_THREE = 1 << 2,
	STATE_FOUR = 1 << 3
}StateMachineStates;

void stateOne(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp);
void stateTwo(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp);
void stateThree(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp);
void stateFour(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp);
unsigned char* stateMachine(unsigned char* v);
void printSeq(char a);

void main()
{
	unsigned char* res;
	unsigned char x = 0xB5;
	printf("=======================\n");
	printf("READING VECTORS RIGHT_TO_LEFT! {lsb to mbs}\nexample: v = [10110101] <<-- is being read as : {1, 0, 1, 0, 1, 1, 0, 1}\nand will result in 1 as the sequence 1010 is found.\n");
	printf("=======================\n");
	res = stateMachine(&x);
	if (res)
	{
		printf("FOUND SEQUENCE!\n");
		printf("Input Seq:  [");
		printSeq(x);
		printf("]\n");
		printf("Output Seq: [");
		printSeq(*res);
		printf("]\n");
	}
	else
	{
		printf("SEQUENCE WAS NOT FOUND\n");
	}

}
void printSeq(char a)
{
	int i;
	for (i = 0; i < 8; i++) {
		printf("%d", !!((a << i) & 0x80));
	}
}
unsigned char* stateMachine(unsigned char* v)
{
	unsigned char *sig,*out, *cmp;
	sig = malloc(sizeof(unsigned char)*1);
	*sig = 0x01; //BIT0; //init signal => manages states of system

	out = malloc(sizeof(unsigned char)*1);
	*out = 0x00; // init output

	cmp = malloc(sizeof(unsigned char)*1);
	*cmp = BIT0; // init comperator => checks if current bit is 1/0

	while ((*cmp <= BIT7) && (*cmp > 0))
	{
		// State one: if read 1 go to state 2 and output 0, otherwise stay in state 1
		if (*sig & STATE_ONE)
		{
			stateOne(sig, v, out, cmp);
			*cmp = *cmp << 1;
		}
		if (*sig & STATE_TWO)
		{
			stateTwo(sig, v, out, cmp);
			*cmp = *cmp << 1;
		}
		if (*sig & STATE_THREE)
		{
			stateThree(sig, v, out, cmp);
			*cmp = *cmp << 1;
		}
		if (*sig & STATE_FOUR)
		{
			stateFour(sig, v, out, cmp);
			*cmp = *cmp << 1;
		}

	}
	return out;
}
void stateOne(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp)
{
	// State one: init state:
	// if read 1 go to state 2 and output 0, otherwise stay in state 1
	if (*cmp & *v) // read 1
	{
		*sig &= ~(STATE_ONE);
		*sig |= STATE_TWO;
	}
}
void stateTwo(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp)
{
	// State two: read the first '1':
	//  if read 0 go to state 3 and output 0, otherwise stay in state 2
	if (!(*cmp & *v)) // read 0
	{
		*sig &= ~(STATE_TWO);
		*sig |= STATE_THREE;
	}
}
void stateThree(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp)
{
	// State three: read the seq '10':
	//  if read 0 go to state 1 {beginning} and output 0, if read 1 go to state four
	if (*cmp & *v) // read 1
	{
		*sig &= ~(STATE_THREE);
		*sig |= STATE_FOUR;
	}
	else // read 0
	{
		*sig &= ~(STATE_THREE);
		*sig |= STATE_ONE;
	}
}
void stateFour(unsigned char* sig, unsigned char* v, unsigned char* out, unsigned char* cmp)
{
	// State four: read the seq '101':
	//  if read 0 go to state 3 {read '10'} and output 1 {found '1010'}, if read 1 go to state two
	if (*cmp & *v) // read 1
	{
		*sig &= ~(STATE_FOUR);
		*sig |= STATE_TWO;
	}
	else // read 0
	{
		*out |= *cmp;
		*sig &= ~(STATE_FOUR);
		*sig |= STATE_THREE;

	}
}