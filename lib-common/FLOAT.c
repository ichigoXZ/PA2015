#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	unsigned sign_a = (a & 0x80000000)>>31, sign_b = (b & 0x80000000) >> 31;
	unsigned sign = sign_a ^ sign_b;	
	unsigned q = (1<<16);
	FLOAT _a = (sign_a ? -a : a);
	FLOAT _b = (sign_b ? -b : b);
	/*unsigned qa = _a / q;
	unsigned qb = _b /q;
	unsigned ya = _a %q;
	unsigned yb = _b % q;
	int result = qa*qb *q + qa * yb +qb*ya + ((ya * yb) >>16);
	return (sign? -result: result);
	//return int2F(F2int(a)*F2int(b));*/
	FLOAT result = (_a/_b) >> 16;
	return (sign ? -result:result);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	nemu_assert(b);

	int i = 16;
	int sign_a = (a & 0x80000000)>>31, sign_b = (b & 0x80000000) >> 31;
	int sign = sign_a ^ sign_b;
	FLOAT _a = (sign_a ? -a : a);
	FLOAT _b = (sign_b ? -b : b);
	/*FLOAT c = 0;

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
	return (sign? -c : c);*/
	FLOAT result = (_a/_b) << 16;
	return (sign ? -result:result);
	//return int2F(F2int(a)/F2int(b));
}

FLOAT f2F(float a) {
	/*int args = *(int *)(&a);
	int sign = args & 0x80000000;
	int e = ((args & 0x7f800000)>>23) - 127;
	int m= (args & 0x007fffff) + 0x00800000;
	int result;
	if(e > 7)  result =(m << (e -7));
	else result = (m >> (7 -e));
	if(sign) result = -result;
	return result;*/
	if(0==a)
		return 0;

	int p = *(int *)(&a);
	int result;
 	int t = (p & 0x7FFFFF) + 0x800000;		//尾数 真实的尾数左移了23位
 	int e = ((p >> 23) & 0xFF) - 127; 	//指数
 	int s = p >> 31; 	//符号位
 	if(e < -31 || e > 31)
  		nemu_assert(0);

 	if(e > 7)  result =(t << (e -7));
	else result = (t >> (7 -e));

 	if(s)
  		result = -result;

  	return result;
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