#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	return a*b/65535;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	return a/b*65535;
}

FLOAT f2F(float a) {
	int p = *(int *)&f;
 //尾数 当然与真实的尾数左移了23位
 int t = (p & 0x7FFFFF) | 0x800000;
 //指数
 int e = (p >> 23) & 0xFF;
 //符号位
 int s = p >> 31;
 if(e - 127 < -31 || e - 127 > 31)
 {
  printf("结果不可知\n");
 }
 e = e-127-23;
 if(e > 0)//左移
  p = t << e;
 else if(e < 0)//右移
  p = t >> -e;
 if(s < 0)
  p = -p;
 if(f == 0)
  p = 0;
	return int2F(p);
}

FLOAT Fabs(FLOAT a) {
	if(a>0)
		return a;
	else 
		return -a;
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

