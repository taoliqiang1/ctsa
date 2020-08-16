/*
============================================================================
Name        : tseries.c
Author      : Rafat Hussain
Version     :
Copyright   :
Description : Hello World in C, Ansi-style
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../header/ctsa.h"

void predict(double *inp, double *xpred, double *amse, int train_len, int pred_len) {

    int i, d, D;
	int p, q, P, Q, s, r;

	auto_arima_object obj;

	p = 15;
	d = 2;
	q = 2;
	P = 0;
	D = 0;
	Q = 0;

	int order[3] = {p,d,q};
	int seasonal[3] = {P,D,Q};

    s = 0;
    r = 0;

	obj = auto_arima_init(order,seasonal,s,r,train_len);
	auto_arima_setApproximation(obj,0);
	auto_arima_setStepwise(obj,0);

	auto_arima_exec(obj,inp,NULL);

	printf("\n p=%d  d=%d  q=%d \n",obj->p,obj->d,obj->q);
	//auto_arima_summary(obj);

	auto_arima_predict(obj, inp, NULL,pred_len, NULL, xpred, amse);


    free(obj);

}
