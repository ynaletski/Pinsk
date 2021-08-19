
//---------------------------
// Отпуск нефтепродуктов
//---------------------------
// Аналоговые входы:
//     7017C Vin0+ (11) - датчик давления
//     7017C Vin1+ (13) - датчик температуры
//
//---------------------------
//                        номер входа (команда 'MKS')
//
// INP_VAR D0   7041-DI 0    1   Кн. Старт верхнего налива
//         D1   7041-DI 1    2   Кн. Стоп  верхнего налива
//         D2   7041-DI 2    3   Кн. Старт нижнего налива
//         D3   7041-DI 3    4   Кн. Стоп  нижнего налива
//         D4   7041-DI 4    5   Уровень нижнего налива
//         D5   7041-DI 5    6
//         D6   7041-DI 6    7
//         D7   7041-DI 7    8
//         D8   7041-DI 8    9
//         D9   7041-DI 9   10
//         D10  7041-DI 10  11   УЗА верхнего и нижнего налива
//         D11  7041-DI 11  12   Консоль
//         D12  7041-DI 12  13   Трап
//         D13  7041-DI 13  14   Уровень верхнего налива
//                          15
//                          16
//
// INP_OFP D0               17       Состояние секций , 1 - 6, 1 - ok
//         D1               18
//         D2               19
//         D3               20
//         D4               21
//         D5               22
//         D6               23       Состояние общее , 1 - ok
//         D7               24       Состояние PE    , 1 - ok
//
//---------------------------
//               номер выхода (команда 'MKO')
//
// OUT_VAR D0   7044-OUT1  1    больший клапан верхнего налива  (p1.MKO)
//         D1   7044-OUT2  2    меньший клапан верхнего налива  (p2.MKO)
//         D2   7044-OUT3  3    больший клапан нижнего  налива  (p5.MKO)
//         D3   7044-OUT4  4    меньший клапан нижнего  налива  (p6.MKO)
//         D4   7044-OUT5  5
//         D5   7044-OUT6  6    Воздух     (не исп.)  p4.MKO
//         D6   7044-OUT7  7    Вкл.насоса (не исп.)  p3.MKO
//         D7   7044-OUT8  8    Нет аварии (не исп.)  p8.MKO
//---------------------------
//       1 2 3 4 5 6 7  8
//  "MKO 2 1 7 6 4 3 0  8"

int result_dlv=0;

int num_out1=1;  //  больший клапан регулирования расхода,пилот нормально открытый , /DN закрыть задвижку (OUT1=0)
int num_out2=2;  //  меньший клапан регулирования расхода,пилот нормально закрытый ,  UP открыть задвижку (OUT2=1)
int num_out3=7;  //  включение насоса
int num_out4=6;  //  включение air switch (OUT4)
int num_out5=3;  //  больший клапан нижнего  налива  // BIO_1 - исполнительный выход для CL1_SRC
int num_out6=4;  //  меньший клапан нижнего  налива  // BIO_1 - исполнительный выход для CL2_SRC
int num_out7=0;  //
int num_out8=8;  //  сигнал аварии , инверсный , 1 - OK.


// MKO 0 3 4 0 1 2 0 8 - BIO

//     1  2  3 4 5  6  7  8  9  10  11  12
// MKS 2 11 14 1 0 12 13  4 11   5   0   3

int num_in1  = 2;    //  ES_IN
int num_in2  = 11;   //  UZA_IN
int num_in3  =14;    //  LVL_IN
int num_in4  = 1;    //  STRT_IN
int num_in5  = 0;    //  STOP_IN
int num_in6  = 12;   //  CONS_IN
int num_in7  =13;    //  TRAP_IN
int num_in8  = 4;    //  ES_IN_L
int num_in9  =11;    //  UZA_IN_L
int num_in10 = 5;    //  LVL_IN_L
int num_in11 =0;     //  STOP_IN_L
int num_in12 = 3;    //  STRT_IN_L
int num_in13 =1;     //  O_SH_IN
int num_in14 = 0;

//---------------------------
  // Дискретные входы-выходы
 //  " MKS 8 2 1 3 4 5 6"
  // ВХОДЫ
  // p1...p7.MKS
  // inp1 - ES
  // inp2 - UZA           7060-IN2 (p.6) 2
  // inp3 - датчик уровня 7060-IN1 (p.3) 1
  // inp4 - кн. "Start"   7060-IN3 (p.4) 3
  // inp5 - кн. "Stop"    7060-IN4 (p.4) 4
  // inp6   трап          7188-DI1 (p.7) 5
  // inp7   консоль       7188-DI2 (p.7) 6


//  "MKO 2 1 7 6 4 3 0  8"
  //p1.MKO  OUT1 -      - меньший клапан верхнего налива
  //p2.MKO  OUT2 -      - больший клапан верхнего налива
  //p3.MKO  OUT3 - PUMP - включение насоса , не исп.
  //p4.MKO  OUT4 - Air switch

  //p5.MKO  OUT5 - меньший клапан нижнего налива
  //p6.MKO  OUT6 - больший клапан нижнего налива

  //p7.MKO  OUT7 = CLP0 - самый больший клапан верхнего налива
  //p8.MKO  OUT8 -   - выход сигнала аварии , сигнализация   7060-OUT3  (p.5)


//---------------------------
void f_alarm();
unsigned int OUT_VAR=0;    // состояние дискретных выходов
unsigned int INP_VAR=0;    // состояние дискретных входов
unsigned int INP_inv=0;    // маска инверсии входов
unsigned int INP_VAR_c=0;  // копия состояния дискретных входов
unsigned int INP_VARi0=0;  // состояние дискретных входов
unsigned int INP_MSK_OK=INP_MSK_OK_v;        // состояние дискретных входов для верхнего налива
unsigned int INP_MSK_OK_L=INP_MSK_OK_v_L;    // состояние дискретных входов для нижнего налива
unsigned int OUT_VAR_c=0;  // копия состояния дискретных входов

//-----------------------------------------------------------------
//---------------------------

int  iv_msk[KOL_D_INP+2]  ;
int  inv_msk[KOL_D_INP+2] ;
int int_fict_11=1;
//---------------------------

void *p_ik[]=
{
&int_fict_11,
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  1  2  3  4
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  5  6  7  8
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  9 10 11 12
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  // 13 14 15 16
&INP_OFP ,&INP_OFP ,&INP_OFP ,&INP_OFP , // 17 18 19 20
&INP_OFP ,&INP_OFP ,&INP_OFP ,&INP_OFP , // 21 22 23 24
};

int msk_ik[]=
{
   1,
   1    ,2    ,4    ,8,  0x10   ,0x20,  0x40  ,0x80,         //  1  2  3  4  5  6  7  8
 0x100,0x200,0x400,0x800,0x1000, 0x2000,0x4000,0x8000,       //  9 10 11 12 13 14 15 16
   1    ,2    ,4    ,8,  0x10   ,0x20,  0x40  ,0x80,         // 17 18 19 20 21 22 23 24
};
//---------------------------
int fict_npn=0;
int *num_pnt[KOL_D_INP+1]={&fict_npn,
&num_in1,&num_in2,&num_in3,&num_in4,&num_in5,&num_in6,&num_in7,&num_in8,
&num_in9,&num_in10,&num_in11,&num_in12,&num_in13,
//&num_in14,&num_in15,&num_in16,&num_in17,&num_in18,
};
//---------------------------
int en_bot[KOL_D_INP+2];

int f_ik(int i)
{
int ii;
   if(en_bot[i] == 1) return 1;
   else if(en_bot[i] == -1) return 0;
   ii=*num_pnt[i];
   return( ( ( *(int *)p_ik[ii]) ^ iv_msk[i]  ) & msk_ik[ii] );
}
//---------------------------
// #define    mski_inp1     inv_msk[1]
void f_init_mki()
{
int i;

  for(i=1;i<=KOL_D_INP;i++)
    {
     if(inv_msk[i] != 0.) iv_msk[i] = 0xffff;
     else   iv_msk[i] = 0;
    }
}
//-------------------------------
void f_var_i()
{

int itmp=0;

   if(LVL_IN ) itmp |= 1;      //D0   - датчик уровня верхнего налива,      1 - ok
   if(UZA_IN ) itmp |= 2;      //D1   - УЗА,                                1 - ok
   if(STRT_IN) itmp |= 4;      //D2   - кнопка "Start" верхнего налива      1 - кнопка нажата
   if(ES_IN  ) itmp |= 8;      //D3   - Аварийная кнопка верхнего налива    1 - ok,0 - кнопка нажата

   if(CONS_IN   ) itmp |=0x10; //D4   - консоль,                            1 - ok
   if(TRAP_IN   ) itmp |=0x20; //D5   - трап,                               1 - ok
// if(          ) itmp |=0x40; //D6 = 0
   if(LVL_IN_L  ) itmp |=0x80; //D7 = - контроллер ограничения наполнения нижнего налива, 1 - ok

   if(STRT_IN_L ) itmp |=0x100;//D8   - кнопка "Start" нижнего налива       1 - кнопка нажата
   if(ES_IN_L   ) itmp |=0x200;//D9   - Аварийная кнопка нижнего налива     1 - ok,0 - кнопка нажата

INP_VARi0=  itmp;

}

//-----------------------------------------------------------------



int flag_ext_t=0;
int Drive_SV_beg[2]={5000,5000};  // 50 Hz, значение частоты вращения
int Drive_SA_beg[2]={20,20}; // текущее состояние времени ускорения 2 сек
int Drive_SD_beg[2]={20,20};  // текущее состояние  замедления 2 сек на 100Гц

int f_St_chk_fict()
{
 return 1;
}
int f_Rn_chk_fict()
{
 return 0;
}
void f_fict()
{
}

void f_fict_stop()
{
 Out_off(OUT3);
}

void f_fict_run()
{
 Out_on(OUT3);
}


int status_fct=0;
int *DriveStatus=&status_fct;

int DriveSV[3]={1000,1000,1000};
int DriveSA[3]={100,100,100};
int DriveSD[3]={100,100,100};
int DriveState[3]={0,0,0};
int DriveCurrent[3]={0,0,0};
int DriveError[3]={0,0,0};

int (*f_Drive_St_chk)()=f_St_chk_fict;
int (*f_Drive_Rn_chk)()=f_Rn_chk_fict;
void (*f_Drive_Stop)()=f_fict_stop;
void (*f_Drive_Run)()=f_fict_run;

//14.06.2021 YN
#if defined(UNDERPRESSURE)
//             температура
//          давление |  разряжение
//                 | | | 
int analog_num[8]={1,2,3,0,0,0,0,0};
#else
//             температура
//          давление |
//                 | |   0x60
int analog_num[8]={1,2,0,0,0,0,0,0};
#endif

float analog_scale[8]={1./NA_scale,250./NA_scale,1./NA_scale,1,1,1,1,1};
float analog_offset[8]= {0,-50,0,0,0,0,0,0};

int fl_GO=0;    // флаг отпуска/приема через MVD
int fl_EQ=0;    // флаг выравнивания давления через MVD
int flagS_ES=0; // признак останова по нажатию ES
int flagS_UZA=0; // признак останова по отсутствию UZA
int flagE_UZA=0; // код ошибки


int flagS_DLT=0; // признак останова по состояниию DELTA

int flag_rcv=0;

long int time_com_cl=2000; // мс, время включения клапанов коммутации
long int time_com_cl_off=2000; // мс, время выключения клапанов  коммутации
long int time_cnt_cl_off=2000; // мс, задержка между остановом счета и выключением клапанов коммутации
long int time_gas_cl_on =2000; // мс, время включения клапанов пара
long int time_gas_cl_off=2000; // мс, время выключения клапанов пара

long int time_EQ_rcv=200; // секунд
long int time_EQ_dlv=200; // секунд
long int time_LIQ_rcv=600; // секунд
long int time_LIQ_dlv=600; // секунд

int flag_dlv_fst=0; // 0 - сбросить Mass total перед началом отпуска приема
int flag_prn_mass=0;

int sw_dlv_liq=0;
int sw_dlv_gas=0;
int sw_rcv_liq=0;
int sw_rcv_gas=0;

int flag_continue=0;

//char *list_avt[]={
char list_avt[][32]={

"Действие отменено.ESC - меню  ",  // 0
"Sht-ESC очистка ошибок        ",  // 1
"   !Нажата аварийная кнопка   ", // 2
"   Действие невозможно.       ", // 3
"    !Нет сигнала УЗА          ", // 4
"Настройка счета расходомера   ", // 5
" !Нет связи с расходомером    ", // 6
"Таймаут связи с расходомером  ", // 7
"Обнуление отсчета расходомера ", // 8
" !Уровень выше допустимого    ", // 9
"Включение потока жидкости     ", //10
" Трап не в рабочем положении  ",//11
"                              ",//12
" Выключение потока жидкости   ",//13
"Консоль не в рабочем положении",//14
"Действие остановлено.ESC-меню",//15
" Ожидание нажатия кн. =Пуск=  ",//16
"Отпуск.Выр-е давл.паровых фаз ",//17
"",//18
"2-продолжить отпуск жидк.фазы ",//19
"ESC - завершить отпуск        ",//20
" Выключение потока паров      ",//21
"        Ошибки очищены        ",//22
" ESC - продолжить             ",//23
"!Драйверы MVD,7060 не включены",//24
"       !Останов отпуска       ",//25
"         Доза отпущена        ",//26
" В расходомере нет жидкой фазы",//27
" ESC - меню                   ",//28
" Расход снизился до пред.знач.",//29
"      Отпуск остановлен       ",//30
"     Нажата кнопка =ESC=      ",//31
" Авария насоса при торможении ",//32
"     Нажата кнопка =STOP=     ",//33
"       ID не корректен        ",//34
"       по команде Host        ",//35
//14.06.2021 YN
#if defined(UNDERPRESSURE)
" Ошибка подачи нефтепродукта  ",//36
#endif
};

char *list_rcv[]={
"Прием.Выр-е давл.паровых фаз  ",//0 17
"2-продолжить прием жидкой фазы",//1 19
"ESC - завершить прием         ",//2 20
"       !Останов приема        ",//3 25
"        Доза принята          ",//4 26
"      Прием остановлен        ",//5 30
};

long int time_t_snd=0;
long int time_t_swtch=0;
int sw_ret=0;

long int Tim_Prep1  =12000;  // ms
long int Tim_Prep_ND=6000; // ms
float    P_beg_valve=0.2;     // MPa
float    F_Dns_off  =20;        // kg/hour

int Flag_dns_flow=0;

//-----------------------------
int key__1=0;
int f_dlv_liq(int key)
{
// подготовка к отпуску и отпуск жидкой фазы
//
//  если key == ESC - останов потока , после завершения процедуры
//  останова потока возвращает значение ESC.
//
// return: ESC -  поток остановлен , переход в меню отпуска
//          по нажатию кнопки 'ESC'
//
    sw_dlv_slv=sw_dlv_liq;
    if((FL_err) && (sw_dlv_liq != -1))
    {
      f_clr_scr_MMI();
      f_disp_error();

//    MmiGotoxy(0,5);   MmiPuts("Отпуск невозможен.ESC - меню");
//    MmiGotoxy(0,6);   MmiPuts("Sht-ESC очистка ошибок");
      MmiGotoxy(0,5);   MmiPuts(list_avt[0]);
      MmiGotoxy(0,6);   MmiPuts(list_avt[1]);


    if(sw_dlv_liq==3)
    {
      f_reg_cmn(9);
    }

      result_dlv=0;

      fl_GO=0;
      fl_EQ=0;
      OUT_VAR=0;
      sw_dlv_liq=-1;
      Temperature=s_MVD[0].Temp;
      Density=s_MVD[0].Dens;
      f_wr_evt(evt_err_f+flag_rcv);
    }

  switch(sw_dlv_liq)
  {
   // начальное состояние - все клапаны закрыты,
   // насос остановлен
   // подготовка к отпуску жидкой фазы

   case 1:  // начало отпуска с жидкой фазы
m1:

   Out_off(OUT4);  // Air switch off

 if( Flag_Low == 0)
 {

#if defined(BIO_1)
    if(OUT_or == 0)
    {

       f_icp_errS(No_Src);
       MmiGotoxy(0,1);    MmiPuts(" !Не задан источник продукта ");
       goto m_err;
    }
#endif

   if( ( ES_IN)==0 )
    {
       f_icp_errS(ES_off);
//     MmiGotoxy(0,1);    MmiPuts("  !Нажата аварийная кнопка   ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[2]);
m_err:
//     MmiGotoxy(0,0);    MmiPuts("  Отпуск невозможен.         ");
       MmiGotoxy(0,0);    MmiPuts(list_avt[3]);
       State_SLV=Cmd_brk;

    if(sw_dlv_liq==3)
      f_reg_cmn(9);
    else if(sw_dlv_liq==1)
       f_reg_cmn(7);
    else
       f_reg_cmn(7);

       sw_dlv_liq=-1;
       return 0;
    }
   if( ( ES_IN_L)==0 )
    {
       f_icp_errS(ES_off);
//     MmiGotoxy(0,1);    MmiPuts("  !Нажата аварийная кнопка   ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[2]);
       goto m_err;
    }
   if( ( UZA_IN)==0  )
    {
       flagE_UZA=UZA_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts("  !Нет сигнала УЗА           ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[4]);
       goto m_err;
    }
    if( id_ok==0)
    {
       flagE_UZA=ID_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts("     Не корректный ID         ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[34]);
       goto m_err;
    }
   if( (LVL_IN)==0 )
    {
       flagE_UZA=LVL_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts(" !Уровень выше допустимого");
       MmiGotoxy(0,1);    MmiPuts(list_avt[9]);
       goto m_err;
    }
   if( ( TRAP_IN)==0 )
    {
       flagE_UZA=TRAP_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts(" Трап не в рабочем положении ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[11]);
       goto m_err;
    }
   if( ( CONS_IN)==0 )
    {
       flagE_UZA=CONS_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts("Консоль не в рабочем положении");
       MmiGotoxy(0,1);    MmiPuts(list_avt[14]);
       goto m_err;
    }
  }
 else
 {  // нижний налив

   if( ( ES_IN)==0 )
    {
       f_icp_errS(ES_off);
//     MmiGotoxy(0,1);    MmiPuts("  !Нажата аварийная кнопка   ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[2]);
       goto m_err;
    }
   if( ( ES_IN_L)==0 )
    {
       f_icp_errS(ES_off);
//     MmiGotoxy(0,1);    MmiPuts("  !Нажата аварийная кнопка   ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[2]);
       goto m_err;
    }

   if( ( UZA_IN_L)==0  )
    {
       flagE_UZA=UZA_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts("  !Нет сигнала УЗА           ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[4]);
       goto m_err;
    }
    if( id_ok==0)
    {
       flagE_UZA=ID_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts("     Не корректный ID         ");
       MmiGotoxy(0,1);    MmiPuts(list_avt[34]);
       goto m_err;
    }
   if( (LVL_IN_L)==0 )
    {
       flagE_UZA=LVL_E;
       f_icp_errS(UZA_off);
//     MmiGotoxy(0,1);    MmiPuts(" !Уровень выше допустимого");
       MmiGotoxy(0,1);    MmiPuts(list_avt[9]);
       goto m_err;
    }

 }

    sw_dlv_liq=3;
    time_t_swtch=TimeStamp;
    flag_continue=0;

    MmiGotoxy(0,1);    MmiPuts(list_avt[16]);
    break;
//-----------------------------
   case  3:
   // ожидание нажатия кнопки "Start"

   f_reg_cmn(8);


// if((STRT_IN && ( Flag_Low == 0) ) || (STRT_IN_L && ( Flag_Low != 0) ) ||(key==Sht_F1)  || (*(int *)list_Slv_int[i_cfm] == Strt_cmd) )
   if((STRT_IN && ( Flag_Low == 0) ) || (STRT_IN_L && ( Flag_Low != 0) ) ||(key==Sht_F1) )
   {

//   *(int *)list_Slv_int[i_cfm]=0;
     MVD_t_rslt[0]=0;
 MVD_Rqst_T[0].answ_com=f_MVD_rd_t;
 MVD_Rqst_T[0].answ_flt=f_MVD_rd_t;
     if(flag_dlv_fst==0)
       sw_dlv_liq=5;
     else
       sw_dlv_liq=7;
     time_t_swtch=TimeStamp;

//     f_reg_cmn(12);
//   if((Flag_Low != 0) && ( Flag_mass==0 )) f_reg_cmn( 12 );
     if((Flag_Low != 0) && ( Flag_mass!=0 )) f_reg_cmn( 22 );
else if((Flag_Low == 0) && ( Flag_mass==0 )) f_reg_cmn( 23 );
else if((Flag_Low == 0) && ( Flag_mass!=0 )) f_reg_cmn( 24 );
else
     f_reg_cmn( 12 );
   }
   else if( ( STOP_IN)==0)
   {
       MmiGotoxy(0,0);   MmiPuts(list_avt[30]);  //" Отпуск остановлен            ",//30
       MmiGotoxy(0,1);   MmiPuts(list_avt[33]);  //"     Нажата кнопка =STOP=     ",//31
       State_SLV=Cmd_brk;
       result_dlv=1; //  Нажата кнопка   ESC
       sw_dlv_liq=-1;
     f_reg_cmn(10);
   }
   else if(key==ESC)
   {
       MmiGotoxy(0,0);   MmiPuts(list_avt[30]);  //" Отпуск остановлен            ",//30
       MmiGotoxy(0,1);   MmiPuts(list_avt[31]);  //" Нажата кнопка   ESC          ",//31
       result_dlv=1; //  Нажата кнопка   ESC
       State_SLV=Cmd_brk;
       sw_dlv_liq=-1;
     f_reg_cmn(10);
   }
   else goto m1;
   break;
//-----------------------------
   case  5:
     // обнуление Mass_Total и Volume_Total
 MVD_t_rslt[0]=0;

 time_t_snd=TimeStamp;
  MmiGotoxy(0,1);    MmiPuts(list_avt[8]); //"Обнуление отсчета расходомера");
   // Reset all totalizers
 if( f_MVD_WR((int)0,(int)MVD_WR_C,(int) 3,(int)1,(long int) 0,(float)0)== 0)
 { // ошибка при отправке посылки MVD
       f_icp_errS(MVD_com);
//  MmiGotoxy(0,1);  MmiPuts("!Нет связи с расходомером    ");
    MmiGotoxy(0,1);  MmiPuts(list_avt[6]);
       goto m_err;
 }
  sw_dlv_liq=6;
  break;
//-----------------------------
   case  6:
    // ожидание завершения команды обнуления totalizers
    if( MVD_t_rslt[0]>0)
    {

     sw_dlv_liq=7;
     flag_ZT=0;
     flag_dlv_fst=1; // Mass Total обнулена

     VolI_beg=s_MVD[0].VolI;
     s_MVD[0].VolT=0.;
     s_MVD[0].MassT=0.;

     Sim_VolT=0.;
     Sim_MassT=0.;

     f_init_DNSA();

     flag_prn_mass=1;
     vol2=0;
     VolT_int=0;
     VolT_id=0;
     break;
    }
//  if(key==ESC)    // переход в меню
//       return ESC;
m_wait:
    if( f_timer(time_t_snd,f_MVD_tim ) )
    {  // время вышло
       MmiGotoxy(0,1);  MmiPuts(list_avt[7]); //"Таймаут связи с расходомером ");
       f_icp_errS(MVD_com);
       goto m_err;
    }
    break;
//-----------------------------

   case  7:
  // разрешение расхода расходомера без счета

 MVD_t_rslt[0]=0;
 time_t_snd=TimeStamp;
  MmiGotoxy(0,1);    MmiPuts("Разрешение счета расходомера");

       // Stop totalizers
 if( f_MVD_WR((int)0,(int)MVD_WR_C,(int) 2,(int)0,(long int) 0,(float)0)== 0)
 { // ошибка при отправке посылки MVD

m_err_mvd:
            f_icp_errS(MVD_com);
      //  "!Нет связи с расходомером    "
         MmiGotoxy(0,1);  MmiPuts(list_avt[6]);
            goto m_err;
 }
    sw_dlv_liq=71;
  break;

//-----------------------------
   case  71:

    if( MVD_t_rslt[0]>0)
    {
      MVD_t_rslt[0]=0;
      //
      time_t_snd=TimeStamp;

    // установка значения среза массового расхода для рабочего режима
      if( f_MVD_WR((int)0,(int)MVD_WR_F,(int) 195,(int)0,(long int) 0,cutoff_on_M) == 0)
      { // ошибка при отправке посылки MVD
          goto m_err_mvd;
      }
      sw_dlv_liq=8;
    }
    else goto m_wait;
  break;

//-----------------------------
   case 8:
    // ожидание завершения команды установки cutoff for Mass Flow
    if( MVD_t_rslt[0]>0)
    {
      MVD_t_rslt[0]=0;
      time_t_snd=TimeStamp;

   // установка значения среза объемного расхода для рабочего режима
      if( f_MVD_WR((int)0,(int)MVD_WR_F,(int) 197,(int)0,(long int) 0,cutoff_on_V) == 0)
      { // ошибка при отправке посылки MVD
       goto m_err_mvd;
      }
      sw_dlv_liq=9;
      break;
    }
    else goto m_wait;
//-----------------------------
   case  9:
    // ожидание завершения команды установки cutoff for Vol Flow

    if( MVD_t_rslt[0]>0)
    {
       sw_dlv_liq=11;
//     MmiGotoxy(0,1);  MmiPuts("Включение потока жидкости    ");
       MmiGotoxy(0,1);  MmiPuts(list_avt[10]);
       break;
    }
    else goto m_wait;
//-----------------------------
   case  11:
       //  Включение потока жидкости
      volume_f=dose_dlvr;
      vol_f=dose_dlvr;
      Dens0=s_MVD[0].Dens;

      f_enable_cnt();
    // запретить открывание клапанов
      Out_en =  ~(OUT1 | OUT2);
//  time_beg_fill=TimeStamp;
//  beg_flch=0;
//  flag_flch=0;
      time_t_swtch=TimeStamp;
      MmiGotoxy(0,0);
       if(dose_dlvr != vBIG_P)
        {
         if ( Flag_mass != 0 )
           MmiPrintf(" Отпуск дозы %g кг         ",dose_dlvr);
         else
           MmiPrintf(" Отпуск дозы %g л          ",dose_dlvr);
        }
       else
          MmiPuts("  Отпуск без дозировки        ");

       f_prn_mode();

//    MmiGotoxy(0,1);      MmiPuts("                              ");
      MmiGotoxy(0,1);      MmiPuts(list_avt[12]);
//    MmiGotoxy(0,2);      MmiPuts("                              ");
      MmiGotoxy(0,2);      MmiPuts(list_avt[12]);

        sw_dlv_liq=110;
        t_prep_MVD=TimeStamp;
        t_prep_MVD1=TimeStamp;

        Flag_dns_flow=0;
   break;
//-----------------------------

   case 110:
      // насос работает,все клапаны закрыты
      // проверка состояния

  time_beg_fill=TimeStamp;
  beg_flch=0;
  flag_flch=0;

      if(key==ESC)
      {
          key__1=key;
          sw_dlv_liq=30;
          f_reg_cmn(14);
      }
      if( ( ( STOP_IN==0) && ( Flag_Low == 0) ) || ( ( STOP_IN_L==0) && ( Flag_Low != 0) )  ) // переход к процедуре отключения
      {
      // нажата кнопка "STOP"
          key__1=STOP_E;
          sw_dlv_liq=30;
          f_reg_cmn(14);
      }

      if( f_timer(t_prep_MVD, Tim_Prep1 ) == 0 )
       {
         if(( s_MVD[0].Press > P_beg_valve) || (flag_prok != 0)  );
         else
         {
          Flag_dns_flow=0;
          t_prep_MVD1=TimeStamp;
         }

         if(( fabs(s_MVD[0].FlowM) < F_Dns_off) || (flag_prok != 0)  ) ;
         else
         {
             Flag_dns_flow=1;
             t_prep_MVD1=TimeStamp;
         }
         if( f_timer(t_prep_MVD1, Tim_Prep_ND ) != 0 )
         { // антидребезг прошел
           t_prep_MVD=TimeStamp;
           sw_dlv_liq=111;

             // Start totalizers


          if(flag_prok == 0)
           {
             MVD_t_rslt[0]=0;
             time_t_snd=TimeStamp;
             if( f_MVD_WR((int)0,(int)MVD_WR_C,(int) 2,(int)1,(long int) 0,(float)0)== 0)
             { // ошибка при отправке посылки MVD
                   goto m_err_mvd;
             }
           }
          else
             MVD_t_rslt[0]=1;

         }
       }
      else
       {  // время начальной проверки вышло, есть утечка или нет давления

         if(Flag_dns_flow != 0)
           f_icp_errS(DNS_not_off);
         else
           f_icp_errS(No_Press);
         break;
       }
       break;

//-----------------------------
  case 111:
    // ожидание завершения команды  Start totalizers

  time_beg_fill=TimeStamp;
  beg_flch=0;
  flag_flch=0;
    if( MVD_t_rslt[0]>0)
    {
      Out_en =  0xffff;
      sw_dlv_liq=12;

      //14.06.2021 YN
      #if defined(UNDERPRESSURE)
         if(permission)
         {
            sw_ctrl_dens = FIRST;
            Dens_new=Dens_old=Dens_diff=tmpf_nominal=0.0;
            tmpl_time=0;
         }
      #endif

      result_dlv=0;
      break;
    }
    else goto m_wait;
//-----------------------------
   case  12:
       //  отпуск

   //14.06.2021 YN
   #if defined(UNDERPRESSURE)
      if(permission)    // Если включен контроль плотности 'UP' p4=1
      {
         if( Flow_dem == Flow_nom || Flow_dem == Flow_nomL) //когда начинаем или вышли на номинальный расход
         {
            Dens_new= s_MVD[0].Dens;

            if( Dens_old > 0.0 )
               if(Dens_new > Dens_old)
               {
                  Dens_diff= Dens_new - Dens_old;

                  switch (sw_ctrl_dens)
                  {
                     case FIRST:
                        if(Dens_diff > allowed_diff[FIRST])   //разница давлений превышает разрешенную[FIRST] (первый проход) 'UP' p1
                        {                                            
                           sw_ctrl_dens= SECOND;              //устанавливаем второй проход

                           if( Flag_Low == 0)
                           {
                              tmpf_nominal= Flow_nom;                //сохраняем начальное значение номинала для последующего восстановления
                              Flow_nom= Flow_nom*procent[FIRST]/100; //уменьшаем номинал на значение 'UP' p.6
                           }
                           else
                           {  //Flag_Low != 0
                              tmpf_nominal= Flow_nomL;                  //сохраняем начальное значение номинала для последующего восстановления
                              Flow_nomL= Flow_nomL*procent[FIRST]/100;  //уменьшаем номинал на значение 'UP' p.6
                           }
                           tmpl_time= TimeStamp;
                        }
                     break;

                     case SECOND:
                        if( f_timer(tmpl_time,nrmlz_time ) )            //ожидаем 'UP p5 секунд, для выхода на новый расход
                        {
                           if(Dens_diff > allowed_diff[SECOND]) //разница давлений превышает разрешенную[SECOND] (первый проход) 'UP' p2
                           {
                              sw_ctrl_dens= THIRD;                //устанавливаем третий проход

                              if( Flag_Low == 0)
                              {
                                 Flow_nom= Flow_nom*procent[SECOND]/100;   //уменьшаем номинал на значение 'UP' p.7
                              }
                              else
                              {  //Flag_Low != 0
                                 Flow_nomL= Flow_nomL*procent[SECOND]/100; //уменьшаем номинал на значение 'UP' p.7
                              }

                              tmpl_time= TimeStamp;
                           }
                        }
                     break;

                     case THIRD:
                        if( f_timer(tmpl_time,nrmlz_time) )          //ожидаем 'UP p5 секунд, для выхода на новый расход
                        {
                           if(Dens_diff > allowed_diff[THIRD]) //разница давлений превышает разрешенную[THIRD] (первый проход) 'UP' p3
                           {
                              sw_ctrl_dens= FOURTH;               //устанавливаем четвертый проход

                              if( Flag_Low == 0)
                              {
                                 Flow_nom= Flow_nom*procent[THIRD]/100;    //уменьшаем номинал на значение 'UP' p.8
                              }
                              else
                              {  //Flag_Low != 0
                                 Flow_nomL= Flow_nomL*procent[THIRD]/100;  //уменьшаем номинал на значение 'UP' p.8
                              }

                              tmpl_time= TimeStamp;
                           }
                        }
                     break;

                     case FOURTH:
                        if( f_timer(tmpl_time,nrmlz_time) )       //ожидаем 'UP p5 секунд, для выхода на новый расход
                        {
                           if(Dens_diff > allowed_diff[THIRD]) //разница давлений превышает разрешенную[THIRD] (первый проход) 'UP' p3
                           {
                              key__1=UND_PRESS;
                              result_dlv=0; //  Ошибка без возможности продолжить
                              sw_dlv_liq=30; 
                              f_reg_cmn(13);
                           }
                        }
                     break;
                  }
               }

            Dens_old= s_MVD[0].Dens;
         }
      }

   #endif




    if(FL_err)    // переход к процедуре отключения
    {
        key__1=ESC;
        result_dlv=0; //  Ошибка
        sw_dlv_liq=30;
        f_reg_cmn(13);

    }
    else if(key==ESC)       // переход к процедуре отключения
    {
        key__1=key;
       result_dlv=1; //  Нажата кнопка   ESC
        sw_dlv_liq=30;
        f_reg_cmn(14);
    }
    if( ( ( STOP_IN==0) && ( Flag_Low == 0) ) || ( ( STOP_IN_L==0) && ( Flag_Low != 0) )  ) // переход к процедуре отключения
    {
    // нажата кнопка "STOP"
        result_dlv=1; //  Нажата кнопка   STOP
        key__1=STOP_E;
        sw_dlv_liq=30;
        f_reg_cmn(14);
    }
    else if(fl_GO==0 ) // переход к завершению по прекращению отпуска
    {
      key__1=-1;
      MmiGotoxy(0,0);   MmiPuts(list_avt[25]);  //"       !Останов отпуска       "
//    MmiGotoxy(0,1);   MmiPuts(list_avt[25]);
      sw_dlv_liq=30;
      f_reg_cmn(13);
    }
    break;
//-----------------------------
   case  30:
      // выключение клапанов регулирующих расход
      // поочередно с задержкой TimDensWt0
/*
      Out_en=~CLP0;
      time_t_snd=TimeStamp;
      sw_dlv_liq=31;
      break;
//-----------------------------
   case  31:

    if( f_timer(time_t_snd,TimDensWt0 ) )
    {  // время вышло
*/

//14.06.2021 YN
#if defined(UNDERPRESSURE)
   if( tmpf_nominal != 0.0 )
   {
      if( Flag_Low == 0)
      {
         Flow_nom= tmpf_nominal;
         tmpf_nominal=0.0;
      }
      else
      {  //Flag_Low != 0
         Flow_nomL= tmpf_nominal;
         tmpf_nominal=0.0;
      }
   }
#endif

if( Flag_Low == 0)
{
  if(valve_mode==1)
  {
      Out_en=~CLP0;
      Out_en &= ~(OUT1);
  }
  else if(valve_mode==2)
  {
      Out_en =  ~(OUT1 );
      Out_en &= ~(OUT2 );
  }
  else if(valve_mode==3)
  {
      Out_en =  ~(OUT1 );
  }

}
else
{
  if(valve_modeL==1)
  {
      Out_en = ~(OUT5);
  }
  else if(valve_modeL==2)
  {
      Out_en =  ~( OUT5);
      Out_en &= ~( OUT6);
  }
  else if(valve_modeL==3)
  {
      Out_en =  ~(OUT5);
  }
}

      time_t_snd=TimeStamp;
      sw_dlv_liq=32;

//    }
      break;
//-----------------------------
   case  32:

    if( f_timer(time_t_snd,TimDensWt0 ) )
    {  // время вышло
      if(valve_mode==1)
      {
        Out_en &= ~(OUT2 | OUT6);
      }
/*
      else if(valve_mode==2)
      {
      }
      else if(valve_mode==3)
      {
      }
 */
      time_t_snd=TimeStamp;
      sw_dlv_liq=33;
    }
      break;
//-----------------------------
   case  33:

    if( f_timer(time_t_snd,TimDensWt0 ) )
    {  // время вышло
      sw_dlv_liq=40;
    }
      break;
//-----------------------------
//-----------------------------
   case  40:
      // останов потока

       time_t_swtch=TimeStamp;
       Temperature=s_MVD[0].Temp;
       Density=s_MVD[0].Dens;
       f_stop_cnt();
       sw_dlv_liq=41;
//     MmiGotoxy(0,1);  MmiPuts(" Выключение потока жидкости   ");
       MmiGotoxy(0,1);  MmiPuts(list_avt[13]);
       time_t_snd=TimeStamp;

    break;
//-----------------------------
   case  41:
      // ожидание останова двигателя насоса

     if( f_timer(time_t_snd,T_Mot_Stop ) )
     {  // время вышло
m_mot_not:
       f_icp_errS(MotNotStop);
       Temperature=s_MVD[0].Temp;
       Density=s_MVD[0].Dens;
       key__1=0;
       f_alarm();
           f_reg_cmn(13);
//     f_wr_evt(evt_est_f+flag_rcv);
//    "      Отпуск остановлен       ", //30
       MmiGotoxy(0,0);  MmiPuts(list_avt[30]);
   // " Авария насоса при торможении ",//32
       MmiGotoxy(0,1);  MmiPuts(list_avt[32]);
   // " ESC - меню                   ",//28
       MmiGotoxy(0,6);  MmiPuts(list_avt[28]);
       sw_dlv_liq=-1;
     }
     else
     {
//     if(Delta[0].status==0)
       if( *DriveStatus ==0)
       {
         sw_dlv_liq=42;
         time_t_snd=TimeStamp;
         break ;
       }
//     if((Delta_State[0] & MSK_RUN_DELTA ) == STP_DELTA )
       if(f_Drive_St_chk() == 1)
       {
         sw_dlv_liq=42;
         time_t_snd=TimeStamp;
       }
     }
      break;
//-----------------------------
//-----------------------------
   case  42:
    // задержка на время между остановом счета и выключением клапанов коммутации
    // time_cnt_cl_off
//  if(OUT_VAR & OUT1) time_t_snd=TimeStamp;
//  if(OUT_VAR & OUT2) time_t_snd=TimeStamp;
//  if(OUT_VAR & OUT3) time_t_snd=TimeStamp;
    if( f_timer(time_t_snd,time_cnt_cl_off ) )
    {  // время вышло
//     MmiGotoxy(0,1);    MmiPuts("Выключение клап-в коммутации ");
//    MmiGotoxy(0,1);    MmiPuts(list_avt[14]);
//    Out_off(GAS1);
//    Out_off(GAS2);
//    Out_off(LIQ2);
      time_t_snd=TimeStamp;
      sw_dlv_liq=44;
    }
    break;
//-----------------------------
   case  44:

    if(ff_serv_N== NULL)
    {
       result_dlv=0;

       f_reg_cmn(17);
       sw_dlv_liq=-1;

       if (FL_err)
       {

         f_reg_cmn(17);
         f_clr_scr_MMI();
         f_disp_error();
//       MmiGotoxy(0,6);       MmiPuts("Отпуск прерван.ESC - меню    ");
         MmiGotoxy(0,6);       MmiPuts(list_avt[15]);
         break;
       }

       //14.06.2021 YN
       #if defined(UNDERPRESSURE)
         else if(key__1== STOP_E)
         {
            f_reg_cmn(13);
            MmiGotoxy(0,0);   MmiPuts(list_avt[30]);  //" Отпуск остановлен            ",//30
            MmiGotoxy(0,1);   MmiPuts(list_avt[36]);  //" Ошибка подачи нефтепродукта  ",//36
            MmiGotoxy(0,6);   MmiPuts(list_avt[28]);  //" ESC - меню                   ",//28
        break;
       }
       #endif

       else if(key__1== ESC)
       {
m_esc:

       f_reg_cmn(18);

       result_dlv=1; //  Нажата кнопка   ESC

        MmiGotoxy(0,0);   MmiPuts(list_avt[30]);  //" Отпуск остановлен            ",//30
        MmiGotoxy(0,1);   MmiPuts(list_avt[31]);  //" Нажата кнопка   ESC          ",//31
        MmiGotoxy(0,6);   MmiPuts(list_avt[28]);  //" ESC - меню                   ",//28
        break;
       }
       else if(key__1== STOP_E)
       {
       f_reg_cmn(18);
       result_dlv=1; //  Нажата кнопка   STOP
        MmiGotoxy(0,0);   MmiPuts(list_avt[30]);  //" Отпуск остановлен            ",//30
        MmiGotoxy(0,1);   MmiPuts(list_avt[33]);  //"     Нажата кнопка =STOP=     ",//31
        MmiGotoxy(0,6);   MmiPuts(list_avt[28]);  //" ESC - меню                   ",//28
        break;
       }
       else if(key__1== STOP_H)
       {
       result_dlv=2; //  Команда Host
       f_reg_cmn(19);
        MmiGotoxy(0,0);   MmiPuts(list_avt[30]);  //" Отпуск остановлен            ",//30
        MmiGotoxy(0,1);   MmiPuts(list_avt[35]);  //" по команде Host        ",//31
        MmiGotoxy(0,6);   MmiPuts(list_avt[28]);  //" ESC - меню                   ",//28
        break;
       }

//          MmiGotoxy(0,0);   MmiPuts(" Отпуск остановлен            "  );
            MmiGotoxy(0,0);   MmiPuts(list_avt[30]);
//          MmiGotoxy(0,6);       MmiPuts(" ESC - меню    ");
            MmiGotoxy(0,6);       MmiPuts(list_avt[28]);
       if((flag_fill_ok!=0)&&(flag_gas ==0) )
          {
//          MmiGotoxy(0,1);       MmiPuts(" Доза отпущена     ");
            MmiGotoxy(0,1);

         if ( Flag_mass != 0 )
              MmiPrintf(" Доза %g кг отпущена            ",dose_dlvr);
         else
              MmiPrintf(" Доза %g л отпущена             ",dose_dlvr);

//          MmiGotoxy(0,1);  MmiPrintf(" Доза           кг отпущена  ");

        //    dose_dlvr=vBIG_P;
                           dose_dlvr=-1;
                //         flag_prn_mass=0;


            result_dlv=3; //  Доза отпущена

       f_reg_cmn(16);
            break;
          }
          else if(flag_gas !=0)
          {
//          MmiGotoxy(0,1);       MmiPuts(" В расходомере нет жидкой фазы");
            MmiGotoxy(0,1);       MmiPuts(list_avt[27]);
            break;
          }
          else if(flag_flch !=0)
          {


       f_reg_cmn(20);
       result_dlv=4; //   Расход снизился до пред.знач.
//          MmiGotoxy(0,1);       MmiPuts(" Расход снизился до пред.знач.");
            MmiGotoxy(0,1);       MmiPuts(list_avt[29]);
            break;
          }
          else break;
    }
    else
     if( f_timer(time_t_snd,(t_mvd_cor*3) ) )
     {  // время вышло
       ff_serv_N = NULL;
       f_icp_errS(MVD_com);
      Temperature=s_MVD[0].Temp;
      Density=s_MVD[0].Dens;
       f_wr_evt(evt_est_f+flag_rcv);
//     MmiGotoxy(0,1);  MmiPuts("!Нет связи с расходомером    ");
       MmiGotoxy(0,1);  MmiPuts(list_avt[6]);
//     MmiGotoxy(0,0);    MmiPuts("  Отпуск невозможен.         ");
       MmiGotoxy(0,0);    MmiPuts(list_avt[3]);
       State_SLV=Cmd_brk;

       f_reg_cmn(21);

       sw_dlv_liq=-1;
       return 0;
     }
    break;
//-----------------------------
//-----------------------------
    case -1:
       if((key==ESC)||(key==ENTER)  )    // переход в меню
       {
#if defined(BIO_1)
      flag_O3=0;
#endif
        return ESC;
       }
       else if ((key==Sht_ESC) && (mode_hst == 0 ))
        {  // очистка ошибок
          f_clr_scr_MMI();
          SetDisplayPage(23);
          f_cl_error();
//        MmiGotoxy(0,2);    MmiPuts("        Ошибки очищены       ");
//        MmiGotoxy(0,4);    MmiPuts(" ESC - продолжить            ");
          MmiGotoxy(0,5);    MmiPuts(list_avt[22]);
          MmiGotoxy(0,6);    MmiPuts(list_avt[23]);
          return ESC;
        }
       else
         return 0;
//-----------------------------
  }
  return 0;
}
//-----------------------------
//-----------------------------
/*
void Out_on(int msk)
{
 OUT_VAR |= msk;
}
//-----------------------------
void Out_off(int msk)
{
 OUT_VAR &= ~msk;
}
*/
//-----------------------------
void f_alarm()
{
   ff_serv_N=f_MVD_cor;
   tm_mvd_p=TimeStamp;
   t_prep_MVD=TimeStamp;
   State_SLV=Stp_cnt;
   f_reg_cmn(13);

   fl_GO=0;
   TimeOut3=TimeStamp-Out3_delay_off;
}
//-----------------------------
void f_sens()
{ // проверяет состояние кнопки ES и UZA при отпуске/приеме СУГ
  // сигналы исчезают  - отпуск/прием прерывается
  // inp1 - ES
  // inp2 - UZA


//  i7060_out[0] = itmp;

  if( (fl_GO | fl_EQ) != 0)
  {

   if( Flag_Low == 0)
   {
#if defined(BIO_1)
    if(OUT_or == 0)
    {
       f_icp_errS(No_Src);
       goto m_err1;
    }
#endif
    if( (INP_VARi0  & INP_MSK_OK) != INP_MSK_OK )
    {

       if((  ES_IN)==0 )
        {
           flagS_ES=1;
           f_icp_errS(ES_off);
           goto m_err1;

        }
       if( ( UZA_IN)==0 )
        {
           flagE_UZA=UZA_E;
m_err:
           flagS_UZA=1;
           f_icp_errS(UZA_off);
m_err1:
           f_alarm();
           f_stop_cnt();
                   f_reg_cmn(13);
           sw_dlv_liq=-1;
           f_clr_scr_MMI();
           f_disp_error();

//         MmiGotoxy(0,5);   MmiPuts("Отпуск невозможен.ESC - меню");
//         MmiGotoxy(0,6);   MmiPuts("Sht-ESC очистка ошибок");
           MmiGotoxy(0,5);   MmiPuts(list_avt[0]);
           MmiGotoxy(0,6);   MmiPuts(list_avt[1]);
           goto m1;
        }
       if( ( LVL_IN)==0 )
        {
           flagE_UZA=LVL_E;
           goto m_err;
        }
       if( (TRAP_IN)==0 )
        {
           flagE_UZA=TRAP_E;
           goto m_err;
        }
       if( ( CONS_IN)==0 )
        {
           flagE_UZA=CONS_E;
           goto m_err;
        }
       if((  ES_IN_L)==0 )
        {
           flagS_ES=1;
           f_icp_errS(ES_off);
           goto m_err1;

        }
    }
   }
   else
   { // нижний налив

    if( (INP_VARi0  & INP_MSK_OK_L) != INP_MSK_OK_L )
     {
       if( ( UZA_IN_L)==0 )
        {
          flagE_UZA=UZA_E;
          goto m_err;
        }
       if((  ES_IN_L)==0 )
        {
           flagS_ES=1;
           f_icp_errS(ES_off);
           goto m_err1;

        }
       if((  ES_IN)==0 )
        {
           flagS_ES=1;
           f_icp_errS(ES_off);
           goto m_err1;

        }
       if( ( LVL_IN_L)==0 )
        {
           flagE_UZA=LVL_E;
           goto m_err;
        }
     }
   }
  }
m1:
  if(flag_O3 != 0)
   if( (( Flag_Low == 0) && ( OUT_VAR & (CLP0|OUT1|OUT2) )) ||
       (( Flag_Low != 0) && ( OUT_VAR & (CLP0|OUT5|OUT6) ))     )
  {
    if( *DriveStatus != 0)
    {
       if(f_Drive_Rn_chk() != 1)
       {
         flagS_DLT=1;
         f_icp_errS(DLT_off);
         f_alarm();
         f_stop_cnt();
                 f_reg_cmn(13);
       }
    }
  }
  return;
}
//-----------------------------
/*
void f_inp7188()
{
// вводит входные сигналы  I7188 и записывает их в
// D4,D4 INP_VAR

 INP_VAR &= 0xff0f;
 INP_VAR |= (GetDi1() << 4) & 0xf0;
 INP_VAR |= (GetDi2() << 4) & 0xf0;
// INP_VARi = INP_VAR ^ INP_inv;

}
*/
//-------------------------------
