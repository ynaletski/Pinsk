

//f_tst(2);
//f_tst1(2);
//f_tst2(2);

void f_en_two_valv();
                  // Тип клапанов верхнего налива
int valve_mode=1; // 1 - два клапана с различными расходами
                  // 2 - регулируемый пилотный клапан NO_NC
                  // 3 - регулируемый клапан с электроприводом UP/DN.

int valve_modeL=0;// Тип клапанов нижнего налива
                  // 1 - два клапана с различными расходами
                  // 2 - регулируемый пилотный клапан NO_NC
                  // 3 - регулируемый клапан с электроприводом UP/DN.
int Flag_Low=0;

void f_reg_valve();
void (*ff_valve)() = f_two_valve;

void f_init_valve()
{
if( Flag_Low == 0)
 switch(valve_mode)
 {
  case 1 : ff_valve=f_two_valve; break;
  case 2 : ff_valve=f_reg_valve; break;
  case 3 : ff_valve=f_reg_valve; break;

  default: ff_valve=NULL; break;
 }
else
 switch(valve_modeL)
 {
  case 1 : ff_valve=f_two_valveL; break;
  case 2 : ff_valve=f_reg_valve; break;
  case 3 : ff_valve=f_reg_valve; break;

  default: ff_valve=NULL; break;
 }


}
//----------------------------------
void f_two_valve()
{
 // отсекание дозы при помощи двух (трех) клапанов с различными расходами

float ftmp,k_vol1,ftmp1;
int itmp,itmp1;
unsigned long int ltmp;
  // CLP0 - самый большой, (#define CLP0 msk_int[num_out7],AVT_CTRL.H )
  // OUT1 - средний клапан, им управляют в функции плотности для снижения
  // расхода
  // OUT2 - меньший клапан
  // OUT3 - включение насоса
  // vol_f - заданная доза отпуска

     VolT_int= f_int_v2();
     k_vol1=k_vol;

//  VolT_int= s_MVD[0].MassT;
//  summ_v_int=VolT_int*cost_v;
  ftmp=  vol_f-VolT_int;

// в ftmp сколько литров осталось

 itmp=0;
 if(fl_GO == 1)
 {
  if(FL_err)
  {
    flag_O3=0;
   if( flag_ext_t == 0)
   //   Delta_Ctrl[0]=STOP_FWD_DELTA;
      f_Drive_Stop();
    f_stop_cnt();
        f_reg_cmn(13);
    goto m_end;
  }
  TimeOut3=TimeStamp;


  if ( Flag_mass != 0 )
   {

    if(s_MVD[0].Dens > 500.)
    {
      ftmp1 = s_MVD[0].Dens*0.001;
      k_vol1 /= ftmp1;
      ftmp /= ftmp1;
    }
   }
   if(flag_O0)
    if( (ftmp-k_vol1*CL_T0 )> CL_val0) itmp |= CLP0;
    else flag_O0=0;

   if(flag_O1)
    if( (ftmp-k_vol1*CL_T1 )> CL_val[0]) itmp |= OUT1;
    else flag_O1=0;

   if( (ftmp-k_vol1*CL_T2 ) > CL_val[1]  ) itmp |= OUT2;

  if(flag_pool==1)
   if( (ftmp-k_vol1*CL_T3 )<= 0)
    {
     flag_pool=0;
     ltmp=TimeStamp+CL_T4;
     I7017C[0].time_stamp_pool=ltmp;
     I7060[0].time_stamp_pool=ltmp;
//   I7060[1].time_stamp_pool=ltmp;
//   MMI.time_stamp_pool=ltmp;
//   WDOG[0].time_stamp_pool=ltmp;
    }
  if(itmp == 0)
  {// завершение по достижении заданного объема
    vol_mom=VolT_int;
    vol_mom0=vol2;
    flow_mom=k_vol;

    flag_fill_ok=1;
    f_stop_cnt();
        f_reg_cmn(13);

    VolT_id=vol_f;
  }
  else
  {
     if(VolT_id< VolT_int)
             VolT_id= VolT_int;
     if(VolT_id>vol_f)  VolT_id=vol_f;
 //  summ_v_id=VolT_id*cost_v;
  }

//  if(flag_O0|flag_O1)
  {
    f_en_two_valv();
    itmp &= i_dens;
  }
 }  //  if(fl_GO == 1)

 if(flag_O3)
#if defined(BIO_1)
   if( f_timer(TimeOut3,(long int)(Out3_delay_off+T_Pmp_delay ) ))
#else
  #if defined(PMP_LG)
   if( f_timer(TimeOut3,Out3_delay_off+(long int)T_Pmp_LG*100 ) )
  #else
   if( f_timer(TimeOut3,Out3_delay_off ))
  #endif
#endif
  {
   flag_O3=0;
   if( flag_ext_t == 0)
   //  Delta_Ctrl[0]=STOP_FWD_DELTA;
      f_Drive_Stop();
  }
  else
  {
    itmp |= OUT3;
    if( flag_ext_t == 0)
   //    Delta_Ctrl[0]=START_FWD_DELTA;
      f_Drive_Run();
  }
 m_end:

// itmp |= lamp_c;

// i7060_out[0] = itmp;

 OUT_VAR &= ~(CLP0 | OUT1 | OUT2 | OUT3);
 OUT_VAR |= (itmp & Out_en);

 f_out();

 if(flag_prn)
 {
// if(io_1!=itmp)
   if(io_1!=OUT_VAR)
   {
    printf("\r\nOut= %02x,%ld ms",OUT_VAR,TimeStamp-time_beg_fill);
   }
   io_1=OUT_VAR;
 }

 if(I7060[0].status)  f_7060(0);
// if(I7060[1].status)  f_7060(1);

 ServiceCOM(I7060[0].port);
// ServiceCOM(I7060[1].port);
 return;
}
/* ---------------------------------------- */
void f_en_two_valv()
{// функция анализирует состояние плотности и формирует маску выключения
 // клапана CL1: i_dens=~OUT1; при необходимости уменьшения расхода отпуска
 float ftmp;
   i_dens=0xffff;
// ftmp=s_MVD[0].Press - s_MVD[0].PressG;

/*
 if( s_MVD[0].Press < P_min_valve)
 {
    i_dens &=~CLP0;
    i_dens &=~OUT1;
    i_dens &= ~OUT2;
    time_beg_fill=TimeStamp;
    return;
 }
*/

 if( f_timer(time_beg_fill,cl0_delay_off) == 0 )
  { // запретить включение CL0 первые cl0_delay_off мс после начала наполнения
    i_dens &=~CLP0;
    time_chg_den0=TimeStamp;
  }
/* ==========================
 else
  {
    if( f_timer(time_chg_den0,tim_dens0) != 0 )
    {
      switch(ModeReduce)
      {
       default:
       case ReduceDens:
               if( k_Den < (-k_Den0_Off) )
               {  // скорость уменьшения плотности превысила допустимый уровень
                   f_cl0_off();
               }
               else if( fabs(k_Den) <  k_Den0_On)
                   f_cl0_on();
            break;
       //---------------------
       case ReducePress:

            if( ftmp > PressLvl_on0 ) f_cl0_on();
            else if( ftmp < PressLvl_off0) f_cl0_off();
            break;
       //---------------------
      }
    }
  }
============*/

 if( f_timer(time_beg_fill,cl1_delay_off) == 0 )
  { // запретить включение CL1 первые cl1_delay_off мс после начала наполнения
    i_dens &=~OUT1;
    time_chg_den=TimeStamp;
  }
 else if(XdN < 0)
  { // запретить включение CL1 если нет запаса давления по кавитации
    i_dens &=~CLP0;
    i_dens &=~OUT1;
    time_chg_den=TimeStamp;
  }
/*
 if ( Flag_mass != 0 )
 {
  if( (s_MVD[0].MassI-MassStamp) < Value_beg)
  {
    i_dens &=~OUT1;
    time_chg_den=TimeStamp;
  }
 }
 else
*/
  if( (s_MVD[0].VolI-VolStamp) < Vol_beg)
  {
    i_dens &=~OUT1;
    i_dens &=~CLP0;
    time_chg_den=TimeStamp;
  }

/*========================
 else
  {
   if( f_timer(time_chg_den,tim_dens) != 0 )
   {
      switch(ModeReduce)
      {
       default:
       case ReduceDens:
               if( k_Den < (-k_Den_Off) )
               {  // скорость уменьшения плотности превысила допустимый уровень
                   f_cl1_off();
               }
               else if( fabs(k_Den) <  k_Den_On)
                   f_cl1_on();
            break;
       //---------------------
       case ReducePress:

            if( ftmp > PressLvl_on1 ) f_cl1_on();
            else if(( ftmp < PressLvl_off1) && (i_dens & OUT1) ) f_cl1_off();
            break;
       //---------------------
      }
   }
  }
===============*/

  if( f_timer(time_beg_fill,cl2_delay_off) ==0 )
  { // запретить включение CL2 первые cl21_delay_off мс после начала наполнения
    i_dens &= ~OUT2;
    time_chg_den2=TimeStamp;
    beg_flch=0;
  }

/*===============
  else
  {
      switch(ModeReduce)
      {
       default:
       case ReduceDens:
              f_cl2_on();
              break;
       //---------------------
       case ReducePress:
           if( ftmp > PressLvl_on2 )
              f_cl2_on();
           else if( ftmp < PressLvl_off2)
              f_cl2_off();
           break;
       //---------------------
      }
  }
===============*/
}
/* ---------------------------------------- */
void f_two_valveL()
{
 // отсекание дозы при помощи двух (трех) клапанов с различными расходами

float ftmp,k_vol1,ftmp1;
int itmp,itmp1;
unsigned long int ltmp;
  // CLP0 - самый большой, (#define CLP0 msk_int[num_out7],AVT_CTRL.H )
  // OUT5 - средний клапан, им управляют в функции плотности для снижения
  // расхода
  // OUT6 - меньший клапан
  // OUT3 - включение насоса
  // vol_f - заданная доза отпуска

     VolT_int= f_int_v2();

//  VolT_int= s_MVD[0].MassT;
//  summ_v_int=VolT_int*cost_v;
  ftmp=  vol_f-VolT_int;

// в ftmp сколько литров осталось

 itmp=0;
 if(fl_GO == 1)
 {
  if(FL_err)
  {
    flag_O3=0;
   if( flag_ext_t == 0)
   //   Delta_Ctrl[0]=STOP_FWD_DELTA;
      f_Drive_Stop();
    f_stop_cnt();
        f_reg_cmn(13);
    goto m_end;
  }
  TimeOut3=TimeStamp;

  k_vol1=k_vol;

  if ( Flag_mass != 0 )
   {

    if(s_MVD[0].Dens > 500.)
    {
      ftmp1 = s_MVD[0].Dens*0.001;
      k_vol1 /= ftmp1;
      ftmp /= ftmp1;
    }
   }
   if(flag_O0)
    if( (ftmp-k_vol1*CL_T0_L )> CL_val_L0) itmp |= CLP0;
    else flag_O0=0;

   if(flag_O1)
    if( (ftmp-k_vol1*CL_T1_L )> CL_val_L[0]) itmp |= OUT5;
    else flag_O1=0;


   if( (ftmp-k_vol1*CL_T2_L ) > CL_val_L[1]  ) itmp |= OUT6;

  if(flag_pool==1)
   if( (ftmp-k_vol1*CL_T3 )<= 0)
    {
     flag_pool=0;
     ltmp=TimeStamp+CL_T4;
     I7017C[0].time_stamp_pool=ltmp;
     I7060[0].time_stamp_pool=ltmp;
//   I7060[1].time_stamp_pool=ltmp;
//   MMI.time_stamp_pool=ltmp;
//   WDOG[0].time_stamp_pool=ltmp;
    }
  if(itmp == 0)
  {// завершение по достижении заданного объема
    vol_mom=VolT_int;
    vol_mom0=vol2;
    flow_mom=k_vol;

    flag_fill_ok=1;
    f_stop_cnt();
        f_reg_cmn(13);

    VolT_id=vol_f;
  }
  else
  {
     if(VolT_id< VolT_int)
             VolT_id= VolT_int;
     if(VolT_id>vol_f)  VolT_id=vol_f;
 //  summ_v_id=VolT_id*cost_v;
  }

//  if(flag_O0|flag_O1)
  {
    f_en_two_valvL();
    itmp &= i_dens;
  }
 }  //  if(fl_GO == 1)

 if(flag_O3)
#if defined(BIO_1)
   if( f_timer(TimeOut3,(long int)(Out3_delay_off+T_Pmp_delay ) ))
#else
  #if defined(PMP_LG)
   if( f_timer(TimeOut3,Out3_delay_off+(long int)T_Pmp_LG*100 ) )
  #else
   if( f_timer(TimeOut3,Out3_delay_off ))
  #endif
#endif

 {
   flag_O3=0;
   if( flag_ext_t == 0)
   //  Delta_Ctrl[0]=STOP_FWD_DELTA;
      f_Drive_Stop();
  }
  else
  {
    itmp |= OUT3;
    if( flag_ext_t == 0)
   //    Delta_Ctrl[0]=START_FWD_DELTA;
      f_Drive_Run();
  }
 m_end:

// itmp |= lamp_c;

// i7060_out[0] = itmp;

 OUT_VAR &= ~(CLP0 | OUT5 | OUT6 | OUT3);
 OUT_VAR |= (itmp & Out_en);

 f_out();

 if(flag_prn)
 {
// if(io_1!=itmp)
   if(io_1!=OUT_VAR)
   {
    printf("\r\nOut= %02x,%ld ms",OUT_VAR,TimeStamp-time_beg_fill);
   }
   io_1=OUT_VAR;
 }

 if(I7060[0].status)  f_7060(0);
// if(I7060[1].status)  f_7060(1);

 ServiceCOM(I7060[0].port);
// ServiceCOM(I7060[1].port);
 return;
}
/* ---------------------------------------- */
void f_en_two_valvL()
{// функция анализирует состояние плотности и формирует маску выключения
 // клапана CL1: i_dens=~OUT5; при необходимости уменьшения расхода отпуска
 float ftmp;
   i_dens=0xffff;
// ftmp=s_MVD[0].Press - s_MVD[0].PressG;

/*
 if( s_MVD[0].Press < P_min_valve)
 {
    i_dens &=~CLP0;
    i_dens &=~OUT5;
    i_dens &= ~OUT6;
    time_beg_fill=TimeStamp;
    return;
 }
*/
 if( f_timer(time_beg_fill,cl0_delay_off) == 0 )
  { // запретить включение CL0 первые cl0_delay_off мс после начала наполнения
    i_dens &=~CLP0;
    time_chg_den0=TimeStamp;
  }
/* ==========================
 else
  {
    if( f_timer(time_chg_den0,tim_dens0) != 0 )
    {
      switch(ModeReduce)
      {
       default:
       case ReduceDens:
               if( k_Den < (-k_Den0_Off) )
               {  // скорость уменьшения плотности превысила допустимый уровень
                   f_cl0_off();
               }
               else if( fabs(k_Den) <  k_Den0_On)
                   f_cl0_on();
            break;
       //---------------------
       case ReducePress:

            if( ftmp > PressLvl_on0 ) f_cl0_on();
            else if( ftmp < PressLvl_off0) f_cl0_off();
            break;
       //---------------------
      }
    }
  }
============*/

 if( f_timer(time_beg_fill,cl1_delay_off) == 0 )
  { // запретить включение CL1 первые cl1_delay_off мс после начала наполнения
    i_dens &=~OUT5;
    time_chg_den=TimeStamp;
  }
 else if(XdN < 0)
  { // запретить включение CL1 если нет запаса давления по кавитации
    i_dens &=~CLP0;
    i_dens &=~OUT5;
    time_chg_den=TimeStamp;
  }
/*
 if ( Flag_mass != 0 )
 {
  if( (s_MVD[0].MassI-MassStamp) < Value_beg)
  {
    i_dens &=~OUT5;
    time_chg_den=TimeStamp;
  }
 }
 else
*/
 /*
  if( (s_MVD[0].VolI-VolStamp) < Vol_beg)
  {
    i_dens &=~OUT5;
    i_dens &=~CLP0;
    time_chg_den=TimeStamp;
  }
*/
/*========================
 else
  {
   if( f_timer(time_chg_den,tim_dens) != 0 )
   {
      switch(ModeReduce)
      {
       default:
       case ReduceDens:
               if( k_Den < (-k_Den_Off) )
               {  // скорость уменьшения плотности превысила допустимый уровень
                   f_cl1_off();
               }
               else if( fabs(k_Den) <  k_Den_On)
                   f_cl1_on();
            break;
       //---------------------
       case ReducePress:

            if( ftmp > PressLvl_on1 ) f_cl1_on();
            else if(( ftmp < PressLvl_off1) && (i_dens & OUT5) ) f_cl1_off();
            break;
       //---------------------
      }
   }
  }
===============*/

  if( f_timer(time_beg_fill,cl2_delay_off) ==0 )
  { // запретить включение CL2 первые cl21_delay_off мс после начала наполнения
    i_dens &= ~OUT6;
    time_chg_den2=TimeStamp;
    beg_flch=0;
  }

/*===============
  else
  {
      switch(ModeReduce)
      {
       default:
       case ReduceDens:
              f_cl2_on();
              break;
       //---------------------
       case ReducePress:
           if( ftmp > PressLvl_on2 )
              f_cl2_on();
           else if( ftmp < PressLvl_off2)
              f_cl2_off();
           break;
       //---------------------
      }
  }
===============*/
}
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
void  f_en_reg_valv();

float   Vol_beg  =100;  // объем цистерны до уровня погружения рукава,л
float Flow_beg=10000;  // 3000, начальное значение расхода для заполнения цистерны до
                       // уровня рукава,кг/ч
float dFP_beg=300;


float Flow_min=3600;// 3600 расход при отсекании дозы,кг/ч
float    Mass_min=40;  // оставшаяся масса для снижения расхода до значения
                       // Flow_min - подготовка к отсеканию дозы
float dFP_min=200;

float Flow_min1=7200;  // 11000 расход при отсекании дозы,кг/ч
float    Mass_min1=120;    // оставшаяся масса для снижения расхода до значения
float    dFP_min1=400;

float Flow_min2=30000;  // 45000 расход при отсекании дозы,кг/ч
float    Mass_min2=450; // оставшаяся масса для снижения расхода до значения
float    dFP_min2=800;

float Flow_nom=100000;  // 20000, номинальный расход при наполнении,кг/ч
float dFP_nom=1000;


float Flow_minL =3600;  // 3600 расход при отсекании дозы,кг/ч
float Mass_minL =40;    // оставшаяся масса для снижения расхода до значения
                       // Flow_min - подготовка к отсеканию дозы
float dFP_minL  =200;
float Flow_min1L=7200;  // 11000 расход при отсекании дозы,кг/ч
float Mass_min1L=120;   // оставшаяся масса для снижения расхода до значения
float dFP_min1L =400;
float Flow_min2L=30000; // 45000 расход при отсекании дозы,кг/ч
float Mass_min2L=450;   // оставшаяся масса для снижения расхода до значения
float dFP_min2L =800;
float Flow_nomL =100000; // 20000, номинальный расход при наполнении,кг/ч
float dFP_nomL  =1000;




int itmp_vv=0;
void f_reg_valve()
{
 // отсекание дозы при помощи регулируемого клапана

float ftmp,k_vol1,ftmp1;

unsigned long int ltmp;
  //
  // OUT1 (OUT5) - пилот нормально открытый
  // OUT2 (OUT6) - пилот нормально закрытый
  //
  // OUT3 - включение насоса
  // vol_f - заданная доза отпуска

  // при достижении заданной дозы OUT1=0,OUT2=0;

     VolT_int= f_int_v2();
     k_vol1=k_vol;

//  VolT_int= s_MVD[0].MassT;
//  summ_v_int=VolT_int*cost_v;
  ftmp=  vol_f-VolT_int;

// в ftmp сколько литров осталось

 itmp_vv=0;
 if(fl_GO == 1)
 {
  if(FL_err)
  {
    flag_O3=0;
   if( flag_ext_t == 0)
   //   Delta_Ctrl[0]=STOP_FWD_DELTA;
      f_Drive_Stop();
    f_stop_cnt();
        f_reg_cmn(13);
    goto m_end;
  }

  TimeOut3=TimeStamp;

 if( Flag_Low == 0)
 {

   if ( (s_MVD[0].VolI-VolStamp) > Vol_beg)
   {
       Flow_dem = Flow_nom;
       dF_perm = dFP_nom;
   }
   else
   {
       Flow_dem = Flow_beg;
       dF_perm = dFP_beg;
   }
   if(ftmp < Mass_min)
        if(Flow_dem > Flow_min)
        {
          Flow_dem = Flow_min;
          dF_perm  = dFP_min;
        }
   if(ftmp < Mass_min1)
        if(Flow_dem > Flow_min1)
        {
          Flow_dem = Flow_min1;
          dF_perm  = dFP_min1;
        }
   if(ftmp < Mass_min2)
        if(Flow_dem > Flow_min2)
        {
           Flow_dem = Flow_min2;
           dF_perm  = dFP_min2;
        }
 }
 else // ( Flag_Low != 0)
 {
   Flow_dem = Flow_nomL;
   dF_perm = dFP_nomL;

   if(ftmp < Mass_minL)
        if(Flow_dem > Flow_minL)
        {
          Flow_dem = Flow_minL;
          dF_perm  = dFP_minL;
        }
   if(ftmp < Mass_min1L)
        if(Flow_dem > Flow_min1L)
        {
          Flow_dem = Flow_min1L;
          dF_perm  = dFP_min1L;
        }
   if(ftmp < Mass_min2L)
        if(Flow_dem > Flow_min2L)
        {
           Flow_dem = Flow_min2L;
           dF_perm  = dFP_min2L;
        }
 }

  if ( Flag_mass != 0 )
   {

    if(s_MVD[0].Dens > 500.)
    {
      ftmp1 = s_MVD[0].Dens*0.001;
      k_vol1 /= ftmp1;
      ftmp /= ftmp1;
    }
   }

if( Flag_Low == 0)
{
  if( (ftmp-k_vol1*CL_T2 )> CL_val[1]) itmp_vv |= OUT2 | OUT1;
/*
  else
  {
   if(flag_prn)
   {
     printf("\n\rftmp=%g,k_vol1=%g,CL_val[1]=%g",ftmp,k_vol1,CL_val[1]);

   }

  }
*/
}
else
{
  if( (ftmp-k_vol1*CL_T2_L )> CL_val_L[1]) itmp_vv |= OUT6 | OUT5;
/*
  else
  {
   if(flag_prn)
   {
     printf("\n\rftmp=%g,k_vol1=%g,CL_val[1]=%g",ftmp,k_vol1,CL_val[1]);

   }

  }
*/
}
  if(flag_pool==1)
   if( (ftmp-k_vol1*CL_T3 )<= 0)
    {
     flag_pool=0;
     ltmp=TimeStamp+CL_T4;
     I7017C[0].time_stamp_pool=ltmp;
//   I7060[0].time_stamp_pool=ltmp;
//   I7060[1].time_stamp_pool=ltmp;
//   MMI.time_stamp_pool=ltmp;
//   WDOG[0].time_stamp_pool=ltmp;
    }


  if(itmp_vv == 0)
  {// завершение по достижении заданного объема
    vol_mom=VolT_int;
    vol_mom0=vol2;
    flow_mom=k_vol;

    flag_fill_ok=1;
    f_stop_cnt();
        f_reg_cmn(13);

    VolT_id=vol_f;
  }
  else
  {
     if(VolT_id< VolT_int)
             VolT_id= VolT_int;
     if(VolT_id>vol_f)  VolT_id=vol_f;
  }

  if(fl_GO == 1)
  {
      f_en_reg_valv();
      itmp_vv &= i_dens;
  }
 }  //  if(fl_GO == 1)

 if(flag_O3)

#if defined(BIO_1)
   if( f_timer(TimeOut3,(long int)(Out3_delay_off+T_Pmp_delay ) ))
#else
  #if defined(PMP_LG)
   if( f_timer(TimeOut3,Out3_delay_off+(long int)T_Pmp_LG*100 ) )
  #else
   if( f_timer(TimeOut3,Out3_delay_off ))
  #endif
#endif

   {
   flag_O3=0;
   if( flag_ext_t == 0)
   //  Delta_Ctrl[0]=STOP_FWD_DELTA;
      f_Drive_Stop();
   }
   else
   {
    itmp_vv |= OUT3;
    if( flag_ext_t == 0)
   //    Delta_Ctrl[0]=START_FWD_DELTA;
      f_Drive_Run();

   }
 m_end:

// itmp |= lamp_c;
// i7060_out[0] = itmp;

 OUT_VAR &= ~( OUT1 | OUT2 | OUT5 | OUT6 | OUT3);
 OUT_VAR |= (itmp_vv & Out_en);

 f_out();

 if(flag_prn)
 {
   if(io_1!=OUT_VAR)
   {
    printf("\r\nOut= %02x,%ld ms",OUT_VAR,TimeStamp-time_beg_fill);
   }
   io_1=OUT_VAR;
 }

 if(I7060[0].status)  f_7060(0);
// if(I7060[1].status)  f_7060(1);

 ServiceCOM(I7060[0].port);
// ServiceCOM(I7060[1].port);
 return;
}
/* ---------------------------------------- */
float P_min_valve=0.05; // МПа, минимальное давление при котором возможно увеличить степень открытия вентиля
float P_min_valveL=0.05; // МПа, минимальное давление при котором возможно увеличить степень открытия вентиля
int State_UpDn=HOLD;

void f_UP_DN();

void  f_en_reg_valv()
{
 // функция формирует актуальные значения для OUT1,OUT2 в виде маски выключения
 // на основании циклограммы работы и заданного значения
 // требуемой величины открытия вентиля Hop  и текущего состояния вентиля
 // Hop_cur

   i_dens=0xffff;

  if( f_timer(time_beg_fill,cl2_delay_off) ==0 )
  { // запретить включение CL2 первые cl21_delay_off мс после начала наполнения
//  i_dens &= ~(OUT1|OUT2);
    time_chg_den2=TimeStamp;
    State_UpDn = DN ;
    beg_flch=0;
//  return;
  }
  else f_UP_DN(); // вычисляет состояние вентиля

   // valve_mode :            1     ;            2             ;          3
   // OUT1       : больший клапан   ; пилот нормально открытый ;  /DN - закрыть задвижку (OUT1=0)
   // OUT2       : меньший клапан   ; пилот нормально закрытый ;   UP - открыть задвижку (OUT2=1)

 if( Flag_Low == 0)
 {
  if(valve_mode==2)
  {   // верхний налив
    if(State_UpDn == HOLD)  i_dens &= ~(OUT2);            // нормальноотокрытый включен
    else if(State_UpDn == UP ) ;                          // оба включены
    else if(State_UpDn == DN )   i_dens &= ~(OUT1|OUT2);  // оба выключены
  }
  else if(valve_mode==3)
  {
    if(State_UpDn == HOLD)  i_dens &= ~(OUT2);     // оба OUT1=1,OUT2=0 - оба контактора выключены
    else if(State_UpDn == UP ) ;                        //  OUT1=1,OUT2=1      контактор "ОТКР"=1, "ЗАКР"=0
    else if(State_UpDn == DN ) i_dens &= ~(OUT1| OUT2); //  OUT1=0,OUT2=0      контактор "ОТКР"=0, "ЗАКР"=1
  }
 }
 else
 {    // нижний налив
  if(valve_mode==2)
  {
    if(State_UpDn == HOLD)  i_dens &= ~(OUT6);            // нормальноотокрытый включен
    else if(State_UpDn == UP ) ;                          // оба включены
    else if(State_UpDn == DN )   i_dens &= ~(OUT5|OUT6);  // оба выключены
  }
  else if(valve_mode==3)
  {
    if(State_UpDn == HOLD)  i_dens &= ~(OUT6);     // оба OUT5=1,OUT6=0 - оба контактора выключены
    else if(State_UpDn == UP ) ;                        //  OUT5=1,OUT6=1      контактор "ОТКР"=1, "ЗАКР"=0
    else if(State_UpDn == DN ) i_dens &= ~(OUT5| OUT6); //  OUT5=0,OUT6=0      контактор "ОТКР"=0, "ЗАКР"=1
  }
 }

}
/* ---------------------------------------- */
long int Per_up_min_v=50;
long int Per_up_max_v=400;
long int Per_dn_min_v=50;
long int Per_dn_max_v=400;
long int Tim_hld=400;

long int Per_UP_dl=30; // время включения клапана UP
long int Per_DN_dl=30; // время включения клапана DN

long int Per_up_min_vL=50;
long int Per_up_max_vL=400;
long int Per_dn_min_vL=50;
long int Per_dn_max_vL=400;
long int Tim_hldL     =400;
long int Per_UP_dlL   =30; // время включения клапана UP
long int Per_DN_dlL   =30; // время включения клапана DN

long int Tim_up=400;
long int Tim_dn=400;
long int time_ch_v=0;

void f_UP_DN()
{
 // вычисляет состояние вентиля на основании
 // требуемой величины открытия вентиля Hop, текущего состояния вентиля
 // Hop_cur и текущей стадии вентиля
 long int ltmp;

if( Flag_Low == 0)
  switch(State_UpDn)
  {
   case HOLD:
    {
      if( f_timer(time_ch_v,Tim_hld) == 0 ) break;
      ltmp= Hop - Hop_cur;
      if(ltmp > Per_up_min_v)
       { // UP
         if( s_MVD[0].Press < P_min_valve)
         {
             Hop= Hop_cur;
             YfN=Hop<<n_shft;
             XfN_2  = 0;
             XfN_1  = 0;
             break;
         }
         State_UpDn=UP;
         time_ch_v=TimeStamp;
         if(ltmp > Per_up_max_v)
         {
           ltmp = Per_up_max_v;
           Hop_cur+=ltmp;
           Hop= Hop_cur;
           YfN=Hop<<n_shft;
           XfN_2  = 0;
           XfN_1  = 0;
         }
         else
         {
           Hop_cur+=ltmp;
         }
           Tim_up=ltmp+Per_UP_dl;

       }
      else if(ltmp < -Per_dn_min_v)
       { // DN
         State_UpDn=DN;
         time_ch_v=TimeStamp;

         if(ltmp < -Per_dn_max_v)
         {
           ltmp = -Per_up_max_v;
           Hop_cur+=ltmp;
           Hop= Hop_cur;
           YfN=Hop<<n_shft;
           XfN_2  = 0;
           XfN_1  = 0;
         }
         else
         {
          Hop_cur+=ltmp;
         }
         Tim_dn=Per_DN_dl-ltmp;
       }
     break;
    }
   case UP  :
    {
     if( f_timer(time_ch_v,Tim_up) == 0 ) break;
     State_UpDn=HOLD;
     time_ch_v=TimeStamp;
     break;
    }

   case DN  :
    {
     if( f_timer(time_ch_v,Tim_dn) == 0 ) break;
     State_UpDn=HOLD;
     time_ch_v=TimeStamp;
     break;
    }

   default:
         State_UpDn=HOLD;
         time_ch_v=TimeStamp;
  }
 else
  switch(State_UpDn)
  {
   case HOLD:
    {
      if( f_timer(time_ch_v,Tim_hldL) == 0 ) break;
      ltmp= Hop - Hop_cur;
      if(ltmp > Per_up_min_vL)
       { // UP
         if( s_MVD[0].Press < P_min_valveL)
         {
             Hop= Hop_cur;
             YfN=Hop<<n_shft;
             XfN_2  = 0;
             XfN_1  = 0;
             break;
         }
         State_UpDn=UP;
         time_ch_v=TimeStamp;
         if(ltmp > Per_up_max_vL)
         {
           ltmp = Per_up_max_vL;
           Hop_cur+=ltmp;
           Hop= Hop_cur;
           YfN=Hop<<n_shft;
           XfN_2  = 0;
           XfN_1  = 0;
         }
         else
         {
           Hop_cur+=ltmp;
         }
           Tim_up=ltmp+Per_UP_dlL;

       }
      else if(ltmp < -Per_dn_min_vL)
       { // DN
         State_UpDn=DN;
         time_ch_v=TimeStamp;

         if(ltmp < -Per_dn_max_vL)
         {
           ltmp = -Per_up_max_vL;
           Hop_cur+=ltmp;
           Hop= Hop_cur;
           YfN=Hop<<n_shft;
           XfN_2  = 0;
           XfN_1  = 0;
         }
         else
         {
          Hop_cur+=ltmp;
         }
         Tim_dn=Per_DN_dlL-ltmp;
       }
     break;
    }
   case UP  :
    {
     if( f_timer(time_ch_v,Tim_up) == 0 ) break;
     State_UpDn=HOLD;
     time_ch_v=TimeStamp;
     break;
    }

   case DN  :
    {
     if( f_timer(time_ch_v,Tim_dn) == 0 ) break;
     State_UpDn=HOLD;
     time_ch_v=TimeStamp;
     break;
    }

   default:
         State_UpDn=HOLD;
         time_ch_v=TimeStamp;
  }
}
/* ---------------------------------------- */


