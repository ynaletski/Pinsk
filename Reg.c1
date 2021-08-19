
//#define P_real   s_MVD[0].Press
#define YN_OUT   DriveSV[0]


int mode_r=0; // тип регулятора :
              //  0 - стабилизатор давления ,  P_dem - заданное значение
              //  1 - стабилизатор избыточного давления ,
              //   P_Dif_d - заданное значение запаса давления

int flag_flow=0;
 // stage             0    1    2      3      4

float Flow_on[5] ={ 1000, 800, 1200 , 2000 , 2200 }; // kg/h
float Flow_off[5]={ 1000, 400, 600  , 1000 , 1200 }; // kg/h


long int P_dem=0.8*K_F_INT;
long int P_dem_c=0.8*K_F_INT;
long int f_Beg=200; // Гц

long int k_p=10,k_i=15,k_d=0; // коэффициенты ПИД для регулятора давления
long int maxP_errP= 0.2 * K_F_INT, maxP_errM = 0.2 * K_F_INT;
long int LIM_P=(long int)(5000L*k_div) ,LIM_M = (long int)(150L*k_div); // предельные частоты привода
long int LIM_P_c=0;

long int K0,K1,K2,K3;
long int YN=0,XN=0,XN_1=0,XN_2=0;


float P_Dif_d=0.2; // МПА
long int kd_p=10,kd_i=5,kd_d=0; // коэффициенты ПИД для регулятора
long int maxPd_errP= 0.2 * K_F_INT, maxPd_errM = 0.2 * K_F_INT;
long int LIMd_P=(long int) (1.6 * K_F_INT*k_div) ,LIMd_M = (long int) (0.6 * K_F_INT*k_div); // предельные давления на выходе насоса, МПа
long int Kd0,Kd1,Kd2,Kd3;
long int YdN=0,XdN=0,XdN_1=0,XdN_2=0;


//#define I_real  Delta_current[0]
// #define  YiN  LIM_P_c , см. reg.h

long int I_dem = 130;
long int ki_p=1000,ki_i=100,ki_d=0; // коэффициенты ПИД для регулятора предела оборотов насоса
long int maxPi_errP= 30, maxPi_errM = 30;
long int LIMi_P=(long int)(5000L*k_div) ,LIMi_M = (long int)(1500L*k_div); // предельные частоты на выходе регулятора предела оборотов насоса
long int Ki0,Ki1,Ki2,Ki3;
long int XiN=0,XiN_1=0,XiN_2=0;
//long int YiN=0;
//---------------------------------
void f_init_reg_P()
{
 YN= f_Beg * k_div;
 XN=0;
 XN_1=0;
 XN_2=0;
 YN_OUT= YN >> n_shft;
 LIM_P_c=LIM_P;

 P_dem_c=P_dem;

// YiN = LIM_P_c;
 XiN=0;
 XiN_1=0;
 XiN_2=0;

}
//---------------------------------
long int tl_reg_b=0;
long int tl_reg_e=0;
long int tl_reg_res=0;
int t_reg_res=0;
int flg_prn_reg=0;

void f_reg_P()
{
// Регулятор давления - изменяет частоту вращения
// двигателя насоса для поддержания
// заданного давления P_dem_c на выходе насоса.
// запускается при приеме значения давления из датчика давления ( см. 7017c.c)

  tl_reg_b=TimeStamp;
  tim1();

 if(flag_O3==0) return;
 if( (mode_r & Mask_reg_p) ==0 )
 {
  YN= LIM_P; // максимальнодопустимая частота, p3.LP
  XN=0;
  XN_1=0;
  XN_2=0;
  YN_OUT= YN >> n_shft;
 }
 else
 {
   P_dem_c=P_dem; //
   XN = P_dem_c - (long int)(s_MVD[0].Press  * K_F_INT);

   if( XN  >  maxP_errP)
   {
      XN =  maxP_errP;
   }
   else if( XN  <  (-maxP_errM) )
   {
      XN = -maxP_errM;
   }

   YN = YN *K0 +K1 *XN-K2 *XN_1 +K3 *XN_2 ;

   if(YN >  LIM_P_c )
      YN = LIM_P_c;
   else  if(YN <  LIM_M )
      YN = LIM_M;

   XN_2  = XN_1 ;
   XN_1  = XN;

   YN_OUT= YN >> n_shft;


  tl_reg_e=TimeStamp;
  tim2();
 }

 if(flg_prn_reg)
  {
    flg_prn_reg=0;
    tl_reg_res=tl_reg_e-tl_reg_b;
    t_reg_res=ttt2-ttt1-10;  // 10 следует отнять - это время присутствует
    if(t_reg_res < 0) t_reg_res+=10000;
    printf("\n\r f_reg_P() time: %ld ms +  %f mks\n\r>",tl_reg_res,(float)(t_reg_res)/10.);
  }

}
//-----------------------

void f_reg_D()
{
 // Вычисляет максимальнодопустимый расход flow_D из условия незакипания
 // жидкости в расходомере.
 // Запускается при приеме значения расхода из расходомера ( см. mvd.c)

if(flag_O3==0) return;
if( (mode_r & Mask_reg_d)==0)
{
m1:
 YdN = LIMd_P;
 flow_D = YdN >> n_shft;
 XdN=0;
 XdN_1=0;
 XdN_2=0;
 flag_flow=0;
 return;
}

   s_MVD[0].PressD = s_MVD[0].Press - s_MVD[0].PressG - s_MVD[0].PressB;
   XdN = (long int) ((  s_MVD[0].PressD - P_Dif_d ) * (float)K_F_INT );

   if( XdN  >  maxPd_errP)
   {
      XdN =  maxPd_errP;
   }
   else if( XdN  <  (-maxPd_errM) )
   {
      XdN = -maxPd_errM;
   }

   YdN = YdN *Kd0 +Kd1 *XdN-Kd2 *XdN_1 +Kd3 *XdN_2 ;

   if(YdN >  LIMd_P )
      YdN = LIMd_P;
   else  if(YdN <  LIMd_M )
      YdN = LIMd_M;

   XdN_2  = XdN_1 ;
   XdN_1  = XdN;
   flow_D = YdN >> n_shft;
}
//-----------------------
int mode_i=0;
void f_reg_I()
{
// регулятор максимальных оборотов насоса -
// формирует предельнодопустимое значение частоты вращения насоса
// (для регулятора давления) исходя из текущего значения тока в
//   двигателе насоса
// запускается при приеме значения тока из привода ( см. sk.c,delta.c)


if(flag_O3==0) return;
if(mode_i==0)
{
m1:
 YiN = LIM_P;
 XiN=0;
 XiN_1=0;
 XiN_2=0;
 return;
}

   XiN = I_dem - (long int)(DriveCurrent[0]);

   if( XiN  >  maxPi_errP)
   {
      XiN =  maxPi_errP;
   }
   else if( XiN  <  (-maxPi_errM) )
   {
      XN = -maxP_errM;
   }

   YiN = YiN *Ki0 +Ki1 *XiN-Ki2 *XiN_1 +Ki3 *XiN_2 ;

   if(YiN >  LIMi_P )
      YiN = LIMi_P;
   else  if(YiN <  LIMi_M )
      YiN = LIMi_M;

   XiN_2  = XiN_1 ;
   XiN_1  = XiN;
}
//-----------------------
long int Flow_dem=-1; // 0...100000  заданное значение потока,кг/ч
                      // Flow_dem < 0 - регулятор расхода отключен
long int Hop=0;       // 0...20000   заданное значение степени открытия
                      // вентиля,регулирующего расход, мсек,
                      // суммарное время открывания

long int Hop_cur=0;   // 0...20000   текущее значение степени открытия
                      // вентиля,регулирующего расход, мсек
                      // суммарное время открывания

long int kf_p=5,kf_i=2,kf_d=0; // коэффициенты для ПИД регулятора
long int Kf0,Kf1,Kf2,Kf3;
long int YfN=0,XfN=0,XfN_1=0,XfN_2=0;
long int dF_perm = 50;

long int  LIM_FLOW_P=(long int)(20000L*k_div);
long int  LIM_FLOW_M=0;
long int  maxF_errP=2000;
long int  maxF_errM=2000;


//-----------------------
#define Min_Dens 0.65
#define Max_Dens 1.2
float Value_beg=200;

void f_init_reg_F()
{  // вызывается в f_enable_cnt()
 float ftmp;
 YfN= 0;
 Hop=0;
 Hop_cur=0;
 XfN=0;
 XfN_1=0;
 XfN_2=0;
 Flow_dem=Flow_beg;
 ftmp=s_MVD[0].Dens/1000.;

 if(ftmp < Min_Dens) ftmp=Min_Dens;
 else if(ftmp > Max_Dens) ftmp=Max_Dens;
 Value_beg= Vol_beg*ftmp;

// init_reg_d

 YdN = LIMd_P;
 flow_D = YdN >> n_shft;
 XdN=0;
 XdN_1=0;
 XdN_2=0;
}
//---------------------------------
long int flow_D=100000; // максимальнодопустимый расход из условия незакипания в расходомере
//-----------------------

void f_reg_F()
{
   // Регулятор расхода.
   // Формирует заданное значение H_op для вентиля управления расходом
   // исходя из заданного значения  расхода Flow_dem
   // запускается при приеме значения расхода s_MVD[0].FlowM , кг/ч
   //  из расходомера ( см. mvd.c)
   //
   float ltmp,fl_dem;
   float ftmp;
if(flag_O3==0) return;
if(valve_mode < 2) return;

   if(s_MVD[0].FlowM<0) ltmp=0;
   else ltmp=s_MVD[0].FlowM;

   fl_dem=Flow_dem;
// if(fl_dem > flow_D) fl_dem=flow_D;

   ftmp = (fl_dem - ltmp);

   if( ftmp >  maxF_errP)
   {
      ftmp=  maxF_errP;
   }
   else if( ftmp <  (-maxF_errM) )
   {
      ftmp= -maxF_errM;
   }
   else
    if(fabs(ftmp) < dF_perm) ftmp=0;

// XfN=ftmp*k_div;
   XfN=ftmp;

   YfN = YfN *Kf0 +Kf1 *XfN-Kf2 *XfN_1 +Kf3 *XfN_2 ;

   if(YfN >  LIM_FLOW_P)
   {
      YfN = LIM_FLOW_P;
   /*
      YfN-= Add_lim;
      Hop_cur-=Add_lim_h;
   */
   /*
      YfN     = Add_lim;
      Hop_cur = Add_lim_h;
      XfN_2  = 0;
      XfN_1  = 0;
   */
   }
   else  if(YfN <  LIM_FLOW_M )
   {
 //   YfN = LIM_FLOW_M;
   /*
      YfN += Add_lim;
      Hop_cur +=Add_lim_h;
   */

      YfN  = Add_lim;
      Hop_cur = Add_lim_h;
      XfN_2  = 0;
      XfN_1  = 0;
   }

   XfN_2  = XfN_1 ;
   XfN_1  = XfN;

   Hop= YfN >> n_shft;
/*
   if(XfN> 0) if(Hop< Hop_cur)
   {
     Hop= Hop_cur;
     YfN=Hop<<n_shft;
     XfN_2  = 0;
     XfN_1  = 0;
   }
   else if(XfN < 0) if(Hop > Hop_cur)
   {
     Hop= Hop_cur;
     YfN=Hop<<n_shft;
     XfN_2  = 0;
     XfN_1  = 0;
   }
 */
}
//-----------------------
//-----------------------

void f_count_cf()
{ /* Пересчет ПИД коэфф-в в коэфф-ты ур-ий в конечных разностях */
 /*
    Y = Kp*x + Ki*Sxdt + Kd*dx/dt
    K0 =  1.0
    K1 =  Kp+Ki/2+Kd
    K2 =  Kp-Ki/2+2*Kd
    K3 =  Kd
    Y(n) =  K0 * Y(n-1) + K1*x(n) - K2*x(n-1) + K3*x(n-2)
 */
        if(k_i ==0.)
         {
           K0  =  0.;
           K1  =  (k_p +k_d );
           K2  =  k_d ;
           K3  =  0.;
         }
        else
         {
           K0  =  1.0;
           K1  =  (k_p  + k_i /2. +    k_d ) ;
           K2  =  (k_p  - k_i /2. + 2.*k_d ) ;
           K3  =  k_d ;
         }

        if(kd_i ==0.)
         {
           Kd0  =  0.;
           Kd1  =  (kd_p +kd_d );
           Kd2  =  kd_d ;
           Kd3  =  0.;
         }
        else
         {
           Kd0  =  1.0;
           Kd1  =  (kd_p  + kd_i /2. +    kd_d ) ;
           Kd2  =  (kd_p  - kd_i /2. + 2.*kd_d ) ;
           Kd3  =  kd_d ;
         }

        if(ki_i ==0.)
         {
           Ki0  =  0.;
           Ki1  =  (ki_p +ki_d );
           Ki2  =  ki_d ;
           Ki3  =  0.;
         }
        else
         {
           Ki0  =  1.0;
           Ki1  =  (ki_p  + ki_i /2. +    ki_d ) ;
           Ki2  =  (ki_p  - ki_i /2. + 2.*ki_d ) ;
           Ki3  =  ki_d ;
         }

        if(kf_i ==0.)
         {
           Kf0  =  0.;
           Kf1  =  (kf_p +kf_d );
           Kf2  =  kf_d ;
           Kf3  =  0.;
         }
        else
         {
           Kf0  =  1.0;
           Kf1  =  (kf_p  + kf_i /2. +    kf_d ) ;
           Kf2  =  (kf_p  - kf_i /2. + 2.*kf_d ) ;
           Kf3  =  kf_d ;
         }

}

