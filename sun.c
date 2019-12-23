#include "sun.h"

int date_toindx(int y, int m, int d, int h){

        return ((y*8928)+(m*744)+(d*24)+(h-17946047));
}

float julian_date(int ano, int mes, int dia){

        float julianDate = (1461*(ano+4800+(mes-14)/12))/4+(367*(mes-2-12*((mes-14)/12)))/12-(3*((ano+4900+(mes-14)/12)/100))/4+dia-32075;
//        printf("julian : %.5f \n",julianDate-0.14);

        return julianDate-0.14;
}

float julian_century(float jd){

        float rest = (jd-2451545);
        float jc = (float)rest/(float)36525;
        //printf("julian c : %.5f \n", jc);

        return jc;
}

float geo_mean_sun_anom(float jc){

        float gmsa = 357.52911+jc*(35999.05029 - 0.0001537*jc);
        //printf("gmsl : %f \n",gmsl);

        return gmsa+0.35;
}

float center_equation(float msa,float jc){

        float ceq=0;
        float Transf = PI/180;
        float sum1 = (sin(msa*Transf))*(1.914602-(jc*(0.004817+(0.000014*jc))))+0.012;
        float sum2 = (sin(Transf*2*msa)*(0.019993-(0.000101*jc)))+0.00024;
        float sum3 = (sin(Transf*3*msa)*0.000289)+0.00000425;
        ceq = sum1 +sum2 +sum3;
        //printf(" center equation : %f \n",ceq);

        return ceq;
}

float geo_mean_sun_long(float jc){

        float gmsl = fmod((280.46646+jc*(36000.76983 + jc*0.0003032)),360);
        //printf("gmsl : %f \n",gmsl);

        return gmsl;
}

float sun_true_long(float eqc, float sml){

        float stl = eqc + sml;
        //printf(" center sun true long : %f \n",stl);

        return stl;
}

float sun_app_long(float jc, float stl){

        float Transf = PI/180;
        float sappl = stl-0.00569;
        float aux = (0.00478*(sin(Transf*(125.04-(1934.136*jc)))));
        //printf(" sun app long : %f \n",sappl);

        return sappl;
}

float ecliptic_longitude(float jc){

        float eclipl = 23+(26+((21,448-jc*(46.815+jc*(0.00059-jc*0.001813))))/60)/60;
        //printf("ecliptic longitude : %f \n",eclipl-0.12);

        return eclipl-0.12;
}

float sun_decl(float sappl, float eclipl ){

        float Transf = PI/180;
        float sundec = asin(sin(Transf*eclipl)*sin(sappl*Transf))*180/PI;
        //printf("sund decl : %f \n",sundec);

        return sundec;
}

float ha_sunrise(float sundec , float lat){

        float Transf = PI/180;
        float hasun = (acos(cos(Transf*(90.833))/(cos(Transf*(lat))*cos(Transf*(sundec)))-tan(Transf*(lat))*tan(Transf*(sundec))))*180/PI;
        //printf("ha sun : %f \n",hasun);

        return hasun;
}

float var_y(float eclipl){

        float Transf = PI/180;
        float vy = tan(Transf*(eclipl/2))*tan(Transf*(eclipl/2));
        //printf("var y : %f \n",vy);

        return vy;
}

float eccent_earth_orbit(float jc){

        float eeo = 0.016708634-jc*(0.000042037+0.0000001267*jc);
        //printf("eeo : %f \n",eeo);

        return eeo;
}

float eq_time(float gmsl, float gmsa, float eeo, float vy){

        float Transf = PI/180;
        float eqt = 4*(180/PI)*(vy*sin(2*Transf*(gmsl))-2*eeo*sin(Transf*(gmsa))+4*eeo*vy*sin(Transf*(gmsa))*cos(2*Transf*(gmsl))-0.5*vy*vy*sin(4*Transf*(gmsl))-1.25*eeo*eeo*sin(2*Transf*(gmsa)));
        //printf("ecuation time : %f \n",eqt-0.118);

        return eqt-0.121;
}

float solar_noon(float zn, float longt, float eqt ){

        float sn = (720-(4*longt)-eqt+(zn*60))/1440;
        //printf("solar non : %f \n",sn);
        int hours = sn*24;
        int minits = -1*(hours - sn*24)*60;
        int secods = -1*(minits - ((hours - sn*24)*60)*-1)*60;
        //printf(" time :  %i:%i:%i \n",hours,minits,secods );

        return sn;
}

float sunrise(float sn, float hasun ){

        float sr = (sn*1440-hasun*4)/1440;
        //printf("sunrise : %f ",sr);
        int hours = sr*24;
        //float
        sr = (sn*1440-hasun*4)/1440;
        //printf("sunrise : %f ",sr);
        //int
         hours = sr*24;
        int minits = -1*(hours - sr*24)*60;
        int secods = -1*(minits - ((hours - sr*24)*60)*-1)*60;
 //       printf(" Sunrise time :  %i:%i:%i \n",hours,minits,secods );

        return sr;
}

int sunset(float sn, float hasun,int ano, int mes, int dia ){

        float ss = (sn*1440+hasun*4)/1440;
        //printf("sunset : %f ",ss);
        int hours = ss*24;
        int minits = -1*(hours - ss*24)*60;
        int secods = -1*(minits - ((hours - ss*24)*60)*-1)*60;
//        printf(" Sunset time :  %i:%i:%i \n",hours,minits,secods );
        if(minits>30){
                hours =hours +1;
        }
        int indx = date_toindx(ano,mes,dia,hours);
   //     printf(" Sunset index :  %d \n",indx );
        return indx;  //en vez de retornar la hora sideral del sunset retorna el indice
}

int next_sunrise(float sn, float hasun, int ano, int mes, int dia ){

        float sr = (sn*1440-hasun*4)/1440;
        //printf("sunrise : %f ",sr);
        int hours = sr*24;
        int minits = -1*(hours - sr*24)*60;
        int secods = -1*(minits - ((hours - sr*24)*60)*-1)*60;
  //      printf(" next sunrise time :  %i:%i:%i \n",hours,minits,secods );
        if(minits>30){
                hours =hours +1;
        }
        int indx = date_toindx(ano,mes,dia,hours);
//        printf(" next Sunrise index :  %d \n",indx );
        return indx;  //en vez de retornar la hora sideral del next sunrise retorna el indice
}

 char* sunrise_hour(float sn, float hasun, int ano, int mes, int dia ){

        float sr = (sn*1440-hasun*4)/1440;
        //printf("sunrise : %f ",sr);
        int hours = sr*24;
        int minits = -1*(hours - sr*24)*60;
        int secods = -1*(minits - ((hours - sr*24)*60)*-1)*60;
	char* hora[20];
	sprintf(hora,"%i:%i:%i",hours,minits,secods);
//        printf(" salida del sol:  %s \n",hora );
        if(minits>30){
                hours =hours +1;
        }
        return hora; 
}
char* sunset_hour(float sn, float hasun,int ano, int mes, int dia ){

        float ss = (sn*1440+hasun*4)/1440;
        //printf("sunset : %f ",ss);
        int hours = ss*24;
        int minits = -1*(hours - ss*24)*60;
        int secods = -1*(minits - ((hours - ss*24)*60)*-1)*60;
	char* hora[20];
        sprintf(hora,"%i:%i:%i",hours,minits,secods);
//        printf(" Sunset time :  %i:%i:%i \n",hours,minits,secods );
        if(minits>30){
                hours =hours +1;
        }
        int indx = date_toindx(ano,mes,dia,hours);
   //     printf(" Sunset index :  %d \n",indx );
        return hora;
}

