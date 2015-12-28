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

/*FLOAT F_div_F(FLOAT a, FLOAT b) {
	unsigned int a0 = a << 16;
	unsigned int a1 = a >> 16;
	unsigned int a2 = a >> 31;
	unsigned int a3 = a >> 31;
	int ans = 0, i;
	for(i = 0; i < 64; ++i)	{
		a3 = (a3 << 1) + (a2 >> 31);
		a2 = (a2 << 1) + (a1 >> 31);
		a1 = (a1 << 1) + (a0 >> 31);
		a0 = a0 << 1;
		ans = ans << 1;
		if(a3 > 0 || a2 >= b) {
			if(a2 < b) a3 --;
			a2 -= b;
			ans++; 
		}
	}
	return ans;
}*/
FLOAT F_div_F(FLOAT a, FLOAT b) {
	long long remain = a<0?-a:a;
	long long divisor = b<0?-b:b;
	int count;
	FLOAT result = 0;;
	remain = remain << 16;
	divisor = divisor << 16;
	count = 16;
	while(remain != 0) {
		if (remain >= divisor) {
			remain = remain - divisor;
			result = result | (1 << count);
		}
		if (count == 0) break;
		divisor = divisor >> 1;
		count --;
	}
	if ((a < 0 &&  b > 0) || (a > 0 && b < 0)) result = -result;
	return result;
}

FLOAT f2F(float a) {
	int f = *(int *)(&a);
	int sign = f & 0x80000000;
	int e = ((f & 0x7f800000)>>23) - 127;
	int m= (f & 0x007fffff) + 0x00800000;
	int result;
	if(e > 7)  result =(m << (e -7));
	else result = (m >> (7 -e));
	if(sign) result = -result;
	return result;
}


FLOAT Fabs(FLOAT a) {
	return a>=0 ? a : -a;
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

