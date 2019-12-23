#include "ftrend.h"

struct Model slope(float Arr1[], float Arr2[], int size){
      float Arr1_prom = 0, Arr2_prom = 0, ac_arr1 = 0, ac_arr2 = 0;
      int i=0;
      for (i = 0; i < size; i++) {
        ac_arr1 = ac_arr1 + Arr1[i];
        ac_arr2 = ac_arr2 + Arr2[i];
      }
      Arr1_prom = ac_arr1/size;
      Arr2_prom = ac_arr2/size;
      
     // printf("Promedio : %f \n",Arr1_prom);
     // printf("Promedio : %f \n",Arr2_prom);
      float Alfa, Beta;
      float sum1 = 0, sum2 =0;
      float aux1, aux2;
        for ( i = 0 ; i < size ; i++) {
            aux1 = ((Arr1[i]-Arr1_prom)*(Arr2[i]-Arr2_prom));
            sum1 = sum1 + aux1;
            aux2 = pow((Arr1[i]-Arr1_prom),2);
            sum2 = sum2 + aux2;

	    //printf("aux1 : %f aux2 : %f\n",aux1,aux2);
            //printf("sum1 : %f sum2 : %f\n",sum1,sum2);	

	}
      Beta=(sum1/sum2);
      Alfa=Arr2_prom-(Beta*Arr1_prom);
     //printf("Slope : %.4f \n",Beta);
     // printf("Intersect : %.4f \n",Alfa);
      struct Model model={Beta, Alfa};
      return model;
}

void Pronostic1(struct Model model, float Arr1[], float Arr2[], float *Arr3, float Arr4[], int size ){
        float pronostic=0, residual=0;
        int i=0;
	for ( i = 0 ; i < size; i++) {
          pronostic=0, residual=0;
          pronostic = model.slope * Arr1[i] + model.intersect;
          residual = Arr2[i]-pronostic;
          Arr3[i]=pronostic;
         //printf("pronosctic : %f \n",pronostic);
	   Arr4[i]=residual;
  //        printf("Pronostic Tm C° : %f  Real Tm C° : %f \n", pronostic,Arr2[i]);
          //<<" residual "<<residual<<endl;
        }
}
void Pronostic2(struct Model model1,struct Model model2, float Arr1[], float Arr2[], float Arr3[], float *Arr4, float Arr5[], int size ){
        float pronostic=0, residual=0;
        int i=0;
	for (i = 0 ; i < size; i++) {
          pronostic=0, residual=0;
          pronostic = model1.slope * Arr1[i] + Arr5[i]*model2.slope + model1.intersect;
          residual = Arr2[i]-pronostic;
          Arr3[i] = pronostic;
          Arr4[i] = residual;
	//printf("Pronostic2 Tm C° : %f  Real Tm C° : %f\n", pronostic, Arr1[i]);
        //printf("Reasidual : %f \n",reasidual);
        }
}
void Tendencia(float To, float Tp, float *Arr3, float diff){
          float ti, b, h=diff;
//	  h = (diff - To) + Tp;  
          b = (float)( Tp - To )/(float)sqrt(h); 
//	  printf("h %f : b : %f \n",h,b);
          int i=0;
	/*
	  for( i=0 ; i< diff; i++){
	    	
	    // Arr3[i]=To +(b*(sqrt(i)));
	     printf(" Tend C° : %f F° : %f \n",Arr3[i],(Arr3[i]*1.8)+32);
          }*/
	  int k=1;
	  for ( i = 0; i < diff; i++) {
              ti =To + (b*sqrt(k));
              Arr3[i]=ti;
 	     // printf(" Tend C° : %f F° : %f \n",ti,(ti*1.8)+32);
            //printf(" Tend C° : %f F° : %f \n",(ti-32)*0.5555,ti);
	      k++;
	  }
         
}
