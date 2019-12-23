#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>

 
struct Model{
  float slope;
  float intersect;
};


struct Model slope(float Arr1[], float Arr2[], int size);
void Pronostic1(struct Model model, float Arr1[], float Arr2[], float *Arr3, float Arr4[], int size );
void Pronostic2(struct Model model1,struct Model model2, float Arr1[], float Arr2[], float *Arr3, float Arr4[], float Arr5[], int size );
void Tendencia(float To, float Tp, float *Arr3, float diff);
