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
#include <iostream>
#include <cmath>

#include "../header/ctsa.h"
#include "../header/matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

int main(void) {
	int i, j, k, d, train_len, pred_len, t_win_s, t_win_e, N, M, offset;
	double *inp, *inpd, *par;
	int p, q;
	double *xpred, *amse;
	FILE *ifp;
	FILE *ifp1;
	double temp[2500];

	pred_len  = 10;
        train_len = 20;
        d=1;

        inp   =  (double*)malloc(sizeof(double)* train_len);
	xpred =  (double*)malloc(sizeof(double)* pred_len);
	amse  =  (double*)malloc(sizeof(double)* pred_len);

	//ifp   = fopen("/home/taolq/workspace/eclipse-workspace/lte-sim-r5/result/ue_1/result.txt", "r");
	ifp   = fopen("../data/eesm_30_1200.dat", "r");
	i = 0;
	if (!ifp) {
	   printf("Cannot Open File");
	   exit(100);
	}

	while (!feof(ifp)) {
	   fscanf(ifp, "%lf \n", &temp[i]);
	   i++;
	}

	N=i;

       for (int i=0;i<N;i++)
          printf("%d  %g\n",i, temp[i]);

       const char* a = "../result/result_";
       const char* c = "_";
       const char* e = ".txt";
       const char* e1 = ".png";
       int b,g;
       char f[150];
       char f1[150];
       for (offset=0; offset<=300; offset+=30) {

           b=offset;
           g=offset + 30;

           sprintf(f,"%s%d%s%d%s",a,b,c,g,e);
           sprintf(f1,"%s%d%s%d%s",a,b+train_len,c,g-1,e1);

           ifp1 = fopen(f,"w");

           t_win_s=offset;
           t_win_e=train_len+t_win_s;

           j=0;
           for (i = t_win_s; i < t_win_e; ++i) {
  	        inp[j++] = temp[i];
           }

           predict(inp, xpred, amse, train_len, pred_len);

           std::vector<double> xlabel(pred_len);
           std::vector<double> presul(pred_len);
           std::vector<double> actual(pred_len);

           k=t_win_e;
           for (i = 0; i < pred_len; ++i) {
        	 xlabel.at(i)=k;
        	 presul.at(i)=xpred[i];
        	 actual.at(i)=temp[t_win_e+i-1];
                 printf("%d   %g   %g \n", k, xpred[i],temp[t_win_e+i-1]);
                 if (i<pred_len-1)
                     fprintf(ifp1,"%d  %g   %g \n ",k++, xpred[i],temp[t_win_e+i-1]);
                 else
                     fprintf(ifp1,"%d  %g   %g ",k++, xpred[i],temp[t_win_e+i-1]);
           }

          fflush(ifp1);
          fclose(ifp1);
          plt::clf();

          char *buf = new char[200];
          sprintf(buf, "%s%d%s%d%s%d%s%d%s", "train data index [", t_win_e-20," , ", t_win_e-1,"]    predict data index [", t_win_e," , ",t_win_e+9, "]");

          plt::title(buf);
          plt::xlim(xlabel.front(), xlabel.back());
          plt::plot(xlabel,presul,"b^");
          plt::plot(xlabel,actual,"r^");
          plt::named_plot("predict", xlabel, presul);
          plt::named_plot("label",   xlabel, actual);
          plt::grid(true);
          plt::legend();
          plt::save(f1);
    }

    fclose(ifp);
    free(inp);
    free(inpd);
    free(xpred);
    free(amse);

    return 0;
}
