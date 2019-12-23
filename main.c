#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "bdd.h"
#include "sun.h"
#include "ftrend.h"
#include <stdbool.h>

struct Indxft{
	int indxf;
	int indxt;
	char* salida;
	char* puesta;
};

struct Indxft indexs(int year, int month, int day, float latitud, float longitud, int zho){
	struct Indxft indxft;
 	float jd = julian_date(year,month,day);
        float next_jd = julian_date(year,month,day+1);
        float jc = julian_century(jd);
        float next_jc = julian_century(next_jd);
        float gmsa = geo_mean_sun_anom(jc);
        float next_gmsa = geo_mean_sun_anom(next_jc);
        float ceq = center_equation(gmsa,jc);
        float next_ceq = center_equation(next_gmsa,next_jc);
        float gmsl = geo_mean_sun_long(jc);
        float next_gmsl = geo_mean_sun_long(next_jc);
        float stl = sun_true_long(ceq,gmsl);
        float next_stl = sun_true_long(next_ceq,next_gmsl);
        float sappl = sun_app_long(jc,stl);
        float next_sappl = sun_app_long(next_jc,next_stl);
        float eclipl = ecliptic_longitude(jc);
        float next_eclipl = ecliptic_longitude(next_jc);
        float sundec = sun_decl( sappl,  eclipl );
        float next_sundec = sun_decl( next_sappl,  next_eclipl );
        float hasun = ha_sunrise(sundec,latitud);
        float next_hasun = ha_sunrise(next_sundec,latitud);
        float vy = var_y(eclipl);
        float next_vy=var_y(next_eclipl);
        float eeo = eccent_earth_orbit(jc);
        float next_eeo = eccent_earth_orbit(next_jc);
        float eqt = eq_time( gmsl,  gmsa,  next_eeo,  vy);
        float next_eqt = eq_time( next_gmsl,  next_gmsa,  next_eeo,  next_vy);
        float sn = solar_noon(zho,longitud, eqt );
        float next_sn = solar_noon(zho,longitud, next_eqt );
        float sr= sunrise( sn, hasun);
	indxft.indxf = sunset( sn,hasun,year,month,day)+2; // -10000;
        indxft.indxt = next_sunrise(next_sn,next_hasun,year,month,day+1); // -10000;		
        indxft.salida = sunrise_hour(next_sn,next_hasun,year,month,day+1);
	indxft.puesta = sunset_hour( sn,hasun,year,month,day);
	return indxft;
}



int main(int argc, char **argv){
	

  int emaid = atoi(argv[1]);
  int y;
  int m;
  int d;	  
  int h;
  
  PGresult *res;
  struct BddResult *Res_ind;
  char Sql[5000];
  PGresult *Res_m;
  int size,ix=0;
  struct tm *parset;
  time_t now;
  struct tm *timeinfo;
  char today[30];
  now = time(NULL); 
  timeinfo=localtime(&now);
  struct Indxft indft;
  strftime (today, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
  printf("  Today %s \n ",today);

  if(!BddConn("dbname=labmovil user=labmovil password=lab.2017")){
        printf("%s\n", BddError());
        return(-1);
  }
	// printf("---------------- ERROR ------------");
	sprintf(Sql, "SELECT ema_name,ema_lat, ema_long FROM emas WHERE ema_id=%d limit 1;", emaid);

	if((Res_ind = BddSql(Sql)) == NULL){
         	printf("Error al buscar la inf: [%s]\n", Sql);
         	return(-1);
  	}

      
        //printf(" %s", BddValue(Res_ind, 0, 1));
        //printf(" %s \n", (string)BddValue(Res_ind, 0, 2));

        //char ema_name[] = atoc(BddValue(Res_ind, 0, 0));
        float latitud = atof(BddValue(Res_ind, 0, 1));
        float longitud = atof(BddValue(Res_ind, 0, 2));
        int zho = -4;  //--------> Zona Horaria
	 printf(" Estacion : %s %i  Latitud: %f Longitud : %f \n", BddValue(Res_ind, 0, 0),emaid,latitud,longitud);

	//printf("---------------- ERROR ------------");
       	sprintf(Sql, " select cix_fecha,cix_hora from climax_new,clidat where clx_tipid=3 and cix_ind=clx_cixind  and clx_emaid=%d and extract(month from cix_fecha) between 5 and 10  ORDER BY cix_ind;", emaid);
        if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
        size = Res_ind->rows;
        char Date[size][30];
	char Hour[size][30];
        for ( ix = 0 ; ix < Res_ind->rows ; ix++){
                strcpy(Date[ix],BddValue(Res_ind,ix,0));
                strcpy(Hour[ix],BddValue(Res_ind,ix,1));
        }
	//printf("---------------- ERROR ------------");
	sprintf(Sql, " select cix_ind, cast (clx_valor as float)/1000 as tmin from climax_new,clidat where clx_tipid=3 and clx_valor IS NOT NULL and cix_ind=clx_cixind and clx_emaid=%d and extract(month from cix_fecha) between 5 and 10  ORDER BY cix_ind;", emaid);
        if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
        float Data[size][5];
        for ( ix = 0 ; ix < Res_ind->rows ; ix++){
		Data[ix][4] = atof(BddValue(Res_ind,ix,0));
                Data[ix][0] = atof(BddValue(Res_ind,ix,1));
        }

   	sprintf(Sql, "select cast (clx_valor as float)/1000 as tmin from climax_new,clidat where clx_tipid=1 and clx_valor IS NOT NULL and cix_ind=clx_cixind and clx_emaid=%d and extract(month  from cix_fecha) between 5 and 10  ORDER BY cix_ind;", emaid);
        if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
        for ( ix = 0 ; ix < Res_ind->rows ; ix++){
                Data[ix][1] = atof(BddValue(Res_ind,ix,0));
        }
	
	sprintf(Sql, "select  cast (clx_valor as float)/1000 as tmin from climax_new,clidat where clx_tipid=8 and clx_valor IS NOT NULL and cix_ind=clx_cixind and clx_emaid=%d and extract(month  from cix_fecha) between 5 and 10  ORDER BY cix_ind;", emaid);
        if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
        for ( ix = 0 ; ix < Res_ind->rows ; ix++){
                Data[ix][2] = atof(BddValue(Res_ind,ix,0));
        }

	sprintf(Sql, "select  cast (clx_valor as float)/1000 as tmin from climax_new,clidat where clx_tipid=5 and clx_valor IS NOT NULL and cix_ind=clx_cixind and clx_emaid=%d and extract( month from cix_fecha) between 5 and 10  ORDER BY cix_ind;", emaid);
        if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
        for ( ix = 0 ; ix < Res_ind->rows ; ix++){
                Data[ix][3] = atof(BddValue(Res_ind,ix,0));       
        }
	
	 float tas_basal=0,tas_procio=0,tas_brocio=0;
	sprintf(Sql,"Select tas_basal,tas_procio,tas_brocio from tasas where tas_emaid=%d",emaid);
	if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
	if(atof(BddValue(Res_ind,0,0))){
		 tas_basal = atof(BddValue(Res_ind,0,0));
		 tas_procio = atof(BddValue(Res_ind,0,1));
		 tas_brocio = atof(BddValue(Res_ind,0,2));
	}
	printf("Tasas Basal: %f  Rocio: %f Brocio: %f \n",tas_basal,tas_procio,tas_brocio);

	bool wind=false;
	int year,month,day,windflag=0,total_data=0;
	float TempM[600],TempP[600],HumrP[600],min=1.0,TMin,HRel,TPro;

	sscanf(Date[0], "%d-%d-%d", &year, &month, &day);
        indft = indexs(year, month, day, latitud, longitud , zho);
	for(ix=0;ix<size;ix++){		

			if((int)Data[ix][4]>= indft.indxf && (int)Data[ix][4]<= indft.indxt){
			
				if((int)Data[ix][4]== indft.indxf){
                                
				     	TMin = Data[ix][0];
                                     	TPro = Data[ix][1];
                                     	HRel = Data[ix][3];
                                }
				if(Data[ix][2]>7.2){
                                   	
				        windflag++;
					if(windflag>=1){
						wind=true;
					}
                                }										
				if(Data[ix][0]<=0){
													
					if(Data[ix][0]<min){
					     min=Data[ix][0];
					    // printf("--- --- -- ---   - !!!!!!!!-   ------->> DATA::%s %f %s %d\n",Date[ix],min,wind ? "true" : "false",(int)Data[ix][4]);
					}		
				}
			}
			else{
				if(min<=0 && wind==false){		
											
						TempP[total_data]=TPro;
						TempM[total_data]=min;
						HumrP[total_data]=HRel;
						total_data++;				
					//	printf("--- --- -- ---   - !!!!!!!!-   ------->> DATA:: %f %f %f \n",TPro,min,HRel);
				}
				min=1,windflag=0,wind=false,TMin=0,HRel=0,TPro=0;
				sscanf(Date[ix], "%d-%d-%d", &year, &month, &day);
        			indft = indexs(year, month, day, latitud,longitud , zho);
			}
			
	}

	int tendsize = total_data +5,i=0;
        y = timeinfo->tm_year+1900, m = timeinfo->tm_mon+1 , d = timeinfo->tm_mday, h = timeinfo->tm_hour;	
	
	if(h >= 0 && h <= 17){
		now=time(NULL)-(86400);
		timeinfo=localtime(&now);		
		y = timeinfo->tm_year+1900, m = timeinfo->tm_mon+1 , d = timeinfo->tm_mday, h = timeinfo->tm_hour;
	}	
	printf(" year:%d  month:%d   day:%d  hour: %d  minits: %d \n",y,m,d,h,timeinfo->tm_min);
	

	indft = indexs(y, m, d, latitud,longitud , zho);
	printf("Salida y Puesta del sol----->:  %s -  %s\n",indft.salida,indft.puesta);
	char salida_sol[20];
	char puesta_sol[20];
	strcpy(salida_sol,indft.salida);
	strcpy(puesta_sol,indft.puesta);
	sprintf(Sql,"Update tasas set tas_puesta='%i/%i/%i %s',tas_amanecer='%i/%i/%i %s' where tas_emaid=%i;",y,m,d,salida_sol,y,m,d+1,puesta_sol ,emaid);
        res=PQexec(CONN,Sql);
        if(PQresultStatus(res)!=PGRES_COMMAND_OK){
                  printf("Error insert: in [%s]",Sql);
        }





	int diff = (indft.indxt - indft.indxf)-2;
	float PtoRo[total_data], Pm1[total_data], Pm2[total_data], Resid[total_data], Resid2[total_data], Tend[tendsize],AdjustT[tendsize],Tasa[tendsize];
	
        for( i = 0 ; i < total_data ; i++ ){
                float ptor=0;
                ptor=pow((HumrP[i]/100), 0.125 )* ( 112 +( 0.9*TempP[i]) ) + ( 0.1*TempP[i] ) - 112;
                PtoRo[i]=ptor;
		//printf(" --------------------------------------> Altura Base Nubes: %f \n",125*(TempP[i]-ptor));

	}		
	
	//printf(" --------------------------------------> DATA: %d \n",total_data);
	
	struct Model model1 = slope(TempP, TempM,total_data);
        Pronostic1(model1,TempP,TempM,Pm1,Resid,total_data);

        struct Model model2 = slope(PtoRo, Resid, total_data);
        Pronostic2(model1,model2,TempP,TempM,Pm2,Resid2,PtoRo,total_data);
	
	//indft = indexs(y, m,d, latitud,longitud , zho);

	sprintf(Sql, "select cast(clx_valor as float)/1000 from climax_new,clidat where clx_emaid=%d and clx_cixind=cix_ind and cix_ind=%d order by clx_cixind,clx_tipid ;",emaid,indft.indxf);
        if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
	float To= atof(BddValue(Res_ind,0,0));
	float Hr=  atof(BddValue(Res_ind,4,0));
	float Tminima = atof(BddValue(Res_ind,2,0));
	char Sunset[diff+3][15];
	

	sprintf(Sql, "select Distinct cix_hora,clx_cixind from climax_new,clidat where clx_emaid=%d and clx_cixind=cix_ind and cix_ind  BETWEEN  %d and %d order by clx_cixind ;",emaid,indft.indxf-24,indft.indxt-23);
        if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
	for(ix=0;ix<diff+3;ix++){
		strcpy(Sunset[ix],BddValue(Res_ind,ix,0));
	}
	
	float tas_value=0;
	float Pr = pow((Hr/100), 0.125 )* ( 112 +( 0.9*To) ) + ( 0.1*To) - 112;
	float Tp = (model1.slope*To)+ (model2.slope*Pr)+ model2.intersect;
	float Tp2 = (model1.slope*To)+ model1.intersect;
	float act_value, delta;
	int act_row;
	printf("%s  To : %f  tp : %f tp2 : %f pr: %f tm: %f\n",Sunset[0],To,Tp,Tp2,Pr,Tminima); 
        Tendencia(To,Tp,Tend,diff+2);
		
	sprintf(Sql,"select cast(clx_valor as float)/1000,cix_ind from climax_new,clidat where clx_emaid=%d and clx_tipid=3 and clx_cixind=cix_ind  ORDER BY clx_cixind Desc limit 3;",emaid);
	if((Res_ind = BddSql(Sql)) == NULL){
                printf("Error al buscar la inf: [%s]\n", Sql);
                return(-1);
        }
	act_row=atoi(BddValue(Res_ind,2,1));
	act_value=atof(BddValue(Res_ind,2,0));

	
	sprintf(Sql,"Delete from heladas where hel_emaid=%d;",emaid);
	res=PQexec(CONN,Sql);
	if(PQresultStatus(res)!=PGRES_COMMAND_OK){
		printf("Error delete: in [%s]",Sql);
	}
	tas_value=Tminima;
	for(i =0;i<diff+2;i++){

		if(act_row > indft.indxf && act_row){
			if(act_row == indft.indxf+1+i){
				delta = act_value - Tend[i];
			}
			AdjustT[i]=Tend[i]+delta;
		}else{
			AdjustT[i]=Tend[i];
		}
	//	printf("tas_value : %f \n",tas_value);
		if(tas_value<=(Pr+0.4) && tas_value>=(Pr-0.4)){
			tas_value = tas_value + tas_procio;
                }else if(tas_value<Pr){
			tas_value= tas_value + tas_brocio;
		}else{
                	tas_value= tas_value+tas_basal;
		}
		printf("Tendencia : %f °C  Adjust: %f Tasa: %f  %s  index : %d \n",Tend[i],AdjustT[i],tas_value,Sunset[i+1],indft.indxf+i+1);

		//printf(" ------------------->DATA cicle: %d \n",total_data);
		if(total_data>0){
			sprintf(Sql,"Insert Into heladas values(%d,%d,%f,%f,%f);",emaid,indft.indxf+i+1,Tend[i],AdjustT[i],tas_value);
			res=PQexec(CONN,Sql);
			if(PQresultStatus(res)!=PGRES_COMMAND_OK){
        	        	printf("Error insert: in [%s]",Sql);
	        	}
		}
	}	
	
	return 0;
}
