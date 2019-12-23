#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define PI 3.14159653589793238462643383279

int date_toindx(int y, int m, int d, int h);
float julian_date(int ano,int mes,int dia);
float julian_century(float jd);
float geo_mean_sun_anom(float jc);
float center_equation(float gmsa, float jc);
float geo_mean_sun_long(float jc);
float sun_true_long(float ceq,float gmsl);
float sun_app_long(float jc,float stl);
float ecliptic_longitude(float jc);
float sun_decl(float sappl, float eclipl );
float ha_sunrise(float sundec,float latitud);
float var_y(float eclipl);
float eccent_earth_orbit(float jc);
float eq_time(float gmsl, float gmsa,float next_eeo,float vy);
float solar_noon(float zho,float longitud,float eqt );
float sunrise(float sn,float hasun);
int sunset(float sn,float hasun,int ano,int mes,int dia);
int next_sunrise(float sn,float hasun,int ano,int mes,int dia);
char* sunrise_hour(float sn,float hasun,int ano,int mes,int dia);
char* sunset_hour(float sn,float hasun,int ano,int mes,int dia);
