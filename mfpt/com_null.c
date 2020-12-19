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


void shuffle(int *array, size_t n) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int usec = tv.tv_usec;
    srand48(usec);


    if (n > 1) {
        size_t i;
        for (i = n - 1; i > 0; i--) {
            size_t j = (unsigned int) (drand48()*(i+1));
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main(int argc,char *argv []){

	char buf[0x100];
	int suma=0,a,a0,a1,i,j,tcolors,tcolors1;
	unsigned int low, high, medium;
	float elem,suma2,w1;
	int *colassign,*netmarker,*equi,*colmarker,maxc,steps,*in,*out,marker=0,n1=0,string1,string2;
	float *cvisit,*nw1,*nw,string3,r1,*colw,suma1=0;
	int marker1,loc,loc0,real,real1,realnumber=10,realnumber1=100,real2,realnumber2=10;


		snprintf(buf,sizeof(buf),"comcsa/network_%s.csv",argv[1]);
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
	   equi=(int *)calloc(n1,sizeof(int));
		colmarker=(int *)calloc(n1+1,sizeof(int));
		netmarker=(int *)calloc(n1+1,sizeof(int));
		for (i=0;i<n1+1;i++){netmarker[i]=-1;colmarker[i]=-1;}



		snprintf(buf,sizeof(buf),"comcsa/network_%s.csv",argv[1]);
		in_file1=fopen(buf,"r");
		while(!feof(in_file1)){
				fscanf( in_file1, "%d;%d;%f\n", &string1, &string2, &string3);
				in[marker]=string2;
				out[marker]=string1;
				nw[marker]=string3;
            equi[string1]=string1;
                    equi[string2]=string2;
				nw1[marker]=string3;
				if (netmarker[string1]==-1){netmarker[string1]=marker;}
				if (string1>n1){n1=string1;}
				if (string2>n1){n1=string2;}

				marker=marker+1;

		}
 fclose(in_file1);
   marker1=0;

   maxc=-1;
    snprintf(buf,sizeof(buf),"comcsa/colorethmix_%s.csv",argv[1]);
    in_file1=fopen(buf,"r");
    while(!feof(in_file1)){
        fscanf( in_file1, "%d;%d;%f\n", &string1, &string2, &string3);
        marker1+=1;

    }
 fclose(in_file1);
    colassign=(int *)calloc(marker1,sizeof(int));
    colw=(float *)calloc(marker1,sizeof(float));
    marker1=0;
    snprintf(buf,sizeof(buf),"comcsa/colorethmix_%s.csv",argv[1]);
    in_file1=fopen(buf,"r");
    while(!feof(in_file1)){
        fscanf( in_file1, "%d;%d;%f\n", &string1, &string2, &string3);
        colassign[marker1]=string2;
        colw[marker1]=string3;
         if (string2>maxc){maxc=string2;}
        if (colmarker[string1]==-1){colmarker[string1]=marker1;}
        marker1=marker1+1;
    }
		 fclose(in_file1);
    maxc=maxc+1;

    colmarker[n1]=marker1;

    netmarker[n1]=marker;


    for(i=0;i<n1;i++){
        suma=0;
        a0=netmarker[i];
        a1=netmarker[i+1];
        for(j=a0;j<a1;j++)
        {suma=suma+nw[j];

        }
        suma2=0;
        for(j=a0;j<a1;j++)
        {

            w1=nw[j]/suma;
            suma2=suma2+w1;
            nw[j]=suma2;


        }
    }

    marker=0;

    FILE *fp11;
    snprintf(buf,sizeof(buf),"results_commu/nullifpt_%s.csv",argv[1]);
    fp11=fopen(buf,"w");

    int t1;
    t1 = (int)clock();
    srand((unsigned)time(NULL)*t1);


		int ncolors=maxc,pcolors[maxc];
		float icp[ncolors][ncolors],icp1[ncolors][ncolors],cnorm[ncolors][ncolors],cnorm1[ncolors][ncolors];

        for (i=0;i<ncolors;i++){ for (j=0;j<ncolors;j++){pcolors[j]=0; cnorm1[i][j]=0;icp1[i][j]=0;}}

     for(real2=0;real2<realnumber2;real2++){
         printf("real   %d\n",real2);
         shuffle(equi,n1);


    for(real=0;real<realnumber;real++){

        for (j=0;j<ncolors;j++){pcolors[j]=0;}
         printf("real   %d\n",real);

        for(i=0;i<n1;i++){
			      a0=colmarker[i];
            a1=colmarker[i+1];
            suma1=0;
      //      c=0;
             r1=((float)rand())/RAND_MAX;
            for (j=a0;j<a1;j++){

                if(suma1<r1 && r1<suma1+colw[j]){
                    cnode[equi[i]]=colassign[j];
                    pcolors[colassign[j]]=1;
            //        c=1;
                    break;
                  }

                if(j==(a1-1)){
                    cnode[equi[i]]=colassign[j];
                    pcolors[colassign[j]]=1;
          //          c=1;
                    break;
                }
                suma1=suma1+colw[j];
        }

        }




	      tcolors=0;
        for (i=0;i<ncolors;i++){   if  (pcolors[i]>0){tcolors=tcolors+1;}   for (j=0;j<ncolors;j++){cnorm[i][j]=0;icp[i][j]=0;}}
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
                        elem=r1;
                        low = a0;
                        high = a1-1;
                        medium = (low + high) / 2;
                        while (low < high){
                            if (elem <= nw[medium]){
                                high = medium;
                            }
                            else{
                                low = medium + 1;
                            }
                            medium = (low + high) / 2;
                        }

                        loc=in[low];
                        if (cvisit[cnode[loc]]==real1){
                            cnorm[cnode[loc0]][cnode[loc]]=cnorm[cnode[loc0]][cnode[loc]]+1;
                            cvisit[cnode[loc]]+=1;
                            icp[cnode[loc0]][cnode[loc]]=icp[cnode[loc0]][cnode[loc]]+steps;
                            tcolors1=tcolors1-1;

                        }

                        if (tcolors1>0){a=0;}
                    }

                }

                for (j=0;j<ncolors;j++){

                    if (cnorm[cnode[loc0]][j]>0){
                        icp1[cnode[loc0]][j]=icp1[cnode[loc0]][j]+icp[cnode[loc0]][j]/cnorm[cnode[loc0]][j];
                        cnorm1[cnode[loc0]][j]=cnorm1[cnode[loc0]][j]+1;
                    }
                    i=cnode[loc0];
                    cnorm[i][j]=0;icp[i][j]=0;
                    cvisit[j]=0;
                }
            }
         }

     }



    for (i=0;i<ncolors;i++){

        for (j=0;j<ncolors;j++){
            if (cnorm1[i][j]>0){
                fprintf(fp11,"%d\t%d\t%f\t%f\t%d\t%d\n",i,j,icp1[i][j],icp1[i][j]/cnorm1[i][j],real,real2);}

        }}



    fclose(fp11);

		free(in);
		free(cvisit);
		free(out);
		free(nw1);
	 free(nw);
	  free(equi);
		free(colmarker);
			free(netmarker);
		free(colassign);
			free(colw);

}
