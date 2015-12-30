#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	int sign1 = a>>31;
	int sign2 = b>>31;
	a = sign1==0 ? a : -a;
	b = sign2==0 ? b : -b;
	unsigned int a1 = (a>>16) & 0xffff, b1 = (b>>16) & 0xffff;
	unsigned int a0 = a & 0xffff, b0 = b & 0xffff;
	unsigned int c0,c1,c2,c3;
	c0 = a0 * b0;
	c1 = c0 / 0xffff + a0 * b1 + a1 * b0; c0 %= 0xffff;
	c2 = c1 / 0xffff + a1 * b1; c1 %= 0xffff;
	c3 = c2 / 0xffff; c2 %= 0xffff;
	int ans = c1 + (c2 << 16);
	if(sign1 != sign2) ans = -ans;
	return ans;
}


FLOAT F_div_F(FLOAT a, FLOAT b) {
	nemu_assert(b);

	int i = 16;
	int sign_a = a >>31, sign_b = b >> 31;
	int sign = sign_a ^ sign_b;
	FLOAT _a = (sign_a ? -a : a);
	FLOAT _b = (sign_b ? -b : b);
	FLOAT c = 0;

	while(i >0){
	    if(_a >= _b){
	        c += (1 << i);
	        _a -= _b;
	    }else{
	        _b = _b>> 1;
	        i --;
	    }
	    if(_b == 0x0 ||_a == 0x0) break;
	}
	return (sign? -c : c);
}

FLOAT f2F(float a) {
	int _a = *(int *)(&a);
	int sign = _a & 0x80000000;	
	int e = ((_a & 0x7f800000)>>23) - 127;	//exponent
	int m= (_a & 0x007fffff) | 0x00800000;	//significand+1
	int ans;
	if(e > 7)  ans =(m << (e -7));
	else ans = (m >> (7 -e));
	if(sign) ans = -ans;
	return ans;
}


FLOAT Fabs(FLOAT a) {
	return a < 0? -a :a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}
