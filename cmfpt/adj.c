//This program is free software. You can use, copy, distribute, modify and/or redistribute
//it under the terms of the MIT/Expat License. See the file LICENSE for more details.

//(c) 2020 Aleix Bassolas

//  If you use this software please consider citing the original paper:
//
//  A. Bassolas, S. Sousa, V. Nicosia
//  "Diffusion segregation and the disproportionate incidence of
//   COVID-19 in African American communities", Journal of The Royal
//   Society Interface,

//   A. Bassolas, and V. Nicosia
//  "First-passage times to quantify and compare structural correlations
//	 and heterogeneity in complex systems." arXiv preprint arXiv:2011.06526 (2020).


#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
double Haversine(double th1, double ph1, double th2, double ph2)
{
		double R=6371;
		double TO_RAD=(3.1415926536 / 180);
		double dx, dy, dz;
		ph1 -= ph2;
		ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;

		dz = sin(th1) - sin(th2);
		dx = cos(ph1) * cos(th1) - cos(th2);
		dy = sin(ph1) * cos(th1);
		return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}
double Haversine1(double th1, double ph1, double th2, double ph2)
{
		double dz;

		dz = sqrt((th1-th2)*(th1-th2)+(ph1-ph2)*(ph1-ph2));

		return dz;
}
int main(int argc,char *argv []){


		char buf[0x100];
		FILE *fp11;
		int marker1,a,a0,a1,i,j;
		int *colassign,*in,*out,*colmarker,*netmarker,marker=0,n1=0,string1,string2,tcolors,tcolors1,steps;
    float *cvisit,*nw1,*nw,string3,r1,*colw;
		int ncolors=7,pcolors[ncolors],r2,loc,loc0,real,real1,realnumber=100,realnumber1=100;
    float icp[ncolors][ncolors],icp1[ncolors][ncolors],cnorm[ncolors][ncolors],cnorm1[ncolors][ncolors],suma1=0;


		snprintf(buf,sizeof(buf),"adjcsa/network_%s.csv",argv[1]);
		FILE *in_file1=fopen(buf,"r");
		while(!feof(in_file1)){
				fscanf( in_file1, "%d;%d;%f\n", &string1, &string2, &string3);
				if (string1>n1){n1=string1;}
				if (string2>n1){n1=string2;}
				marker=marker+1;

		}
 fclose(in_file1);
		int n2=marker;
		marker=0;
		n1=n1+1;
			int cnode[n1];
		in=(int *)calloc(n2,sizeof(int));
    cvisit=(float *)calloc(n1,sizeof(float));
		out=(int *)calloc(n2,sizeof(int));
		nw1=(float *)calloc(n2,sizeof(float));
		nw=(float *)calloc(n2,sizeof(float));
		colmarker=(int *)calloc(n1+1,sizeof(float));
		netmarker=(int *)calloc(n1+1,sizeof(float));


		for (i=0;i<n1+1;i++){netmarker[i]=-1;colmarker[i]=-1;}

		snprintf(buf,sizeof(buf),"adjcsa/network_%s.csv",argv[1]);
		in_file1=fopen(buf,"r");
		while(!feof(in_file1)){
				fscanf( in_file1, "%d;%d;%f\n", &string1, &string2, &string3);
				in[marker]=string2;
				out[marker]=string1;
				nw[marker]=string3;
				nw1[marker]=string3;
				if (netmarker[string1]==-1){netmarker[string1]=marker;}
				if (string1>n1){n1=string1;}
				if (string2>n1){n1=string2;}
				marker=marker+1;

		}
 fclose(in_file1);
    marker1=0;
    snprintf(buf,sizeof(buf),"adjcsa/coloreth_%s.csv",argv[1]);
    in_file1=fopen(buf,"r");
    while(!feof(in_file1)){
        fscanf( in_file1, "%d;%d;%f\n", &string1, &string2, &string3);
        marker1+=1;

    }
 fclose(in_file1);
		colassign=(int *)calloc(marker1,sizeof(int));
    colw=(float *)calloc(marker1,sizeof(float));

    marker1=0;

    snprintf(buf,sizeof(buf),"adjcsa/coloreth_%s.csv",argv[1]);
    in_file1=fopen(buf,"r");
    while(!feof(in_file1)){
        fscanf( in_file1, "%d;%d;%f\n", &string1, &string2, &string3);
        colassign[marker1]=string2;
        colw[marker1]=string3;
        if (colmarker[string1]==-1){colmarker[string1]=marker1;}
        marker1=marker1+1;
    }
 fclose(in_file1);
    colmarker[n1]=marker1;
    netmarker[n1]=marker;
    marker=0;

    snprintf(buf,sizeof(buf),"results_spatial/ifpt_%s.csv",argv[1]);
    fp11=fopen(buf,"w");

    int t1;
    t1 = (int)clock();
    srand((unsigned)time(NULL)*t1);

     for (j=0;j<ncolors;j++){pcolors[j]=0; for (i=0;i<ncolors;i++){cnorm1[i][j]=0;icp1[i][j]=0;}}
    for(real=0;real<realnumber;real++){

        for (j=0;j<ncolors;j++){pcolors[j]=0; }

  // Here I assign a color/class from the distribution
     printf("real   %d;%d\n",real,realnumber);
        for(i=0;i<n1;i++){

            a0=colmarker[i];
            a1=colmarker[i+1];
            suma1=0;
    //        c=0;
             r1=((float)rand())/RAND_MAX;
            for (j=a0;j<a1;j++){
                if(suma1<r1 && r1<suma1+colw[j]){
                    cnode[i]=colassign[j];
                    pcolors[colassign[j]]=1;
      //              c=1;
                    break;
                  }
                if(j==a1-1){
                    cnode[i]=colassign[j];
                    pcolors[colassign[j]]=1;
        //            c=1;
                    break;
                }
                suma1=suma1+colw[j];
        }



        }

      tcolors=0;
        for (i=0;i<ncolors;i++){  cvisit[i]=0; if  (pcolors[i]>0){tcolors=tcolors+1;}   for (j=0;j<ncolors;j++){cnorm[i][j]=0;icp[i][j]=0;}}
            for (loc0=0;loc0<n1;loc0++){

                 for (real1=0;real1<realnumber1;real1++){
                     steps=0;
                     loc=loc0;
                     a=0;
                     tcolors1=tcolors;
                     while (a==0){
                         steps=steps+1;
                         a0=netmarker[loc];
                         a1=netmarker[loc+1];
                         suma1=0;
                             a=1;
                         r1=((float)rand())/RAND_MAX;
                         r2=(int)(a1-a0)*r1;
                         if (r2==(int)(a1-a0)){r2=r2-1;}
                         loc=in[a0+r2];
                         if (cvisit[cnode[loc]]==real1){
                             cnorm[cnode[loc0]][cnode[loc]]=cnorm[cnode[loc0]][cnode[loc]]+1;
                             cvisit[cnode[loc]]+=1;
                             icp[cnode[loc0]][cnode[loc]]=icp[cnode[loc0]][cnode[loc]]+steps;
                             tcolors1=tcolors1-1;
                         }
                           if (tcolors1>0){a=0;}
                     }

                 }
                for (j=0;j<ncolors;j++){ if (cnorm[cnode[loc0]][j]>0){icp1[cnode[loc0]][j]=icp1[cnode[loc0]][j]+icp[cnode[loc0]][j]/cnorm[cnode[loc0]][j];
                cnorm1[cnode[loc0]][j]=cnorm1[cnode[loc0]][j]+1;}
                     cnorm[cnode[loc0]][j]=0;icp[cnode[loc0]][j]=0;
                    cvisit[j]=0;
                 }

            }



         }


    for (i=0;i<ncolors;i++){
        for (j=0;j<ncolors;j++){
            if (cnorm1[i][j]>0){
                fprintf(fp11,"%d\t%d\t%f\t%f\t%d\n",i,j,icp1[i][j],icp1[i][j]/cnorm1[i][j],real);}

        }}
			free(in);
		  free(cvisit);
			free(out);
			free(nw1);
		 free(nw);
			free(colmarker);
				free(netmarker);
			free(colassign);
				free(colw);
    fclose(fp11);

}
