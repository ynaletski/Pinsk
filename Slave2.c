
//----------------------------------
// данные для передачи в нижний контроллер
int    CMD_IN   = 0;
int    i_LVL    = 0;
int    ii_LVL[max_nn] = {0,0};
float  LIn_val  = 0.;

//----------------------------------
int   FlagNewDT   = 0 ;      // признак передачи вниз данных
int   Tank1F      =0;        // о плотности и температуре
float New_Dens1   =0.;
float New_Temp1   =0.;
float New_Dens2   =0.;
float New_Temp2   =0.;
float New_Dens1c  =0.;
float New_Temp1c  =0.;
float New_Dens2c  =0.;
float New_Temp2c  =0.;
//----------------------------------
// данные принимаемые из нижнего контроллера

float Vol_kd   =0.;
float Mass_kd  =0.;
float Press1   =0.;
float Press2   =0.;
float Temp1    =0.;
float Temp2    =0.;

int CMD_OUT   =0;
int CMD_OUT_p =0;
int Flag_rd_cmd=0;
int Reg_State =0;
int Reg_err   =0;
//----------------------------------

void (*ff_Slv2)() = NULL;

int flag_Slv2=1;

int ComPortSlv2 = 2;
int AddrSlv2=5;
int State_SLV2=0;

int  WD_PMP2_flag=0;
int  WD_PMP2_Fl=0;

#define WD_PMP2_P1 (2000)

long int WD_PMP2_time=0;
long int WD_PMP2_t1=0;
long int WD_PMP2_Per=20000; // величина таймаута по приему сообщения

void *list_Slv2_int[]={

&n_sl2_int1,     // 0 колич.перем int
&n_sl2_flt1,     // 1 колич.перем float

&FlagNewDT  ,     // 2    // признак передачи вниз данных о плотности и температуре , биткодированный D0 - 1 рез-к,D1 - 2 рез-р
&Slv2_int[3],     // 3    // флаг обновления плотн,темп 1
&Slv2_int[4],     // 4    // флаг обновления плотн,темп 2

&Slv2_int[4],     // 5
&Slv2_int[5],     // 6

&Slv2_int[6],     // 7   CMD_REG регистр команды

&CMD_IN       ,   // 8   данные в нижний контроллер
&CMD_OUT      ,   // 9   данные с нижнего контроллера
&Reg_State    ,   // 10  данные с нижнего контроллера
&Reg_err      ,   // 11  данные с нижнего контроллера
&i_LVL        ,   // 12  данные в нижний контроллер
&OUT_VAR      ,   // 13
&INP_VAR      ,   // 14

&Slv2_int[8],     // 10

&Slv2_int[9],     // 18
&Slv2_int[10],    // 19
&Slv2_int[11],    // 20

NULL
};

int n_sl2_int=(sizeof(list_Slv2_int) / sizeof(list_Slv2_int[0])) - 1 ;
int n_sl2_int1=(sizeof(list_Slv2_int) / sizeof(list_Slv2_int[0])) - 1 ;

float *list_Slv2_flt[]={
&LIn_val      ,     //1000  0 операнд команды
&Slv2_float[1],     //1002  1 копия операнда
                    // резервные переменные
&Slv2_float[2],     // 1004  2
&Slv2_float[3],     // 1006  3

&New_Dens1,         // 1008 4  Новое значение плотн.1
&New_Temp1,         // 1010 5  Новое значение темп.1
&New_Dens2,         // 1012 6  Новое значение плотн.2
&New_Temp2,         // 1014 7  Новое значение темп.2
&Slv2_float[9],     // 1016 8
&Slv2_float[9],     // 1018 9
                    // Данные из расходомера
&s_MVD[0].MassI,    // 1020 10
&s_MVD[0].MassT,    // 1022 11
&s_MVD[0].VolI,     // 1024 12
&s_MVD[0].VolT,     // 1026 13
&s_MVD[0].FlowM,    // 1027 14
&s_MVD[0].Dens,     // 1030 15
&s_MVD[0].Temp,     // 1032 16
&s_MVD[0].Press,    // 1034 17

&s_MVD[1].MassI,    // 1036  18
&s_MVD[1].MassT,    // 1038  19
&s_MVD[1].VolI,     // 1040  20
&s_MVD[1].VolT,     // 1042  21
&s_MVD[1].FlowM,    // 1044  22
&s_MVD[1].Dens,     // 1046  23
&s_MVD[1].Temp,     // 1048  24
&Vol_kd       ,     // 1050  25
&Mass_kd      ,     // 1052  26
&Press1       ,     // 1054  27
&Press2       ,     // 1056  28
&Temp1        ,     // 1058  29
&Temp2        ,     // 1060  30

NULL
};
long int Com_Baud_Slave2=9600;
long int *list_Slv2_lng[]={
                    // Addr = 1042
&Com_Baud_Slave2,    //  21  Частота обмена по Modbus
&Com_Baud_Slave2,    //  22  Частота обмена по Modbus

NULL
};

int n_sl2_lng=(sizeof(list_Slv2_lng) / sizeof(&FL_err)) - 1 ;
int n_sl2_lng1=(sizeof(list_Slv2_lng) / sizeof(&FL_err)) - 1 ;

int n_sl2_flt=((sizeof(list_Slv2_flt) / sizeof(&FL_err)) - 1)+ ((sizeof(list_Slv2_lng) / sizeof(&FL_err)) - 1) ;
int n_sl2_flt1=((sizeof(list_Slv2_flt) / sizeof(&FL_err)) - 1)+ ((sizeof(list_Slv2_lng) / sizeof(&FL_err)) - 1) ;
int n_sl2_flt0=(sizeof(list_Slv2_flt) / sizeof(&FL_err)) - 1 ;

int Slv2_int[20]={
  0,0,0,0,0,
  0,0,0,0,0,
};

//--------------------------------
float Slv2_float[20]={
0,
0,
2,
3,
4,
5,
};
//--------------------------------
long int Slv2_lng[10]={
  0,1,2,3,4,
  5,6,7,8,9,
};

//---------------------------------------
struct eee_dat S1D;  //T_INT   T_INT_L T_FLOAT

// Addr2Slot1 2000
//  данные, читаемые нижним контроллером

struct eee_dat Slv2_Slot1[]=
{
&CMD_IN        , T_INT   ,        //1
&i_LVL         , T_INT   ,        //1
&LIn_val       , T_FLOAT ,        //2
&s_DGM[0].Prod_lvl    , T_FLOAT , //2
&s_DGM[1].Prod_lvl    , T_FLOAT , //2
&s_DGM[0].Temp        , T_FLOAT , //2
&s_DGM[1].Temp        , T_FLOAT , //2
&s_DGM[0].VolLiq      , T_FLOAT , //2
&s_DGM[1].VolLiq      , T_FLOAT , //2
&s_DGM[0].MassLiq     , T_FLOAT , //2
&s_DGM[1].MassLiq     , T_FLOAT , //2
&s_DGM[0].MassGas     , T_FLOAT , //2
&s_DGM[1].MassGas     , T_FLOAT , //2
&s_DGM[0].DensLiq     , T_FLOAT , //2
&s_DGM[1].DensLiq     , T_FLOAT , //2
};

int Slv2_slt1_lgth= 28 ;
int Slv2_slt1_max=sizeof(Slv2_Slot1) / sizeof(S1D) ;

//---------------------------------------
struct eee_dat Slv2_Short[]=
{
&Tank1F               , T_INT   , //1       //1
&New_Dens1            , T_FLOAT , //2       //2
&New_Temp1            , T_FLOAT , //2
&New_Dens2            , T_FLOAT , //2
&New_Temp2            , T_FLOAT , //2
};                              // 9

int Slv2_short_lgth= 9 ;
int Slv2_short_max=sizeof(Slv2_Short) / sizeof(S1D) ;

//---------------------------------------
// Addr2Slot2   3000
// данные, записываемые нижним контроллером

struct eee_dat Slv2_Slot2[]=
{
&CMD_OUT_p          , T_INT   ,  //1
&Reg_State          , T_INT   ,  //1
&Reg_err            , T_INT   ,  //1

&OUT_VAR            , T_INT   ,  //1
&INP_VAR            , T_INT   ,  //1
&Vol_kd             , T_FLOAT ,  //2
&Mass_kd            , T_FLOAT ,  //2
&Press1             , T_FLOAT ,  //2
&Press2             , T_FLOAT ,  //2
&Temp1              , T_FLOAT ,  //2
&Temp2              , T_FLOAT ,  //2
};
int Slv2_slt2_lgth= 17 ;
int Slv2_slt2_max=sizeof(Slv2_Slot2) / sizeof(S1D) ;
//---------------------------------------
float Dns1=0,TTmp1=0;
float Dns2=0,TTmp2=0;

// Addr2Slot3    4000
// данные, записываемые нижним контроллером

struct eee_dat Slv2_Slot3[]=
{
&s_MVD[0].MassI       , T_FLOAT ,
&s_MVD[0].MassT       , T_FLOAT ,  //   MassT1_tmp
&s_MVD[0].VolI        , T_FLOAT ,
&s_MVD[0].VolT        , T_FLOAT ,
&s_MVD[0].FlowM       , T_FLOAT ,
&s_MVD[0].Dens        , T_FLOAT ,  //  Den_tmp
&s_MVD[0].Temp        , T_FLOAT ,  //  Temp_tmp
&s_MVD[0].Press       , T_FLOAT ,
&Dns1                 , T_FLOAT ,  //&s_DGM[0].DensReal
&TTmp1                , T_FLOAT ,  //&s_DGM[0].TempReal
};
int Slv2_slt3_lgth= 20 ;
int Slv2_slt3_max=sizeof(Slv2_Slot3) / sizeof(S1D) ;
//---------------------------------------

// Addr2Slot4    5000
// данные, записываемые нижним контроллером
struct eee_dat Slv2_Slot4[]=
{
&s_MVD[1].MassI       , T_FLOAT ,
&s_MVD[1].MassT       , T_FLOAT ,  //   MassT2_tmp
&s_MVD[1].VolI        , T_FLOAT ,
&s_MVD[1].VolT        , T_FLOAT ,
&s_MVD[1].FlowM       , T_FLOAT ,
&s_MVD[1].Dens        , T_FLOAT ,
&s_MVD[1].Temp        , T_FLOAT ,
&Dns2                 , T_FLOAT , //&s_DGM[1].DensReal
&TTmp2                 , T_FLOAT , //&s_DGM[1].TempReal
};
int Slv2_slt4_lgth=  18;
int Slv2_slt4_max=sizeof(Slv2_Slot4) / sizeof(S1D) ;
//---------------------------------------

unsigned long ComBaudSlv2 =9600L;
int Com_lgthSlv2  =8;
int Com_paritySlv2=0;// None  //0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)
int Com_stopSlv2  =1;

void f_Slave2RTU( )
{
int itmp;
//  int  hour,min,sec;
// вводит команду из Host (ICP CON) и интерпретирует ее по протоколу Modbus RTU


         if(ffgets_Slv2RTU(ComPortSlv2) )
         {

          _fmemcpy(intrpr.bfcom,&cb_COM[ComPortSlv2][0],(long int)(n_bcom[ComPortSlv2]-2) );
          n_bcom[ComPortSlv2]=0;
          f_intr_Slv2RTU();
         }
}
/*-------------------------------------------*/
/*-------------------------------------------*/

int n_max_rcv2=NN_fict;

int ffgets_Slv2RTU(int ComPort)
{

//  Функция принимает символы из порта COM ComPort  в буфер cb_COM[ComPort].
//  При таймауте команда считается введенной и функция возвращает
// значение 1.
//

/* ComPort - номер COM   1...4 */
 char tmp;
 int tmp1;
 int i;
m1:
     if(IsCom(ComPort)==0)
     {
         if( WD_PMP2_Per !=0 )
           if( f_timer(WD_PMP2_time,WD_PMP2_Per ))
           { // нет успешного приема

            if(WD_PMP2_flag != 0)
            {
              WD_PMP2_flag=0;
              f_icp_errS(WD_host2);
            }
            WD_PMP2_time=TimeStamp;
           }

           if( f_timer(WD_PMP2_t1,WD_PMP2_P1 ))
           { // нет успешного приема
            WD_PMP2_Fl=0;
            WD_PMP2_t1=TimeStamp;
           }

         if( f_timer(time_snd[ComPort],ToutAns[ComPort]))
         { // истек таймаут приема ответа на команду
            time_snd[ComPort]= TimeStamp;
m_end:
            if((n_bcom[ComPort] >= RTU_min)&& (cb_COM[ComPort][0]==AddrSlv2))
            {
              if(f_check_crc_RTU(cb_COM[ComPort],n_bcom[ComPort])==0)
               {
                  goto m_err;
               }

       LedToggle();  // мигание светодиодом на процессоре

              n_max_rcv2= NN_fict;

              WD_PMP2_time=TimeStamp;
              WD_PMP2_t1=TimeStamp;
              WD_PMP2_Fl=1;

              if(ICP_error[icp_lst_max] & WD_host2)
              {
               ICP_error[icp_lst_max] &= ~(unsigned int)WD_host2;
               ICP_error_c[icp_lst_max]=ICP_error[icp_lst_max];
               for(i=0,tmp1=0;i<=icp_lst_max;i++)
               {
                 tmp1 |=ICP_error[i];
               }
//              printf("\r\n tmp1= %04x",tmp1);

               if(tmp1==0)
               {
                FL_err=0;
               }
              }
              return 1;
            }
            else
             {
m_err:
               n_bcom[ComPort]=0;
               n_max_rcv2= NN_fict;
               return 0;
             }
         }
         return 0;
     }
     tmp=ReadCom(ComPort);
#if(Slv_deb != 0)
     printf("%02x ",(int)tmp & 0xff);
#endif

     ToutAns[ComPort]=byte_timout_rtu[ComPort];

     cb_COM[ComPort][n_bcom[ComPort]]=tmp;
     if( (++n_bcom[ComPort]) >= n_bufcom )
      {
        n_bcom[ComPort]--;
  /*
        printf("\n Command buffer overflow !\n--");
        puts(cb_COM1);
  */
      }

      if(n_bcom[ComPort] == 2)
         n_max_rcv2=f_get_numRTU2(ComPort);
      else if ((n_bcom[ComPort] == 7) && (n_max_rcv2 == NN_fict)  )
         n_max_rcv2=f_get_numRTU7(ComPort);

      if(n_bcom[ComPort]>= n_max_rcv2) goto m_end;

      time_snd[ComPort]= TimeStamp;
      goto m1;
}
/*-------------------------------------------*/
/*-------------------------------------------*/
  unsigned char buf_tmp[n_buf_RTU_s];
void  f_intr_Slv2RTU()
{
  int exc;
  int fn_cod;
  int addr_beg,i,i1,i2,itmp;
  int Slv_addrO,Slv_nmbO;
  float ftmp;
  union  { float f; unsigned long l; } o;
  struct eee_dat *LIn_D;

// принятая команда проверена на контрольную сумму и находится в intrpr.bfcom
//  (без контрольной суммы)

//---------------------------------------------------
  fn_cod=intrpr.bfcom[1];

  _fmemcpy(buf_tmp,intrpr.bfcom,(long int)(8) );
  if(fn_cod== 3)
  {   // чтение Int
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    Slv_nmbO=f_get_intS(&intrpr.bfcom[4]);

    if(Slv_nmbO>125)
        goto m_err_addr;

    if( f_is_int2(Slv_addrO) &&  f_is_int2(Slv_addrO+Slv_nmbO-1) )
    { // запрошены данные int

      buf_tmp[0]=AddrSlv2;
      buf_tmp[1]=fn_cod;
      buf_tmp[2]=Slv_nmbO+Slv_nmbO;

      for(i1=3,i=0;(i<Slv_nmbO)&&(i1< (n_buf_RTU_s-3)) ;i++)
       {
//       itmp=Slv_int[Slv_addrO+i];
         itmp=*(int *)list_Slv2_int[Slv_addrO+i];
         buf_tmp[i1++]=(itmp>>8) & 0xff;
         buf_tmp[i1++]= itmp & 0xff;
       }
      goto out_b;
    }
    else
     if( f_is_float2(Slv_addrO) &&  f_is_float2(Slv_addrO+Slv_nmbO-2) )
     { // запрошены данные float
      if(Slv_addrO & 0x01) goto m_err_addr;
      if(Slv_nmbO  & 0x01) goto m_err_val;

      buf_tmp[0]=AddrSlv2;
      buf_tmp[1]=fn_cod;
      buf_tmp[2]=Slv_nmbO+Slv_nmbO;
      Slv_nmbO=Slv_nmbO>>1;

      addr_beg=(Slv_addrO-Addr2LowFloat)>> 1;
      for(i1=3,i=0;(i<Slv_nmbO)&&(i1< (n_buf_RTU_s-3));i++,i1+=4)
       {
         if((addr_beg+i) < n_sl_flt0)
           f_set_float( &buf_tmp[i1],*list_Slv2_flt[addr_beg+i],cod_float);
         else
           f_set_float( &buf_tmp[i1],(float)*list_Slv2_lng[addr_beg+i-n_sl_flt0],cod_float);
//         f_set_long( &buf_tmp[i1],*list_Slv2_lng[addr_beg+i-n_sl_flt0]);
       }
      goto out_b;

     }
     else if((Slv_addrO == Addr2Slot1 ) && ( Slv_nmbO <= Slv2_slt1_lgth))
     {  // Slot1

      LIn_D=&Slv2_Slot1[0];
      if(FlagNewDT != 0)
      {
        Tank1F = FlagNewDT;
        LIn_D=&Slv2_Short[0];
        Slv_nmbO=Slv2_short_lgth;
      }
      else Flag_rd_cmd=0;

m_slt:
      buf_tmp[0]=AddrSlv2;
      buf_tmp[1]=fn_cod;
      buf_tmp[2]=Slv_nmbO+Slv_nmbO;

      for(i1=3,i=0,i2=0;(i<Slv_nmbO)&&(i1< (n_buf_RTU_s-3)) ;i2++)
       {
             switch( LIn_D[i2].l )
             {
              case T_INT:
                f_set_int( &buf_tmp[i1], *((int *) LIn_D[i2].ptr)  );
                i1+=2;
                i++;
                break;

              case T_INT_L:
                f_set_long( &buf_tmp[i1], *((long int *) LIn_D[i2].ptr) );
                i1+=4;
                i+=2;
                break;

              case T_FLOAT:
                f_set_float( &buf_tmp[i1], *((float *) LIn_D[i2].ptr),1 );
                i1+=4;
                i+=2;
                break;
             }
       }
      goto out_b;

     }
    else goto m_err_addr;
  }
  if(fn_cod== 6)
  {   // запись Int
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    if( f_is_int2(Slv_addrO) )
    { // запись данных int
      *(int *)list_Slv2_int[Slv_addrO]=f_get_intS(&intrpr.bfcom[4]);
      if((exc=f_int_fnc2(Slv_addrO)) != 0)
         goto m_err;
      i1=6;
      goto out_b;
    }
    else goto m_err_addr;
  }
  if(fn_cod == 16 )
  {   // запись нескольких значений
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    Slv_nmbO=f_get_intS(&intrpr.bfcom[4]);
    if( f_is_int2(Slv_addrO) &&  f_is_int2(Slv_addrO+Slv_nmbO-1) )
    { //   данные int

     for(i1=7,i=0;i<Slv_nmbO;i++,i1+=2)
     {
      *(int *)list_Slv2_int[Slv_addrO+i]=f_get_intS(&intrpr.bfcom[i1]);
      if((exc=f_int_fnc2(Slv_addrO+i)) != 0)
         goto m_err;
     }

     i1=6;
     goto out_b;

    }
    else
     if( f_is_float2(Slv_addrO) &&  f_is_float2(Slv_addrO+Slv_nmbO-2) )
    { //  данные float
      if(Slv_addrO & 0x01) goto m_err_addr;
      if(Slv_nmbO  & 0x01) goto m_err_val;

      addr_beg=(Slv_addrO-Addr2LowFloat)>> 1;
      Slv_nmbO=Slv_nmbO>>1;
      // i1=7-3;
      for(i1=4,i=0;i<Slv_nmbO;i++,i1+=4 )
      {
         if((addr_beg+i) < n_sl_flt0)
            *list_Slv2_flt[addr_beg+i]=f_get_float( &intrpr.bfcom[i1], cod_float);
         else
            *list_Slv2_lng[addr_beg+i-n_sl_flt0]=f_get_float( &intrpr.bfcom[i1], cod_float);
      }
      i1=6;
      goto out_b;
    }

    else if((Slv_addrO == Addr2Slot2 ) && ( Slv_nmbO <= Slv2_slt1_lgth))
    {
     // Slot2

      if(f_set_slv( &Slv2_Slot2[0],Slv_nmbO )) goto m_err_val;
      i1=6;
      if((CMD_OUT_p & 0xfffc) ==Short_OK)
            FlagNewDT=0;

      if( (CMD_IN != CMD_BAD) && (CMD_OUT_p != CMD_BAD) && (Flag_rd_cmd == 0) )CMD_OUT=CMD_OUT_p;
      goto out_b;

    }
    else if((Slv_addrO == Addr2Slot3 ) && ( Slv_nmbO <= Slv2_slt3_lgth))
    {  // Slot3
      if(f_set_slv( &Slv2_Slot3[0],Slv_nmbO )) goto m_err_val;
      if((FlagNewDT & 1) == 0)
      {
       if(Reg_State & DEN_fin )
       {
        if((s_DGM[0].DensReal != Dns1) ||
          (s_DGM[0].TempReal != TTmp1) )
          {
            s_DGM[0].DensReal=Dns1 ;
            s_DGM[0].TempReal=TTmp1 ;

            D1_eee = Dns1  ;
            T1_eee = TTmp1 ;
            f_wr_Dns();

            goto mm10;
          }
       }

        s_DGM[0].DensReal=Dns1 ;
        s_DGM[0].TempReal=TTmp1 ;

      }
mm10:

      f_cnt_totals(); // вычисление суммарных значений при приеме
      i1=6;
      goto out_b;
    }
    else if((Slv_addrO == Addr2Slot4 ) && ( Slv_nmbO <= Slv2_slt4_lgth))
    {  // Slot4
      if(f_set_slv( &Slv2_Slot4[0],Slv_nmbO )) goto m_err_val;

      if((FlagNewDT & 2) == 0)
      {

       if(Reg_State & DEN_fin )
       {
        if((s_DGM[1].DensReal != Dns2) ||
          (s_DGM[1].TempReal != TTmp2) )
          {
            s_DGM[1].DensReal=Dns2 ;
            s_DGM[1].TempReal=TTmp2 ;

            D2_eee = Dns2  ;
            T2_eee = TTmp2 ;
            f_wr_Dns();

            goto mm20;
          }
       }

        s_DGM[1].DensReal=Dns2 ;
        s_DGM[1].TempReal=TTmp2 ;
      }
mm20:
      i1=6;
      goto out_b;
    }
    else goto m_err_addr;
  }
  else  goto m_err_fn ;

//---------------------------------------------------
m_err_fn  : // функция не распознана
 exc=01;
 goto m_err;

m_err_addr: // недопустимый адрес
 exc=02;
 goto m_err;

m_err_val : // недопустимые данные
 exc=03;
 goto m_err;

m_err_slv : // ошибка в Slave
 exc=04;
 goto m_err;

m_err:

 fn_cod |= 0x80;

 buf_tmp[0]=AddrSlv2;
 buf_tmp[1]=fn_cod;
 buf_tmp[2]=exc;
 i2=3;

#if(Slv_deb != 0)

 printf("\n\r====");
 for(i=0;i<i2;i++)
 {
  printf("%02X ",buf_tmp[i]);
 }
 printf("====\n\r");
#endif

 f_prepareRTU(buf_tmp,&i2);

 ToComBufn(ComPortSlv2,buf_tmp,i2);
 return;

out_b:

 buf_tmp[0]=AddrSlv2;
 buf_tmp[1]=fn_cod;

 i2=i1;

#if(Slv_deb != 0)

 printf("\n\r====");
 for(i=0;i<i2;i++)
 {
  printf("%02X ",buf_tmp[i]);
 }
 printf("====\n\r");

#endif

// MmiGotoxy(0,1);    MmiPrintf("Before prep i2= %d    ",i2);
 if(f_prepareRTU1(buf_tmp,&i2)) goto m_err_val;
/*
 MmiGotoxy(0,2);    MmiPrintf("After prep i2= %d     ",i2);
 MmiGotoxy(0,3);    MmiPrintf("%2x %2x %2x %2x %2x %2x     ",buf_tmp[0],buf_tmp[1],buf_tmp[2],buf_tmp[3],buf_tmp[4],buf_tmp[5]);
 MmiGotoxy(0,4);    MmiPrintf("                             ");
 MmiGotoxy(0,5);    MmiPrintf("                              ");
*/
 ToComBufn(ComPortSlv2,buf_tmp,i2);


 if(ff_Slv2 != NULL)
 {
  (*ff_Slv2)(); // старт функции
  ff_Slv2 = NULL;
 }
 return;

}
/*-------------------------------------------*/
/*-------------------------------------------*/
int f_is_int2(int addr)
{
 if((addr >= Addr2LowInt) && (addr <= Addr2HighInt) )
    return 1;
 else return 0;
}
/*-------------------------------------------*/
int f_is_float2(int addr)
{
 if((addr >= Addr2LowFloat) && (addr <= Addr2HighFloat) )
    return 1;
 else return 0;
}
/*-------------------------------------------*/
int  f_int_fnc2(int Addr)
{ // return 1 - error, 0 - OK
 int i_ret;
 i_ret=0;
 switch( Addr)
 {
  case Cmd2_reg:
       switch(*(int *)list_Slv2_int[Addr] )
       {
        case 1:
         break;
        case 2:
         break;
        case 4:
         i_ret= f_rst_slv();
         break;
        case 5:
         i_ret= f_CE_slv();
         break;
        case 6:
         i_ret= f_WE_slv2();
         break;
        case 7:
         i_ret= f_Baud_slv2();
         break;
        case 17:  // fls
         i_ret= f_fls2();
         break;

        default:
           f_cmd2_prn();
           i_ret= RTU_Val;
         break;
       }
   default:
       break;

 }
 return i_ret;
}
/*-------------------------------------------*/
/*-------------------------------------------*/
int f_fls2()
{ // return 1 - error, 0 - OK

 if(ComPortSlv2 == 4)
 {
  flag_Slv2=0;
  RestoreCom(ComPortHost);
  InstallCom(ComPortHost,115200L,8,0,1);
  ShowCom(ComPortHost,8,115200L);
 }
 return 0;
}
/*-------------------------------------------*/
/*-------------------------------------------*/
/*-------------------------------------------*/
/*-------------------------------------------*/
int f_cmd2_prn()
{ // return 1 - error, 0 - OK

#if(Slv_deb != 0)
 printf(" \n\r== Command %d ==",*(int *)list_Slv2_int[4]);
#endif

 return 0;
}
/*-------------------------------------------*/
int f_Baud_slv2()
{
int i_ret;
   if(f_get_nb(Com_Baud_Slave2) < 0  )
   {
 fin_e:
    Com_Baud_Slave2=ComBaudSlv2;
    i_ret = RTU_Val;
   }
   else
   {
     ComBaudSlv2 = Com_Baud_Slave2;

     RestoreCom(ComPortSlv2 );
     if( InstallCom(ComPortSlv2 ,ComBaudSlv2,Com_lgthSlv2,Com_paritySlv2,Com_stopSlv2) < 0 ) goto fin_e;

     if(ComPortSlv2==4)
          ShowCom(ComPortSlv2,Com_lgthSlv2,ComBaudSlv2);

     i_ret = 0;
   }
   return i_ret;
}
/*-------------------------------------------*/
int f_WE_slv2()
{ // return 1 - error, 0 - OK
 ff_Slv2=ff_we_slv;
 return 0;
}
/*-------------------------------------------*/
int f_set_slv( struct eee_dat *LIn_D, int Slv_nmbO )
{
int i,i1,i2;
      for(i1=4,i=0,i2=0;i<Slv_nmbO;i2++ )
      {

             switch( LIn_D[i2].l )
             {
              case T_INT:

                *((int *) LIn_D[i2].ptr)=f_get_int(&intrpr.bfcom[i1]);
                i1+=2;
                i++;
                break;

              case T_INT_L:

                *((long int *) LIn_D[i2].ptr)= f_get_long(&intrpr.bfcom[i1+3]);
                i1+=4;
                i+=2;
                break;

              case T_FLOAT:

                *((float *) LIn_D[i2].ptr)  = f_get_float( &intrpr.bfcom[i1], cod_float);
                i1+=4;
                i+=2;
                break;

              default:
                   return 1;
             }
      }

   return 0;
}
//--------------------------------------
float MassTotal=0,VolTotal=0,DenTotal=-1,TempTotal=-100;
float MassGRK=0., VolGRK=0.;
float Compos_rcv=0;
void f_cnt_totals()
{
  float Dens,M1,M2,V1;
  if(Reg_State & 0x90) // (RCV_act | RCV_fin)
  {
     if((Dens=s_MVD[0].Dens) < 450.) Dens=540.;
     if((M1=s_MVD[0].MassT) == vBIG_P) M1=0.;
     if((M2=s_MVD[1].MassT) == vBIG_P) M2=0.;
     if((V1=s_MVD[0].VolT) == vBIG_P) V1=0.;

     MassTotal = M1+M2 ;
     VolTotal=V1+ M2 * 1000./Dens;
     if(VolTotal != 0)
        DenTotal=MassTotal*1000./VolTotal;
     else
        DenTotal= s_MVD[0].Dens;
     TempTotal=s_MVD[0].Temp;

 //  Compos_rcv=f_get_composition(s_MVD[0].Dens,s_MVD[0].Temp); ...
 // !!! Недоделано
 //  TempTotal =f_get_temp(DenTotal,Compos_rcv);

  }

  if((Reg_State & 0x90)==0)       // RCV_act
     if(( s_MVD[0].VolI!= vBIG_P) && ( s_MVD[0].MassI != vBIG_P))
     {
      VolGRK=-(s_MVD[0].VolI+Vol_kd);
      MassGRK=-(s_MVD[0].MassI+Mass_kd);
     }
     else
     {
      VolGRK=0;
      MassGRK=0.;
     }
}


