#include"emudef.h"
#include"shifter.h"
#include<stdio.h>
typedef int32_t I;
typedef uint32_t U;
#define SIZE 1000
#define C_BIT 29

void output_binary(U a) {
	U mask = 1 << 31;
	while (mask) {
		printf("%d", ((mask & a) != 0));
		mask >>= 1;
	}
	printf("\n");
}


int main(void) {
	
	I *cpsr = malloc(sizeof(I));
	U i = 0,tc,c,r;

	while( i++ < SIZE) {
		U a = (U) rand();
		U b = (U) rand()%32;
		*cpsr = rand();

	
		/*test LSL*/
		tc = *cpsr;
		c = b? (a >> (32-b) != 0): 0;
		tc = tc| c*(1<<C_BIT);
		r = shift(a,b,LSL,cpsr);
		assert(r == (a<<b));
		/*
		if (*cpsr != tc) {
			printf("a is %u, b is %u, a >> 32 is %u\n", a, b, a >> 31);
			printf("the carry out flag is %d\n",c);
			printf("the cpsr should be\n");
			output_binary(tc);
			printf("but is\n");
			output_binary(*cpsr);
			r = shift(a,b,LSL,cpsr);
		}*/

		assert(*cpsr == tc);
		
		
		/*test LSR*/
		tc = *cpsr;
		c = b ? (a << (32-b) != 0):0;
		tc = tc| c*(1<<C_BIT);
		r = shift(a,b,LSR,cpsr);
		assert(r == (a>>b));
		assert(*cpsr == tc);


		/*test ASR*/
		tc = *cpsr;
		c = b?(a << (32-b) != 0):0;
		tc = tc| c*(1<<C_BIT);
		r = shift(a,b,ASR,cpsr);
		assert((I)r == ((I)a>>b));
		assert(*cpsr == tc);
		

		/*test ROR*/

		a = 830941034; 
		U b1 = 1;
		U b2 = 2;
		U b3 = 3;
		U b4 = 4;
		U b5 = 5;
		U b6 = 6;
		U b24 = 24;

		U r1 = 415470517;
		U r2 = 2355218906;
		U r3 = 1177609453;
		U r4 = 2736288374;
		U r5 = 1368144187;
		U r6 = 2831555741;
		U r24 = 2267507249;
		
		U rr1 = shift(a,b1,ROR,cpsr);
		U rr2 = shift(a,b2,ROR,cpsr);
		U rr3 = shift(a,b3,ROR,cpsr);
		U rr4 = shift(a,b4,ROR,cpsr);
		U rr5 = shift(a,b5,ROR,cpsr);
		U rr6 = shift(a,b6,ROR,cpsr);
		U rr24 = shift(a,b24,ROR,cpsr);
		
		assert(r1 == rr1);
		assert(r2 == rr2);
		assert(r3 == rr3);
		assert(r4 == rr4);
		assert(r5 == rr5);
		assert(r6 == rr6);
		/*
		printf("the original number is\n");
		output_binary(a);
		printf("the result should be\n");
		output_binary(r24);
		printf("but is\n");
		output_binary(rr24);
		*/
		
		assert(r24 == rr24);
	}

	free(cpsr);
}
