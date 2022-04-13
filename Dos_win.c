
//#define ZeroPage 0
//#define ZeroPage 6


//   f_reg_cmn(0);

#define n_mmi_str 7 // количество строк на дисплее MMI
#define n_mmi_str_1 (n_mmi_str-1)
#define n_mmi_str_2 (n_mmi_str-2)
#define n_mmi_str_3 (n_mmi_str-3)
#define n_mmi_str_4 (n_mmi_str-4)
float ftmp_DP=1;
int mode_hst = 0;
//
//  Файл содержит символьные данные в DOS кодировке символов.
//  Для отображения на дисплее MMI необходимо перекодировать в
//  кодировку Windows (выполняется командой dos_win.exe в .bat)
//

char str_prod[22]=" -------- ";
char str_dsp1[31]="";
char str_dsp2[31]="";
char str_dsp3[31]="";
char str_dsp4[31]="";
char str_dsp5[31]="";
char str_dsp6[31]="";
char str_dsp7[31]="";

int type_dlv=0;
int sect_num=0;
int sect_mass=0;
int sect_vol=0;
int sect_rslt=0;

float MassStamp=-1000.;
float VolStamp=-1000.;

float vBIG_P=BIG_P;
long int tm_md=0;

int flag_prok=0;
char mode_prok[9] ="Отпуск";
char otpusk[9]  ="Отпуск  ";
char  prok_ka[9]   ="Прокачка";
char str_empt[]="                               ";

long int fl_zeroing=0;
#define Time_zeroing (50000)
long int zero_time;

char s_empty[]="                              ";
char str_ESC[]="ESC  Возврат";

void f_d_ESC()
{
  MmiGotoxy(0,6); MmiPuts(str_ESC);
}

int PageD=0;
int flg_ch=0;
int Flag_f_corr=0,flag_mass=0,pass_ret=0;
float k_v_flow=1,k_m_flow=1;
float k_dens=1.;

float k_mv_tmp;
long int pass1=0,pass2=0;

#if defined(BIO_1)
  float P_inp=0;
#endif

long int n_id_a=0;

void f_prn_ID();
int flag_esc=1;
void  f_prn_begin()
{
  if(flag_esc == 0) goto m1;

  if( id_ok==0) f_prn_ID();
  else if(CRC_err) f_prn_CRC_error();
  else if(FL_err) f_prn_error();
  else
  {
 m1:
    f_clr_scr_MMI();
            //  MmiPrintf("                              ");

#if defined(BIO_1)
  MmiGotoxy(1,1);  MmiPrintf("  Система учета отпуска  ");
  MmiGotoxy(1,2);  MmiPrintf("      биокомпонента      ");
#else
  MmiGotoxy(1,1);  MmiPrintf("  Система учета отпуска  ");
  MmiGotoxy(1,2);  MmiPrintf("      нефтепродуктов     ");
#endif


            //             "  Показания счетчика          "

#if defined(BIO_1)
          if(PageD==0)
          {
  MmiGotoxy(0,3) ; MmiPrintf("Масса       :                ");
  MmiGotoxy(0,4) ; MmiPrintf("Объем       :                ");
  MmiGotoxy(0,5) ; MmiPrintf("Pвх =            ");
  MmiGotoxy(15,5); MmiPrintf("Pвых=            ");
          }
#else
          if(PageD==0)
          {
  MmiGotoxy(0,3) ; MmiPrintf("Масса       :                ");
  MmiGotoxy(0,4) ; MmiPrintf("Объем       :                ");
  MmiGotoxy(0,5) ; MmiPrintf("P=            ");
  MmiGotoxy(15,5); MmiPrintf("T=            ");
          }



#endif

          else if(PageD==1)
          {


          }
          else
          {
  MmiGotoxy(0,3) ; MmiPrintf("PageD = %d",PageD);

          }

  }
}
//-------------------------------------
void f_prn_ID()
{
    f_clr_scr_MMI();
            //  MmiPrintf("                              ");

#if defined(BIO_1)
  MmiGotoxy(1,1);  MmiPrintf("  Система учета отпуска  ");
  MmiGotoxy(1,2);  MmiPrintf("      биокомпонента      ");
#else
  MmiGotoxy(1,1);  MmiPrintf("  Система учета отпуска  ");
  MmiGotoxy(1,2);  MmiPrintf("      нефтепродуктов     ");
#endif


  MmiGotoxy(0,3) ; MmiPrintf("!Не корректен ID=%ld ",n_id);
  MmiGotoxy(0,4) ; MmiPrintf(" SN %08lX %08lX",*( unsigned long int *)(ID_ADDR),*( unsigned long int *)(ID_ADDR+4) );
  MmiGotoxy(0,5) ; MmiPrintf("Обратитесь к поставщику за ID ");
  MmiGotoxy(0,6) ; MmiPrintf(" F2 - ввод ID ");
}
//-------------------------------------
void f_prn_slv1()
{

        SetDisplayPage(EmptPage);
        f_clr_scr_MMI();

        MmiGotoxy(0,0);
        if(dose_dlvr != vBIG_P)
         {
          if ( Flag_mass != 0 )
            MmiPrintf(" Отпуск  %g кг",dose_dlvr);
          else
            MmiPrintf(" Отпуск  %g л ",dose_dlvr);
         }
        else
           MmiPrintf(" Отпуск без дозы");

       f_prn_mode();
       MmiGotoxy(0,6);  MmiPuts("ESC - СТОП");

}
//-------------------------------------

//--------------------
void f_prn_no_sig_L( int row)
{
int i;

    if(valve_modeL == 0) return;
    if( (INP_VARi0  & INP_MSK_OK_L) == INP_MSK_OK_L ) return;

  MmiGotoxy(0,row); MmiPuts( str_empt );

  MmiGotoxy(0,row);
    if( valve_mode != 0)
    {
      MmiPuts("Нижн. нет:");
      i=10;
    }
    else
    {
          MmiPuts("Нет:");
      i=4;
    }

              if( LVL_IN_L==0)
              {
                MmiGotoxy(i,row); MmiPuts(" Уровень");
               i+=9;

               if(INP_OFP & 0x4000)
                {
                i+=2;
                MmiPuts(" ");
 if((INP_OFP & 1)==0){ MmiPuts("1"); i++; }
 if((INP_OFP & 2)==0){ MmiPuts("2"); i++; }
 if((INP_OFP & 4)==0){ MmiPuts("3"); i++; }
 if((INP_OFP & 8)==0){ MmiPuts("4"); i++; }
 if((INP_OFP & 0x10)==0){ MmiPuts("5"); i++; }
 if((INP_OFP & 0x20)==0){ MmiPuts("6"); i++; }
 if((INP_OFP & 0x100)==0){ MmiPuts("7"); i++; }
 if((INP_OFP & 0x200)==0){ MmiPuts("8"); i++; }
                MmiPuts(" ");
                }
              }
              if( UZA_IN_L==0)
              {
               if((i+4)< 30)
                {
                 MmiGotoxy(i,row); MmiPuts(" УЗА");
                 i+=4;
                }
              }

    if( valve_mode != 0)
    {
              if( ES_IN_L==0)
              {
               if((i+11)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" АВ.КН.НИЖН");
                 i+=11;
               }
              }

          if( Flag_Low != 0)
              if( ES_IN==0)
              {
               if((i+12)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" АВ.КН.ВЕРХН");
                 i+=12;
               }
              }
    }
    else
    {
              if( ES_IN_L==0)
              {
               if((i+12)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" АВАР.КНОПКА");
                 i+=12;
               }
              }
    }


}
//--------------------
void f_prn_no_sig_H( int row)
{
int i;

    if( valve_mode == 0) return;

    if( (INP_VARi0  & INP_MSK_OK) == INP_MSK_OK ) return;


     MmiGotoxy(0,row); MmiPuts( str_empt );
              MmiGotoxy(0,row);


    if( valve_modeL != 0)
    {
      MmiPuts("Верхн.нет:");
      i=10;
    }
    else
    {
      MmiPuts("Нет:");
      i=4;
    }

              if( TRAP_IN==0)
              {
               if((i+5)< 30)
               {
                 MmiGotoxy(i,row); MmiPuts(" ТРАП");
                 i+=5;
               }
              }

              if( CONS_IN==0)
              {
               if((i+8)< 30)
               {
                 MmiGotoxy(i,row); MmiPuts(" КОНСОЛЬ");
                 i+=8;
               }
              }

              if( LVL_IN==0)
              {
               if((i+8)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" Уровень");
                i+=8;
               }
              }
              if( UZA_IN==0)
              {
               if((i+4)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" УЗА");
                i+=4;
               }
              }

    if( valve_modeL != 0)
    {
              if( ES_IN==0)
              {
               if((i+12)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" АВ.КН.ВЕРХН");
                 i+=12;
               }
              }

          if( Flag_Low == 0)
              if( ES_IN_L==0)
              {
               if((i+11)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" АВ.КН.НИЖН");
                 i+=11;
               }
              }
    }
    else
    {
              if( ES_IN==0)
              {
               if((i+12)< 30)
               {
                MmiGotoxy(i,row); MmiPuts(" АВАР.КНОПКА");
                 i+=12;
               }
              }
    }
}
//--------------------
//-------------------------------------
int un_mmi1=1;
int un_mmi2=2;
int un_mmi3=3;
char *list_un_mmi[]={
"",
" Объем,л   " ,  //1
"Сумма,руб  " ,  //2
"Цена,руб/л " ,  //3
"Масса,кг   " ,  //4
"Сумма,руб  " ,  //5
"Цена,руб/кг" ,  //6
};
//-------------------------------------
int CRC_err=0;
//-------------------------------------
void f_prn_CRC_error()
{
 MmiGotoxy(0,2);   MmiPrintf("!Ошибка контрольной суммы   ");
 MmiGotoxy(0,3);   MmiPrintf("!Параметры потеряны         ");
 MmiGotoxy(0,4);   MmiPrintf("!Необходима перенастройка   ");
 MmiGotoxy(0,5);   MmiPuts(s_empty);
 CRC_err=1;
}
//-------------------------------------
void f_prn_mode()
{
   if((valve_mode != 0) && (valve_modeL != 0))
   {
      if( Flag_Low == 0)
      {
        MmiGotoxy(22,0);
        MmiPuts(" ВЕРХНИЙ        ");
      }
      else if( Flag_Low == 1)
      {
        MmiGotoxy(23,0);
        MmiPuts(" НИЖНИЙ         ");
      }
   }


}
//-------------------------------------
void f_prn_proc()
{
 float V_tmp;


          if(PageD==0)
          {
        /*
            if ( Flag_mass != 0 )
            {
              MmiGotoxy(0,4);  MmiPuts(" Плотность     |   Расход      ");
              MmiGotoxy(0,5);MmiPrintf("%8.2f кг/м3 | %8.1f кг/ч  ",s_MVD[0].Dens,s_MVD[0].FlowM);
            }
            else
        */
            {
    //        MmiGotoxy(0,4);  MmiPuts(" Плотность     |   Расход      ");
    //        MmiGotoxy(0,5);MmiPrintf("%8.2f кг/м3 | %8.1f л/ч  ",s_MVD[0].Dens,s_MVD[0].FlowV);


#if !defined(BIO_1)

// nothing

//#else
              if( flag_AD != 0)
              {
                MmiGotoxy(0,4);  MmiPuts(" Плотн.средняя | Темп.средняя ");

                //18.11.2021 YN
                //MmiGotoxy(0,5);  MmiPrintf("%7.2f кг/м3  | %7.2f C     ",Dens_a,Temp_a);
                #if defined(weightedAverageTemperature)
                  if( waTempOn ) 
                  {
                    if(s_MVD[0].MassT != 0)
                    {
                      //30.03.2022 YN
                      averageTemp = waTemp/s_MVD[0].MassT;
                      MmiGotoxy(0,5);  
                      MmiPrintf("%7.2f кг/м3  | %7.2f C     ",Dens_a,averageTemp);
                    }
                    else
                    {
                      if ((mode_temp==EXT) && (s_MVD[0].TempR > -49.5) )
                      {
                        MmiGotoxy(0,5);  
                        MmiPrintf("%7.2f кг/м3  | %7.2f C     ",Dens_a,s_MVD[0].TempR);
                      }
                      else
                      {
                        MmiGotoxy(0,5);  
                        MmiPrintf("%7.2f кг/м3  | %7.2f C     ",Dens_a,s_MVD[0].Temp);
                      }
                    }
                  }
                  else
                  {
                    MmiGotoxy(0,5);  
                    MmiPrintf("%7.2f кг/м3  | %7.2f C     ",Dens_a,Temp_a);
                  }
                #else
                  MmiGotoxy(0,5);  
                  MmiPrintf("%7.2f кг/м3  | %7.2f C     ",Dens_a,Temp_a);
                #endif

              }
              else
#endif
              {
                if ((mode_temp==EXT) && (s_MVD[0].TempR > -49.5) )
                {
                MmiGotoxy(0,4);  MmiPuts("Плотн,кг/м3|Темпер.,C|Расх,л/ч ");
                 MmiGotoxy(0,5); MmiPrintf(" %8.2f  | %7.2f |%8.1f ",s_MVD[0].Dens,s_MVD[0].TempR,s_MVD[0].FlowV);
                }
                else
                {
                 MmiGotoxy(0,4);  MmiPuts("Плотн,кг/м3|ТемпMVD,C|Расх,л/ч ");
                 MmiGotoxy(0,5); MmiPrintf(" %8.2f  | %7.2f |%8.1f ",s_MVD[0].Dens,s_MVD[0].Temp,s_MVD[0].FlowV);
                }
              }
            }
          }
          else if(PageD==1)
          {
#if defined(BIO_1)
              MmiGotoxy(0,4);  MmiPuts(" Пл15,кг/м3|P вх ,МПа|P вых,МПа");

              MmiGotoxy(0,5);
         //   if(flag_prn_mass != 0)
         //     V_tmp=V_ref;
         //   else
         //     V_tmp=0.;
              if(D_ref< 400)
                MmiPrintf(" --------  |%8.3f | %8.3f ",s_MVD[0].Press);
              else
                MmiPrintf(" %8.2f  |%8.3f | %8.3f ",D_ref,P_inp,s_MVD[0].Press);
#else
              MmiGotoxy(0,4);  MmiPuts(" Пл15,кг/м3|Объем15,л|Давл,МПа");
              MmiGotoxy(0,5);
              if(flag_prn_mass != 0)
                V_tmp=V_ref;
              else
                V_tmp=0.;
              if(D_ref< 400)
                MmiPrintf(" --------  | ------- | %8.3f ",s_MVD[0].Press);
              else
                MmiPrintf(" %8.2f  |%8.2f | %8.3f ",D_ref,V_tmp,s_MVD[0].Press);
#endif
          }
          else if(PageD==3)
          {

     MmiGotoxy(0,4);

if( Flag_Low == 0)
{

#if defined(BIO_1)

 MmiGotoxy(0,4);   MmiPuts(s_empty);
 MmiGotoxy(0,5);   MmiPuts(s_empty);
     MmiGotoxy(0,4);
     MmiPuts("Кл.мнш.:");
     if( OUT_VAR & OUT2  )
       MmiPuts("1       ");
     else
       MmiPuts("0  ");

     MmiGotoxy(0,5);
     MmiPuts("Кл.блш.:");
     if( OUT_VAR & OUT1  )
       MmiPuts("1       ");
     else
       MmiPuts("0  ");


     MmiGotoxy(15,4);              MmiPuts("Емк. 1:");
     if( i7060_out[0] & OUT5  )
       MmiPuts("1  ");
     else
       MmiPuts("0  ");

     MmiGotoxy(26,4);   MmiPuts("Н:");
     if( i7060_out[0] & OUT3  )
       MmiPuts("1      ");
     else
       MmiPuts("0      ");


     MmiGotoxy(15,5);   MmiPuts("Емк. 2:");
     if( i7060_out[0] & OUT6  )
       MmiPuts("1  ");
     else
       MmiPuts("0  ");
 }
}
#else

   MmiGotoxy(0,4);
  if(valve_mode==1)      MmiPuts("Клап.мнш.В.:");
  else if(valve_mode==2) MmiPuts("Пилот НЗ.В.:");
  else if(valve_mode==3) MmiPuts("Кл.закр.В.:");

     if( OUT_VAR & OUT2  )
       MmiPuts("1       ");
     else
       MmiPuts("0       ");
     MmiGotoxy( 0,5);
  if(valve_mode==1)      MmiPuts("Клап.блш.В.:");
  else if(valve_mode==2) MmiPuts("Пилот НО.В.:");
  else if(valve_mode==3) MmiPuts("Кл.откр.В.:");
     if( OUT_VAR & OUT1  )
       MmiPuts("1       ");
     else
       MmiPuts("0       ");
}
else if( Flag_Low == 1)
{
  if(valve_modeL==1)      MmiPuts("Клап.мнш.Н.:");
  else if(valve_modeL==2) MmiPuts("Пилот НЗ.Н.:");
  else if(valve_modeL==3) MmiPuts("Кл.закр.Н.:");

     if( OUT_VAR & OUT6  )
       MmiPuts("1       ");
     else
       MmiPuts("0       ");
     MmiGotoxy( 0,5);
  if(valve_modeL==1)      MmiPuts("Клап.блш.Н.:");
  else if(valve_modeL==2) MmiPuts("Пилот НО.Н.:");
  else if(valve_modeL==3) MmiPuts("Кл.откр.Н.:");
     if( OUT_VAR & OUT5  )
       MmiPuts("1       ");
     else
       MmiPuts("0       ");

}

     MmiGotoxy(15,4);   MmiPuts("Воздух  :");
     if( OUT_VAR & OUT4  )
       MmiPuts("1  ");
     else
       MmiPuts("0  ");

     MmiGotoxy(26,4);   MmiPuts("Н:");
     if( OUT_VAR & OUT3  )
       MmiPuts("1      ");
     else
       MmiPuts("0      ");

     MmiGotoxy(15,5);   MmiPuts("Готовн. :");
     if( OUT_VAR & OUT8  )
       MmiPuts("1      ");
     else
       MmiPuts("0      ");
          }

#endif
}
//-------------------------------------
void f_prn_MassSt_dlv()
{
  MmiGotoxy(0,1);
         MmiPrintf("Всего %7.1f кг, %7.1f л  ",s_MVD[0].MassI-MassStamp,s_MVD[0].VolI-VolStamp);
/*
  if ( Flag_mass != 0 )
    MmiPrintf("  Всего отпущено %8.2f кг  ",s_MVD[0].MassI-MassStamp);
  else
    MmiPrintf("  Всего отпущено %8.2f л   ",s_MVD[0].VolI-VolStamp);
 */
}
//-------------------------------------
/*
void f_prn_MassSt_rcv()
{
  MmiGotoxy(0,1);
  if ( Flag_mass != 0 )
    MmiPrintf("   Всего принято %8.2f кг  ",s_MVD[0].MassI-MassStamp);
  else
    MmiPrintf("   Всего принято %8.2f л   ",s_MVD[0].VolI-VolStamp);
}
*/
//-------------------------------------
void f_prn_error()
{
   f_clr_scr_MMI();

#if( Test == 0)
   if(flag_mvd_ton)
   {
#if defined(BIO_1)
  MmiGotoxy(1,1);  MmiPrintf("  Система учета отпуска  ");
  MmiGotoxy(1,2);  MmiPrintf("      биокомпонента      ");
#else
  MmiGotoxy(1,1);  MmiPrintf("  Система учета отпуска  ");
  MmiGotoxy(1,2);  MmiPrintf("      нефтепродуктов     ");
#endif

  MmiGotoxy(0,3);  MmiPuts(s_empty);
  MmiGotoxy(0,4);  MmiPrintf("  !Инициализация расходомера ");
  MmiGotoxy(0,5);  MmiPuts(s_empty);
   }
   else
#endif
   {
    MmiGotoxy(0,1);   MmiPrintf("                        ");
    MmiGotoxy(0,2);   MmiPrintf("!Ошибка функционирования ");
    MmiGotoxy(0,3);   MmiPrintf("Sht-F1 отображение ошибок");
    MmiGotoxy(0,4);   MmiPrintf("Sht-ESC очистка ошибок   ");
   }
}
//-------------------------------------
void  f_disp_error()
{ // распечатывает ошибки

 struct s_icp_dev *ICPl_DEV;
int i,i1,i2;
unsigned int itmp;
   i1=0;
   i2=0;
 for(i=0;i<=icp_lst_max;i++)
   if(ICP_error[i])
   {
    i2++;
    ICPl_DEV=ICP_dd[i];
  if(i != icp_lst_max)
  {
 MmiGotoxy(0,i1++);   MmiPrintf("Ошибка в драйвере %9s:",ICPl_DEV->name );
  }
  else
  {
 MmiGotoxy(0,i1++);   MmiPrintf("Ошибка системы учета:           ");
  }
    if(i1 >= n_mmi_str) return;
    itmp=ICP_error[i];
  if(i != icp_lst_max)
  {
    if(itmp & RD_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка чтения устройства");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & WR_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка записи в устройство");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & WD_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка Watch Dog");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & Drive_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка в устройстве,%02X", (itmp>>8) & 0xff);
    if(i1 >= n_mmi_str) return;
    }

  }
  else //(i == icp_lst_max)
  {
    if(itmp == No_Src )
    {
MmiGotoxy(0,i1++); MmiPrintf("Не задан источник продукта");
//  if(i1 >= n_mmi_str) return;
      return;
    }
    if(itmp == Err_Key )
    {
MmiGotoxy(0,i1++); MmiPrintf("Ав.останов с клавиатуры ВРФ");
//  if(i1 >= n_mmi_str) return;
      return;
    }
    if(itmp == EEE_CRC_error)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка CRC EEPROM процессора");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & Flash_erase_error )
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка стирания FLASH");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & Flash_wr_error  )
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка записи во FLASH");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & No_gas_error  )
    {
MmiGotoxy(0,i1++); MmiPrintf("Расходомер не заполнен");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & No_Z_mass_err )
    {
MmiGotoxy(0,i1++); MmiPrintf("Расх-р не обнулил массу");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & No_F_dat_err )
    {
MmiGotoxy(0,i1++); MmiPrintf("Нет обновл.данных для журнала");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & DNS_not_off)
    {
MmiGotoxy(0,i1++); MmiPrintf("Утечка при старте ");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & DNS_st)
    {
MmiGotoxy(0,i1++); MmiPrintf("Утечка при завершении");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & No_Press)
    {
MmiGotoxy(0,i1++); MmiPrintf("Низкое давление при старте ");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & Gun_off )
    {
MmiGotoxy(0,i1++); MmiPrintf("Насос отключен при отпуске");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp == WD_host)
    {
MmiGotoxy(0,i1++); MmiPrintf("Нет связи с HOST");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp == No_flow )
    {
MmiGotoxy(0,i1++); MmiPrintf("Поток прекратился");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & ES_off )
    {
MmiGotoxy(0,i1++); MmiPrintf("Кнопка ES при отпуске");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & UZA_off )
    {
     switch (flagE_UZA)
     {
      case UZA_E:
MmiGotoxy(0,i1++); MmiPrintf("Нет УЗА при отпуске");
        if(i1 >= n_mmi_str) return;
        break;
      case LVL_E:
MmiGotoxy(0,i1++); MmiPrintf("Достигнут предельный уровень");
        if(i1 >= n_mmi_str) return;
        break;
      case ID_E:
MmiGotoxy(0,i1++); MmiPrintf("Некорректный ID ");
        if(i1 >= n_mmi_str) return;
        break;
      case TRAP_E:
MmiGotoxy(0,i1++); MmiPrintf("Трап не в раб.позиции ");
        if(i1 >= n_mmi_str) return;
        break;
      case CONS_E:
MmiGotoxy(0,i1++); MmiPrintf("Консоль не в раб.позиции ");
        if(i1 >= n_mmi_str) return;
        break;
     }
    }
    if(itmp & MVD_cfg )
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка конф.расход-ра ");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp == MotNotStop )
    {
MmiGotoxy(0,i1++); MmiPrintf("Сбой насоса при торм.");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp == DLT_off)
    {
MmiGotoxy(0,i1++); MmiPrintf("Насос выкл. при отпуске");
    if(i1 >= n_mmi_str) return;
    }
    if((itmp & MVD_com)== MVD_com )
    {
MmiGotoxy(0,i1++); MmiPrintf("Нет связи с расхдм.");
    if(i1 >= n_mmi_str) return;
    }
    if((itmp & Hst_alm)==Hst_alm)
    {
MmiGotoxy(0,i1++); MmiPrintf("Авария от Host");
    if(i1 >= n_mmi_str) return;
    }
    if((itmp & No_Drv)==No_Drv)
    {
MmiGotoxy(0,i1++); MmiPrintf("Драйверы MVD,7041 не включены");
    if(i1 >= n_mmi_str) return;
    }
  }
   }
   if(i2==0)
   {
    MmiGotoxy(5,2); MmiPrintf("Ошибок нет");
    MmiGotoxy(2,4); MmiPrintf("Enter - продолжить");
   }
}
//-------------------------------------
void f_drv_list_MMI(int *nn)
{ // показать список драйверов устройств
int i,i1,i2;
char str[31];
  struct s_icp_dev *ICPl_DEV;

   f_clr_scr_MMI();

  //  if( (*nn+n_mmi_str_3) > icp_lst_max) i=icp_lst_max-n_mmi_str_3;
  //  else
    i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < icp_lst_max) i1=i+n_mmi_str_3;
    else i1=icp_lst_max;

    *nn=i;
    MmiGotoxy(0,0); MmiPuts("        Список драйверов      ");
    MmiGotoxy(0,1); MmiPuts(" N     Имя   Сост. Порт Адр.");
   for(i2=0;i<i1;i++,i2++)
    {
      ICPl_DEV=ICP_dd[i];
      if(ICPl_DEV==NULL) break;
      MmiGotoxy(0,3+i2);
//         MmiPuts(" N      Имя   Сост. Порт Адр.");
//                 "xx.....xxxxx  .x    .x   xx"
      MmiPrintf("%2d%10s  %2d    %2d   %02X",i+1,ICPl_DEV->name,ICPl_DEV->status,ICPl_DEV->port,ICPl_DEV->addr);
    }
}
//-------------------------------
void f_drv_stat_MMI(int *nn)
{ // показать статистику драйверов устройств
int i,i1,i2;
char str[31];
  struct s_icp_dev *ICPl_DEV;

   f_clr_scr_MMI();

//  if( (*nn+n_mmi_str_3) > icp_lst_max) i=icp_lst_max-n_mmi_str_3;
//  else
    i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < icp_lst_max) i1=i+n_mmi_str_3;
    else i1=icp_lst_max;

    *nn=i;
    MmiGotoxy(0,0);  MmiPuts(" Статистика работы драйверов");
    MmiGotoxy(0,1);  MmiPuts(" N     Имя    Послано  Ошибок");
    for(i2=0;i<i1;i++,i2++)
    {
      ICPl_DEV=ICP_dd[i];
      if(ICPl_DEV==NULL) break;
      MmiGotoxy(0,3+i2);
//         MmiPuts(" N     Имя    Послано  Ошибок");
//                 "xx.....xxxxx  xxxxxxx xxxxxxx"
         MmiPrintf("%2d%10s  %7ld %7ld",i+1,ICPl_DEV->name,ICPl_DEV->n_transaction,ICPl_DEV->n_timeout_error+ICPl_DEV->n_CRC_error);
    }
}
//-------------------------------
//-------------------------------
void f_mod_list_MMI(int *nn)
{ // показать список обнаруженных модулей
int i,i1,i2;
 char str[31];

  struct icp_mod *ICPl_MOD;

  i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
  f_clr_scr_MMI();
  if(i1==0)
  {
    MmiGotoxy(3,2);   MmiPuts("    Список пуст." );
    MmiGotoxy(3,3);   MmiPuts("Используйте поиск для" );
    MmiGotoxy(3,4);   MmiPuts("  обновления списка." );
  }
  else
  {
    MmiGotoxy(0,0);    MmiPuts(" Список обнаруженных модулей  ");
    MmiGotoxy(0,1);    MmiPuts(" N    Имя Порт.Адр.Скор. TT FF");

      i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < i1) i1=i+n_mmi_str_3;
    *nn=i;

    for(i2=0;i<i1;i++,i2++)
    {
      ICPl_MOD=f_get_mod_str(i);
      if(ICPl_MOD==NULL) break;
      MmiGotoxy(0,3+i2);

//         MmiPuts(" N    Имя Порт.Адр.Скор. TT FF");
//                 "xx.....xxxx  x xx xxxxxx xx xx"
      MmiPrintf("%2d%9s  %1d %02X %6ld %02X %02X",i+1,ICPl_MOD->name,ICPl_MOD->port,ICPl_MOD->addr,baudrate_val[ICPl_MOD->CC],ICPl_MOD->TT,ICPl_MOD->FF);
    }
  }
}
//-------------------------------
int mode_prn=1;
void f_prn_current()
{ // показать состояние отпуска на текущий момент
  int year,month,day,hour,min,sec;
 MmiGotoxy(0,0);   MmiPrintf("Tекущее значение счетчика");
 MmiGotoxy(0,3);   MmiPrintf("  Дата     Время     Масса,кг");
//MmiGotoxy(0,3);  MmiPrintf("21.02.09 12:10:11  _123456.90");

 GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
 MoveToXY(0,5); MmiPrintf("%02d.%02d.%02d %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);
 MmiGotoxy(18,5);  MmiPrintf("%11.3f",s_MVD[0].MassI);
                                   // s_MVD[0].VolI
}
//-------------------------------
void f_prn_den_t()
{
float ftmp,ftmp1;
 //  Отображение Массы,плотности,температуры,давления
 //  Текущее значение.
 MmiGotoxy(0,0);   MmiPrintf("Текущие показания расходомера");
 MmiGotoxy(0,1);   MmiPrintf(" Масса      %10.3f кг",s_MVD[0].MassT);
 MmiGotoxy(0,2);   MmiPrintf(" Объем      %10.3f л ",s_MVD[0].VolT);
// MmiGotoxy(0,1);   MmiPrintf(" Масса      %10.3f кг",s_MVD[0].MassI);
// MmiGotoxy(0,2);   MmiPrintf(" Объем      %10.3f л ",s_MVD[0].VolI);
// MmiGotoxy(0,2);   MmiPrintf(" Объем      %10.3f кг",s_MVD[0].MassT);
 MmiGotoxy(0,3);   MmiPrintf(" Расход    %10.2f кг/ч",s_MVD[0].FlowM);
 MmiGotoxy(0,4);   MmiPrintf(" Плотность %10.2f кг/м3",s_MVD[0].Dens);
// MmiGotoxy(0,2);   MmiPrintf(" Давление     %6.3f МПа  ",s_MVD[0].Press);
// MmiGotoxy(0,3);   MmiPrintf(" Температура  %6.2f град.C",s_MVD[0].TempR);
 MmiGotoxy(0,5);   MmiPrintf(" Темп.расхдм.  %6.2f град.C",s_MVD[0].Temp);
 MmiGotoxy(0,6);   MmiPuts(" ESC-меню");
}
//-------------------------------
//-------------------------------
char *rcv_mm[]={
"отпуск",
"прием",
};
char *Rcv_mm[]={
"Отпуск",
"Прием",
};
// rcv_mm[f_rcv]
int  f_prn_mmi_a(struct arch_str *a_str,int i_n)
{
 int ii,itmp,i_ret;
 float ftmp,ftmp1;
 struct tm *tdat;
 int f_rcv;

 if(a_str->event & (int)RCV_FLG)
       f_rcv=1;
 else
       f_rcv=0;

 MoveToXY(0,i_n);
   i_ret=i_n;
   if(i_ret>=n_mmi_str) return i_ret;

//  MmiPrintf("%02d.%02d.%02d %02d:%02d:%02d" ,a_str->day,a_str->month,a_str->year,a_str->hour,a_str->min,a_str->sec);

  tdat = localtime( &a_str->time );
  MmiPrintf("%02d.%02d.%02d %02d:%02d:%02d",
  tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);


 if( (a_str->event==slt_wr_jrnI) || (a_str->event==slt_wr_jrnF)  );
 else
  if((a_str->MassI != vBIG_P)&& (a_str->MassI != M_one) )
     MmiPrintf("%10.3f кг",a_str->MassI);
  else
     MmiPrintf(" ------- " );

// i_ret=i_n+1;
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;

 MoveToXY(0,i_ret);

 if( (a_str->event==slt_wr_jrnI) || (a_str->event==slt_wr_jrnF)  );
 else
  if((a_str->VolI != vBIG_P)&& (a_str->VolI != M_one) )
  {
     MmiPrintf("V=%10.3f л",a_str->VolI);
     i_ret++;
     if(i_ret>=n_mmi_str) return i_ret;
     MoveToXY(0,i_ret);
  }
  /*
  else
     MmiPrintf(" ------- " );
  */
// switch( (int) a_str->event)
 switch( (int)( a_str->event & ~(int)RCV_FLG) )
 {
  case evt_beg_eq:
    MmiPrintf("Старт %sа.",rcv_mm[f_rcv]);
    goto m_val0;

  case slt_wr_jrnF:
    MmiPrintf(" Знач. MVD,float:");

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

    MmiPrintf("Addr=%5.0f, n= %2.0f:",a_str->MassI,a_str->VolI/2.);
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

    MmiPrintf(" %g",a_str->u.fill.VolT);

    if( a_str->VolI  > 2 )
      MmiPrintf(", %g", a_str->u.fill.Amount);

    if( a_str->VolI  > 4 )
      MmiPrintf(", %g,", a_str->u.fill.UnitPrice);
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

    if( a_str->VolI  > 6 )
      MmiPrintf(" %g", a_str->u.fill.Density);

    if( a_str->VolI  > 8 )
      MmiPrintf(", %g", a_str->u.fill.Temp);

    break;

  case slt_wr_jrnI:
    MmiPrintf(" Знач. MVD,int:");
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    MmiPrintf(" Addr=%5.0f, n= %2.0f:",a_str->MassI,a_str->VolI);
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

    for(ii=0;ii<a_str->VolI;ii++)
    {
      if((ii== 4) || (ii== 8) )
      {
       i_ret++;
       if(i_ret>=n_mmi_str) return i_ret;
       MoveToXY(0,i_ret);
      }
      MmiPrintf(" %6d",a_str->u.err.err[ii] );
    }
    break;

  case evt_start_fL:
    MmiPrintf("Старт Нижн.");
    goto m_val0;

  case evt_start_f:
    MmiPrintf("Старт Верхн.");

m_val0:
//   i_ret=i_n+2;
// if(i_ret>=n_mmi_str) return i_ret;
// MoveToXY(0,i_ret);
  if(a_str->u.fill.VolT != vBIG_P)
  {
      if ( Flag_mass != 0 )
        MmiPrintf("Доза %8.3fкг ",a_str->u.fill.VolT    );
      else
        MmiPrintf("Доза %8.3f л",a_str->u.fill.VolT    );
  }
  else
      MmiPrintf("Без дозы    ",a_str->u.fill.VolT    );

m_val1:
// i_ret=i_n+2;
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

   MmiPrintf("Плотн.%6.3fкг/м3;" ,a_str->u.fill.Density);
   MmiPrintf("Т=%6.2fC;" ,a_str->u.fill.Temp);

   i_ret++;
// i_ret=i_n+3;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    MmiPrintf("P=%6.3f МПа;" ,a_str->u.fill.UnitPrice);
//  MmiPrintf("Т=%6.2fC;" ,a_str->u.fill.Amount);

    break;

  case evt_cont_f:
    MmiPrintf(" Продолжение %sа,",rcv_mm[f_rcv]);
    goto m_val;
  case evt_exm_f:
    MmiPrintf(" Завершено,недолив ");

m_val:

// i_ret=i_n+2;
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    if(a_str->u.fill.VolT != vBIG_P)
         MmiPrintf(" Отп.%9.3f кг,%9.3f л",a_str->u.fill.VolT,a_str->u.fill.Amount );

// i_ret=i_n+3;
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    MmiPrintf("Плотн.%6.3f кг/м3;" ,a_str->u.fill.Density);
    MmiPrintf("Т=%6.2fC;" ,a_str->u.fill.Temp);

// i_ret=i_n+4;
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    MmiPrintf("P=%6.3f МПа ;" ,a_str->u.fill.UnitPrice);
//    MmiPrintf("Т=%6.2fC;" ,a_str->u.fill.Amount);

    break;

  case evt_exp_f:
    MmiPrintf(" Завершено,перелив");
    goto m_val;

  case evt_end_gas:
    MmiPrintf(" %s прерв.нет ждк.фазы",Rcv_mm[f_rcv]);
    goto m_val;

  case evt_end_f:
    MmiPrintf(" Завершено ОК  ");
    goto m_val;

  case evt_est_f:
    MmiPrintf(" %s прерван командой",Rcv_mm[f_rcv]);
    goto m_val;
  case evt_est_fh:
    MmiPrintf(" %s прерван Host ",Rcv_mm[f_rcv]);
    goto m_val;
  case evt_err_f:
    MmiPrintf(" %s прерван ошибкой",Rcv_mm[f_rcv]);
    goto m_val;
  case evt_eq_pr :
    MmiPrintf("Старт выр.давл. паровых фаз");
    goto m_val;
  case evt_stop_eq :
  printf("Заверш.выр.давл. паровых фаз");
    goto m_val;

  case evt_ep1_f:
    MmiPrintf(" %s прерван,Host",Rcv_mm[f_rcv]);
    goto m_val;

  case evt_overfl:
    MmiPrintf(" %s прерван ост.потока",Rcv_mm[f_rcv]);
    goto m_val;

  case evt_ES_off:
    MmiPrintf(" %s прерв.Авар.кнопкой ES",Rcv_mm[f_rcv]);
    goto m_val;

  case evt_UZA_off:
    MmiPrintf(" %s прерв.Нет сигнала УЗА",Rcv_mm[f_rcv]);
    goto m_val;
  case evt_LVL_off:
    MmiPrintf(" %s  прерван: Уровень.",Rcv_mm[f_rcv]);
    goto m_val;
  case evt_TRAP_off:
    MmiPrintf(" %s  прерван: Трап.",Rcv_mm[f_rcv]);
    goto m_val;
  case evt_CONS_off:
    MmiPrintf(" %s  прерван: Консоль.",Rcv_mm[f_rcv]);
    goto m_val;

  case evt_DLT_off:
    MmiPrintf(" %s прерв.Насос не включен",Rcv_mm[f_rcv]);
    goto m_val;

  case evt_wd_pmp:
    MmiPrintf(" %s прерв.WD PMLAN",Rcv_mm[f_rcv]);
    goto m_val;


  case evt_no_dev:
    MmiPrintf(" Старт не принят.Не вкл.драйв");
    goto m_val;

  case evt_clr_err:
    MmiPrintf(" Сброс ошибок. ");
    break;

  case evt_MVD_CL_strt:
    MmiPrintf(" Старт калибровки расходомера");
    break;

  case evt_MVD_CL_BS1 :
    MmiPrintf(" !Попытка клб.расх.при отп.");
    break;

  case evt_MVD_CL_BS2 :
    MmiPrintf(" !Сбой при старте клб.расх.");
    break;

  case evt_MVD_CL_OK :
    MmiPrintf(" Калибровка расходомера OK");
    break;

  case evt_MVD_CL_TM :
    MmiPrintf(" Таймаут калибровки расходом.");
          //  |                              |

    break;

  case evt_ch_date:
    MmiPrintf(" Дата изменена.");
    break;

  case evt_ch_time:
    MmiPrintf(" Время изменено.");
    break;
  case evt_ch_sum:
    MmiPrintf(" Переход на летнее время.");
    break;
  case evt_ch_win:
    MmiPrintf(" Переход на зимнее время.");
    break;
  case evt_res:
    MmiPrintf(" Команда Сброс");
    break;
  case evt_Slot:
    MmiPrintf(" MVD:Конфигурир-е устройства");
    break;
  case evt_chg_param:
    if(a_str->u.fill.VolT == Par_VF)
    {
     MmiPrintf(" !Изм.множ.объема");
    }
    else if(a_str->u.fill.VolT == Par_MF)
    {
     MmiPrintf(" !Изм.множ.массы");
    }
    else if(a_str->u.fill.VolT == Par_MC)
    {
     MmiPrintf(" !Изм.срез.м.расх.");
    }
    else if(a_str->u.fill.VolT == Par_PASS)
    {
     MmiPrintf(" !Изменен пароль");
    }
    else if(a_str->u.fill.VolT == Par_ID)
    {
     MmiPrintf(" !Изменен ID");
    i_ret++;
    if(i_ret>=n_mmi_str) return i_ret;
    MoveToXY(0,i_ret);

    MmiPrintf("Было :%ld; ",(long int)a_str->u.fill.Amount  );
    MmiPrintf("Стало:%ld" ,(long int)a_str->u.fill.UnitPrice);
    break;
    }
    else
     {
//    MmiPrintf(" !Изм. парам. ");
      MmiPrintf(" !Изменен параметр ");
      MmiPrintf("N%g ",a_str->u.fill.VolT );
     }

// i_ret=i_n+2;
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

    MmiPrintf("Было:%g; ",a_str->u.fill.Amount  );
    MmiPrintf("Стало:%g" ,a_str->u.fill.UnitPrice);
    break;
  case evt_exit:
    MmiPrintf(" Команда Выход в ДОС");
    break;
  case evt_re:
    MmiPrintf(" Чтение конф.из EEPROM");
    break;
  case evt_re_err:
    MmiPrintf(" Ошибка чтения конф.из EEPROM");
    break;
  case evt_we:
    MmiPrintf(" Запись конф. в EEPROM");
    break;
  case evt_we_err:
    MmiPrintf(" Ош.записи конф. в EEPROM");
    break;

  case evt_wrong_start:
    MmiPrintf(" Cтарт не принят.Ошибка.");
    goto m_val;

  case evt_err:
  case evt_none:
 mm_0:
    itmp=0;
    for(ii=0;(ii<=icp_lst_max)&& (ii<10) ;ii++)
      itmp |= a_str->u.err.err[ii];
  if(itmp)
  {
    MmiPrintf(" Ошибка:");
 mm_e:

// i_ret=i_n+2;
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

 MmiPrintf("%X" ,a_str->u.err.err[0] );    // MVD
 MmiPrintf(" %01X" ,a_str->u.err.err[1] );   // 7060
 MmiPrintf(" %01X" ,a_str->u.err.err[2] );   // 7041
 MmiPrintf(" %01X" ,a_str->u.err.err[3] );   // 7017
 MmiPrintf(" %01X" ,a_str->u.err.err[4] );   // TSD[0]
 MmiPrintf(" %01X" ,a_str->u.err.err[5] );   // Drive
 MmiPrintf(" %X" ,a_str->u.err.err[6] );   // OFP
 MmiPrintf(" %01X" ,a_str->u.err.err[7] );   // MMI
 MmiPrintf(" %04X:" ,a_str->u.err.err[8] );  // System
 MmiPrintf("%04X" ,a_str->u.err.err[9] );    // код
  }
  else
 MmiPrintf(" Ошибок нет ");

  break;

  case evt_t_on:
 MmiPrintf(" Включение устройства");
    break;
  case evt_t_off:
 MmiPrintf(" Выключение устройства");
    break;
  default:
    if( (a_str->event & evt_start_hst) == evt_start_hst )
    {

    ii=a_str->event;
    MmiPrintf("Ком.Host ");

    if(ii & 4)
      MmiPrintf("долив ");
    else
      MmiPrintf("старт ");

    if(ii & 1)
      MmiPrintf("Н.нал.");
    else
      MmiPrintf("В.нал.");

    if(ii & 2)
    {
      MmiPrintf("по массе.");
      i_ret++;
      if(i_ret>=n_mmi_str) return i_ret;
      MoveToXY(0,i_ret);
      MmiPrintf("Доза %8.3f кг; " ,a_str->u.fill.VolT );
    }
    else
    {
       MmiPrintf("по объему." );
       i_ret++;
       if(i_ret>=n_mmi_str) return i_ret;
       MoveToXY(0,i_ret);
       MmiPrintf("Доза  %8.3f л ; " ,a_str->u.fill.VolT );
     }
     goto m_val1;
     }
    else
 MmiPrintf(" Событие не известно");
   break;
 }
 return i_ret;
}

//----------------------------------
void f_MVD_M1()
{
// отображает параметры MVD  на MMI

     ff_serv=NULL;
/*

  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD");
     ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 20:
   MVD_fn[0]=MVD_RD_L;
   s_MVD[0].r_addr=127;
   sw_mvd_m1=1;
   break;

  case 1:
  if(MVD_fn[0] != F_FAULT)
   {MoveToXY(0,sw_MMI_cur++);MmiPrintf("SN   %ld",s_MVD[0].l);}
  if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 21:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=425;
   s_MVD[0].n=8;
   sw_mvd_m1=2;
   break;

  case 2:
  if(MVD_fn[0] != F_FAULT)
  { MoveToXY(0,sw_MMI_cur++); MmiPrintf("S  : %s",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 22:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].n=8;
   s_MVD[0].r_addr=96;
   sw_mvd_m1=3;
   break;

  case 3:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("TXM:%s",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 23:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=104;
   s_MVD[0].n=16;
   sw_mvd_m1=4;
   break;

  case 4:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("M:%s",s_MVD[0].a); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 24:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr=16;
   sw_mvd_m1=5;
   break;

  case 5:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);MmiPrintf("TXM версия: %d",s_MVD[0].i);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 25:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=72;
   s_MVD[0].n=5;
   sw_mvd_m1=6;
   break;

  case 6:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Flow : --%s--",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 26:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=80;
   s_MVD[0].n=7;
   sw_mvd_m1=7;
   break;

  case 7:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Тemp.: --%s--",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 27:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=155;
   sw_mvd_m1=8;
   break;

  case 8:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("D1:                  %.3f",s_MVD[0].f); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 28:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=157;
   sw_mvd_m1=9;
   break;

  case 9:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);MmiPrintf("D2:                  %.3f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 29:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=159;
   sw_mvd_m1=10;
   break;

  case 10:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("K1:                  %.3f",s_MVD[0].f); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 30:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=161;
   sw_mvd_m1=11;
   break;

  case 11:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);MmiPrintf("K2:                  %.3f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 31:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=279;
   sw_mvd_m1=12;
   break;

  case 12:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Множитель массы:     %.6f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 32:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=281;
   sw_mvd_m1=13;
   break;

  case 13:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Множитель объема:    %.6f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 33:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=283;
   sw_mvd_m1=14;
   break;

  case 14:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Множитель плотности: %.6f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 34:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=195;
   sw_mvd_m1=15;
   break;

  case 15:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Срез масс.расхода:   %.6f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 35:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=197;
   sw_mvd_m1=16;
   break;

  case 16:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Срез об. расхода:    %.6f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 36:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=149;
   sw_mvd_m1=17;
   break;

  case 17:
   if(MVD_fn[0] != F_FAULT)
  { MoveToXY(0,sw_MMI_cur++); MmiPrintf("Срез плотности :     %.6f",s_MVD[0].f);}
   ff_serv=NULL;
   break;
  }
 */
}
/* ---------------------------------------- */
void f_MVD_M1f()
{
// отображает параметры коррекции расхода  MVD  на MMI
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
     MoveToXY(0,4); MmiPrintf(" !Таймаут чтения MVD");
     ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 31:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=279;
   sw_mvd_m1=12;
   break;

  case 12:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,4);MmiPrintf("Множитель массы:    %.6f",s_MVD[0].f);}
     k_m_flow=s_MVD[0].f;

  case 32:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=281; // Множитель объема
//   s_MVD[0].r_addr=195;  // Cuttoff for mass flow
   sw_mvd_m1=13;
   break;

  case 13:
   if(MVD_fn[0] != F_FAULT)
  { MoveToXY(0,5);MmiPrintf("Множитель объема:   %.6f",s_MVD[0].f);}
// { MoveToXY(0,5); MmiPrintf("Срез м.расх.: %10.2f кг/ч",s_MVD[0].f);}
   k_v_flow=s_MVD[0].f;

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=283; // Множитель объема
   sw_mvd_m1=14;
   break;

  case 14:
   if(MVD_fn[0] != F_FAULT)
  { MoveToXY(0,6);MmiPrintf("Множитель плотн.:   %.6f",s_MVD[0].f);}
   k_dens=s_MVD[0].f;

   ff_serv=NULL;
   Flag_f_corr=1;
   break;
 }
}
/* ---------------------------------------- */
void f_MVD_M1s()
{
// отображает параметры коррекции расхода  MVD  на MMI
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
     MoveToXY(0,4); MmiPrintf(" !Таймаут чтения MVD");
     ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 31:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=195;
   sw_mvd_m1=12;
   break;

  case 12:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,4); MmiPrintf("Срез м. расх.: %8.2f кг/ч",s_MVD[0].f);}
     k_m_flow=s_MVD[0].f;

  case 32:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=197;  // Cuttoff for mass flow
   sw_mvd_m1=13;
   break;

  case 13:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,5); MmiPrintf("Срез об.расх.: %8.3f л/сек",s_MVD[0].f);}
   k_v_flow=s_MVD[0].f;
   ff_serv=NULL;
   Flag_f_corr=1;
   break;
 }
}
/* ---------------------------------------- */
/* ---------------------------------------- */
char *unit1=NULL;//I39
char *unit2=NULL;//I45
char *unit3=NULL;//42 // Volume flow codes
char *unit4=NULL;// Volume totalizer or volume inventory codes I 0046
char *unit5=NULL;// Density codes I0040
char *unit6=NULL;// Temperature codes I0041
float MMITemp,MMIDens,MMICompos;
void f_MVD_M2()
{
char *unit;
int u_cod;
// отображает данные MVD

        ff_serv=NULL;
/*
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD");
        ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 20:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr=39; // I039 Mass flow codes
   s_MVD[0].i=-1;
   sw_mvd_m1=1;
   break;

  case 1:
   u_cod=s_MVD[0].i;
    if(u_cod>0)  unit1=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=247;
   sw_mvd_m1++;
   break;

  case 2:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit1 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход м. : %f %s",s_MVD[0].f,unit1);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }

  case 21:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr=45; // Mass totalizer or mass inventory unit codes I 0045
   s_MVD[0].i=-1;
   sw_mvd_m1=3;
   break;

  case 3:
   u_cod=s_MVD[0].i;
    if(u_cod>0)  unit2=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=259;
   sw_mvd_m1++;
   break;

  case 4:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса общ.: %10.3f %s",s_MVD[0].f,unit2);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 22:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=263;
   sw_mvd_m1=5;
   break;

  case 5:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса инв.: %10.2f %s",s_MVD[0].f,unit2); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 23:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 42 ; // Volume flow codes I0042
   s_MVD[0].i=-1;
   sw_mvd_m1=6;
   break;

  case 6:
   u_cod=s_MVD[0].i;
   if(u_cod>0)  unit3=f_get_unit(u_cod);

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=253;
   sw_mvd_m1++;
   break;

  case 7:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit3 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход об.: %f %s",s_MVD[0].f,unit3);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 24:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 46 ; // Volume totalizer or volume inventory codes I 0046
   s_MVD[0].i=-1;
   sw_mvd_m1=8;
   break;

  case 8:
   u_cod=s_MVD[0].i;
   if(u_cod>0)  unit4=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=261;
   sw_mvd_m1++;
   break;

  case 9:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем общ.: %10.3f %s",s_MVD[0].f,unit4);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 25:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=265;
   sw_mvd_m1=10;
   break;

  case 10:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем инв.: %10.2f %s",s_MVD[0].f,unit4);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 26:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 40 ; // Density codes I0040
   s_MVD[0].i=-1;
   sw_mvd_m1=11;
   break;

  case 11:
   u_cod=s_MVD[0].i;
   if(u_cod>0)
   {
     unit5=f_get_unit(u_cod);
//     printf("\n\r density unit :%s",unit5);
   }
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=249;
   sw_mvd_m1++;
   break;

  case 12:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit5 != NULL)
    {
      MoveToXY(0,sw_MMI_cur++);MmiPrintf("Плотность  :%.5f %s",s_MVD[0].f,unit5);
      MMIDens=s_MVD[0].f;
    }
   else sw_MMI_cur++;
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  else sw_MMI_cur++;
  case 27:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 41 ; // Temperature codes I0041
   s_MVD[0].i=-1;
   sw_mvd_m1=13;
   break;

  case 13:
   u_cod=s_MVD[0].i;
   if(u_cod>0)  unit6=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=251;
   sw_mvd_m1++;
   break;

  case 14:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit6 != NULL)
   {
    MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.потока: %.3f %s",s_MVD[0].f,unit6);
    MMITemp=s_MVD[0].f;
   }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 28:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=383;
   sw_mvd_m1=17;
   break;

  case 17:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.платы : %.3f C",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 29:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=385;
   sw_mvd_m1=18;
   break;

  case 18:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Напр.пит.  : %.3f V",s_MVD[0].f);}
   ff_serv=NULL;
   break;
  default:
     ff_serv=NULL;
  }
*/
}
/* ---------------------------------------- */
void f_MVD_MM2()
{
char *unit;
// отображает данные MVD


        ff_serv=NULL;
/*
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD");
        ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 20:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=247;
   sw_mvd_m1=2;
   break;

  case 2:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit1 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход м. : %f %s",s_MVD[0].f,unit1);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }

  case 21:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=259;
   sw_mvd_m1=4;
   break;

  case 4:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса общ.: %10.3f %s",s_MVD[0].f,unit2);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 22:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=263;
   sw_mvd_m1=5;
   break;

  case 5:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса инв.: %10.2f %s",s_MVD[0].f,unit2); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 23:

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=253;
   sw_mvd_m1=7;
   break;

  case 7:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit3 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход об.: %f %s",s_MVD[0].f,unit3);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 24:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=261;
   sw_mvd_m1=9;
   break;

  case 9:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем общ.: %10.3f %s",s_MVD[0].f,unit4);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 25:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=265;
   sw_mvd_m1=10;
   break;

  case 10:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем инв.: %10.2f %s",s_MVD[0].f,unit4);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 26:

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=249;
   sw_mvd_m1=12;
   break;

  case 12:
  if((MVD_fn[0] != F_FAULT)&&(unit5 != NULL))
  {
     MoveToXY(0,sw_MMI_cur++); MmiPrintf("Плотность  :%.5f %s",s_MVD[0].f,unit5);
     MMIDens=s_MVD[0].f;
  }
  else
  {
     MoveToXY(0,sw_MMI_cur++);MmiPrintf("Ошибка чтения");
  }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 27:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=251;
   sw_mvd_m1=14;
   break;

  case 14:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit6 != NULL)
    {
     MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.потока: %.3f %s",s_MVD[0].f,unit6);
     MMITemp=s_MVD[0].f;
    }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 28:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=383;
   sw_mvd_m1=17;
   break;

  case 17:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.платы : %.3f C",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 29:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=385;
   sw_mvd_m1=18;
   break;

  case 18:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Напр.пит.  : %.3f V",s_MVD[0].f);}
   ff_serv=NULL;
   break;
  default:
     ff_serv=NULL;
  }
*/
}
/* ---------------------------------------- */
void f_MVD_M3()
{
int i,i2;
unsigned long int ltmp,i1;
// отображает ошибки MVD
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD");
        ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 0:

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=245;
   sw_mvd_m1++;
   break;

  case 1:
   if(MVD_fn[0] != F_FAULT)
   {
     ltmp=(unsigned long int)s_MVD[0].f &0xffffff;
     fl_zeroing=ltmp;
     if(ltmp==0)
     {
   MoveToXY(0,3); MmiPrintf("         Ошибок нет.          ");
        ff_serv=NULL;
        return;
     }
     else
     {
       i2=0;
       for(i=0,i1=1;i<24;i++,i1=i1<<1)
       {
         if(ltmp & i1)
         {
          i2++;
         }
       }


     if(sw_mmi!=18)
     {
      MoveToXY(0,0);

      if(i2==1)
          MmiPrintf("   Ошибка расходомера:");
      else if( i>1)
          MmiPrintf("   Ошибки расходомера:");
       i2=1;
     }
     else
     {

      MoveToXY(0,0);
         MmiPrintf("Состояние/ошибки расходомера:");
       i2=2;
     }

       for(i=0,i1=1;i<24;i++,i1=i1<<1)
       {
         if(ltmp & i1)
         {
           MoveToXY(0,i2++);
           MmiPrintf("!%s",err_F425[i]);
           if(i2>=7)
           {
            ff_serv=NULL;
            return;
           }
         }
       }
       for(;i2<6;i2++)
       {
           MoveToXY(0,i2);
           MmiPuts(s_empty);
       }
     }
   }
   ff_serv=NULL;
   break;
  }
}
//------------------------
void f_MVD_MM4()
{
// запрещает счет и устанавливает срезмассового расхода == 0
 if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
 {
   if( f_timer(tm_mvd,f_MVD_tim ) )
   {
    MoveToXY(0,4); MmiPrintf(" !Таймаут чтения MVD");
    f_icp_errS(MVD_com);
    ff_serv=NULL;
   }
   return;
 }
 tm_mvd=TimeStamp;

 switch(sw_mvd_m1)
 {
  case 20:
       // Stop totalizers
   MVD_fn[0]=MVD_WR_C;
   s_MVD[0].r_addr=2;
   s_MVD[0].i=0;
   s_MVD[0].f=0;
   sw_mvd_m1=30;
   break;

  case 30:   // Срез м. расх.
   MVD_fn[0]=MVD_WR_F;
   s_MVD[0].r_addr=195;
   s_MVD[0].f=0;
   sw_mvd_m1=31;
   break;

  case 31:   // Срез м. расх.

   State_SLV = Mod_flow_a;
   f_reg_cmn(Mod_flow_a_cmn);
   ff_serv=NULL;
   break;

  default:
   ff_serv=NULL;
   break;
 }
}
/* ---------------------------------------- */


//------------------------
//------------------------
//-------------------------------

char *list1_dsr[]={

"MMI:Статус",                 //  0
"Флаг массы ",         //  1
"Флаг прокачки",         //  2
"",         //  3
"",          //  4
"MMI:Флаг CRC",               //  5

"7017/1:Статус",                 //  6
"LOW mode",            //  7
"HIGH mode",                  //  8
"",              //  9
"P_demand",             // 10
"K_D",               // 11
"K_P         ",                // 12
"K_I         ",           // 13
"Mode P    ",                 // 14
"Addr Modbus  ",             // 15
"Delta :Статус",                // 16
"Флаг Slave      ",             // 17

"7060:Статус",                 // 18
"K_FD",                   // 19
"K_FP",                   // 20
"K_FI",                   // 21
"7041:Статус",          // 22
"OFP:Статус",          // 23

"7060/2:Статус",                // 24
"7060/2:Номер порта",           // 25
"7060/2:Адрес",                 // 26
"7060/2:Опрос,мс ",             // 27
"7060/2:Таймаут,мс",            // 28
"7060/2:Флаг CRC",              // 29

"",                // 30
"COM2 Скорость",                // 31
"Макс.коррекц,л",                // 32
"",                // 33

"",            // 34
"",           // 35
"",            // 36

"WDOG:Статус",                // 37
"",           // 38
"",            // 39
"",           // 40
"",              // 41

"Addr reg",   // 42
"n reg",      // 43
"Tim,ms",     // 44
"Mult",       // 45
"nnn=-1",     // 46
"int 1,float 2",// 47
"",        // 48

"",           // 49
"",        // 50

"MVD:Статус",                 //  51
"",            //  52
"",                  //  53
"",              //  54
"",             //  55

"TSD:Статус",                 //  56
"",            //  57
"",                  //  58
"",              //  59
"",             //  60

"Кн.Н.=Stop= N вх.",     // 61
"Кн.Н.=Stop=фл.инв.",    // 62
"Кн.Н.=Start= N вх.",   // 63
"Кн.Н.=Start=фл.инв",   // 64

"",           // 65
"",           // 66
"",           // 67
"",           // 68

"",           // 69
"",           // 70
"",           // 71
"",           // 72

"f нач нап,0.01Гц",           // 73
"f номин. ,0.01Гц",           // 74
"f замедл.,0.01Гц",           // 75
"",           // 76

"",        // 77
"",         // 78
"",       // 79

"Кл.В.Блш N вых.",        // 80
"Кл.В.Мнш N вых.",        // 81
"Флаг расш.отчета.",      // 82
"",      // 83
"Р-возд.В.Н.N вых.",      // 84
"Кл.Н.Блш N вых.",     // 85
"Кл.Н.Мнш N вых.",     // 86
"Нет аварии N вых",        // 87
"",           // 88
"Переход на лет.вр.",       // 89
"------------------",       // 90
"P полная шкала,МПа",       // 91
"P смещение,    МПа",       // 92
"T полная шкала,C",         // 93
"T смещение,    C",         // 94
"ост.Vol В сниж.ч",        // 95
"ост.Vol Н сниж.ч",        // 96
"",        // 97
"ост.Vol Н Б.кл.1",         // 98    p1-p6.CLV
"ост.Vol Н.М.кл.2",         // 99
"T срабат.Бкл.1",       // 100
"T срабат.Мкл.2",       // 101
"нач.запр.оп.MMI,мс",      // 102
"длит.запр.о.MMI,мс",       // 103
"подстр.В.н. 0...1",        // 104   p1-p2.KA
"подстр.Н.н. 0...1",        // 105
"зад-ка зап.отч.,мс",       // 106

"тестовый режим 1/0",       // 107    p1.CTRL
"",       // 108    p2.CTRL
"срез расх.для TST",        // 109    p3.CTRL

"первичная M,кг",           // 110    p1-p5.DSC
"экстраполир-я M,кг",        // 111
"расход при сраб.кл",       // 112
"коррекция M,кг",            // 113
"коррекция T,мс",           // 114


"Сгн.'КОНСОЛЬ' N вх.",      // 115
"Сгн.'КОНСОЛЬ'фл.инв",      // 116
"T между вкл.кл.,мс ",      // 117
"T между выкл.кл.,мс ",     // 118
"Режим t отп.0-2",          // 119
"Режим отпуска 1-3",        // 1 2 0


"Дтч.ур.Ниж.N вх.",    // 121
"Дтч.ур.Ниж.фл.инв",   // 122
"Зад. вкл.кл1,мс",      // 123
"Tmin выкл. кл1,мс",       // 124
"Tmin вкл.  кл1,мс",        // 125

"Дтч.ур.Вер.N вх.",      // 126
"Дтч.ур.Вер.фл.инв",      // 127
"Кн.В.=Start= N вх.",        // 128
"Кн.В.=Start=фл.инв",      // 129
"Кн.В.=Stop= N вх.",      // 130
"Кн.В.=Stop=фл.инв.",      // 131
"Сигн.'ТРАП' N вх.",      // 132
"Сигн.'ТРАП'фл.инв.",      // 133
"MAX недолив",              // 134
"MAX перелив",              // 135
"WD пер.MASTER,мс",        // 136
// FLCH
"Ав.кн.Нижн. N вх.",    // 137
"Ав.кн.Нижн.фл.инв",    // 138
"Сигн.УЗА Н.N вх.",       // 139
"Сигн.УЗА Н фл.инв.",     // 140
"Ном.расх.Н.Н.кг/ч. ",     // 141
"Вкл.насоса N вых.",        // 142
"Ав.кн.Верхн. N вх.",      // 143
"Ав.кн.Верхн.фл.инв",      // 144
"Сигн.УЗА В.N вх.",      // 145
"Сигн.УЗА В фл.инв.",      // 146
"",       // 147
"Давление N анлг.вх.",     // 148
"Температ.N анлг.вх.",     // 149
"-------------------",     // 150
"f тока насоса,0.1Гц",     // 151
"T разг.насоса,0.1с",      // 152
"Задержка вкл.кл2,мс",     // 153
"Зад.выкл.насоса,мс",      // 154
"T торм.насоса,0.1с",      // 155
"ост.Vol В.Б.кл.1",        // 156   p1-p2.CLM
"ост.Vol В.М.кл.2",        // 157
"Ном.расх.В.Н.кг/ч",         // 158
"Масса 1,кг  ",            // 159
"Расх.1,кг/ч. ",           // 160
"Масса 2,кг  ",            // 161
"Расх.2,кг/ч. ",           // 162
"Масса 3,кг  ",            // 163
"Расх.3,кг/ч. ",           // 164
"Тип нефтепродукта",       // 165

//14.06.2021 YN
#if defined(DENSITY_CONTROL)
"Допуст.ошибка N1  ",      //166
"Допуст.ошибка N2  ",      //167
"Допуст.ошибка N3  ",      //168
"Контроль плотн-ти ",      //169
"Время ожидания    ",      //170
"Процент замедл N1 ",      //171
"Процент замедл N2 ",      //172
"Процент замедл N3 ",      //173
#else
"",                        //166
"",                        //167
"",                        //168
"",                        //169
"",                        //170
"",                        //171
"",                        //172
"",                        //173
#endif

//20.08.2021 YN
#if defined(UNDERPRESSURE)
"Разряж.  N анлг.вх",      //174
"Разряж. шкала, МПа",      //175
"Разряж. смеще. МПа",      //176
"Контроль разряж.  ",      //177
"Макс.допуст.разр. ",      //178
"Время ожидания up ",      //179
"Умен.ном.расх.N1% ",      //180 
"Умен.ном.расх.N2% ",      //181
"Умен.ном.расх.N3% ",      //182
#else
"",                        //174
"",                        //175
"",                        //176
"",                        //177
"",                        //178
"",                        //179
"",                        //180
"",                        //181
"",                        //182
#endif

//18.11.2021 YN
#if defined(weightedAverageTemperature)
"Средневзвеш.темп.",       //183
#else
"",                        //183
#endif

"",
};
//-------------------------------

char *list2_dsr[]={
" Колич.данных",            //  0
" Знач.АЦП,дискр.",         //  1
" Знач.Давл.,МПа",          //  2
" Знач.Темп.,C",            //  3
};
//-------------------------------
//#define mmi_page   page_MMI_c
#define mmi_page   page_MMI
char mmi_point=0;
//#define size_max 29
int mmi_prm=0;
int mmi_flag=0;

int num_page=0.;
int sw_mmi=0;
int mmi_str=0;
int mmi_com=1;
int flag_chg=0;
int nn_chg=0;
unsigned int pass_cnt;
int max_par=1;
#define NumPlace 22

int FlagWinSum=0;

struct arch_str_t adate11;

struct dis_set_MMI *ds_list=NULL;


long int Password=5555;
long int PassW=0;
long int Password_m=0;
long int PassWm=0;
int k_ext=0;
int k_ext1=0;
float chg_nn_par=0,chg_old=0,chg_new=0;
float dose_dlvr=0;
int n_adc_mm=0;
int f_menu_MMI()
{
  time_t ltime1;
  time_t ltime2;
  int year,month,day,hour,min,sec;
  int key;
  char str[31];
  int i,j,k;
  int itmp;
  struct s_icp_dev *ICPl_DEV;
struct arch_str *ar_str_ptr;
long int ltmp;
 struct tm *tdat;

//   s_MVD[0].Press=( ADC[0] + analog_offset[0]) * analog_scale[0];
   sw_mmi_slv= sw_mmi;
   if((i=fstin())==fst_OK)
   {  // введено числовое значение
     key=DATA;  // признак окончания ввода строки
     sw_fst=0;
   }
   else if(i==fst_ESC)
   {
     key=DATA_ESC;  // признак окончания ввода строки
     sw_fst=0;
   }
   else if( MmiKbhit())
   {  // ввод поступившего символа из клавиатуры
    key=MmiGetch();
   }
   else if(sw_fst==0)
   {
    key=PASS; // признак холостого прохода (без введенного символа)
              // используется для индикации изменяющихся значений
   }
   else return 0;

   switch(sw_mmi)
    {
     case 0: /*заголовок*/


//---------------------------------
       if((key==F2) && (id_ok !=0) )
        {
         flag_esc=0;
         PageD=0;
         f_prn_begin();
        }
       else if(key==F3)
        {
         flag_esc=0;
         PageD=1;
         f_prn_begin();
        }
       else if(key==F1)
        {
         flag_esc=0;
         f_prn_begin();
        }
       else if ((key==F2) && (id_ok ==0) )
        {
       m_0f2:
          flag_esc=1;
          MmiGotoxy(1,1);
          f_clr_scr_MMI();

           chg_nn_par=Par_ID;
           chg_old=n_id;
          MmiPuts("Введите ID:" );
          sprintf( fst_str,"%8ld",n_id);
          fst_n=9;
          sw_fst=1;
          MmiGotoxy(10,3);
  //      MmiGotoxy(15,3);
          SetDisplayPage(15); // ввод значения
          break;
        }
       else if(key==DATA)
        {
          flag_esc=1;
          sscanf(fst_str,"%ld",&n_id);
          if(n_id<0) { n_id=0; goto m_0f2;}

           chg_new= n_id;
           f_wr_evt(evt_chg_param);
           if( f_check_id(n_id) == 0)
           {
             id_ok=1;
             if(f_wrt_eee()<0)
             {
               f_wr_evt(evt_we_err);
             }
             else
             {
               f_wr_evt(evt_we);
             }
           }
           else
           {
            id_ok=0;
           }
          goto mm00;
        }
       else if(key==DATA_ESC)
        {
          flag_esc=1;
          goto mm00;
        }
   /*
       m_0f2:
          MmiGotoxy(1,1);
          f_clr_scr_MMI();
          MmiPuts("Введите номер страницы:" );
          sprintf( fst_str,"%d",num_page);
          fst_n=2;
          sw_fst=1;
//        MmiGotoxy(10,3);
          MmiGotoxy(15,3);
          SetDisplayPage(15); // ввод значения
          break;
        }
       else if(key==DATA)
        {
          sscanf(fst_str,"%d",&num_page);
          if(num_page<0) { num_page=0; goto m_0f2;}
          else if(num_page > 63) {num_page=63; goto m_0f2;}

          SetDisplayPage((int)num_page);
          sw_mmi=100;
          f_clr_scr_MMI();
          break;
        }
       else if(key==DATA_ESC)
        {
          goto m0_f1;
        }
      */
        else
//---------------------------------
        if (key==Sht_ESC)
        {  // очистка ошибок

          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);
          sw_mmi=26;
          f_cl_error();
          MmiGotoxy(5,2);    MmiPuts("   Ошибки очищены   ");
          MmiGotoxy(0,4);    MmiPuts(" Enter - продолжить ");
          flag_esc=1;

        }
        else if (key==Sht_F1)
        {  // распечатка ошибок

          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);
          sw_mmi=26;
          f_disp_error();
          flag_esc=1;
        }
        else if(key==Sht_F2)
        {  // MVD zeroing

          flag_esc=1;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);

          if(f_reset_zer_MVD() == 0)
          {
          MoveToXY(0,3);
          MmiPrintf(" Ошибка запуска калибровки ");
          f_wr_evt(evt_MVD_CL_BS2);
  m_SF2:
          MmiGotoxy(0,6); MmiPuts("ESC,ENTER  Возврат");
          sw_mmi=-1;
          break;
          }
          f_wr_evt(evt_MVD_CL_strt);
          sw_mmi=18;
          zero_time=TimeStamp;
          fl_zeroing=1;
          break;
        }
       else if(key==ESC)
       {

        PassWm=0;


        flag_esc=1;
        if(mode_hst == 0 )
        {
m0_f1:
main_menu:


m_120p:

         if(Password_m != 0)
          if(PassWm != Password_m)
          {
           pass_ret=20;
           goto m_pass;
          }

          PageD=0;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);
          f_prepare_t (&adate00);
          sw_mmi=120;


          Out_off(OUT4);  // Air switch off
          PassW=0;

            MmiGotoxy(0,0);    MmiPuts("     Меню ");


            MmiGotoxy(0,1);


#if defined(BIO_1)

  MmiPuts("1  Отпуск биокомпонента");
            MmiGotoxy(0,2);    MmiPuts("2  Источник продукта ");
#else
         MmiPuts("1  Отпуск нефтепродуктов");
//          MmiGotoxy(0,2);    MmiPuts("2  Прокачка ");
//            MmiGotoxy(0,2);    MmiPuts("            ");
            MmiGotoxy(0,2);    MmiPuts("2  Показания расходомера");
#endif

//          MmiGotoxy(0,2);    MmiPuts("2  Версия ПО ");
            MmiGotoxy(0,3);    MmiPuts("3  Просмотр журнала событий");
            MmiGotoxy(0,4);    MmiPuts("4  Сервисные функции");
#if !defined(BIO_1)
            MmiGotoxy(0,5);    MmiPuts("5  Ввод кода доступа");
#endif
            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");
        }
        else
        {
           f_clr_scr_MMI();
           f_prn_begin();
        }

        break;
       }
       else if (key=='-')
        {
          flag_esc=0;
          if(PageD==1)
          { // включить симуляцию OFP
             f_OFP_WR(0,7,1);
             INP_OFP=0xc0ff;
             key=PASS;
          }
          break;
        }
//     else if (key=='0')
       else if (key=='5')
        {
          flag_esc=0;
          if(PageD==1)
          { // выключить симуляцию OFP
             f_OFP_WR(0,7,2);
             INP_OFP=0x0;
          }
          break;
        }
       else if(key==PASS)
        {  // отображение даты и времени

         if( id_ok ==0) break;
         if( ((FL_err|CRC_err) == 0) || ( flag_esc == 0 ))
         {
          if(PageD==0)
            {
               MmiGotoxy(14,3);  MmiPrintf("%9.2f кг    ",s_MVD[0].MassI);
               MmiGotoxy(14,4);  MmiPrintf("%9.2f л     ",s_MVD[0].VolI);
#if defined(BIO_1)
               MmiGotoxy(5,5);
  MmiPrintf("%6.4f МПа ",P_inp);

               MmiGotoxy(20,5);
  MmiPrintf("%6.4f МПа ",s_MVD[0].Press);
#else
               MmiGotoxy(3,5);
  MmiPrintf("%6.4f МПа ",s_MVD[0].Press);
               MmiGotoxy(17,5);  MmiPrintf("%6.2f град.C",s_MVD[0].TempR);
#endif

              // MmiGotoxy(18,4);
              // f_prn_comps();
            }
          else if(PageD==1)
            {  // OFP
         if( ((FL_err|CRC_err) == 0) || ( flag_esc == 0 ))
          {

           MmiGotoxy(0,3);     MmiPuts("   Контроллер переполнения   ");
           if(OFP[0].status==0)
           {
            MmiGotoxy(0,4);MmiPuts( str_empt );
            MmiGotoxy( 10 ,4);         MmiPuts("Отключен ");
            MmiGotoxy(0,5);MmiPuts( str_empt );
           }
           else if((INP_OFP & 0xC000) == 0xC000)
           {
            MmiGotoxy(0,4);MmiPuts( str_empt );
            MmiGotoxy(10,4);    MmiPuts("Эмуляция");
            MmiGotoxy(0,5);MmiPuts( str_empt );
            MmiGotoxy(0,5);MmiPuts("5 - норм.режим");
           }
           else
           {
           MmiGotoxy(0,4);MmiPuts( str_empt );
           MmiGotoxy(3,4);
            if(INP_OFP & 0x8000)      MmiPuts("5-ти проводная система" );
            else if(INP_OFP & 0x4000) MmiPuts("2-х проводная система" );
                                else  MmiPuts("Система не определена");

           MmiGotoxy(0,5);MmiPuts( str_empt );
           MmiGotoxy(0,5);
            if(INP_OFP & 0x80) MmiPuts("GND : Разреш." );
            else   MmiPuts("GND : Запрещ." );


           MmiGotoxy(15,5);

            if((INP_OFP & 0xc000) && (INP_OFP & 0x80))
            {
              if(INP_OFP & 0x40)
              {
                MmiPuts("ПЕРЕП.: Разреш." );
              }
              else
              {

                MmiPuts("ПЕРЕП.: Запрещ." );
           /*
                EkdMoveToXY(0,13);EkdPrintf("WET PROBE(S): " );
                EkdMoveToXY(0,14);
                 if((INP_OFP & 0x1)==0) EkdPrintf(" N1");
                 if((INP_OFP & 0x2)==0) EkdPrintf(" N2");
                 if((INP_OFP & 0x4)==0) EkdPrintf(" N3");
                 if((INP_OFP & 0x8)==0) EkdPrintf(" N4");
           */
              }
            }
            else
            {
                MmiPuts("ПЕРЕП.: Запрещ." );
            }

           }
          }


            }
           else
           {
  MmiGotoxy(0,3) ; MmiPrintf("PageD = %d",PageD);
           }
         }
          GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
          MoveToXY(4,6);
          if((WD_PMP_Fl != 0) && (flag_Slv != 0))
            MmiPrintf("%02d.%02d.%02d   %02d:%02d:%02d  Link " ,day,month,year-2000 ,hour,min,sec);
          else
            MmiPrintf("%02d.%02d.%02d   %02d:%02d:%02d       " ,day,month,year-2000 ,hour,min,sec);
        }

        break;
 /*========================================*/
    case 11:  /*меню выбора точки для просмотра параметров */
       n_adc_mm=0;
       if(key==ESC)    /* переход в меню M3, Сервисные функции */
       {
         goto m_m3;
       }
       else if(key=='1')  // Базовый список #1
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=0;
        }
       else if(key=='2')  // Базовый список #2
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=max_par/5;
        }
       else if(key=='3')   // Базовый список #3
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=(max_par*2)/5;
        }
       else if(key=='4')  // Базовый список #4
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=(max_par*3)/5;
        }
       else if(key=='5')  // Дополнительный список
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=(max_par*4)/5;
        }
       else break;
       flag_chg=0;
       nn_chg=0;

       f_clr_scr_MMI();
       SetDisplayPage(EmptPage);    // Чистая страница
  par_out14:
       for(i=0;i<n_mmi_str_1;i++)
       {

        k=mmi_prm+i;
        if(k>= max_par) continue;
        MoveToXY(0,i);
           MmiPrintf("%2d %s",k+1,ds_list[k].descr[ds_list[k].ind]);
        MoveToXY(NumPlace,i);
           PrintMmiOp(&ds_list[k],1);
       }
       if(flag_chg==0)
       {
         MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"Enter\"Изм. ");
       }
       else
       {
         MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\",\"F3\",\"ESC\",\"Enter\"         ");
         MmiGotoxy(2,nn_chg);  MmiPuts("#");
       }
       sw_mmi=14;
       break;
 /*========================================*/
    case 14:/* просмотр до 6 параметров */
       if(key==Sht_F2)    /* сохранение изменений в EEPROM */
       {
        f_clr_scr_MMI();
        SetDisplayPage(EmptPage);    // Чистая страница
        MmiGotoxy(0,1);    MmiPuts("Сохранение данных в EEPROM");
        MmiGotoxy(0,3);    MmiPuts(" 1    Сохранить");
        MmiGotoxy(0,4);    MmiPuts("ESC   Не сохранять,продолжить");
        sw_mmi=15;
        break;
       }
       if(key==Sht_F1)    /* инит COR */
       {
//        f_init_COR(0);
//        f_init_COR(1);
        f_clr_scr_MMI();
        SetDisplayPage(EmptPage);    // Чистая страница
        goto par_out14;
       }
       if(key==Sht_F3)    /* чтение данных из EEPROM */
       {
        f_clr_scr_MMI();
        SetDisplayPage(EmptPage);    // Чистая страница
        MmiGotoxy(0,1);    MmiPuts("Чтение данных из EEPROM");
        MmiGotoxy(0,3);    MmiPuts(" 1    Прочитать ");
        MmiGotoxy(0,4);    MmiPuts("ESC   Не считывать,продолжить");
        sw_mmi=16;
        break;
       }
       if(flag_chg==0)
       {
         if(key==ESC)    /* переход в меню выбора точки для просмотра*/
         {
          goto m_m3_3;
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=n_mmi_str_1)
           {
            mmi_prm -=n_mmi_str_1;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto par_out14;
           }
           else
           {
            mmi_prm = 0;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto par_out14;
           }
         else if((key== F3 )&& (mmi_prm < (max_par-n_mmi_str_1)) )
         {
          mmi_prm +=n_mmi_str_1;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto par_out14;
         }
         if(key== ENTER)
         {
          flag_chg=1;

          if((nn_chg+mmi_prm) >= max_par)
             nn_chg=max_par-mmi_prm-1;
          goto par_out14;
         }
       }
       else  // flag_chg!=0
       {
         if(key==ESC)    // очистка флага редактирования
         {
          flag_chg=0;
          if((ds_list[0].descr == list2_dsr)&&
               (ds_list[0].ind == 0 ))
          {
//          f_init_COR(0);
//          f_init_COR(1);
            f_clr_scr_MMI();
       //   SetDisplayPage(EmptPage);    // Чистая страница
            goto par_out14;
          }
          goto par_out14;
         }
         if(key== F1 )
         {
           if((ds_list[nn_chg+mmi_prm].descr == list2_dsr)&&
               (ds_list[nn_chg+mmi_prm].ind == 1 ))
           {
            if(PassW != Password)
               goto m_pass;
//    printf("\r\n op=%g, ADC[%d]=%g ",*(float*)ds_list[nn_chg+mmi_prm].op,n_adc_mm,ADC[n_adc_mm] );
            *(float*)ds_list[nn_chg+mmi_prm].op=ADC[n_adc_mm];
    //      f_init_COR(0);
    //      f_init_COR(1);
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto par_out14;
           }
           else
               break;
         }
         if((key== F2 )&& (nn_chg>0) )
         {
          nn_chg -=1;
          goto par_out14;
         }
         else if((key== F3 )&& (nn_chg < n_mmi_str_2) )
         {
          nn_chg +=1;

          if((nn_chg+mmi_prm) >= max_par)
             nn_chg=max_par-mmi_prm-1;
          goto par_out14;
         }
         if(key==ENTER)    // редактирование параметра
         {
          if(PassW != Password)
          {
           pass_ret=0;
m_pass:
           MmiGotoxy(1,1);
           f_clr_scr_MMI();
           MmiPuts("Введите пароль:" );
           sprintf( fst_str,"******");
           SetDisplayPage(17);    // Ввод пароля
           sw_mmi=121;
           fst_n=7;
           sw_fst=1;
           MmiGotoxy(11,3);
//         MmiGotoxy(15,3);
           break;
          }

          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
          MmiGotoxy(2,nn_chg);  MmiPuts(">");
          MoveToXY(NumPlace,nn_chg);

          PrintMmiOp(&ds_list[nn_chg+mmi_prm],0);
          fst_n=8;
          sw_fst=1;
          break;
         }
       }
       if(key==DATA)    // ввод полученных данных
       {
         ScanfMmiOp(&ds_list[nn_chg+mmi_prm]);
         f_after_MMI();
         chg_nn_par=nn_chg+mmi_prm+1;
         f_wr_evt(evt_chg_param);
m_14_ni:
         MoveToXY(NumPlace,nn_chg); MmiPuts("         ");
         goto par_out14;
       }
       else if(key==DATA_ESC)
        {
          goto m_14_ni;
        }
       break;
 /*========================================*/
    case 15:/* сохранение данных в EEPROM */
         if((key==ESC)||(key==ENTER)  )   /* возврат к отображению параметров */
         {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto par_out14;
         }
         else if(key=='1')    // сохранить данные
         {

           if(f_wrt_eee()<0)
           {
              f_wr_evt(evt_we_err);
              MmiGotoxy(0,5);   MmiPuts("Ошибка при сохранении данных");
           }
           else
           {
              f_wr_evt(evt_we);
              MmiGotoxy(0,5);   MmiPuts(" Данные сохранены успешно");
           }
              MmiGotoxy(0,6);   MmiPuts("      Enter - продолжить");
         }
         break;
 /*========================================*/
    case 16:/* чтение данных из EEPROM */
         if((key==ESC)||(key==ENTER)  )   /* возврат к отображению параметров */
         {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto par_out14;
         }
         else if(key=='1')    // прочитать
         {

           f_chk_EEE_CRC();
           if(f_rd_eee()<0)
           {
              f_wr_evt(evt_re_err);
              MmiGotoxy(0,5);   MmiPuts("Ошибка при чтении данных");
              CRC_err=1;
           }
           else
           {
              f_wr_evt(evt_re);
              MmiGotoxy(0,5);   MmiPuts("  Данные считаны успешно");
              CRC_err=0;
           }
           f_rd_cor();

           MmiGotoxy(0,6);   MmiPuts("      Enter - продолжить");
         }
         break;
 /*========================================*/
    case 18:
       if( f_timer(zero_time,Time_zeroing ))
       {
             f_clr_scr_MMI();
             MoveToXY(3,3);
            MmiPrintf(" Таймаут калибровки");
          fl_zeroing=0;
          ff_serv=NULL;
          f_wr_evt(evt_MVD_CL_TM);
          State_SLV = Zer_err;
         f_reg_cmn(Zer_err_cmn);
          goto m_SF2;
       }
          if(ff_serv==NULL)
          {
           if(fl_zeroing==0)
           {
             f_clr_scr_MMI();
             MoveToXY(1,3);
         MmiPrintf("Калибровка завершена успешно");
         MmiGotoxy(0,1);MmiPrintf("D=%6.1f кг/м3; t =%6.2f C ",s_MVD[0].Dens,s_MVD[0].TempR);

         State_SLV = Zer_end;
         f_reg_cmn(Zer_end_cmn);

//                |                              |
             f_wr_evt(evt_MVD_CL_OK);
             goto m_SF2;
           }
           fl_rd_err=0;
           sw_mvd_m1=0;  // нач.параметр
           ff_serv=f_MVD_M3;
          }

         MmiGotoxy(0,1);MmiPrintf("D=%6.1f кг/м3; t =%6.2f C ",s_MVD[0].Dens,s_MVD[0].TempR);

         GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
         MoveToXY(4,6);
         MmiPrintf("%02d.%02d.%02d   %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);
         break;
 /*========================================*/
    case 120:  // Основное меню

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
mm00:      sw_mmi=0;
           PassWm=0;

           PageD=0;
//         SetDisplayPage(6);
           SetDisplayPage(ZeroPage);
           f_clr_scr_MMI();
           f_prn_begin();
           break;
       }
#if defined(BIO_1)
       else if((key== '/') && (id_ok !=0) ) // Sht_2  Прокачка
       {
        flag_prok=1;
        strcpy(mode_prok,prok_ka);
        Flag_Low=-1;
        goto m1_1;
       }
       else if((key== '2') && (id_ok !=0) ) // 2  Источник продукта
       {
mm2_b0:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
mm2_b:
            MmiGotoxy(0,0);    MmiPuts(" Меню 2. Источники продукта.");

            MmiGotoxy(0,1);    MmiPuts("0..3 Режим источников  ");
//           MmiGotoxy(0,2);    MmiPuts("4 Режим насоса         ");
            MmiGotoxy(0,2);    MmiPuts("4 Задержка выключения насоса  ");

       itmp= OUT_or & ( CL1_SRC | CL2_SRC);

       MmiGotoxy(0,4);
       if ( itmp == CL1_SRC   )
              MmiPuts(" Подключена емкость 1  ");
       else if ( itmp == CL2_SRC   )
              MmiPuts(" Подключена емкость 2  ");
       else if ( itmp == (CL1_SRC | CL2_SRC)  )
              MmiPuts(" Подключены емкости 1,2");
       else
              MmiPuts(" Емкости отключены     ");

       MmiGotoxy(0,5);
          MmiPrintf("Задержка выкл.насоса %6.0f с",T_Pmp_delay/1000.);
/*
       if(OUT_Pmp & Pump_MDB)
           MmiPuts(" Насос включен непрерывно    ");
       else
           MmiPuts(" Насос включается при отпуске");
*/

            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");
         sw_mmi=130;
         break;
       }
#else
       else if((key== '2') && (id_ok !=0) ) // 2  Прокачка
       {
 /*
        flag_prok=1;
        strcpy(mode_prok,prok_ka);
        Flag_Low=-1;
        goto m1_1;

 */

        goto m_4_4_1;

       }
#endif

       else if((key== '1') && (id_ok !=0) ) //  1  Отпуск нефтепродуктов
       {
//       sw_mmi=190;
#if defined(BIO_1)
 /// nothing
#else
                   flag_prok=0;
#endif
        strcpy(mode_prok,otpusk);
        Flag_Low=-1;
m1_1:
        if(mode_hst == 0 ) //  1  Отпуск нефтепродуктов
        {
         MassStamp=s_MVD[0].MassI;
         VolStamp=s_MVD[0].VolI;
         dose_dlvr=0;
         flag_dlv_fst=0;
         flag_prn_mass=0;
         PageD=0;
        }
         f_prepare_t ( &adate00);
m_m5:
         flag_AD=0;
         if(valve_mode > 0)
               Out_on(OUT4);  // Air switch on
         SetDisplayPage(EmptPage);

m_m5_01:
         f_clr_scr_MMI();

    MmiGotoxy(0,0);

#if defined(BIO_1)
    MmiPrintf("Меню 1.%s биокомпонента.",mode_prok);
#else
    MmiPrintf("Меню %d.%s нефтепродуктов.",flag_prok+1,mode_prok);
#endif

    if((dose_dlvr > 0) && (flag_prn_mass == 0))
    {
         MmiGotoxy(0,1);
         if( (valve_mode > 0) && (valve_modeL > 0 ))
         {
            if( Flag_Low == 0)
               MmiPuts("Доза Верхн.налива ");
            else  if( Flag_Low == 1)
               MmiPuts("Доза Нижн. налива ");
         }
         else
               MmiPuts("Доза  ");


              if ( Flag_mass != 0 )
                MmiPrintf("%8.2f кг    ",dose_dlvr);
              else
                MmiPrintf("%8.2f л    ",dose_dlvr);

    }
 if((id_ok !=0)&& (mode_hst == 0) )
 {
      //    f_prn_MassSt_dlv();

    if( (valve_mode > 0) && (valve_modeL > 0 ))
    {
          MmiGotoxy(0,2);    MmiPuts("1 Задать дозу верхнего налива");
          MmiGotoxy(0,3);    MmiPuts("2 Задать дозу нижнего  налива");

          if((flag_prn_mass != 0) && (dose_dlvr > 0) )
          {  MmiGotoxy(0,4);
            if( Flag_Low == 0)
               MmiPuts("4 Продолжить Верхн.:");
            else  if( Flag_Low == 1)
               MmiPuts("8 Продолжить Нижн.:");

              if ( Flag_mass != 0 )
                MmiPrintf("Д=%8.2f кг    ",dose_dlvr);
              else
                MmiPrintf("Д=%8.2f л    ",dose_dlvr);

            MmiGotoxy(0,5);
        //    MmiGotoxy(15,5);
            MmiPrintf("О=%8.2f кг,%8.2f л ",s_MVD[0].MassT,s_MVD[0].VolT);
          }
          else if(dose_dlvr > 0)
          {
            MmiGotoxy(0,4);
            if( Flag_Low == 0)
               MmiPuts("4 Старт Верхний налив");
            else if( Flag_Low == 1)
               MmiPuts("8 Старт Нижний налив ");
          }


    }
    else
    {
          MmiGotoxy(0,2);    MmiPuts("1 Задать дозу ");

          if((flag_prn_mass != 0)   && (dose_dlvr > 0))
          {  MmiGotoxy(0,4);
              MmiPuts("4 Продолжить :");
              if ( Flag_mass != 0 )
                MmiPrintf("Д=%8.2f кг    ",dose_dlvr);
              else
                MmiPrintf("Д=%8.2f л    ",dose_dlvr);

            MmiGotoxy(0,5);
        //    MmiGotoxy(15,5);
            MmiPrintf("О=%8.2f кг,%8.2f л ",s_MVD[0].MassT,s_MVD[0].VolT);
          }
          else if(dose_dlvr > 0)
          {
            MmiGotoxy(0,4);
               MmiPuts("4 Старт ");
          }

    }
 }
//--------------------------
     if( Flag_Low < 0)
     {
       f_prn_no_sig_H( 4) ;
       f_prn_no_sig_L( 5) ;
     }
    else if( Flag_Low == 0)
       f_prn_no_sig_H(4) ;
    else if( Flag_Low == 1)
       f_prn_no_sig_L(4) ;

//--------------------------

#if defined(BIO_1)

          MmiGotoxy(0,6);    MmiPuts("ESC Возврат");
#else
         MmiGotoxy(0,6);
        if( ( valve_mode > 0) && ( Flag_Low != 1)  )
            MmiPuts("0-парам.нач.нап.; ESC Возврат");
        else
            MmiPuts("ESC Возврат");

#endif
         sw_mmi=190;
         break;
       }
       else if(key== '3') // 3  Просмотр журнала событий
       {
m_m2:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();

            i= f_get_max_n_arch();
            ar_str_ptr=f_get_fst_stor();
            MmiGotoxy(0,0);    MmiPuts(" Меню 3.Журнал событий.");
            MmiGotoxy(0,1);    MmiPrintf("%4d  ",i);
            j=i%100;
            if((j<=20)&& (j>5))
              MmiPrintf("записей");
            else
            switch(i%10)
            {
             case 1:
             MmiPrintf("запись");
             break;

             case 2:
             case 3:
             case 4:
             MmiPrintf("записи");
             break;

             case 0:
             case 5:
             case 6:
             case 7:
             case 8:
             case 9:
             MmiPrintf("записей");
             break;
            }

   tdat = localtime( &ar_str_ptr->time );
//   MmiPrintf(" с %02d.%02d.%02d %02d:%02d:%02d",
//   tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);
   MmiPrintf(" с %02d.%02d.%02d",
   tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100);

            MmiGotoxy(0,2);    MmiPuts(" Просмотр журнала событий");
            MmiGotoxy(0,3);    MmiPuts("1  По дате");
            MmiGotoxy(0,4);    MmiPuts("2  По номеру записи");
            MmiGotoxy(0,5);    MmiPuts("3  Последние записи");
            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");

         PassW=0;
         sw_mmi=140;
         break;
       }
       else if(key== '4') // 4  Сервисные функции
       {
m_m3:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Меню 4.Сервисные функции.");
            MmiGotoxy(0,1);    MmiPuts("1  Список драйверов");
            MmiGotoxy(0,2);    MmiPuts("2  Статистика  драйверов");
            MmiGotoxy(0,3);    MmiPuts("3  Просмотр/ввод параметров");
            MmiGotoxy(0,4);    MmiPuts("4  Другие функции");
            MmiGotoxy(0,5);    MmiPuts("5  Время, дата");
            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");
         sw_mmi=150;
         PassW=0;
       }

       else if(key== '5') // 5  ввод кода доступа автомобиля
       {
          sw_mmi=200;
          break;
       }
       else if(key== '+') // Sht_5  ввод типа налива
       {
          sw_mmi=210;
          break;
       }
       else if(key== '6') // 6 Отображение аналоговых входов
       {
m_m6:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
   MmiGotoxy(0,0); MmiPrintf(" Состояния аналоговых входов:");
   MmiGotoxy(0,1); MmiPrintf("    I7017C, дискреты АЦП");
// MmiGotoxy(0,1); MmiPrintf(" xxx.xx xxx.xx xxx.xx xxx.xx");// 20./3276.8=0.006103515625


// MmiGotoxy(0,4); MmiPrintf(" Состояния дискретных входов:");

//_fmemset( &ADC_c[0],0xff,sizeof(ADC)); // очистка буфера экрана
  INP_VAR_c=0xffff;
         sw_mmi=25;
         break;

       }
       else if(key== '7') // 7 Отображение дискр.выходов
       {
m_m7:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
   MmiGotoxy(0,0); MmiPrintf(" Состояния дискретных выходов:");
   MmiGotoxy(0,6);

#if defined(BIO_1)
  MmiPuts(" ESC-возврат,1...5 - изменить");
#else
  MmiPuts(" ESC-возврат,1...7 - изменить");
#endif
         OUT_VAR_c=0xffff;
         sw_mmi=28;
         break;

       }
       else if(key== '8') // 8 Версия sw
       {
m_m8:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();

      MmiGotoxy(0,0);   MmiPrintf("   VER %s",sw_ver);
      MmiGotoxy(0,2);   MmiPrintf("   MD5 counting ...");
      MmiGotoxy(0,6);   MmiPuts(" ESC   - возврат в меню");
         tm_md=TimeStamp;
         sw_mmi=271;
         break;
       }
       else if(key== '-' )// SHT_9  Коррекция расхода
       {
m_m9:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Меню S_9.Коррекция расхода.");
            MmiGotoxy(0,1);    MmiPuts("1 Коррекция массового расхода");
            MmiGotoxy(0,2);    MmiPuts("2 Коррекция объемного расхода");
            MmiGotoxy(0,3);    MmiPuts("3 Множитель плотности");
  //        MoveToXY(0,4);   MmiPrintf("Множитель массы:     %.6f",s_MVD[0].f);
  //        MoveToXY(0,5);   MmiPrintf("Множитель объема:    %.6f",s_MVD[0].f);
  //        MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");

         Flag_f_corr=0;
         sw_mvd_m1=31;
         tm_mvd=TimeStamp;
         ff_serv=f_MVD_M1f;
         sw_mmi=125;
         break;
       }
       else if(key== '9' )// 9  Срез расхода
       {
m_m9s:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Меню 9.Срез расхода.");
            MmiGotoxy(0,1);    MmiPuts("1 Срез массового расхода");
            MmiGotoxy(0,2);    MmiPuts("2 Срез объемного расхода");
  //        MoveToXY(0,4);   MmiPrintf("Множитель массы:     %.6f",s_MVD[0].f);
  //        MoveToXY(0,5);   MmiPrintf("Множитель объема:    %.6f",s_MVD[0].f);
            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");

         Flag_f_corr=0;
         sw_mvd_m1=31;
         tm_mvd=TimeStamp;
         ff_serv=f_MVD_M1s;
         sw_mmi=127;
         break;
       }
       break;
 /*========================================*/
    case 121: // // ввод пароля на изменение данных
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница

 m121:
          switch(pass_ret)
          { // пароль не введен
          case 0:  goto par_out14;
          case 1:  goto m_m9;
          case 2:  goto m_m9;
          case 3:  goto m_m3;
          case 5:  goto m_m9s;
          case 6:  goto m_m9s;
          case 7:  goto m_m9;
          case 20: goto m_main;

          default:
                goto main_menu;
          }
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%ld",&ltmp);

         if(pass_ret < 20)
         {
          if(ltmp == Password)
                PassW=Password;
          else goto m_pass;
         }
         else
         {
          if(ltmp == Password_m)
                PassWm=Password_m;
          else goto m_pass;
         }


          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          switch(pass_ret)
          { // пароль введен корректно
          case 0:  goto par_out14;
          case 1:
                sw_mmi=125;
                goto m_m9_1;
          case 2:
                sw_mmi=125;
                goto m_m9_2;
          case 7:
                sw_mmi=125;
                goto m_m9_3;
          case 3:
                sw_mmi=162;
                goto m_m3_6_1;
          case 5:
                sw_mmi=127;
                goto m_m9s_1;
          case 6:
                sw_mmi=127;
                goto m_m9s_2;
          case 20:
                sw_mmi=0;
                goto m_120p;
          default:
                goto main_menu;
          }
        }
       else if(key==DATA_ESC)
        {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m121;
        }

 /*========================================*/
    case 122:
 //  Отображение Плотности,температуры,состава
 //  Текущее значение.
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {

         s_MVD[0].r_addr=195; // Срез масс.расх.
         s_MVD[0].f = cutoff_off_M;
         MVD_fn[0]=MVD_WR_F;;


         State_SLV = Mod_flow_e;
         f_reg_cmn(Mod_flow_e_cmn);

         goto main_menu;
       }
       else if(key==PASS)
       {
         f_prn_den_t();
       }
       else if (key==DEL)
       {
          MVD_fn[0]=MVD_WR_C;
          s_MVD[0].r_addr=3; // Reset all totalizers
          s_MVD[0].i=1;
       }
       else if (key=='*')
       {
          MVD_fn[0]=MVD_WR_C;
          s_MVD[0].r_addr=2; // разрешение счета
          s_MVD[0].i=1;
       }
       else if (key=='/')
       {
          MVD_fn[0]=MVD_WR_C;
          s_MVD[0].r_addr=2; // запрет счета
          s_MVD[0].i=0;
       }
       else if(key==Sht_F1)
       {
        if(flg_ch==2)
        {
          MVD_fn[0]=MVD_WR_C;
          s_MVD[0].r_addr=04; // Reset all inventories
/*
          s_MVD[0].r_addr=56; // Reset mass total
          s_MVD[0].r_addr=04; // Reset all inventories
          s_MVD[0].r_addr=57; // Reset volume totales
*/
          s_MVD[0].i=1;
        }
       }
       else if(key==Sht_F2)
       {
        if(flg_ch==1) flg_ch++;
        else flg_ch=0;
       }
       else if(key==Sht_F3)
       {
        if(flg_ch==0) flg_ch++;
        else flg_ch=0;
       }
       else flg_ch=0;
       break;
 /*========================================*/
    case 125:
 //  Меню 9.Коррекция расхода.
       if((key==ESC)||(key==ENTER) )    /* переход в меню */
       {
         sw_fst=0;
         ff_serv=NULL;
         goto main_menu;
       }

       if((key=='1')&&(Flag_f_corr!=0))
       {

          if(PassW != Password)
          {
           pass_ret=1;
           goto m_pass;
          }

m_m9_1:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите коррекцию массы:" );
          k_mv_tmp=k_m_flow;
          flag_mass=1;
m_m9_1_1:

          MmiGotoxy(0,n_mmi_str_2);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"ESC - возврат");
          sprintf( fst_str,"%.6f",k_mv_tmp);
          fst_n=9;
          sw_fst=1;
          MmiGotoxy(10,3);
          break;
       }
       if((key=='2')&&(Flag_f_corr!=0))
       {
          if(PassW != Password)
          {
           pass_ret=2;
           goto m_pass;
          }
m_m9_2:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите коррекцию объема:" );
//        MmiPuts("Введите срез масс.расх.кг/ч:" );
          k_mv_tmp=k_v_flow;
          flag_mass=0;
          goto m_m9_1_1;
       }
       if((key=='3')&&(Flag_f_corr!=0))
       {
          if(PassW != Password)
          {
           pass_ret=7;
           goto m_pass;
          }
m_m9_3:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите множитель плотн.:" );
          k_mv_tmp=k_dens;
          flag_mass=2;
          goto m_m9_1_1;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%f",&k_mv_tmp);

         if(flag_mass==1)
         {
          if((k_mv_tmp<0.8)||(k_mv_tmp>1.2) )
                goto m_m9_1;
         }
         if(flag_mass==2)
         {
          if((k_mv_tmp<0.8)||(k_mv_tmp>1.2) )
                goto m_m9_3;
         }
         else
         {
          if(k_mv_tmp<0) goto m_m9_2;
         }

      // введенное значение в k_mv_tmp
          if(flag_mass==1)
          {
           s_MVD[0].r_addr=279; // Множитель массы.

           chg_nn_par=Par_MF;
           chg_old=k_m_flow;
           chg_new= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          else if(flag_mass==2)
          {
           s_MVD[0].r_addr=283; // Множитель плотности

           chg_nn_par=Par_DN;
           chg_old=k_dens;
           chg_new= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          else
          {
           s_MVD[0].r_addr=281; // Множитель объема.
           chg_nn_par=Par_VF;

           chg_old=k_v_flow;
           chg_new= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          MVD_fn[0]=MVD_WR_F;
          s_MVD[0].f = k_mv_tmp;
          goto m_m9;
        }
       else if(key==DATA_ESC)
        {
          goto m_m9;
        }

       break;
 /*========================================*/
    case 126:
 //  Меню 1.4.Коррекция параметров начального наполнения
       if((key==ESC)||(key==ENTER) )    /* переход в меню */
       {
         goto m_m5;
       }

     if(valve_mode != 1)
       if(key=='2')
       {

m_m5_4_1:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите начальный расход,кг/ч:" );
          k_mv_tmp=Flow_beg;
          flag_mass=1;
m_m5_4_1_1:

          MmiGotoxy(0,n_mmi_str_2);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("ESC-возврат,Enter-ввод данных");
          if(flag_mass!=0)
          {
           sprintf( fst_str,"%6.0f",k_mv_tmp);
           fst_n=7;
          }
          else
          {
           sprintf( fst_str,"%6.0f",k_mv_tmp);
           fst_n=7;
          }
          sw_fst=1;
          MmiGotoxy(10,3);
          break;
       }
       if(key=='1')
       {
m_m5_4_2:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите начальный объем,л :" );
          k_mv_tmp=Vol_beg;
          flag_mass=0;
          goto m_m5_4_1_1;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%f",&k_mv_tmp);

         if(flag_mass)
         {
          if((k_mv_tmp<0)|| (k_mv_tmp> 200000))
                goto m_m5_4_1;
         }
         else
         {
          if((k_mv_tmp<0)|| (k_mv_tmp> 400000))
                goto m_m5_4_2;
         }

      // введенное значение в k_mv_tmp
          if(flag_mass)
          {

           chg_nn_par=Par_FB;
           chg_old= Flow_beg;
           chg_new= k_mv_tmp;

           Flow_beg=k_mv_tmp; // начальный расход,кг/ч
           f_wr_evt(evt_chg_param);
          }
          else
          {
           chg_nn_par=Par_VB;

           chg_old=Vol_beg;
           chg_new= k_mv_tmp;
           Vol_beg=k_mv_tmp;   // начальный объем,л
           f_wr_evt(evt_chg_param);
          }
          goto m_m5_4;
        }
       else if(key==DATA_ESC)
        {
          goto m_m5_4;
        }

       break;
 /*========================================*/
    case 127:
 //  Меню 9S.Срез  расхода.
       if((key==ESC)||(key==ENTER) )    /* переход в меню */
       {
         sw_fst=0;
         ff_serv=NULL;
         goto main_menu;
       }

       if((key=='1')&&(Flag_f_corr!=0))
       {

          if(PassW != Password)
          {
           pass_ret=5;
           goto m_pass;
          }

m_m9s_1:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите срез м.расх.кг/ч:" );
          k_mv_tmp=k_m_flow;
          flag_mass=1;
m_m9s_1_1:

          MmiGotoxy(0,n_mmi_str_2);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"ESC - возврат");
          if(flag_mass!=0)
          {
           sprintf( fst_str,"%8.1f",k_mv_tmp);
           fst_n=9;
          }
          else
          {
           sprintf( fst_str,"%8.3f",k_mv_tmp);
           fst_n=9;
          }
          sw_fst=1;
          MmiGotoxy(10,3);
          break;
       }
       if((key=='2')&&(Flag_f_corr!=0))
       {
          if(PassW != Password)
          {
           pass_ret=6;
           goto m_pass;
          }
m_m9s_2:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите срез об.расх.л/сек:" );
          k_mv_tmp=k_v_flow;
          flag_mass=0;
          goto m_m9s_1_1;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%f",&k_mv_tmp);

         if(flag_mass)
         {
          if((k_mv_tmp<0)||(k_mv_tmp>100000.) )
                goto m_m9s_1;
         }
         else
         {
          if((k_mv_tmp<0)||(k_mv_tmp>1000.) )
                goto m_m9s_2;
         }

      // введенное значение в k_mv_tmp
          if(flag_mass)
          {
           s_MVD[0].r_addr=195; // Срез масс.расх.

           chg_nn_par=Par_MC;
           chg_old=k_m_flow;
           chg_new= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          else
          {
           s_MVD[0].r_addr=197; // Срез об. расх.
           chg_nn_par=Par_VC;

           chg_old=k_v_flow;
           chg_new= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          MVD_fn[0]=MVD_WR_F;;
          s_MVD[0].f = k_mv_tmp;
          goto m_m9s;
        }
       else if(key==DATA_ESC)
        {
          goto m_m9s;
        }

       break;

 /*========================================*/
#if defined(BIO_1)
    case 130: // Источник продукта
       if((key==ESC) || (key==ENTER))    /* переход в меню */
       {
           goto main_menu;
       }
        else if (key=='0')
        {
          OUT_or &= ~( CL1_SRC | CL2_SRC);
        }
        else if (key=='1')
        {
          OUT_or &= ~( CL1_SRC | CL2_SRC);
          OUT_or |=  CL1_SRC;
        }
        else if (key=='2')
        {
          OUT_or &= ~( CL1_SRC | CL2_SRC);
          OUT_or |=  CL2_SRC;
        }
        else if (key=='3')
        {
          OUT_or |=  (CL1_SRC | CL2_SRC);
        }
        else if (key=='4')
        {
//         OUT_Pmp  ^=  Pump_MDB;

m_m2_b:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите задержку выключения,с:" );
          k_mv_tmp=T_Pmp_delay/1000.;
m_m2_b_1:

          MmiGotoxy(0,n_mmi_str_2);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("ESC-возврат,Enter-ввод данных");
           sprintf( fst_str,"%6.0f",k_mv_tmp);
           fst_n=7;
          sw_fst=1;
          MmiGotoxy(10,3);
          break;
        }
       else if(key==DATA)
        {
          sscanf(fst_str,"%f",&k_mv_tmp);

          if(k_mv_tmp<0)
                goto m_m2_b_1;

      // введенное значение в k_mv_tmp


          T_Pmp_delay=k_mv_tmp * 1000.; // начальный расход,кг/ч
          goto mm2_b0;
        }
       else if(key==DATA_ESC)
        {
          goto mm2_b0;
        }



        goto mm2_b;
#endif
 /*========================================*/
    case 140: // 2  Просмотр журнала событий

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           goto main_menu;
       }
       else if (key==DEL)
       {

          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          MmiGotoxy(0,1);    MmiPuts(" 1 - Перезагрузка");
          MmiGotoxy(0,2);    MmiPuts(" 6 - Выход из программы в ОС");
          MmiGotoxy(0,3);    MmiPuts("    (при  отладке)");
          MmiGotoxy(0,5);    MmiPuts("ESC   отмена");
          sw_mmi=142;
          break;
       }
       else if(key== '1')  //1  По дате
       {
m_m2_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
           MmiGotoxy(0,0);    MmiPuts("   Дата записи журнала ");
           MmiGotoxy(0,1);    MmiPuts("      День.Мес.Год");
        // MmiGotoxy(0,2);    MmiPuts("        08.01.2009");
            MmiGotoxy(0,4);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
      sprintf(fst_str,"%02d.%02d.%04d",(int)adate00.day,(int)adate00.month,(int)(adate00.year+2000) );
          fst_n=11;
          sw_fst=1;
          sw_mmi=143;
          break;
       }
       else if(key== '2')  //2  По номеру записи
       {
m_m2_2:
          max_par= f_get_max_n_arch();
          MmiGotoxy(1,1);
          f_clr_scr_MMI();
          MmiPuts("Введите номер записи:" );
          sprintf( fst_str,"%d",max_par);
          fst_n=4;
          sw_fst=1;
          MmiGotoxy(15,3);
          SetDisplayPage(15); // ввод значения
          break;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%d",&i);
          if(i<1) { goto m_m2_2;}
          else if(i>max_par) { goto m_m2_2;}
          mmi_prm=i-1;
          goto m_m2_3v;
        }
       else if(key==DATA_ESC)
        {
          goto m_m2;
        }
       else if(key== '3')  //3  Последние записи
       {
m_m2_3:
         max_par= f_get_max_n_arch();
         mmi_prm=max_par-2;

m_m2_30:
         if(mmi_prm>(max_par-1))
            mmi_prm=max_par-2;
         if(mmi_prm<0) mmi_prm=0;


m_m2_3v:
         f_clr_scr_MMI();
         SetDisplayPage(EmptPage);    // Чистая страница

   //----------------
        ar_str_ptr= f_get_astr(mmi_prm);
        k=f_prn_mmi_a(ar_str_ptr,0);
        if(k>=3)
        {
          k_ext1=0;
          k_ext=7;
          goto m_m2_32;
        }
  if(mmi_prm==(max_par-1)) goto m_m2_32;

        ar_str_ptr= f_get_astr(mmi_prm+1);
        k_ext=f_prn_mmi_a(ar_str_ptr,k+1);
        k_ext1=0;
  if(mmi_prm==(max_par-2)) goto m_m2_32;

        if(k_ext<4)
        {
          ar_str_ptr= f_get_astr(mmi_prm+2);
          k_ext=f_prn_mmi_a(ar_str_ptr,k_ext+1);
          k_ext1=1;
        }
   //----------------
m_m2_32:
        MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");

        sw_mmi=141;
       }
       break;
 /*========================================*/
    case 141:/*просмотр 2 записей журнала*/
         if(key==ESC)    /* переход в меню выбора точки для просмотра*/
         {
          goto m_m2;
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=2)
           {
            mmi_prm -=2;
            goto m_m2_3v;
           }
           else
           {
            mmi_prm = 0;
            goto m_m2_3v;
           }
         else if (key== F3 )
         {
           max_par= f_get_max_n_arch();
           if(mmi_prm <= (max_par-2))
           {
            if(k_ext1==0)
             if(k_ext<=5)
               mmi_prm +=2;
             else
               mmi_prm +=1;
            else
             if(k_ext<=5)
               mmi_prm +=3;
             else
               mmi_prm +=2;
            if(mmi_prm >= max_par)
              mmi_prm=max_par-1;
            goto m_m2_3v;
           }
         }
    break;
 /*========================================*/
    case 142:/* выход из программы */
             /* возврат к просмотру */
       if((key==ESC)||(key==ENTER))    /* переход в меню */
         {
          goto m_m2;
         }
         else if(key== '1' )
         { //  1 - Перезагрузка
           f_wr_evt(evt_res);
  StopUserTimerFun();
  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
  RestoreCom(4);
  f_free_SlvBuf();

          _asm cli
          _asm hlt

         }
         else if(key== '6' )
         {// 6 - Выход из программы в ОС
            f_wr_evt(evt_exit);
            return 1; // выход из программы *.exe в ОС
         }
         break;
 /*========================================*/
    case 143:

 //  Ввод даты
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m2;
       }
       else if(key==DATA)
        {
          year=(int)(adate00.year+2000);
          month=(int)adate00.month;
          day=(int)adate00.day;

          sscanf(fst_str,"%d.%d.%d",&day,&month,&year);
          if((year<2000)||(year > 2100)) goto m_m2_1;
          if((month<1)|| (month > 12))  goto m_m2_1;
          if((day<1)|| (day > 31))  goto m_m2_1;

             adate00.year = year-2000 ;
             adate00.month = month;
             adate00.day = day;
            // дата введена

m_m2_1_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
           MmiGotoxy(0,0);    MmiPuts("   Время записи журнала ");
           MmiGotoxy(0,1);    MmiPuts("       Час Мин.");
        // MmiGotoxy(0,2);    MmiPuts("        08.01");
            MmiGotoxy(0,4);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
      sprintf(fst_str,"%02d.%02d",(int)adate00.hour,(int)adate00.min );
          fst_n=6;
          sw_fst=1;
          sw_mmi=144;
       }
          break;

 /*========================================*/
    case 144:
 //  Ввод времени
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m2_1;
       }
       else if(key==DATA)
        {
          hour=(int)adate00.hour;
          min=(int)adate00.min;

          sscanf(fst_str,"%d.%d",&hour,&min);
          if((min<0)|| (min > 59))  goto m_m2_1_1;
          if((hour<0)|| (hour > 23))  goto m_m2_1_1;


             adate00.min = min;
             adate00.hour = hour;
             adate00.sec = 0;
            // время введено

            SetDisplayPage(EmptPage);
            f_clr_scr_MMI();

         ltime1=f_get_time_t(&adate00);
         k=f_get_dat_n(ltime1);
         if(k<0)
         {
           ltime2=f_get_time(f_get_fst_stor());
           if(ltime1<=ltime2)
           {
             k=0;
           }
           else k=f_get_max_n_arch();
         }
         mmi_prm=k;
         max_par= f_get_max_n_arch();
         goto m_m2_30;
        }
         break;
 /*========================================*/
    case 150: // 3  Сервисные функции

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           goto main_menu;
       }

       else if(key== '1')  // 1  Список драйверов
       {
         mmi_str=0;
         max_par=icp_lst_max;
m20_2:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         f_drv_list_MMI(&mmi_str);
         sw_mmi=22;
       }
       else if(key== '2')  // Статистика  драйверов
       {
         mmi_str=0;
         max_par=icp_lst_max;
m20_3:
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
         f_drv_stat_MMI(&mmi_str);
         pass_cnt=0;
         sw_mmi=23;
       }
       else if(key== '3')   //  3  Просмотр/ввод параметров
       {
m_m3_3:

   //     PassW=0;
        // переход в меню выбора точки для просмотра параметров
//          SetDisplayPage(11);
          sw_mmi=11;
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Меню выбора списка просмотра");
            MmiGotoxy(0,3);    MmiPuts("1...5  номер страницы ");
//          MmiGotoxy(0,3);    MmiPuts("6 Табл.тарир.датчика давления");
//          MmiGotoxy(0,4);    MmiPuts("7 Табл.тарир.датчика температ.");
            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");
          break;
       }
       else if(key== '4')   //  4  Другие функции
       {
m_m3_4:
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Меню 4.4.Другие функции. ");
            MmiGotoxy(0,1);    MmiPuts("1  Показания расходомера");
            MmiGotoxy(0,2);    MmiPuts("2  Параметры расходомера");
            MmiGotoxy(0,3);    MmiPuts("3  Ошибки расходомера");
            MmiGotoxy(0,4);    MmiPuts("4  Поиск ICP/RTU модулей");
            MmiGotoxy(0,5);    MmiPuts("5  Список найденных модулей");
            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");
         sw_mmi=160;
          break;
       }
       else if(key== '5')   //  5  Время, дата
       {
m_m3_5:
         f_prepare_t ( &adate00);
         adate11=adate00;
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
           MmiGotoxy(0,0);    MmiPuts("      Текущая дата ");
           MmiGotoxy(0,1);    MmiPuts("      День.Мес.Год");
        // MmiGotoxy(0,2);    MmiPuts("        08.01.2009");
           MmiGotoxy(0,3);
if(FlagWinSum==0)             MmiPuts("Переход лет/зим.время выключен");
else                          MmiPuts("Переход лет/зим.время включен");

/*
 if(ReadNVRAM(nr_win_sum))
 // 1 - летнее
                              MmiPuts("      Летнее время");
     else                     MmiPuts("      Зимнее время");
*/
            MmiGotoxy(0,4);   MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);   MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);   MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
      sprintf(fst_str,"%02d.%02d.%04d",(int)adate00.day,(int)adate00.month,(int)(adate00.year+2000) );
          fst_n=11;
          sw_fst=1;
          sw_mmi=165;
          break;
       }
       else if(key== '6')   //  6  Изменить пароль
       {
m_m3_6:
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Меню 4.6.Изменение пароля ");
            MmiGotoxy(0,2);    MmiPuts("1  Изменить пароль");
            f_d_ESC();
         sw_mmi=162;
          break;
       }
       break;
 /*========================================*/
    case 160: // M3.4  Другие функции

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           goto m_m3;
       }
       else if(key== '1') //  1  Показания расходомера
       {
m_4_4_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         f_prn_den_t();
         flg_ch=0;

         State_SLV = Mod_flow_s;
         f_reg_cmn(Mod_flow_s_cmn);

         sw_mvd_m1=20;
         ff_serv=f_MVD_MM4;

/*
         s_MVD[0].r_addr=195; // Срез масс.расх.
         s_MVD[0].f = 0.;
         MVD_fn[0]=MVD_WR_F;;
*/
         sw_mmi=122;
         break;

       }

       else if(key== '2') //  2  Параметры расходомера
       {
         max_par=17;
         mmi_prm=0;

m_m3_4_2v:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");

         if(ff_serv==NULL)
         {
          sw_MMI_cur=0; // строка MMI
          sw_mvd_m1=mmi_prm+20;  // нач.параметр
          sw_MMI_lim=6; // конечная строка

          ff_serv=f_MVD_M1;
          sw_mmi=172;
         }
         else
          sw_mmi=174;
         break;
       }
       else if(key== '3') // 3  Ошибки расходомера
       {
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         MmiGotoxy(0,6);    MmiPuts("ESC,ENTER  Возврат");
         sw_mvd_m1=0;
         ff_serv=f_MVD_M3;
         sw_mmi=180;

         break;

       }

       else if(key== '5') //  5  Список подключенных модулей
       {
         mmi_str=0;
         max_par=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
m20_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         f_mod_list_MMI(&mmi_str);
         sw_mmi=21;
       }
       else if(key== '4')  // 4  Поиск ICP/RTU модулей
       {
m20_4:
          f_clr_scr_MMI();
          MmiGotoxy(1,1);
          MmiPuts("Введите номер порта : " );
          sprintf( fst_str,"%2d",mmi_com);
          fst_n=3;
          sw_fst=1;
          MmiGotoxy(13,3);
          SetDisplayPage(15);
          break;
       }
       else if(key== DATA )
       {
          sscanf(fst_str,"%d",&mmi_com);  // номер COM
          if((mmi_com>0)&&(mmi_com<4)) licp_port=mmi_com;
          else
          {
            if(mmi_com<1)  mmi_com=1;
            else if (mmi_com>3)  mmi_com=3;
            goto m20_4;
          }

          if(ComProt[mmi_com]==0)
          {
            licp_sw=1;
            ff_serv=f_look_ICP;
            flag_print_licp=0;
            LICP_Rqst.timeout=ToutLicpScan;
            licp_crc_flag=LICP_Rqst.CRC_flag;
          }
          else
          {
            licp_sw=1;
            ff_serv=f_look_RTU;
            flag_print_licp=0;
            LICP_Rqst_RTU.timeout=ToutLicpScan;
          }

       f_clr_scr_MMI();
       SetDisplayPage(EmptPage);

       if(ComProt[mmi_com]==0)
       {
         MmiGotoxy(0,0);     MmiPrintf("ICP COM%d: таймаут %d мс",licp_port,ToutLicpScan);
         MmiGotoxy(0,1);  MmiPrintf("Макс.Адр %d,флаг CRC=%d",licp_amax,LICP_Rqst.CRC_flag);
       }
       else
       {
         MmiGotoxy(0,0);     MmiPrintf("RTU COM%d: таймаут %d мс",licp_port,ToutLicpScan);
         MmiGotoxy(0,1);  MmiPrintf("Макс.Адр %d",licp_amax);
       }

       MmiGotoxy(0,6);  MmiPuts("  Enter - продолжить");
       sw_mmi=24;
       break;
       }
       else if(key==DATA_ESC)
        {
          goto m_m3_4;
        }
       break;
 /*========================================*/
    case 162:
 //  Изменение пароля
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m3;
       }
       else if(key== '1')   // 1  Изменить пароль
       {

          if(PassW != Password)
          {
           pass_ret=3;
           goto m_pass;
          }
m_m3_6_1:

          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите новый пароль(1-й раз):" );
           pass1=-1;
           sprintf( fst_str,"******");
           SetDisplayPage(17);    // Ввод пароля
           sw_mmi=163;
           fst_n=7;
           sw_fst=1;
           MmiGotoxy(11,3);
           break;
       }
       break;
 /*========================================*/
    case 163: // // ввод нового пароля 1 раз
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
 m163:
          goto m_m3;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%ld",&pass1);
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
           // пароль введен корректно
           if(pass1>= 0)
           {
             MmiGotoxy(0,1);
             SetDisplayPage(EmptPage); // ввод значения
             f_clr_scr_MMI();
             MmiPuts("Введите новый пароль(2-й раз):" );
             pass2=-1;
             sprintf( fst_str,"******");
             SetDisplayPage(17);    // Ввод пароля
             fst_n=7;
             sw_fst=1;
             MmiGotoxy(11,3);
             sw_mmi=164;
             break;
           }
           else goto m_m3_6_1;
        }
       else if(key==DATA_ESC)
        {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m163;
        }

 /*========================================*/
    case 164: // // ввод нового пароля 2 раз
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m163;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%ld",&pass2);
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
           // пароль введен корректно
           if(pass2 == pass1)
           {
             chg_nn_par=Par_PASS;
             chg_old=Password_m;
             chg_new= pass1;
         //  Password=pass1;
             Password_m=pass1;
             f_wr_evt(evt_chg_param);

             if(f_wrt_eee()<0)
             {
                f_wr_evt(evt_we_err);
                MmiGotoxy(1,3);   MmiPuts("Ошибка при сохранении пароля");
             }
             else
             {
                f_wr_evt(evt_we);
                MmiGotoxy(3,3);   MmiPuts("Пароль сохранен успешно");
             }
             f_wr_cor();
             MmiGotoxy(0,6);   MmiPuts("      Enter - продолжить");
             break;
           }
           else goto m_m3_6_1;

        }
       else if(key==DATA_ESC)
        {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m163;
        }

 /*========================================*/

 /*========================================*/
    case 165:

 //  Ввод даты для изменения
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m3;
       }
       else if(key==DATA)
        {
          year=(int)(adate00.year+2000);
          month=(int)adate00.month;
          day=(int)adate00.day;

          sscanf(fst_str,"%d.%d.%d",&day,&month,&year);
          if((year<2000)||(year > 2100)) goto m_m3_5;
          if((month<1)|| (month > 12))  goto m_m3_5;
          if((day<1)|| (day > 31))  goto m_m3_5;

             adate00.year = year-2000 ;
             adate00.month = month;
             adate00.day = day;
            // дата введена
           if((adate00.year  != adate11.year ) ||
              (adate00.month != adate11.month) ||
              (adate00.day   != adate11.day  ) )
           {

// printf("\n\r %02d.%02d.%02d" ,day,month,year-2000);

            f_wr_evt(evt_none);
            SetDate( year, month, day);

            WriteNVRAM((int)nr_year  ,(int)(year-2000));
            WriteNVRAM((int)nr_month ,(int)month  );
            WriteNVRAM((int)nr_day   ,(int)day    );
            init_win_sum(0);
            f_wr_evt(evt_ch_date);
            adate11=adate00;
//          printf("\n\r Date have changed ");
           }

m_m3_5_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
           MmiGotoxy(0,0);    MmiPuts("      Текущее время ");
           MmiGotoxy(0,1);    MmiPuts("       Час Мин Сек");
        // MmiGotoxy(0,2);    MmiPuts("         08.01.00");
           MmiGotoxy(0,3);
if(FlagWinSum==0)             MmiPuts("Переход лет/зим.время выключен");
else                          MmiPuts("Переход лет/зим.время включен");
/*
 if(ReadNVRAM(nr_win_sum))
 // 1 - летнее
                              MmiPuts("      Летнее время");
     else                     MmiPuts("      Зимнее время");
*/





            MmiGotoxy(0,4);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
      sprintf(fst_str,"%02d.%02d.%02d",(int)adate00.hour,(int)adate00.min,(int)adate00.sec);
          fst_n=9;
          sw_fst=1;
          sw_mmi=166;
       }
          break;

 /*========================================*/
    case 166:
 //  Ввод времени
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m3_5;
       }
       else if(key==DATA)
        {
          hour=(int)adate00.hour;
          min=(int)adate00.min;
          sec=(int)adate00.sec;

          sscanf(fst_str,"%d.%d.%d",&hour,&min,&sec);
          if((min<0)|| (min > 59))  goto m_m3_5_1;
          if((sec<0)|| (sec > 59))  goto m_m3_5_1;
          if((hour<0)|| (hour > 23))  goto m_m3_5_1;

             adate00.min = min;
             adate00.hour = hour;
             adate00.sec = sec;
            // время введено
           if((adate00.hour  != adate11.hour ) ||
              (adate00.min   != adate11.min) ||
              (adate00.sec   != adate11.sec  ) )
           {
//          printf("\n\r Time have changed ");
            f_wr_evt(evt_none);
            SetTime( adate00.hour, adate00.min,adate00.sec);
    WriteNVRAM((int)nr_hour   ,(int)hour   );
    WriteNVRAM((int)nr_min    ,(int)min    );
    WriteNVRAM((int)nr_sec    ,(int)sec    );
            init_win_sum(0);
            f_wr_evt(evt_ch_time);
           }

         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();

         goto m_m3;
        }
         break;
 /*========================================*/
    case 200:

         // ввод кода доступа автомобиля
m1_5:

          PassWm=0;

          sw_mmi=201;
          f_reg_cmn(0);
          MmiGotoxy(5,1);
          f_clr_scr_MMI();
          n_id_a=0;
          n_id_af=0.;
          chg_nn_par=Par_ID;
          chg_old=n_id_a;
          MmiPuts("Введите код доступа:" );
//        sprintf( fst_str,"%8ld",n_id_a);
          sprintf( fst_str,"");
          fst_n=9;
          sw_fst=1;
          MmiGotoxy(10,3);
  //      MmiGotoxy(15,3);
          SetDisplayPage(15); // ввод значения
          break;

 /*========================================*/
    case 201:



        if(key==DATA)
        {
          sscanf(fst_str,"%ld",&n_id_a);
          if(n_id_a<=0) { n_id_a=0; goto m1_5;}

           chg_new= n_id_a;
           n_id_af= n_id_a;
           f_wr_evt(evt_chg_param);
                f_reg_cmn(2);
          goto mm00;
        }
       else if(key==DATA_ESC)
        {
          n_id_a=-1;
          n_id_af=-1.;
                f_reg_cmn(1);
          goto mm00;
        }
    break;
/*========================================*/
    case 210:

         // страница выбора типа налива


          PassWm=0;
          sect_rslt=0;
          sw_mmi=211;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);

m210:
          f_reg_cmn(0);

str_prod[20]=0;
 MmiGotoxy(0,1);   MmiPuts("  НАЛИВ   :");
   if(type_dlv==2) MmiPuts("  ВЕРХНИЙ               ");
   if(type_dlv==1) MmiPuts("   НИЖНИЙ                ");
   else            MmiPuts(" --------      ");

MmiGotoxy(0,2); MmiPrintf("  ТОПЛИВО :%s",str_prod);
 MmiPuts( str_empt );


MmiGotoxy(0,3);
if(sect_num>0)
{
  MmiPrintf("  СЕКЦИЯ N:     %d  ",sect_num);
}
else
{
  MmiPrintf("  СЕКЦИЯ N:     --");
}

MmiGotoxy(0,4);
if((sect_mass != 0) && (sect_vol==0.))
 MmiPrintf("  МАССА,кг:%7d          ",sect_mass);
else if((sect_vol != 0) && (sect_mass == 0.))
 MmiPrintf("  ОБЪЕМ, л:%7d          ",sect_vol);
else
 MmiPrintf("  --------: --------");
MmiGotoxy(0,5);
 MmiPuts(" 1-Нижний; 2 - Верхний ");

MmiGotoxy(0,6);
 MmiPuts(" ESC - выход  ");

         if((type_dlv==1) || (type_dlv==2))
           if(sect_num>0)
             if (((sect_mass != 0) && (sect_vol==0.)) || ((sect_vol != 0) && (sect_mass == 0.))  )
              {
  MmiPuts(", Enter-ввод");
              }


          break;

 //========================================
    case 211:

         // страница выбора типа налива
         // анализ нажатия кнопок



       if(key==ESC)   // переход на начальную страницу
       {
         sect_rslt=-1;
         type_dlv=0;

         f_reg_cmn(3);
         goto mm00;
       }
       else if(key== '1')
       {
         type_dlv=1;
         sect_rslt=0;

       }
       else if(key== '2')
       {
         type_dlv=2;
         sect_rslt=0;
       }
       if(key==ENTER)    // переход на начальную страницу
       {
         if((type_dlv==1) || (type_dlv==2))
           if(sect_num>0)
             if (((sect_mass != 0) && (sect_vol==0.)) || ((sect_vol != 0) && (sect_mass == 0.))  )
              {
                sect_rslt=1;

                f_reg_cmn(4);
                goto mm00;
              }
       }

       goto m210;

 //========================================
    case 220:

         // страница отображения строк 1-6

        PassWm=0;

          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);
          sw_mmi=221;

          f_reg_cmn(0);

str_dsp1[30]=0;
str_dsp2[30]=0;
str_dsp3[30]=0;
str_dsp4[30]=0;
str_dsp5[30]=0;
str_dsp6[30]=0;
str_dsp7[30]=0;

m220:

 MmiGotoxy(0,0);
 MmiPuts(str_dsp1); MmiPuts( str_empt );


 MmiGotoxy(0,1);
 MmiPuts(str_dsp2); MmiPuts( str_empt );


 MmiGotoxy(0,2);
 MmiPuts(str_dsp3); MmiPuts( str_empt );

 MmiGotoxy(0,3);
 MmiPuts(str_dsp4); MmiPuts( str_empt );


 MmiGotoxy(0,4);
 MmiPuts(str_dsp5); MmiPuts( str_empt );


 MmiGotoxy(0,5);
 MmiPuts(str_dsp6); MmiPuts( str_empt );

 MmiGotoxy(0,6);
 MmiPuts(str_dsp7); MmiPuts( str_empt );

          break;

 //========================================
    case 221:

         // страница выбора типа налива
         // анализ нажатия кнопок



       if(key==ESC)   // переход на начальную страницу
       {

         f_reg_cmn(5);
         goto mm00;
       }
       if(key==ENTER)    // переход на начальную страницу
       {
         f_reg_cmn(6);
         goto mm00;
       }

       goto m220;

 //========================================
 //========================================
    case 21:  // просмотр списка модулей

       max_par=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
       if((key==ESC)||(key==ENTER))   /* переход в меню выбора точки для просмотра*/
       {
          goto m_m3_4;
       }
       else if((key== F2 )&& (mmi_str>0) )
       {
        mmi_str -=n_mmi_str_3;
        goto m20_1;
       }
       else if((key== F3 )&& (mmi_str < (max_par-n_mmi_str_3)) )
       {
        mmi_str +=n_mmi_str_3;
        goto m20_1;
       }
       break;
 /*========================================*/
    case 22: // просмотр списка драйверов

       if((key==ESC)||(key==ENTER))    /* переход в меню выбора точки для просмотра*/
       {
        goto m_m3;
       }
       else if((key== F2 )&& (mmi_str>0) )
       {
        mmi_str -=n_mmi_str_3;
        goto m20_2;
       }
       else if((key== F3 )&& (mmi_str < (max_par-n_mmi_str_3)) )
       {
        mmi_str +=n_mmi_str_3;
        goto m20_2;
       }
       break;
 /*========================================*/
    case 23: // просмотр статистики драйверов

       if((key==ESC)||(key==ENTER))    /* переход в меню выбора точки для просмотра*/
       {
        goto m_m3;
       }
       else if((key== F2 )&& (mmi_str>0) )
       {
        mmi_str -=n_mmi_str_3;
        goto m20_3;
       }
       else if((key== F3 )&& (mmi_str < (max_par-n_mmi_str_3)) )
       {
        mmi_str +=n_mmi_str_3;
        goto m20_3;
       }
       else if (key==Sht_ESC)
        {  // обнуление счетчиков ошибок
           for(i=0;i<icp_lst_max;i++)
           {
             ICPl_DEV=ICP_dd[i];
             if(ICPl_DEV!=NULL)
             {
      //      ICPl_DEV->n_transaction=0;
              ICPl_DEV->n_timeout_error=0;
              ICPl_DEV->n_CRC_error=0;
             }
           }
        }
       else //PASS
       {
         pass_cnt++;
         if((pass_cnt & 0xf) == 0 )
           f_drv_stat_MMI(&mmi_str);
       }
       break;
 /*========================================*/
    case 24: // отображение сканирования порта
       if((key==ESC)||(key==ENTER))
       {
           goto m_m3_4; //  4  Другие функции
       }
       else //PASS
       {
          MmiGotoxy(0,3);  MmiPrintf("Опрошено   %3d",licp_addr);
          MmiGotoxy(0,4);  MmiPrintf("Обнаружено %3d",licp_fnd);
       }
       break;
 /*========================================*/
    case 25: // отображение аналоговых входов

       if((key==ESC)||(key==ENTER))
       {
           goto m0_f1;
       }
       else if((key== '2') && (id_ok !=0) ) // 2  Прокачка
       {
        flag_prok=1;
        strcpy(mode_prok,prok_ka);
        Flag_Low=-1;
        goto m1_1;
       }
       else // PASS
       {
// MmiGotoxy(0,0); MmiPrintf(" Состояния аналоговых входов:");
// MmiGotoxy(0,1); MmiPrintf("       7017C/1 , мА");
// MmiGotoxy(0,1); MmiPrintf(" xxx.xx xxx.xx xxx.xx xxx.xx");// 20./3276.8=0.006103515625

// if( _fmemcmp( &ADC[0], &ADC_c[0], sizeof(ADC_c4)) != 0)
   {
   MmiGotoxy(0,2); MmiPrintf(" Давление     %8g (In%d)",(float)ADC[0],analog_num[0]-1);
   MmiGotoxy(0,3); MmiPrintf(" Температура  %8g (In%d)",(float)ADC[1],analog_num[1]-1);
//    _fmemcpy( &ADC_c[0], &ADC[0], sizeof(ADC_c4));
   }


//-------------------
     MmiGotoxy(0 ,4); MmiPuts("СТАРТ.В=");
     if( STRT_IN)
       MmiPuts("1");
     else
       MmiPuts("0");

      //20.08.2021 YN
      #if defined(UNDERPRESSURE)
        MmiGotoxy(10,4); MmiPuts("СТОП.В=");
        //   if( STOP_IN)
        if( ES_IN )
          MmiPuts("1");
        else
          MmiPuts("0");
        MmiGotoxy(19,4); MmiPuts("UP= ");
        MmiGotoxy(23,4);
        MmiPrintf("%4.4f",Under_Press);
      #else
        MmiGotoxy(12,4); MmiPuts("СТОП.В=");
        //   if( STOP_IN)
        if( ES_IN )
          MmiPuts("1");
        else
          MmiPuts("0");
      #endif

//-------------------
     MmiGotoxy(0 ,5); MmiPuts("СТАРТ.Н=");
     if( STRT_IN_L)
       MmiPuts("1");
     else
       MmiPuts("0");

     MmiGotoxy(12,5); MmiPuts("СТОП.Н=");

     if( ES_IN_L )
       MmiPuts("1");
     else
       MmiPuts("0");

     MmiGotoxy(23,5); MmiPuts("УР.Н.=");
     if(LVL_IN_L)
       MmiPuts("1");
     else
       MmiPuts("0");

//-------------------
     MmiGotoxy( 0,6); MmiPuts("УР.В.=");
     if(LVL_IN)
       MmiPuts("1");
     else
       MmiPuts("0");

     MmiGotoxy( 8,6); MmiPuts("ТРАП=");
     if(TRAP_IN)
       MmiPuts("1");
     else
       MmiPuts("0");
     MmiGotoxy(16,6); MmiPuts("КОНС.=");
     if( CONS_IN)
       MmiPuts("1");
     else
       MmiPuts("0");

     MmiGotoxy(24,6); MmiPuts(" УЗА=");
     if( UZA_IN )
       MmiPuts("1");
     else
       MmiPuts("0");
//-----------------------

       }

       break;
 /*========================================*/
    case 26:/* ожидание нажатия ESC,Enter */
         if((key==ESC)||(key==ENTER)  )
         {
          goto m_main;
         }
         break;
 /*========================================*/
    case 27:/* ожидание нажатия ESC,Enter */
         if((key==ESC)||(key==ENTER)  )
         {
           goto m0_f1;
         }
         break;
 /*========================================*/
    case 28: // отображение дискретных выходов
       if((key==ESC)||(key==ENTER))
       {
           goto m0_f1;
       }

#if defined(BIO_1)

       else if(key== '1')
       {
        OUT_VAR ^= OUT2;
       }
       else if(key== '2')
       {
        OUT_VAR ^= OUT1;
       }
       else if(key== '3')
       {
        OUT_VAR ^= OUT5;
       }
       else if(key== '4')
       {
        OUT_VAR ^= OUT6;
       }
       else if(key== '5')
       {
        OUT_VAR ^= OUT3;
       }

#else

       else if(key== '1')
       {
        OUT_VAR ^= OUT1;
       }
       else if(key== '2')
       {
        OUT_VAR ^= OUT2;
       }

       else if(key== '3')
       {
        OUT_VAR ^= OUT5;
       }
       else if(key== '4')
       {
        OUT_VAR ^= OUT6;
       }


       else if(key== '5')
       {  // ВОЗДУХ
        OUT_VAR ^= OUT4;
       }

       else if(key== '6')
       { // НАСОС
        OUT_VAR ^= OUT3;
       }


       else if(key== '7')
       {  // OK
        OUT_VAR ^= OUT8;
       }
#endif
       else // PASS
       {
         {

#if defined(BIO_1)

          MmiGotoxy(0,2);
     MmiPuts("Клап.мнш.:");
     if( OUT_VAR & OUT2  )
       MmiPuts("1       ");
     else
       MmiPuts("0  ");

          MmiGotoxy(15,2);
     MmiPuts("Клап.блш.:");
     if( OUT_VAR & OUT1  )
       MmiPuts("1       ");
     else
       MmiPuts("0  ");

     MmiGotoxy(0,3);              MmiPuts("Емк. 1:");
     if( i7060_out[0] & OUT5  )
       MmiPuts("1  ");
     else
       MmiPuts("0  ");


     MmiGotoxy(15,3);   MmiPuts("Емк. 2:");
     if( i7060_out[0] & OUT6  )
       MmiPuts("1  ");
     else
       MmiPuts("0  ");



     MmiGotoxy( 0,4);   MmiPuts("Насос :");
     if( i7060_out[0] & OUT3  )
       MmiPuts("1      ");
     else
       MmiPuts("0      ");


#else
          MmiGotoxy(0,2);
      MmiPuts("В.налив:");
  if(valve_mode==1)
      MmiPuts("Кл.Блш.");
  else if(valve_mode==2)
      MmiPuts("Пил.Н.О");
  else if(valve_mode==3)
      MmiPuts("Кл.откр");

          if(OUT_VAR & OUT1)
            MmiPuts(":1 ");
          else
            MmiPuts(":0 ");

          MmiGotoxy(18,2);
  if(valve_mode==1)
         MmiPuts(" Кл.мал.");
  else if(valve_mode==2)
         MmiPuts("Пил.Н.З.");
  else if(valve_mode==3)
         MmiPuts("~Кл.закр");

          if(OUT_VAR & OUT2)
            MmiPuts(":1 ");
          else
            MmiPuts(":0 ");

          MmiGotoxy(0,3);
      MmiPuts("Н.налив:");
  if(valve_modeL==1)
      MmiPuts("Кл.Блш.");
  else if(valve_modeL==2)
      MmiPuts("Пил.Н.О");
  else if(valve_modeL==3)
      MmiPuts("Кл.откр");

          if(OUT_VAR & OUT5)
            MmiPuts(":1 ");
          else
            MmiPuts(":0 ");

          MmiGotoxy(18,3);
  if(valve_modeL==1)
         MmiPuts(" Кл.мал.");
  else if(valve_modeL==2)
         MmiPuts("Пил.Н.З.");
  else if(valve_modeL==3)
         MmiPuts("~Кл.закр");

          if(OUT_VAR & OUT6)
            MmiPuts(":1 ");
          else
            MmiPuts(":0 ");

          MmiGotoxy(0,4); MmiPuts("Воздух ");
          if(OUT_VAR & OUT4)
            MmiPuts(":1 ");
          else
            MmiPuts(":0 ");

          MmiGotoxy(15,4); MmiPuts("Насос  ");
          if(OUT_VAR & OUT3)
            MmiPuts(":1 ");
          else
            MmiPuts(":0 ");

          MmiGotoxy(0,5); MmiPuts("НЕТ АВАРИИ   ");
          if(OUT_VAR & OUT8)
            MmiPuts(":1 ");
          else
            MmiPuts(":0 ");
#endif

         }
       }
       break;
 /*========================================*/
 /*========================================*/
 /*
    case 171://  1  Показания расходомера
         if((key==ESC)||(key==ENTER)  )
         {
          if((ff_serv==f_MVD_MM2)||(ff_serv==f_MVD_M2) ) ff_serv=NULL;
          goto m_m3_4;
         }
         else if(key== PASS )
         {
//        if ((ff_serv==NULL) && ( (flag_nal==0) || (fl_rd_err!=0) ) )
          if ((ff_serv==NULL) && ( (fl_rd_err!=0) ) )
          {
           fl_rd_err=0;
           sw_MMI_cur=0; // строка MMI
           sw_mvd_m1=mmi_prm+20;  // нач.параметр
           sw_MMI_lim=6; // конечная строка
           MMIDens=-1;
           MMITemp=-300;
           ff_serv=f_MVD_MM2;
          }
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=6)
           {
            mmi_prm -=6;
    //       printf("\n\rmmi_prm=%d",mmi_prm);
            goto m_m3_4_1v;
           }
           else
           {
            mmi_prm = 0;
            goto m_m3_4_1v;
           }
         else if (key== F3 )
         {
           if(mmi_prm <= (max_par-6))
           {
            mmi_prm +=6;
      //     printf("\n\rmmi_prm=%d",mmi_prm);
            goto m_m3_4_1v;
           }
         }

         break;
*/
 /*========================================*/

    case 172://  2  Параметры расходомера
         if((key==ESC)||(key==ENTER)  )
         {
          if(ff_serv==f_MVD_M1) ff_serv=NULL;
          goto m_m3_4;
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=6)
           {
            mmi_prm -=6;
            goto m_m3_4_2v;
           }
           else
           {
            mmi_prm = 0;
            goto m_m3_4_2v;
           }
         else if (key== F3 )
         {
           if(mmi_prm <= (max_par-6))
           {
            mmi_prm +=6;
            goto m_m3_4_2v;
           }
         }
 /*========================================*/
         break;
 /*========================================*/
    case 173:
         if((key==ESC)||(key==ENTER)  )
         {
          ff_serv=NULL;
          goto m_m3_4;
         }
         if(key==PASS  )
         {
          if(ff_serv==NULL)
          {
           SetDisplayPage(EmptPage);
           f_clr_scr_MMI();
           MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");
           sw_MMI_cur=0; // строка MMI
           sw_mvd_m1=mmi_prm+20;  // нач.параметр
           sw_MMI_lim=6; // конечная строка
           MMIDens=-1;
           MMITemp=-300;
           ff_serv=f_MVD_M2;
           sw_mmi=171;
          }
         }
         break;
 /*========================================*/
    case 174:
         if((key==ESC)||(key==ENTER)  )
         {
          ff_serv=NULL;
          goto m_m3_4;
         }
         if(key==PASS  )
         {
          if(ff_serv==NULL)
          {
           SetDisplayPage(EmptPage);
           f_clr_scr_MMI();
           MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");
           sw_MMI_cur=0; // строка MMI
           sw_mvd_m1=mmi_prm+20;  // нач.параметр
           sw_MMI_lim=6; // конечная строка

           ff_serv=f_MVD_M1;
           sw_mmi=172;
          }
         }
         break;
 /*========================================*/
    case 180:

       if((key==ESC)||(key==ENTER))  /* переход на начальную страницу */
       {
         if(ff_serv==f_MVD_M3) ff_serv=NULL;
            goto m_m3_4;
       }
       break;
 /*========================================*/
    case 190: //  1  Отпуск нефтепродуктов

       if(key==ESC)    /* переход в меню */
       {
           goto main_menu;
       }

       else if((key== '1') && (id_ok !=0) && (mode_hst == 0) )  // 1  Ввод дозы
       {

         if( (valve_mode > 0) && (valve_modeL > 0 ))  Flag_Low=0;
         else if( valve_mode > 0) Flag_Low=0;
         else if( valve_modeL > 0) Flag_Low=1;
         else
          {
           Flag_Low=-1;
           goto m_m5;
          }
m_m5_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         MmiGotoxy(0,0);       MmiPrintf("  %s дозы ",mode_prok);
         f_prn_MassSt_dlv();

         MmiGotoxy(0,2);
          if ( Flag_mass != 0 )
             MmiPuts(" Введите дозу ,кг");
          else
             MmiPuts(" Введите дозу ,л ");

         MmiGotoxy(0,6); MmiPuts(" Enter - ввод данных  ");

      // sprintf( fst_str,"");
         strcpy( fst_str,"");
         fst_n=5;
         sw_fst=1;
         flag_dlv_fst=0;
         dose_dlvr= 0;
         flag_prn_mass=0;
         MmiGotoxy(10,4);
         break;
       }
       else if(key==DATA)
        {
          dose_dlvr=0;
          sscanf(fst_str,"%f",&dose_dlvr);
          if(dose_dlvr<=0) { dose_dlvr=0; goto m_m5_1;}
//          else if(dose_dlvr > 10000) {dose_dlvr=10000; goto m_m5_1;}
         flag_dlv_fst=0;
         flag_prn_mass=0;
         goto m_m5;
m_m5_1o:
          flag_rcv=0;

         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         MmiGotoxy(0,0);

           MmiPrintf(" Отпуск дозы %g ",dose_dlvr);
         if ( Flag_mass != 0 )
           MmiPrintf("кг");
         else
           MmiPrintf("л ");

         f_prn_mode();

         MmiGotoxy(0,6);       MmiPuts("ESC - завершить.");

         sw_dlv_liq=1;
         sw_mmi=193;
m_m5_1d:
/*==========
          MmiGotoxy(0,2);      MmiPuts("  Масса        |  Отпущено");
                //             MmiPuts("xxxxx.xx кг    | xxxxx.xx кг");
          MmiGotoxy(0,3);
          if(flag_prn_mass)   MmiPrintf("%8.2f кг    | %8.2f кг   ",s_MVD[0].MassI,s_MVD[0].MassT);  // VolT_id, VolT_int
          else  MmiPrintf("%8.2f кг    | %8.2f кг   ",s_MVD[0].MassI,(float)0.);  // VolT_id, VolT_int
===========*/

          MmiGotoxy(0,2);      MmiPuts("           Отпущено          ");
//        MmiGotoxy(0,2);      MmiPuts("  M,кг   --Отпущено-- V,л    ");
//        MmiGotoxy(0,2);      MmiPuts("  Масса        |  Отпущено");
                //             MmiPuts("xxxxx.xx кг    | xxxxx.xx кг");
          MmiGotoxy(0,3);
          if(flag_prn_mass)   MmiPrintf("М=%9.2f кг |О= %9.2f л  ",s_MVD[0].MassT,s_MVD[0].VolT);  // VolT_id, VolT_int
          else                MmiPrintf("М=%9.2f кг |О= %9.2f л  ",(float)0.,(float)0.);  // VolT_id, VolT_int

          //20.08.2021 YN
          #if defined(UNDERPRESSURE)
          MmiGotoxy(16,6);
          if (Under_Press < 0){
            MmiPrintf(" Д= %4.3f МПа", Under_Press);
          }
          else
            MmiPrintf(" Д= %4.3f  МПа", Under_Press);
          #endif

m_m5_1e:
          f_prn_proc();
          break;
        }
       else if(key==DATA_ESC)
        {
          goto m_m5;
        }
//     else if(key== '2')  // 2  Доза нижнего налива
//---------------------------------------
       else if((key== '2') && (id_ok !=0) && (mode_hst == 0) && (valve_mode > 0) && (valve_modeL > 0 ) )
       {
         Flag_Low=1;
         goto m_m5_1;
       }
//-----------------------------------------------
       else if((key== '4') && ( (Flag_Low == 0) || ( (Flag_Low == 1)  && (valve_mode == 0) ) ) && (id_ok !=0) && (mode_hst == 0) )
       {
         /*
         if((flag_prn_mass) && (dose_dlvr > 0))
          {
           goto m_m5_1o;
          }
         else
          */
         if(dose_dlvr > 0)
               goto m_m5_1o;
       }
       else if((key== '8') && (Flag_Low == 1) && (id_ok !=0) && (mode_hst == 0) && (valve_mode > 0) && (valve_modeL > 0 )  )
       {
         /*
         if((flag_prn_mass)&& (dose_dlvr > 0))
          {
           goto m_m5_1o;
          }
         else
         */
         if(dose_dlvr > 0)
               goto m_m5_1o;
       }

#if !defined(BIO_1)
       else if((key== '0' )&& (Flag_Low != 1)  )//  0  Коррекция параметров начального наполнения
#else
       else if((key== 'Z' )&& (Flag_Low != 1)  )//  0  Коррекция параметров начального наполнения
#endif

       {
m_m5_4:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts("Парам.Начального Наполнения ");
            MmiGotoxy(0,1);    MmiPuts("1 Изменить начальный объем ");
            MoveToXY(0,4);   MmiPrintf("Начальный объем :%6.0f л",Vol_beg);
       if(valve_mode != 1)
       {
            MmiGotoxy(0,2);    MmiPuts("2 Изменить начальный расход ");
            MoveToXY(0,5);   MmiPrintf("Начальный расход:%6ld кг/ч",(long int)Flow_beg);
       }
            MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");

         sw_mmi=126;
         break;
       }

   /*
       else  if(mode_hst == 0 )
          f_prn_MassSt_dlv();
   */

       goto m_m5_01;
//     break;
 /*========================================*/
//===========================================
    case 193: //  Отпуск
       if(key==F2)    // индикация Давление,Температура
       {
           PageD=0;
       }
       else if(key==F3)    // индикация Плотность,Состав
       {
           PageD=1;
       }
       else if(key==F1)    // индикация выходов
       {
           PageD=3;
       }
       else if(key=='\b')    // 'Del'
       {
         f_icp_errS(Err_Key);
       }

       key=f_dlv_liq(key);
       if(key==ESC)    // переход в меню
       {
        if(mode_hst == 0)
        {

          goto m_m5;
        }
        else
        {
          goto mm00;
        }
       }

       if(flag_prok)
       {
         MmiGotoxy(0,0);
         MmiPuts( str_empt );
         MmiGotoxy(5,0);
         MmiPrintf("Прокачка");
       }

  // отображение общей и отпущенной массы
       goto m_m5_1d;

 /*========================================*/
    case 271:/* ожидание нажатия ESC,Enter */
         if((key==ESC)||(key==ENTER)  )
         {
           goto m0_f1;
         }
         if( (f_timer(tm_md,(long int)1000 )) | fl_md_fst )
         {
           f_md5(1);
           sw_mmi=272;
         }
         break;
 /*========================================*/
    case 272:

       if((key==ESC)||(key==ENTER))  /* переход на начальную страницу */
       {
           goto m0_f1;
       }
       break;
 /*========================================*/
       default:
       if((key==ESC)||(key==ENTER))  /* переход на начальную страницу */
       {
 m_main:

           PassWm=0;

           SetDisplayPage(ZeroPage);
           sw_mmi=0;
           f_clr_scr_MMI();
           f_prn_begin();
           break;
       }
       break;
    }
    return 0;
}
/*-----------------------------------------*/
int flag_Slv_1=0;

void f_before_MMI()
{
    ftmp_naMMI[0]=analog_scale[0]*NA_scale;
    ftmp_naMMI[1]=analog_scale[1]*NA_scale;

    //20.08.2021 YN
    #if defined(UNDERPRESSURE)
      ftmp_naMMI[2]=analog_scale[2]*NA_scale;
    #endif

    ftmp_DP=((float)P_dem) / K_F_INT;
    f_LIM_P_beg=LIM_P_beg/k_div;
    f_LIM_P    =LIM_P    /k_div;
    f_LIM_P_dec=LIM_P_dec/k_div;

  flag_Slv_1=flag_Slv;
}
/*-----------------------------------------*/
void f_after_MMI()
{
int i;
    analog_scale[0]=ftmp_naMMI[0]/NA_scale;
    analog_scale[1]=ftmp_naMMI[1]/NA_scale;

    //20.08.2021 YN
    #if defined(UNDERPRESSURE)
      analog_scale[2]=ftmp_naMMI[2]/NA_scale;
    #endif

    P_dem = (ftmp_DP * K_F_INT);

    LIM_P_beg=f_LIM_P_beg*k_div;
    LIM_P    =f_LIM_P    *k_div;
    LIM_P_dec=f_LIM_P_dec*k_div;

    f_init_mki();
    f_count_cf();
  if(flag_Slv_1 != flag_Slv)
  {

          if(flag_Slv !=0 )
          {
             RestoreCom(ComPortSlv );
             InstallCom(ComPortSlv ,ComBaudSlv,Com_lgthSlv,Com_paritySlv,Com_stopSlv);
             if(ComPortSlv==4)
                ShowCom(ComPortSlv,Com_lgthSlv,ComBaudSlv);
          }
          else
          {
           RestoreCom( ComPortHost );
           i=ComPortHost;
           InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);

          }

  }
  flag_Slv_1=flag_Slv;
}