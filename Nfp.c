
/*
     Main
*/

#include "ALL.h"

//  Отпуск нефтепродуктов , 2 клапана с разными расходами
//  с приводом "Commander SK" ,"Delta" или с мотором с контактором.
//  При использовании привода - регулятор давления изменяет частоту вращения
// насоса для поддержания заданного давления (MDP==0).
//  Если MDI == 1 , в функции тока двигателя
// изменяется верхний предел частоты вращения двигателя.

//  Emerson 800 Keypad Display
//  Gilbarco Display (3-х строчный семисегментный)

char sw_ver[20]="N3.26 18 Mar 2011";  // COM0 COM1 COM2 COM3 COM4

unsigned long ComBaud[5]={9600,9600,9600,9600,115200};
int Com_lgth[5]={8,8,8,8,8};
int Com_parity[5]={0,0,0,0,0};//0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)

int Com_stop[5]={1,1,1,1,1};

int  ComProt[5]={0,0,0,0,0};

long int baudrate_val[]={1200,1200,1200,1200,2400,4800,9600,19200,38400,57600,115200};

/*
int flag_Serv=0; // 0 - связь с Host PC для отладки
unsigned long ComBaudPmp1=115200L;
int Com_lgthPmp1  =8;
int Com_parityPmp1=0;// Odd   //0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)
int Com_stopPmp1  =1;
int ComPortPmp1 = 4;
int P_addr=1;
//int flag_spec = 0; // флаг расширенного отчета
//long int WD_PMP_Per=20000;
*/

int  MMI_inp_err=0;
int  MMI_out_err=0;
int  ZeroPage=0;
int  EmptPage=31;
//------------------------
int f_get_nb( long int baud)
{
int i;
 for(i=3;i<11;i++)
  if(baudrate_val[i]== baud)
    return i;
 return -1;
}
//------------------------
/*
float var[3000]={
 1.235,
 3.546,
 789000,
 345.678,
 45.678,
 5.678,
 0.678,
 0.0678,

};
*/
//--------------------------
void ShowPort(int port)
{
  Show5DigitLedWithDot(1,port);
}
void ShowChecksum(int checksum)
{
  Show5DigitLedWithDot(2,checksum);
}
void ShowBaudrate(long baud)
{ int data;

  if(baud>100000) data=baud/1000;
  else data=baud/100;

  Show5DigitLed(5,data%10);
  data/=10;
  Show5DigitLed(4,data%10);
  data/=10;
  Show5DigitLed(3,data%10);
}

void ShowCom( int port,int checksum,long int baud)
{
  ShowPort(port);
  ShowChecksum(checksum);
  ShowBaudrate(baud);

}
//-------------------
int i_ish;
void ShowNumber(long Num)
{ // выводит число в десятичной форме на 5DigitLed
int i,i1,dig;
char buf[10];

i_ish=Num;
return;
/*
  sprintf(buf,"%ld",Num);
  i=strlen(buf);
  if(i>0) i--;

  for(i1=0;i1<5;i1++,i--)
  {
    if(i>=0)
    {
     dig=buf[i]-'0';
    }
    else dig=0;
      Show5DigitLed(i1,dig);
  }
  printf("\n\r==%s==\n\r",buf);
*/

}
//-------------------
int getch_host()
{
while(!IsCom(ComPortHost));
 return(ReadCom(ComPortHost));
}
//-------------------
int puts_host(char *str)
{
int i;
 for(i=0;str[i];i++)
 putch(str[i]);
 return i;
}
//-------------------
void  f_cycle_cmn()
{

     f_time_dd();
     if( WDOG[0].status)  f_WDOG(0);

      f_ctrl_v2();

     if(flag_Slv ==0 )
     {
       ServiceCOM(1);
       ServiceCOM(2);
       ServiceCOM(3);
     }
     else
     {
      if(ComPortSlv == 3)
      {
        ServiceCOM(1);
        ServiceCOM(2);
      }
      else if(ComPortSlv == 2)
      {
        ServiceCOM(1);
        ServiceCOM(3);
      }
      else if(ComPortSlv == 1)
      {
        ServiceCOM(2);
        ServiceCOM(3);
      }
      else if(ComPortSlv == 4)
      {
        ServiceCOM(1);
        ServiceCOM(2);
        ServiceCOM(3);
      }
     }


#if( Test == 0)
        if(MVD[0].status)  f_MVD(0);
        else MVD_fl[0]=0;
#else
        f_sim_MVD();
#endif

       f_ctrl_v2();

       i7060_out[0] = OUT_VAR & 0xf;
       i7060_out[1] = (OUT_VAR>>4) & 0xf;

       if(I7060[0].status)  f_7060(0);
       else I7060_fl[0]=0;

       f_inp7188();

#if defined(RTU_ComSK)
        if(ComSK[0].status)  f_ComSK(0);
        else ComSK_fl[0]=0;
#endif

#if ( defined(RTU_Delta) || defined(RTU_Delta_C))
        if(Delta[0].status)  f_Delta(0);
        else Delta_fl[0]=0;
#endif

      f_print_TSD(0);
#if defined(ICP_TSD)
      if(TSD[0].status)  f_TSD(0);
#endif

#if defined(TSD_GLB)
      if(TSD[0].status)  f_TSD_GLB(0);
      else TSD_GLB_fl[0]=0;
#endif
        f_ctrl_v2();

     if(ff_serv_N != NULL) (*ff_serv_N)(); // старт,завершение отпуска/приема

     f_sens();

     if(flag_pool)
     {
#if defined(ICP_7017C)
          if(I7017C[0].status)  f_7017C(0);
#endif
       f_one_sec();
       if(ff_serv != NULL) (*ff_serv)(); // сканирование магистрали,
     }
}
/* ================================================== */
int f_cycle()
{  // основной цикл

     f_cycle_cmn();

//     if(flag_Slv !=0 )
//           f_SlaveRTU();

     if(flag_pool)
     {
       if(f_menu_MMI()) return -1;
       if(MMI.status)
       {
         f_MMI();
       }
       if(Ekd.status)
       {
         if(f_menu_Ekd()) return -1;
         f_Ekd();
       }
       else Ekd_fl=0;
     }

//     if(f_host()) return -1;
     if((flag_Slv !=0 )&&(ComPortSlv == ComPortHost) );
//   if((flag_Slv !=0 )&&(ComPortSlv == 4) );
     else  if(f_host()) return -1;
     trace();
     return 0;
}
/* ================================================== */
int f_cycle0()
{   // без вызова f_host()
     f_cycle_cmn();
     if(flag_pool)
     {
       if(MMI.status)
       {
         if(f_menu_MMI()) return -1;
         f_MMI();
       }
       if(Ekd.status)
       {
         if(f_menu_Ekd()) return -1;
         f_Ekd();
       }
       else Ekd_fl=0;
     }
     trace();
     return 0;
}
//-------------------
int f_cycle1()
{   // без вызова f_MMI()

     f_cycle_cmn();


     if((flag_Slv !=0 )&&(ComPortSlv == 4) );
     else  if(f_host()) return -1;

     if(flag_Slv !=0 )
//           f_SlaveRTU();
//
     trace();
     return 0;
}
/* ================================================== */

/* ---- main ------------------------------------------------------------- */

  int ttt1,ttt2,ttt3,ttt4;
  int itt1,itt2,itt3,itt4;
  long int rltt1,rltt2,rltt3,rltt4;
  long int ltt1,ltt2,ltt3,ltt4;
  int  flag_prn=0;

  unsigned long ttt1_l,ttt2_l,ttt3_l;

  int flag_H=0;
void main(void)
{
  double dtmp1,dtmp2,dtmp3,dtmp4;
  float ftmp,ftmp1,ftmp2,ftmp3,ftmp4;
  int iitmp1,iitmp2,iitmp3;
  int i,j;

  char cmd[40];
//  int checksum=1;
  int key;
  int iRet;
  int year,month,day,hour,min,sec;

  int itmp;
/*
  if(!Is7188xa())
  {
    printf("MMICON.EXE must run on I-7188xa");
    exit (0);
  }
  else
    printf("7188xa is found. Flash size %d\n",itmp);
*/

  InitLib();
  tzset();
  f_ns_count();

  daylight=0;
  timezone=0;

  ComPortHost=4;

  *(long int *)&(TimeStamp)=0;
  InstallCom(ComPortHost,115200L,8,0,1);

  f_get_SlvBuf();
  Com_Baud_Slave=ComBaudSlv;

  for(;TimeStamp < 600;)
  {
    if(!IsCom(ComPortHost)) continue;
    key=ReadCom(ComPortHost);
    if(key == '\b')
    {
      flag_H=1;
      printf("\n\r BS pushed.RTU Slave disabled.");
      while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */

      break;
    }
    if(key == 0x1b )
    {
      flag_H=2;
      printf("\n\r ESC pushed  ");
      while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */

      break;
    }
  }

  RestoreCom(4);
/*
  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  RestoreCom(4);
*/
//  X607_Init();
  InitEEPROM();

  f_get_flash_p();

  f_wr_evt(evt_t_off);
  f_wr_evt(evt_t_on);


  flag_echo_host=1;

  f_queue_init();
  f_ee_num_init();
  Init5DigitLed();

  f_init_et();
  if( flag_H == 2)  goto m_n_ee;
  if(f_chk_EEE_CRC()<0)
  {
 m_n_ee:
    InstallCom(ComPortHost,ComBaud[4],8,0,1);
    ShowCom(ComPortHost,8,ComBaud[ComPortHost]);
    printf("CRC Error.Default loaded\n");
    f_icp_errS(EEE_CRC_error);
    sw_mmi=99;
  }
  else
  {
    f_rd_eee();

//  Out_on(OUT8);

    if( flag_H != 0) flag_Slv=0;

    if((flag_Slv==0)||(ComPortHost != ComPortSlv ) )
    {
     if(f_get_nb(ComBaud[ComPortHost]) <0 ) ComBaud[ComPortHost]=115200L;
     InstallCom(ComPortHost,ComBaud[ComPortHost],Com_lgth[ComPortHost],Com_parity[ComPortHost],Com_stop[ComPortHost]);
     ShowCom(ComPortHost,Com_lgth[ComPortHost],ComBaud[ComPortHost]);
    }

    if(flag_Slv==1)
    {
     if(f_get_nb(ComBaudSlv) <0 ) ComBaudSlv=9600L;
     InstallCom(ComPortSlv,ComBaudSlv,Com_lgthSlv,Com_paritySlv,Com_stopSlv);
     ShowCom(ComPortSlv,Com_lgthSlv,ComBaudSlv);
    }

    if(FlagWinSum) init_win_sum(1);
  }

  Com_Baud_Slave=ComBaudSlv;

  f_rd_cor();

  f_init_COR(0);
  f_init_COR(1);

           if( f_check_id(n_id) == 0)
           {
            id_ok=1;
           }
           else
           {
            id_ok=0;
           }
//  baud=ComBaud[4];
//ShowPort(ComPortHost);
//ShowChecksum(checksum);
//ShowBaudrate(ComBaud[ComPortHost]);
/*=========
    if(EepType==16)
     printf("EEPROM 24LC16 is found (2k bytes)\n");
    else if (EepType==128)
     printf("EEPROM 24WC128 is found (16k bytes)\n");
    else
     printf("Unknown EEPROM.\n");
===========*/
//  TimerOpen();

  f_clr_scr_MMI();

#if defined(EKD_800)
  f_clr_scr_Ekd();
  f_clr_scr_c_Ekd();
  SetEkdPage(ZeroPage);
  f_prn_beginEkd();
#endif

#if defined(ICP_TSD)
  f_clr_TSD(0);
//f_clr_TSD(1);
  f_rev_TSD(0);
//f_rev_TSD(1);
#endif

  if(sw_mmi==0 )
  {

   if((flag_Slv==0)||(ComPortHost != ComPortSlv ) )
   {
#if( Test == 0)
     printf("\nFacom Ver.%s",sw_ver);
#else
     printf("\nFacom.Test version.MVD simulate. Ver. %s",sw_ver);
#endif

      if(id_ok==0)
            printf("\n\r == Not correct ID !");
   }
   f_prn_begin();
  }
  else f_prn_CRC_error();

  SetDisplayPage(ZeroPage);

 /* MMI use I_7188 COM1(RS-232,RS-485),9600,N,8,1 */

  for(i=1;i<4;i++)
  {
   if(i != ComPortSlv)
   {
    if(f_get_nb(ComBaud[i]) <0)  ComBaud[i]=9600L;
    InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);
   }
  }

#if defined(ICP_TSD)
   f_init_TSD();
#endif

#if defined(TSD_GLB)
   f_init_TSD();
#endif

 if((flag_Slv==0)||(ComPortHost != ComPortSlv ) )
 printf("\n\r>");

  if(I7017C[0].status == 0 )
  {
     s_MVD[0].Press= analog_offset[0];
     s_MVD[0].TempR= analog_offset[1];

    //14.06.2021 YN
    #if defined(UNDERPRESSURE)
      Under_Press= analog_offset[2];
    #endif

  }
  one_sec_time=TimeStamp;
  while(1)
  {
     if(f_cycle()<0) break;
  }
printf("\n\rExit\n\r");

  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
  RestoreCom(4);
  f_free_SlvBuf();
}

/* ================================================== */
int last_out[5]={-1,-1,-1,-1,-1};
long int scom_tim[5]={1,1,1,1,1};
long int Tm_snd[5]= {0,0,0,0,0};

void ServiceCOM ( int ii)
/*
  Функция принимает ответ из SLAVE контроллера
  Функция принимает ответ на команду посланную в  COM ii.
  После завершения приема команды, принятая команда обрабатывается
  потребителем, пославшим команду в Slave
*/
{
int i,Addr_rcv,itmp;
struct COM_rqst *request;
struct s_icp_dev *ICPl_DEV;
int  tmp;

 if(scom_tim[ii]!=0)
  if(  f_timer(time_rcv[ii],scom_tim[ii]) == 0)
  {
   if(IsCom(ii)!=0)
   {
     while(IsCom(ii)!=0)
     {
      tmp= ReadCom(ii);

    if(flag_Deb)
      printf("\n%02X",tmp&0xff);
      time_rcv[ii]= TimeStamp;
     }
   }
   return;
  }

  if(COM_que[ii].busy_ptr != COM_que[ii].empt_ptr)
  {
    if(last_out[ii] != COM_que[ii].empt_ptr)
    {
      request=COM_que[ii].request[COM_que[ii].empt_ptr];
      if(request==NULL)
         goto m1;

      last_out[ii]=COM_que[ii].empt_ptr;

      // подготовка к обслуживанию запроса и вывод в порт

      ff_gets_com[ii]=request->gets_com;
      ff_answ_com[ii]=request->answ_com;
      ff_answ_flt[ii]=request->answ_flt;
      ToutAns[ii]=request->timeout;
      time_snd[ii]= TimeStamp;
      Tm_snd[ii]= TimeStamp;

      ICPl_DEV=request->ICP_dd;
      ICPl_DEV->time_stamp_snd=TimeStamp-request->time_stamp;

      // вывод в порт
//  ltt1=TimeStamp;
//  tim1();

#if defined(I7188D)
  Set485DirToTransmit(ii); //qqq
#endif

   ToComBufn(ii,request->Cmd,request->cmd_lgth);
/*
   if(ii == 3)
   {  printf("\n\rOUT_C3=");
      for(i=0;i<request->cmd_lgth;i++)
      putch(request->Cmd[i]);
   }
*/
#if defined(I7188D)
  WaitTransmitOver(ii);
  Set485DirToReceive(ii);
#endif

//  tim2();
//  ltt2=TimeStamp;

//   itt1=ttt2-ttt1-10;  // 10 следует отнять - это время присутствует
//   if(itt1 < 0) itt1+=10000;
//   rltt1=ltt2-ltt1;

//   if(flag_prn)
//   {
//     flag_prn=0;
//     printf("\n %d bytes send, time=%ld ms, %.1f mks",request->cmd_lgth,rltt1,(float)(itt1)/10.);
//   }
      request->status=2;
    }
  }

m1:
  if( (*ff_gets_com[ii])(ii) )
 /* Ввод ответа на команду. Указатель на функцию ввода
    устанавливает функция, инициировавшая передачу команды
 */
  {
     time_rcv[ii]= TimeStamp;
     request=COM_que[ii].request[COM_que[ii].empt_ptr];
     ICPl_DEV=request->ICP_dd;
     ICPl_DEV->time_stamp_rcv=TimeStamp - request->time_stamp;

   /* в cb_COM[ii][] находится команда длиной  n_bcom[ii]  */

   /* Обработка команды. Указатель на функцию обработки устанавливает
      функция, инициирующая передачу
   */

     ff_gets_com[ii]=f_empty_gets;
     (*ff_answ_com[ii])(ii);

     ff_answ_com[ii]=f_empty;
     n_bcom[ii]=0;
  }
}
/* ================================================== */
int f_timer(unsigned long int time_stamp,unsigned long int delay)
{
// если время с момента записи временного штампа превышает
 // delay мс - возвращает 1, иначе 0.
// if((TimeStamp-time_stamp) > delay) return 1;
// else return 0;
 if((TimeStamp-time_stamp - delay) & 0x80000000 ) return 0;
 else return 1;
}
/* ================================================== */

unsigned long int time_snd[5]={0,0,0,0,0};
unsigned long int time_rcv[5]={0,0,0,0,0};


 char cb_COM[5][n_bufcom];
 int ghj=0;
 int n_bcom[5]={0,0,0,0,0};

void (*ff_serv)() = NULL;

int  (*ff_gets_com[5])(int ii) = {f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets};
void (*ff_answ_com[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
void (*ff_answ_flt[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
int i_empty;
void f_empty(int ii)
{
 i_empty=ii;
}
/* ---------------------------------------- */
/*================================================================*/

//14.06.2021 YN
#if defined(UNDERPRESSURE)
  float ftmp_naMMI[3];
#else
  float ftmp_naMMI[2];
#endif

struct dis_set_MMI ds_list1[]=
{
//-----------
  list1_dsr,
  17,
  &flag_Slv,
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  15,
  &AddrSlv,
  0,
  255,
  T_INT,
//----------------
  list1_dsr,
  158,
  &Flow_nom,
  0,
  BIG_P,
  T_INT_L,
//-----------
  list1_dsr,
  165,
  &type_liq ,
  1,
  6,
  T_INT,
//-----------
  list1_dsr,
  159,
  &Mass_min ,
 -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  160,
  &Flow_min,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  161,
  &Mass_min1 ,
 -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  162,
  &Flow_min1,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  163,
  &Mass_min2 ,
 -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  164,
  &Flow_min2,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  91,
  &ftmp_naMMI[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  93,
  &ftmp_naMMI[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,

  //14.06.2021 YN
  #if defined(UNDERPRESSURE)
    list1_dsr,
    167,
    &ftmp_naMMI[2],
    -BIG_P,
    BIG_P,
    T_FLOAT,
  #endif

//-----------
  list1_dsr,
  92,
  &analog_offset[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list1_dsr,
  94,
  &analog_offset[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,

  //14.06.2021 YN
  #if defined(UNDERPRESSURE)
    list1_dsr,
    168,
    &analog_offset[2],
    -BIG_P,
    BIG_P,
    T_FLOAT,
  #endif

/*----------------*/
  list1_dsr,
  157,
  &CL_valV[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list1_dsr,
  99,
  &CL_val[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list1_dsr,
  101,
  &CL_T2,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//---------------
  list1_dsr,
  98,
  &CL_val[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//---------------
  list1_dsr,
  156,
  &CL_valV[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//---------------
  list1_dsr,
 100,
  &CL_T1,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//---------------
  list1_dsr,
 102,
  &CL_T3,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//---------------
  list1_dsr,
 103,
  &CL_T4,
  -BIG_P,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  123,
  &cl1_delay_off,
  0,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  153,
  &cl2_delay_off,
  0,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  154,
  &Out3_delay_off,
  0,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  117,
  &TimDensWt1,
  0,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  118,
  &TimDensWt0,
  0,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  106,
  &t_mvd_cor,
  0,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  104,
  &fct_offs,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//---------------
  list1_dsr,
  105,
  &fct_delay,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  80,               // Клапан Блш N вых.
  &num_out1,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  81,               // Клапан Мнш N вых.
  &num_out2,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  142,               // Вкл.насоса N вых.
  &num_out3,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  147,               // Сигнализ-я N вых.
  &num_out8,
  0,
  8,
  T_INT,
//----------------
  list1_dsr,
  148,               // Давление N анлг.вх.
  &analog_num[0],
  0,
  8,
  T_INT,
//----------------
  list1_dsr,
  149,               // Температ.N анлг.вх.
  &analog_num[1],
  0,
  8,
  T_INT,

  //14.06.2021 YN
  #if defined(UNDERPRESSURE)
    list1_dsr,
    166,               // Разряж. N анлг.вх
    &analog_num[2],
    0,
    8,
    T_INT,
  #endif

//-----------
  list1_dsr,
  143,               // Аварийная кн. N вх.
  &num_in1,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  144,               // Аварийная кн.флаг инверсии
  &mski_inp1,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  145,               // Сигнал УЗА. N вх.
  &num_in2,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  146,               // Сигнал УЗА. флаг инверсии
  &mski_inp2,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  126,               // Датчик уровня N вх.
  &num_in3,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  127,               // Датчик уров. фл.инв
  &mski_inp3,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  128,               // Кн.  =Start=  N вх.
  &num_in4,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  129,               // Кн.  =Start= фл.инв
  &mski_inp4,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  130,               // Кн.  =Stop=   N вх.
  &num_in5,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  131,               // Кн.  =Stop=  фл.инв
  &mski_inp5,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  132,               // Сигнал 'ТРАП' N вх.
  &num_in6,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  133,               // Сигнал 'ТРАП' фл.инв
  &mski_inp6,
  0,
  8,
  T_INT,
//-----------
  list1_dsr,
  115,               // Сгн.'КОНСОЛЬ' N вх.
  &num_in7,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  116,               // Сгн.'КОНСОЛЬ' фл.инв
  &mski_inp7,
  0,
  8,
  T_INT,
//-----------
//----------------
  list1_dsr,
  0,
  &MMI.status,
  0.,
  1.,
  T_INT,
//-----------
//-----------
  list1_dsr,
  51,
  &MVD[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  16,   // Delta :Статус
  &Delta[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  56,
  &TSD[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  37,
  &WDOG[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  18,
  &I7060[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  6,
  &I7017C[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  34,
  &ToutLicpScan,
  0.,
  BIG_I,
  T_INT,
//-----------
  list1_dsr,
  35,
  &licp_amax,
  0.,
  255,
  T_INT,
//-----------
  list1_dsr,
  36,
  &LICP_Rqst.CRC_flag,
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  85,
  &ComProt[1],
  0,
  2,
  T_INT,
//-----------
  list1_dsr,
  86,
  &ComProt[2],
  0,
  2,
  T_INT,
//-----------
  list1_dsr,
  87,
  &ComProt[3],
  0,
  2,
  T_INT,
//-----------
  list1_dsr,
  4,
  &TSD_vn[0][0] ,
  1,
  8,
  T_INT,
//-----------
  list1_dsr,
  95,
  &TSD_vn[0][1] ,
  1,
  8,
  T_INT,
//-----------
  list1_dsr,
  96,
  &TSD_vn[0][2] ,
  1,
  8,
  T_INT,
//-----------
  list1_dsr,
  1,
  &n_dot[0][0],
  1,
  6,
  T_INT,
//-----------
  list1_dsr,
  2,
  &n_dot[0][1],
  1,
  6,
  T_INT,
//-----------
  list1_dsr,
  3,
  &n_dot[0][2],
  1,
  6,
  T_INT,
//-----------
  list1_dsr,
  97,
  &TSD_period,
  1,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  108,
  &tim_MVD_delay,
  -BIG_P,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  121,
  &k_Den_Off,
  0,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  122,
  &k_Den_On,
  0,
  BIG_P,
  T_FLOAT,
//-------------
  list1_dsr,
  137,           // Время запр.переп,тик
  &TimeBegFlch,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  138,           // Масса начала опред.переп
  &val0_flch,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  139,           // Масса опред.переп BIG
  &val_flch,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  140,           // Масса опред.переп LTL
  &val_flch2,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  141,           // Время переполн,тик
  &time_flch,
  0,
  max_fl_ch_m1,
  T_INT,
//-----------
  list1_dsr,
  32,
  &ComBaud[3],
  1200,
  115200,
  T_INT_L,
//-----------
  list1_dsr,
  33,
  &ComBaud[4],
  1200,
  115200,
  T_INT_L,
//-----------
  list1_dsr,
  151,
  &Drive_SV_beg[0],
  0.,
  BIG_P,
  T_INT,
//----------------
  list1_dsr,
  152,
  &Drive_SA_beg[0],
  0.,
  BIG_P,
  T_INT,
//----------------
  list1_dsr,
  155,
  &Drive_SD_beg[0],
  0.,
  BIG_P,
  T_INT,
//-----------
  list1_dsr, //
  14,        // Mode P
  &mode_r ,
  0,
  3,
  T_INT,
//----------------
  list1_dsr, //
  10,
  &ftmp_DP,
  0,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr, //
  12,
  &k_p,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr, //
  13,
  &k_i,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr, //
  11,
  &k_d,
  0,
  BIG_P,
  T_INT_L,
//-----------
  list1_dsr, //
  19,
  &kf_p,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr, //
  20,
  &kf_i,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr, //
  21,
  &kf_d,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr, // Переход летнее время
  89,
  &FlagWinSum,
  0,
  1,
  T_INT,
//-----------
//-----------
  NULL,
  0,
  NULL,
  0,
  1,
  T_INT,
//-----------
//-----------
};
//-----------------------------------------
/*
  list1_dsr, // Тип уч.выборки
  88,
  &s_mode,
  1,
  4,
  T_INT,
*/
/*
  list1_dsr,
  124,
  &TimDensOff,
  0,
  BIG_P,
  T_INT_L,
//----------------------
  list1_dsr,
  125,
  &TimDensOn,
  0,
  BIG_P,
  T_INT_L,
*/
/*
  list1_dsr,
  109,
  &cutoff_off,
  0,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  110,
  &vol_mom0,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  111,
  &vol_mom,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  112,
  &flow_mom,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  113,
  &offs_cor,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  114,
  &dsc_tmp,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
*/

/*
  list1_dsr,
  107,
  &t_ctrl,
  0,
  2,
  T_INT,
*/
/*
  list1_dsr,
  38,
  &WDOG[0].port,
  1.,
  3.,
  T_INT,
//-----------
  list1_dsr,
  39,
  &WDOG[0].pool_time,
  50,
  10000,
  T_INT_L,
//-----------
  list1_dsr,
  40,
  &WDOG_Rqst[0].timeout,
  1,
  255,
  T_INT_L,
//-----------
  list1_dsr,
  41,
  & WDOG[0].CRC_flag,
  0,
  1,
  T_INT,
//-----------
*/
/*
  list1_dsr,
  19,
  &I7060[0].port,
  1.,
  3.,
  T_INT,
//-----------
  list1_dsr,
  20,
  &I7060[0].addr,
  0,
  255,
  T_INT,
//-----------
  list1_dsr,
  21,
  &I7060[0].pool_time,
  50,
  10000,
  T_INT_L,
//-----------
  list1_dsr,
  22,
  &I7060_Rqst[0].timeout,
  100,
  20000,
  T_INT_L,
//-----------
  list1_dsr,
  23,
  &I7060[0].CRC_flag,
  0,
  1,
  T_INT,
//-----------

//-----------
  list1_dsr,
  47,
  &I7060_wd_E[0],
  0.,
  1,
  T_INT,
//-----------
  list1_dsr,
  48,
  &I7060_wd_VV[0],
  0.,
  255,
  T_INT,
//-----------
*/
/*
  list1_dsr,
  52,
  &MVD[0].port,
  1.,
  3.,
  T_INT,
//-----------
  list1_dsr,
  53,
  &MVD[0].addr,
  0,
  255,
  T_INT,
//-----------
  list1_dsr,
  54,
  &MVD[0].pool_time,
  30,
  10000,
  T_INT_L,
//-----------
  list1_dsr,
  55,
  &MVD_Rqst[0].timeout,
  100,
  20000,
  T_INT_L,
//-----------
  list1_dsr,
  57,
  &TSD[0].port,
  1.,
  3.,
  T_INT,
//-----------
  list1_dsr,
  58,
  &TSD[0].addr,
  0,
  255,
  T_INT,
//-----------
  list1_dsr,
  59,
  &TSD[0].pool_time,
  50,
  10000,
  T_INT_L,
//-----------
  list1_dsr,
  60,
  &TSD_Rqst[0].timeout,
  100,
  20000,
  T_INT_L,
//-----------
*/
/*
  list1_dsr,
  7,
  &I7017C[0].port,
  1.,
  3.,
  T_INT,
//-----------
  list1_dsr,
  8,
  &I7017C[0].addr,
  0,
  255,
  T_INT,
//-----------
  list1_dsr,
  9,
  &I7017C[0].pool_time,
  50,
  10000,
  T_INT_L,
//-----------
  list1_dsr,
  10,
  &I7017_Rqst[0].timeout,
  100,
  20000,
  T_INT_L,
//-----------
  list1_dsr,
  11,
  &I7017C[0].CRC_flag,
  0,
  1,
  T_INT,
//-----------
*/
/*
  list1_dsr,
  1,
  &MMI.port,
  1.,
  3.,
  T_INT,
//-----------
  list1_dsr,
  2,
  &MMI.addr,
  0,
  255,
  T_INT,
//-----------
  list1_dsr,
  3,
  &MMI.pool_time,
  50,
  10000,
  T_INT_L,
//-----------
  list1_dsr,
  4,
  &MMI_Rqst.timeout,
  100,
  20000,
  T_INT_L,
//-----------
  list1_dsr,
  5,
  &MMI.CRC_flag,
  0,
  1,
  T_INT,
//-----------
*/
/*
  list1_dsr,
  64,
  &Com_lgth[4],
  7,
  8,
  T_INT,
//-----------
  list1_dsr,
  68,
  &Com_parity[4],
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  72,
  &Com_stop[4],
  1,
  2,
  T_INT,
//-----------
//-----------
  list1_dsr,
  30,
  &ComBaud[1],
  1200,
  115200,
  T_INT_L,
//-----------
  list1_dsr,
  61,
  &Com_lgth[1],
  7,
  8,
  T_INT,
//-----------
  list1_dsr,
  65,
  &Com_parity[1],
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  69,
  &Com_stop[1],
  1,
  2,
  T_INT,
//-----------
//-----------

//-----------
  list1_dsr,
  31,
  &ComBaud[2],
  1200,
  115200,
  T_INT_L,
//-----------
  list1_dsr,
  62,
  &Com_lgth[2],
  7,
  8,
  T_INT,
//-----------
  list1_dsr,
  66,
  &Com_parity[2],
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  70,
  &Com_stop[2],
  1,
  2,
  T_INT,
//-----------
*/
/*
  list1_dsr,
  63,
  &Com_lgth[3],
  7,
  8,
  T_INT,
//-----------
  list1_dsr,
  67,
  &Com_parity[3],
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  71,
  &Com_stop[3],
  1,
  2,
  T_INT,
//-----------
*/
/*-----------------------------------------*/
struct dis_set_MMI ds_list2[]=
{
//-----------
  list2_dsr,
  0,
  &n_dat_et[0],
  0.,
  MAX_COR_D-1,
  T_INT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][0],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][1],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][2],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][2],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][3],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][3],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][4],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][4],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][5],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][5],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][6],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][6],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][7],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][7],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[0][8],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  2,
  &val_et[0][8],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
//-----------
  NULL,
  0,
  NULL,
  0,
  1,
  T_INT,
//-----------
//-----------
};

/*-----------------------------------------*/
struct dis_set_MMI ds_list3[]=
{
//-----------
  list2_dsr,
  0,
  &n_dat_et[1],
  0.,
  MAX_COR_D-1,
  T_INT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][0],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][1],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][2],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][2],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][3],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][3],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][4],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][4],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][5],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][5],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][6],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][6],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][7],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][7],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  1,
  &adc_et[1][8],
  -MaxADC,
  MaxVADC,
  T_FLOAT,
/*----------------*/
  list2_dsr,
  3,
  &val_et[1][8],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
//-----------
  NULL,
  0,
  NULL,
  0,
  1,
  T_INT,
//-----------
//-----------
};

/*-----------------------------------------*/
int max_list1=(sizeof(ds_list1)/sizeof(ds_list1[0])) -1;
int max_list2=(sizeof(ds_list2)/sizeof(ds_list2[0])) -1;
int max_list3=(sizeof(ds_list3)/sizeof(ds_list2[0])) -1;
/*-----------------------------------------*/
long int time1=0,dd_time,dd_max=-1,dd_min=10000000;
void f_time_dd()
{
long int ltmp;
ltmp=TimeStamp;
dd_time=ltmp-time1;
time1=ltmp;
if(dd_time > dd_max) dd_max=dd_time;
else if(dd_time < dd_min) dd_min=dd_time;
}
//---
void f_print_dd()
{
 printf(" %ld , min= %ld, max=%ld",dd_time,dd_min,dd_max);
 dd_min=10000000;
 dd_max=-1;
}
/*-----------------------------------------*/
unsigned long int one_sec_time=0;

void f_one_sec()
{
union  { float f; char c[4]; } o;
  int win_sum;
  TIME_DATE t;

 if(flag_pool)
 {
  if( f_timer(one_sec_time,1000L) == 0)  return;

  one_sec_time+=1000;

  GetTimeDate(&t);


  if(FlagWinSum)
   {
      if((t.sec <= 2)&& (t.minute==0))
        if((t.day >= 25)&&(t.weekday==0))
        {
          if( (t.month==3)&& (t.hour==2) )
          // в 2 часа ,псл Вск, mart,ч+=1
          {  /*переход на летнее время */

           win_sum = ReadNVRAM( nr_win_sum )  ;
           if(win_sum==0)
           { // еще не переведено
            f_wr_evt(evt_none);
            t.hour++;
            win_sum=1;  // 1 - лето
            SetTime((int)t.hour,(int)t.minute,(int)t.sec);

            WriteNVRAM((int)nr_win_sum,(int)win_sum );
            f_wr_evt(evt_ch_sum);
           }
          }
          else if((t.month==10)&& (t.hour==3) )
           // в 3 часа ,псл Вск, oct,ч-=1
          { //переход на зимнее время

            win_sum  = ReadNVRAM( nr_win_sum )  ;
            if(win_sum)
            { // еще не переведено
              f_wr_evt(evt_none);
              t.hour--;
              win_sum=0; // зимнее
              SetTime((int)t.hour,(int)t.minute,(int)t.sec);
              WriteNVRAM((int)nr_win_sum,(int)win_sum );
              f_wr_evt(evt_ch_win);
            }
          }
        }
   }
    WriteNVRAM((int)nr_year   ,(int)(t.year-2000));
    WriteNVRAM((int)nr_month  ,(int)t.month  );
    WriteNVRAM((int)nr_day    ,(int)t.day    );
    WriteNVRAM((int)nr_hour   ,(int)t.hour   );
    WriteNVRAM((int)nr_min    ,(int)t.minute );
    WriteNVRAM((int)nr_sec    ,(int)t.sec    );
    o.f=  s_MVD[0].VolI;
    WriteNVRAM((int) nr_v0   ,(int)o.c[0] );
    WriteNVRAM((int) nr_v1   ,(int)o.c[1] );
    WriteNVRAM((int) nr_v2   ,(int)o.c[2] );
    WriteNVRAM((int) nr_v3   ,(int)o.c[3] );
    o.f=  s_MVD[0].MassI;
    WriteNVRAM((int) nr_m0   ,(int)o.c[0] );
    WriteNVRAM((int) nr_m1   ,(int)o.c[1] );
    WriteNVRAM((int) nr_m2   ,(int)o.c[2] );
    WriteNVRAM((int) nr_m3   ,(int)o.c[3] );
 }
}
/*-----------------------------------------*/
float cl_val_c=-100000;
float cl_t_c=-100000;
int num_clp_sv=1; // 1...3 номер клапана сохраняемого в NVRAM
                  // 1 - самый маленький

#define CL_MassP  (*lst_Mass[num_clp_sv])
#define CL_VolP    (*lst_Vol[num_clp_sv])

float *lst_Mass[4]={
  &CL_val[1],
  &CL_val[1],  //
  &CL_val[0],
  &CL_val0
};

float *lst_Vol[4]={
  &CL_valV[1],
  &CL_valV[1],  //
  &CL_valV[0],
  &CL_valV0
};

void f_wr_cor()
{
float ftmp;
union  { float f; char c[4]; } o;

    ftmp=CL_MassP+CL_VolP;

    if( (cl_val_c != CL_MassP) || (cl_t_c != CL_VolP ) )
    {
       o.f= CL_MassP ;
       cl_val_c=o.f;
       WriteNVRAM((int) nr_dv0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_dv1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_dv2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_dv3   ,(int)o.c[3] );

       o.f= CL_VolP;
       cl_t_c=o.f;
       WriteNVRAM((int) nr_dt0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_dt1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_dt2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_dt3   ,(int)o.c[3] );

       o.f= ftmp;
       WriteNVRAM((int) nr_sm0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_sm1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_sm2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_sm3   ,(int)o.c[3] );
    }
}
/*-----------------------------------------*/
void f_rd_cor()
{
union  { float f; char c[4]; } o;
float ftmp,ftmp1,ftmp2,ftmp3;

 o.c[0]= ReadNVRAM( nr_dv0 );
 o.c[1]= ReadNVRAM( nr_dv1 );
 o.c[2]= ReadNVRAM( nr_dv2 );
 o.c[3]= ReadNVRAM( nr_dv3 );
 ftmp = o.f;
 o.c[0]= ReadNVRAM( nr_dt0 );
 o.c[1]= ReadNVRAM( nr_dt1 );
 o.c[2]= ReadNVRAM( nr_dt2 );
 o.c[3]= ReadNVRAM( nr_dt3 );
 ftmp1 = o.f;
 ftmp3=ftmp+ftmp1;
 o.c[0]= ReadNVRAM( nr_sm0 );
 o.c[1]= ReadNVRAM( nr_sm1 );
 o.c[2]= ReadNVRAM( nr_sm2 );
 o.c[3]= ReadNVRAM( nr_sm3 );
 ftmp2 = o.f;
 if(ftmp2== ftmp3)
 {
   CL_MassP=ftmp;
   CL_VolP=ftmp1;
 }
}
/*-----------------------------------------*/
/*-----------------------------------------*/
int init_win_sum(int cor)
{
//  возвращает признак зимнего или летнего времени
//  для текущего момента: 1 - летнее время , 0 - зимнее время
//  если cor != 0 корректирует текущее время при переходе зим./летн.

  int year,month,day,hour,min,sec;
  int win_sum;
  time_t ltime1,ltime2;
  int prev_ws;

  struct tm *tdat;

 int i,i1;
 TIME_DATE t;
struct arch_str_t tim_0;

 f_prepare_t(&tim_0);
 GetTimeDate(&t);
/*
 tim_0.year  = t.year-2000;
 tim_0.month = t.month;
 tim_0.day   = t.day;
 tim_0.hour  = t.hour;
 tim_0.min   = t.minute;
 tim_0.sec   = t.sec;
*/

// printf("\n\r %02d.%02d.%02d  " ,tim_0.day,tim_0.month,tim_0.year);
// printf("%02d.%02d.%02d -- ",(int)tim_0.hour,(int)tim_0.min,(int)tim_0.sec);

 ltime1=f_get_time_t(&tim_0); // текущее время

  switch((int)tim_0.month)
  {
   case 11:
   case 12:
   case 1:
   case 2: // зима
         win_sum=0;
         break;
   case 4:
   case 5:
   case 6:
   case 7:
   case 8:
   case 9: // лето
         win_sum=1;
         break;
   case 3:  // март
        if(t.day>= 10) i=t.day-t.weekday;
        else i=t.day+7-t.weekday;
        for(;i<=31;i+=7)
         {
          i1=i;
         }
//        printf("\n\r==Last Sunday - %d Mart ",i1);
      // i1 - число последнего воскресенья марта
         tim_0.day=i1;
         tim_0.hour=2;
         tim_0.min=0;
         tim_0.sec=0;

    ltime2=f_get_time_t(&tim_0); // время Ч

    if(ltime1<ltime2)
         win_sum=0;
    else
         win_sum=1;
    break;
   case 10:  // октябрь
        if(t.day>= 10) i=t.day-t.weekday;
        else i=t.day+7-t.weekday;
        for(;i<=31;i+=7)
         {
          i1=i;
         }
      // i1 - число последнего воскресенья октября
         tim_0.day=i1;
         tim_0.hour=3;
         tim_0.min=0;
         tim_0.sec=0;
 //       printf("\n\r==Last Sunday - %d Oct ",i1);

// printf("\n\r %02d.%02d.%02d  " ,tim_0.day,tim_0.month,tim_0.year);
// printf("%02d.%02d.%02d -- ",(int)tim_0.hour,(int)tim_0.min,(int)tim_0.sec);

    ltime2=f_get_time_t(&tim_0); // время Ч
//    printf("\n\rltime1=%ld,ltime2=%ld",ltime1,ltime2);

    if( (ltime2>ltime1)&& ((ltime2 - ltime1)< 3600L))
    {
      win_sum=ReadNVRAM(nr_win_sum);
      break;
    }
    if(ltime1<ltime2)
    {
         win_sum=1;
    }
    else
    {
         win_sum=0;
    }
    break;
  }

  if(cor)
  {
    prev_ws=ReadNVRAM(nr_win_sum);
    if((prev_ws==0)&&(win_sum!=0))
    {// переход на летнее время
      ltime1+=3600L;

      tdat = localtime( &ltime1 );
      year   = tdat->tm_year+1900;
      month  = tdat->tm_mon+1;
      day    = tdat->tm_mday ;
      hour   = tdat->tm_hour;
      min    = tdat->tm_min;
      sec    = tdat->tm_sec;

      SetTime(hour, min,sec);
      SetDate(year, month, day);
      f_wr_evt(evt_ch_sum);
    }
    else if((prev_ws!=0)&&(win_sum==0))
    {// переход на зимнее время
      ltime1-=3600L;

      f_wr_evt(evt_ch_win);

      tdat = localtime( &ltime1 );
      year   = tdat->tm_year+1900;
      month  = tdat->tm_mon+1;
      day    = tdat->tm_mday ;
      hour   = tdat->tm_hour;
      min    = tdat->tm_min;
      sec    = tdat->tm_sec;
      SetTime(hour,min,sec);
      SetDate( year, month, day);
    }
  }
  WriteNVRAM((int)nr_win_sum,(int)win_sum );
  return win_sum;
}
/*-----------------------------------------*/

// см.  device.h

#include "MMI.c"
#include "ekd.c"

#if defined(ICP_7017C)
#include "7017c.c"
#endif

#include "i7060.c"


#if defined(RTU_ComSK)
#include "sk.c"
#endif

#if defined(RTU_Delta)
#include "delta.c"
#endif

#if defined(RTU_Delta_C)
#include "deltaC.c"
#endif


#if defined(RTU_MVD)
#include "MVD.c"
#endif

#include "valve.c"

#include "avt_ctrw.c"

// допустима только одна из двух строк
// необходимо объявить через #define требуемый флаг в device.h

#if defined(ICP_TSD)
// трехстрочный дисплей с протоколом ICP
  #include "tsd.c"
#endif

#if defined(TSD_GLB)
// трехстрочный дисплей Gilbarco
  #include "tsd_glb.c"
#endif

#include "icp.c"
#include "queue.c"
#include "ref_t.c"

/*-----------------------------------------*/
