
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//#include "main.c"
//---------------------------------
/*

 Вычисление референсной плотности выполняется либо по состоянию на конец налива
(p2.MDT==0 ), либо (p2.MDT== 1 )   усредняя значения в процессе налива
с начала отпуска дозы.

  Температура нефтепродукта берется либо по показаниям расходомера (p1.MDT==0)
 либо по внешнему термометру (p1.MDT==1).

p1.MDT - mode_temp
p2.MDT - mode_refD

*/

//---------------------------------
//---------------------------------

int type_liq=2;  // 2 Fuel Oils   - диэтопливо
                 // 5 Gazolines   - бензин
                 // 1 Crude Oils

//
// значения коэффициентов и формулы согласно API Standard 2540 from 1980
// (из документа Saab Rosemount "TankMaster,Inventory Calculations")
//
#define Kerosene (4)

double KK0[7]={ //type_liq
613.97226,     //0 Crude Oils
613.97226,     //1 Crude Oils
186.9696,     //2 Fuel Oils   - диэтопливо
594.5418,     //3 Jet  group
594.5418,     //4 Kerosene
346.42278,    //5 Gazolines   - бензин
0.0,          //6 Lubricating Oils
};

double KK1[7]={ //type_liq
0.0,          //0 Crude Oils
0.0,          //1 Crude Oils
0.48618,       //2 Fuel Oils
0.00,         //3 Jet  group
0.00,         //4 Kerosene
0.43884,       //5 Gazolines
0.62780,      //6 Lubricating Oils
};

float Aa= -0.00336312, Bb= 2680.3206;

float f_get_VCF(float Temp, float D_ref)
{
// API Standard 2540 from 1980
// MI 2632
// время выполнения 3040 мкс, CPU 80188,40 MHz
// по текущей температуре , референсной плотности 15C
// вычисляет VCF ( for D_Ref_15)
// использует type_liq - тип продукта
//
//  V_ref=V_obs * f_get_VCF(Temp,D_ref,T_Ref);
//

double TecTref;
double  dT;
float VCF;

  if((type_liq > 6) || (type_liq < 1)) return 1.;

  if( (D_ref > 1000) || (D_ref < 600)  ) return 1;


//  dT=Temp-T_ref;
    dT=Temp-15.;

  if(type_liq != Kerosene)
     TecTref = (KK0[type_liq]+KK1[type_liq]* D_ref)/(D_ref*D_ref);
  else
     TecTref= Aa + Bb/(D_ref*D_ref);

  VCF=exp( -TecTref * dT * (1.0 + (0.8*TecTref*dT) ));

 return VCF;
}
/*-------------------------------------------*/

double C1=-1.6208;
double C2=0.00021592;
double C3=870960;
double C4=4209.2;

float f_get_gamma ( float Temp, float D_ref_15)
{
// по текущей температуре и референсной плотности при 15C
// вычисляет коэффициент сжимаемости нефтепродукта
//
//  D_obs_PT =  D_obs_T / (1- Press * f_get_gg(Press,Temp,D_ref_15) );
//

float  gamma;

    if( (D_ref_15 > 1000) || (D_ref_15 < 600)  ) return 0;

  gamma = 0.001 *  exp( C1 + C2*Temp+ ((C3+C4*Temp)/(D_ref_15*D_ref_15 ) ) );

 return gamma;
}
/*-------------------------------------------*/
#define n_max 10
#define MinDNS 0.001

float f_get_ref_Dns(float D_obs, float Temp,float Press, float T_Ref)
{
// по наблюдаемой плотности D_obs и текущей температуре Temp
// вычисляет референсную плотность для заданной референсной температуры T_Ref

int i;
float D_ref_new,D_ref_old;
float VCF_ref;
float gamma;


//if( (D_obs > 1000) || (D_obs < 600)  ) return 0;

  D_ref_old=D_obs;

      if(flag_Slv !=0 )
               f_SlaveRTU();

      f_ctrl_v2();

  for(i=0;i<n_max;i++)
  {
    VCF_ref= f_get_VCF( Temp,D_ref_old);
//  if(VCF_ref == 0) return 1;
    if(VCF_ref == 0) return D_obs;
    gamma=f_get_gamma (Temp,D_ref_old);
//  if(gamma == 0) return 1;
    D_ref_new=D_obs * (1.- gamma * Press) / VCF_ref;
    if( fabs(D_ref_old-D_ref_new) < MinDNS) break;
    D_ref_old=D_ref_new;

        if(flag_Slv !=0 )
               f_SlaveRTU();

        f_ctrl_v2();
  }

//  printf("\n\r f_get_ref_Dns, i= %d,D_ref=%g,VCF=%g",i,D_ref_new,VCF_ref);

  if(T_Ref == 15.)
      return D_ref_new;
  else
  {

    D_ref_new =  D_ref_new * f_get_VCF( T_Ref,D_ref_new) ;

      if(flag_Slv !=0 )
               f_SlaveRTU();

      f_ctrl_v2();

    return D_ref_new;
  }

}
/*----------------------------------------------------*/
/*----------------------------------------------------*/
float f_get_T(float D1,float D_ref, float T_ref)
{
// return Temperature for current D1 for known D_ref,T_ref

double TecTref;
double  dT;
float Temp;
double A,B,C,DD,VCF;
double D_ref_15;


        if(flag_Slv !=0 )
               f_SlaveRTU();

        f_ctrl_v2();

  if((type_liq > 6) || (type_liq < 1)) return -1000.;
  if(D_ref <=0) return -1000.;
  if(D1 <= 0) return -1000.;


  if(T_ref == 15.0)
       D_ref_15= D_ref;
  else
   {
       D_ref_15= f_get_ref_Dns(D_ref, T_ref , 0., 15.);
//     printf("\nD_ref_15=%f",D_ref_15);
   }

  VCF=D1/D_ref_15;

  if(type_liq != Kerosene)
     TecTref = (KK0[type_liq]+KK1[type_liq]* D_ref_15)/(D_ref_15*D_ref_15);
  else
     TecTref= Aa + Bb/(D_ref_15*D_ref_15);

        if(flag_Slv !=0 )
               f_SlaveRTU();

      f_ctrl_v2();

   A= -TecTref  * 0.8 * TecTref;
   B= -TecTref ;
   C= -log(VCF);

      if(flag_Slv !=0 )
               f_SlaveRTU();

      f_ctrl_v2();

  DD= (B*B-4.0*A*C);
  dT=(-B - sqrt(DD) )/(2.0*A);
  Temp = dT + 15.;

        if(flag_Slv !=0 )
               f_SlaveRTU();

      f_ctrl_v2();

  return Temp;
}
//----------------------------------------------------
double Mass_cmn=0;
double Dens_cmn=0;
float Dens_cmn_f=0;

void f_init_DNSA()
{

Mass_cmn=0;
Dens_cmn=0.0001;
//Dens_cmn_f=Dens_cmn;
}

//----------------
void f_cnt_DnsA()
{
// Вычисляет среднее значение приведенной плотности
// Данные D,Press, берутся из расходомера  T = f(EXT)

double dM,mass_c,d_ref;
float Temp_obs;

    if ((mode_temp==EXT) && (s_MVD[0].TempR > -49.5) )
    {
      Temp_obs=s_MVD[0].TempR;
    }
    else
      Temp_obs=s_MVD[0].Temp;

    if(flag_Slv !=0 )
               f_SlaveRTU();

      f_ctrl_v2();

    if( mode_refD == 1 )
        {
     if(Dens_cmn == 0) Dens_cmn= 0.001;
     mass_c=s_MVD[0].MassT;
     dM=fabs(mass_c-Mass_cmn);

     if(dM != 0)
         {
      d_ref=f_get_ref_Dns( s_MVD[0].Dens, Temp_obs,s_MVD[0].Press, T_Ref);
      Dens_cmn_f=d_ref;

      if((d_ref > 600)  && (d_ref < 1200))
        Dens_cmn=mass_c/((Mass_cmn/Dens_cmn) + (dM/d_ref));
      Mass_cmn=mass_c;
         }
     else if (mass_c == 0)
         {
          Dens_cmn=f_get_ref_Dns( s_MVD[0].Dens, Temp_obs,s_MVD[0].Press, T_Ref);
          Dens_cmn_f=Dens_cmn;
         }
     else
           Dens_cmn_f= (float)f_get_ref_Dns( s_MVD[0].Dens, Temp_obs,s_MVD[0].Press, T_Ref);
        }
    else
        {
         mass_c=s_MVD[0].MassT;
         Dens_cmn=f_get_ref_Dns( s_MVD[0].Dens, Temp_obs,s_MVD[0].Press, T_Ref);
         Dens_cmn_f=Dens_cmn;
        }

          if( flag_AD == 0)
          {
            D_ref=Dens_cmn;

            if(D_ref==0) D_ref=0.001;
            V_ref=s_MVD[0].MassT*1000./ D_ref;
          }

    if(flag_Slv !=0 )
               f_SlaveRTU();

      f_ctrl_v2();
}

/*----------------------------------------------------*/

