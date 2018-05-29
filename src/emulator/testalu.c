#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include"emudef.h"
#include"operations.h"

typedef uint32_t U;
typedef int32_t I;
#define SIZE 10000

int main(void) {	
	I a, b,td,tc,n,z,c,v,nzcv,s=1;
	I *d = malloc(sizeof(I));
	I *cpsr = malloc(sizeof(I));
	*d = *cpsr = 0;

	I i = 0;
	while (i < SIZE) {
	a = (I) rand();
	b = (I) rand();

	/*test and*/
	td = *d = rand();
	tc = *cpsr = rand();

	alu_execute(AND, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == (a&b));

	n = ((U)*d >> 31);
	z = (*d == 0);
	c = ( (tc & 1 << 29) != 0);
	v = ( (tc & 1 << 28) != 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ((U)*cpsr >> 28));

	*d = td;
	*cpsr = tc;
	alu_execute(AND, (U)a, (U)b, d, cpsr, 0);
	assert(*d == (a&b));
	assert(*cpsr == tc);


	/*test eor*/
	td = *d = rand();
	tc = *cpsr = rand();

	alu_execute(EOR, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == (a^b));

	n = ((U)*d >> 31);
	z = (*d == 0);
	c = ( (tc & 1 << 29) != 0);
	v = ( (tc & 1 << 28) != 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ((U)*cpsr >> 28));
	
	*d = td;
	*cpsr = tc;
	alu_execute(EOR, (U)a, (U)b, d, cpsr, 0);
	assert(*d == (a^b));
	assert(*cpsr == tc);



	/*test sub*/
	td = *d = rand();
	tc = *cpsr = rand();
		
	alu_execute(SUB, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == (a-b));
	
	n = ((U)*d >> 31);
	z = (*d == 0);
	v = (((U)tc & (1 << 28)) != 0);
	c = (a > 0 && -b > 0 && *d < 0) || (a < 0 && -b < 0 && *d > 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ( ((U)*cpsr)>>28) );
	
	*d = td;
	*cpsr = tc;
	alu_execute(SUB, (U)a, (U)b, d, cpsr, 0);
	assert(*d == (a-b));
	assert(*cpsr ==tc);
	
	/*test rsb*/
	td = *d = rand();
	tc = *cpsr = rand();
		
	alu_execute(RSB, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == (b-a));
	
	n = ((U)*d >> 31);
	z = (*d == 0);
	v = (((U)tc & (1 << 28)) != 0);
	c = (b > 0 && -a > 0 && *d < 0) || (b < 0 && -a < 0 && *d > 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ( ((U)*cpsr)>>28) );

	*d = td;
	*cpsr = tc;
	alu_execute(RSB, (U)a, (U)b, d, cpsr, 0);
	assert(*d == (b-a));
	assert(*cpsr ==tc);

	/*test add*/
	td = *d = rand();
	tc = *cpsr = rand();
		
	alu_execute(ADD, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == (a+b));
	
	n = ((U)*d >> 31);
	z = (*d == 0);
	v = (((U)tc & (1 << 28)) != 0);
	c = (a > 0 && b > 0 && *d < 0) || (a < 0 && b < 0 && *d > 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ( ((U)*cpsr)>>28) );
	
	*d = td;
	*cpsr = tc;
	alu_execute(ADD, (U)a, (U)b, d, cpsr, 0);
	assert(*d == (b+a));
	assert(*cpsr ==tc);


	/*test tst*/
	td = *d = rand();
	tc = *cpsr = rand();

	alu_execute(TST, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == td);

	*d = a&b;
	n = ((U)*d >> 31);
	z = (*d == 0);
	c = ( (tc & 1 << 29) != 0);
	v = ( (tc & 1 << 28) != 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ((U)*cpsr >> 28));
	
	*d = td;
	*cpsr = tc;
	alu_execute(TST, (U)a, (U)b, d, cpsr, 0);
	assert(*d == td);
	assert(*cpsr == tc);

	/*test teq*/
	td = *d = rand();
	tc = *cpsr = rand();

	alu_execute(TEQ, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == td);

	*d = a^b;
	n = ((U)*d >> 31);
	z = (*d == 0);
	c = ( (tc & 1 << 29) != 0);
	v = ( (tc & 1 << 28) != 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ((U)*cpsr >> 28));
	
	*d = td;
	*cpsr = tc;
	alu_execute(TEQ, (U)a, (U)b, d, cpsr, 0);
	assert(*d == td);
	assert(*cpsr == tc);

	
	/*tst cmp*/
	td = *d = rand();
	tc = *cpsr = rand();
		
	alu_execute(CMP, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == td);
	
	*d = a-b;
	n = ((U)*d >> 31);
	z = (*d == 0);
	v = (((U)tc & (1 << 28)) != 0);
	c = (a > 0 && -b > 0 && *d < 0) || (a < 0 && -b < 0 && *d > 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ( ((U)*cpsr)>>28) );

	*d = td;
	*cpsr = tc;
	alu_execute(CMP, (U)a, (U)b, d, cpsr, 0);
	assert(*d == td);
	assert(*cpsr == tc);

	



	/*test ORR*/
	td = *d = rand();
	tc = *cpsr = rand();

	alu_execute(ORR, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == (a|b));

	n = ((U)*d >> 31);
	z = (*d == 0);
	c = ( (tc & 1 << 29) != 0);
	v = ( (tc & 1 << 28) != 0);
	nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
	assert(nzcv == ((U)*cpsr >> 28));
	
	*d = td;
	*cpsr = tc;
	alu_execute(ORR, (U)a, (U)b, d, cpsr, 0);
	assert(*d == (a|b));
	assert(*cpsr == tc);

	
	/*test mov*/
	td = *d = rand();
	tc = *cpsr = rand();
	alu_execute(MOV, (U)a, (U)b, d, cpsr, (U)s);
	assert(*d == b);
	assert(*cpsr == tc);
	
	*d = td;
	*cpsr = tc;
	alu_execute(MOV, (U)a, (U)b, d, cpsr, 0);
	assert(*d == b);
	assert(*cpsr == tc);

	
	
	
	i ++;
	}

	free(d);
	free(cpsr);
	return 0;
}

	/*	
	if (nzcv != ((U)*cpsr >> 28)) {
		printf("a is %d, b is %d\n", a, b);
		printf("%d & %d = %d but result is %d\n",a, b, a & b, *d);
		printf("n,z,c,v is %d,%d,%d,%d\n", n,z,c,v);
		printf("nzcv is %d\n", nzcv);
		printf("tc is %d\n", (U)tc >> 28);
		printf("cpsr is %d\n and condition is %d", *cpsr, (U)*cpsr>>28);	
		*d = td;
		*cpsr = tc;
		alu_execute(AND, (U)a, (U)b, d, cpsr, (U)s);
	}
	*/
