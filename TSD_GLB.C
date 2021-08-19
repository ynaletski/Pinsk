
//    Трех-строчный дисплей Gilbarco,
//  6 символов в 2 верхних строках, в нижней строке 4 символа
//
//   Baud: 2400,7,Odd,1
//   CMx  2400 7 2 1

#define TSD_GLB

//---------------
int TSD_vn[max_TSD][3]={
23,9,4,           // заданное зн.дозы, текущее , плотность
23,9,4,
};
//---------------
long int TSD_time_pool=0;
long int TSD_period=100;  // ms
//----------------------
float fict_TSD=0;

float TSD_val_c[max_TSD][3]={
 -1,-1,-1,
 -1,-1,-1
};

int  tsd_swf[max_TSD]={0,0};
//---------------
/*
// перенесено в host.c
//---------------
void f_init_TSD()
{
 int i,i1;
for(i1=0;i1<2;i1++)
 for(i=0;i<3;i++)
  TSD_value[i1][i]=list_ns[TSD_vn[i1][i]];
}

*/
//---------------
//
float fict_TSD;
float *TSD_value[max_TSD][3]={
//&TSD_tst[0],&TSD_tst[1],&TSD_tst[2],
//&s_MVD[0].VolT,&s_MVD[0].Dens,&s_MVD[0].FlowM,
//&s_MVD[0].VolT,&summ_v,&cost_v,
&fict_TSD,&fict_TSD,&fict_TSD,
&fict_TSD,&fict_TSD,&fict_TSD,
};
//---------------
int n_dot[max_TSD][3]={
 1,2,2,
 2,2,0
};
//---------------
struct s_icp_dev TSD[max_TSD]=
{
// device 1
0,                // status
3,                // port
1,                // addr
"TSD_G/1",       // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B2.8",           // firmware
0,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&TSD_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
3,                // port
2,                // addr
"TSD_G/2",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B2.8",           // firmware
0,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&TSD_Rqst[1],   // *request
};
//----------------------
struct COM_rqst TSD_Rqst[max_TSD]={
//----------------------
// device 1
//--------------------------
0,                   //  состояние запроса
ffgets_com_GLB,      //  gets_com
f_GLB_rd,            //  answ_com
f_GLB_flt,           //  answ_flt
"GLB1",              //  name
0,                   //  n_dev номер устройства в группе однотипных устройств
&TSD[0],             //  *ICP_dd
0,                   //  текущая функция
ToutAnsTSD,          //  timeout
0,                   //  time_stamp
0,                   //  com_lgth
1,                   //  CRC_flag
"",                  //  com[]
//--------------------------
// device 2
//--------------------------
0,                   //  состояние запроса
ffgets_com_rtu,      //  gets_com
f_GLB_rd,            //  answ_com
f_GLB_flt,        //  answ_flt
"GLB2",             //  name
1,                   //  n_dev номер устройства в группе однотипных устройств
&TSD[1],             //  *ICP_dd
0,                   //  текущая функция
ToutAnsTSD,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

int TSD_fn[max_TSD]={0,0};    // регистр функций драйвера

//  Если записать значение функции в регистр функции ,
//  драйвер выполнит заданную функцию
//  После постановки запроса с установленной функцией в очередь на вывод в COM,
// регистр функции очищается драйвером.

int TSD_sw[max_TSD]={0,0};
int TSD_seq[16]={0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0};  // последовательность функций вывода

unsigned int TSD_inp_err[max_TSD]={0,0};  // счетчик ошибок ввода

char TsdBuf[max_TSD][3][10];
char TsdBuf_c[max_TSD][3][10];
char TsdBuf1[3][10];

char TsdStr[max_TSD][22]={
                                 //индекс в массиве
                                 //|
0x60,                            // 0, код команды; вывести 3 строки
0x20,0x20,0x20,0x20,0x20,0x20,   // 1, строка 1
0x20,'0','6','-','1','0',        // 7, строка 2
0x20,0x20,0x20,0x20,             //13, строка 3
0x30,0x32,0x30,                  //17, место запятых
0x08                             //20, терминатор
};

int tsd_sw[max_TSD]={0,0};
int TsdStrN[max_TSD];
int fl_tsd_new[max_TSD]={1,1};
int fl_tsd_new1[max_TSD]={0,0};
int TSD_GLB_fl[max_TSD]={0,0};

//----------------------
//----------------------
char TsdG_em[3]={
0x16, // увеличить на 1 электромагнитный счетчик
0
};
int flag_G_em=0;
long int G_em_time=0;
long int G_em_per=100;

void f_TSD_GLB(int ii)
{
 // ii - номер устройства
 //
 //  TsdStr[ii] содержит информацию о 3-х строках и запятых

int fn,sw;
unsigned char buf_tmp[20];
unsigned char *buf1_tmp;

 if(TSD[ii].status == 0) return;

 if( TSD_GLB_fl[0] >= 0 )
           f_init_TSD_GLB(ii);

//  f_print_TSD(ii);
/////  f_chk_new_TSD(ii);

  if((TSD_Rqst[ii].status == Req_Flt) ||
     (TSD_Rqst[ii].status == Req_OK));
  else return;

 if(f_queue_chk(TSD[ii].port)== 0) return;// очередь переполнена

 if( (flag_G_em!=0) &&  (f_timer(G_em_time,G_em_per ) !=0 ) )
      fn=TSD_EM;
 else if((TSD_fn[ii] != 0) && (TSD_fn[ii] < F_PROGRESS))
      fn=TSD_fn[ii];
 else if( f_timer(TSD[ii].time_stamp_pool,TSD[ii].pool_time ))
      fn=TSD_WR_P;
 else return;

      switch(fn)
      {
          case TSD_WR_S:  // вывод строки
//            buf1_tmp=TsdStr[ii];
//            TSD_fn[ii]=F_PROGRESS;
              TSD_fn[ii]=0;
              break;

          case TSD_WR_P:  // циклическое  обновление
          // формирует команды записи

              TSD_fn[ii]=0;
              fn=+1;
              break;

          case TSD_EM:  // увеличение на 1 электромеханического счетчика
              fn=TSD_EM;
              TSD_fn[ii]=0;
              G_em_time=TimeStamp;
           strcpy(TSD_Rqst[ii].Cmd,TsdG_em);

              goto m1;

          default:
          TSD_fn[ii]=0;
          return;
      }

// TsdStr[ii] - содержание строк и запятых


//   printf("\n\r==%s==",TsdStr[ii]);

           strcpy(TSD_Rqst[ii].Cmd,TsdStr[ii]);
m1:
           TSD_Rqst[ii].cmd_lgth=strlen(TSD_Rqst[ii].Cmd);
           TSD_Rqst[ii].function = fn;
           TSD_Rqst[ii].time_stamp=TimeStamp;
           TSD[ii].time_stamp_pool=TimeStamp;
           TSD[ii].n_transaction++;
           f_queue_put(TSD[ii].port, &TSD_Rqst[ii]) ;
}
/*----------------------------*/


#define  sw_GLB  TSD_GLB_fl
unsigned long int iGLBit[max_TSD];
void f_init_TSD_GLB(int ii)
 {
 int i;


   TSD[ii].time_stamp_pool=TimeStamp;
   TSD_time_pool=TimeStamp;

   switch ( sw_GLB[ii])
   {
    case 0:

       if(TSD[ii].port == 3)
       {
        i=3;
        ComBaud[i]=2400;
        Com_lgth[i]=7;
        Com_parity[i]=2;
        Com_stop[i]=1;
        RestoreCom(i);
        InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);
       }
       iGLBit[ii]=TimeStamp;

    // if(TSD_fn[ii] != 0) break;
       TSD_fn[ii]=TSD_WR_S;
       sprintf(TsdStr[ii],"%c",0x11); // инициализация
       sw_GLB[ii]=1;
       break;
    case 1:
       if(TSD_fn[ii] != 0) break;
       TSD_fn[ii]=TSD_WR_S;
       sprintf(TsdStr[ii],"%c",0x14); // зажечь все сегменты
//     sw_GLB[ii]=4;  // ,без выдержки свечения всех сегментов
       sw_GLB[ii]=2;
       break;
    case 2:
       if(TSD_fn[ii] != 0) break;
       iGLBit[ii]=TimeStamp;
       sw_GLB[ii]=3;
       break;

    case 3:
 if( f_timer(iGLBit[ii],300L) == 0) break;
       sw_GLB[ii]=4;
     //break;

    case 4:
       if(TSD_fn[ii] != 0) break;
       TSD_fn[ii]=TSD_WR_S;
       sprintf(TsdStr[ii],"%c",0x2); // погасить все сегменты
       sw_GLB[ii]=5;
       iGLBit[ii]=TimeStamp;
       break;

    case 5:
       if(TSD_fn[ii] != 0) break;


       TsdStr[ii][0]=0x60;
       TsdStr[ii][1]=0x20;
       TsdStr[ii][20]=0x8;

       TSD[ii].n_transaction    = 0;
       TSD[ii].n_success        = 0;
       TSD[ii].n_timeout_error  = 0;
       TSD[ii].n_CRC_error      = 0;

       TSD_val_c[ii][0]=-BIG_P;
       TSD_val_c[ii][1]=-BIG_P;
       TSD_val_c[ii][2]=-BIG_P;

       G_em_time=TimeStamp;
       sw_GLB[ii]=-1;
       break;

    case -1:
       break;
    default:break;
   }
 }
//----------------------
int ffgets_com_GLB( int ii)
{
//  Адрес функции будет устанавлен в указателе  ff_gets_com[Port]
// при посылке команды в устройство TSD_GLB .
//  Функция принимает символы из порта COM ii  в буфер cb_COM[ii].
//  При получении разделителя 'x' команда считается введенной и функция возвращает
//   значение 1.
//  Если прошло время ToutAns[ii] мс с момента приема последнего символа
// или с момента передачи команды - фиксируется таймаут, и в указатель
// функции обработчика ff_answ_com записывается адрес функции для обработки
// таймаута ( адрес которой записан в указателе ff_answ_flt).
//

/* ii - номер COM   1...4 */
 char tmp;
 int i;
struct COM_rqst *request;

m1:
     if(IsCom(ii)==0)
     {

        if(!IsTxBufEmpty(ii)) time_snd[ii] = TimeStamp;
        if( f_timer(time_snd[ii],ToutAns[ii]))
        { // истек таймаут приема ответа на команду
           ff_answ_com[ii]=ff_answ_flt[ii];
    //     printf("\n ToutAns = %ld, n_bcom = %d\n",ToutAns[ii],n_bcom[ii]);
    //     for(i=0;i<n_bcom[ii];i++)
    //      putch(cb_COM[ii][i]);

//    request=COM_que[ii].request[COM_que[ii].empt_ptr];
//    ToComBufn(4,request->Cmd,request->cmd_lgth);
//    printf("\nt_send= %ld",time_snd[ii]-request->time_stamp );
//    printf("\nt_cur= %ld",TimeStamp-request->time_stamp );

//         printf("\n");
           return 1;
        }
        return 0;
     }
     tmp=ReadCom(ii);
     cb_COM[ii][n_bcom[ii]]=tmp;
     if( (++n_bcom[ii]) >= n_bufcom )
      {
        cb_COM[ii][n_bufcom-1]=0;
        n_bcom[ii]=0;
  /*
        printf("\n Command buffer overflow !\n--");
        puts(cb_COM1);
  */
      }

   time_snd[ii]= TimeStamp;
   if(tmp=='x')     return 1;

   goto m1;
}
/* ---------------------------------------- */
/*----------------------------*/
void f_GLB_rd(int ii)
{
// ii - номер порта

 struct COM_rqst *request;
 int i,nn,fn;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства  (0,1..)
  fn=request->function;

           TSD[nn].n_success++;
           TSD_inp_err[nn]=0;
           if(fn == TSD_EM)
             if( flag_G_em > 0) flag_G_em -=1;
           f_queue_free(ii,&TSD_Rqst[nn]);
        // TSD_fn[nn]=0;
           return;
}
/*-------------------------------------------*/
void f_GLB_flt(int ii)
{
 struct COM_rqst *request;
 int nn,fn;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];

 nn=request->n_dev; // номер устройства (0,1..)

 TSD[nn].time_stamp_rcv=0;
 TSD[nn].n_timeout_error++;

 TSD_inp_err[nn]++;
 if(TSD_inp_err[nn]>=TSD_max_inp_err)
 {
   TSD_inp_err[nn]=TSD_max_inp_err;
   fn=request->function;
   if(fn == TSD_EM)
      f_icp_error(&TSD[nn],RD_ERR );
   else
      TSD_GLB_fl[nn]=0;
 }
// TSD_fn[nn]=0;
// f_queue_Flt(ii);
   f_queue_free(ii,&TSD_Rqst[nn]);
}
/* ---------------------------------------- */
void f_clr_TSD(int ii)
{  // очистка экранного буфера
  _fmemset( TsdBuf[ii][0],' ',sizeof(TsdBuf1)); // очистка буфера экрана
   fl_tsd_new[ii]=1;
}
//----------------------
#define TSD_unused_char  0xff
void f_rev_TSD(int ii)
{
   // вызывает передачу всего экранного буфера в TSD
  _fmemset( TsdBuf_c[ii][0],TSD_unused_char,sizeof(TsdBuf1));  // заполнение буфера копии неиспоьзуемым значением
   fl_tsd_new[ii]=1;
}
//----------------------
//----------------------------
int pnt_beg_G[3]={1,7,13}; // индекс в TsdStr[ii] на первый символ строки
int pnt_dot_G[3]={17,18,19}; // индекс в TsdStr[ii] на первый символ строки

void f_prep_StrG(int ii,int mm)
{
// по значению строки TsdBuf[ii][mm]  модифицирует содержимое
// TsdStr[ii] - буфер команды вывода на дисплей Gilbarco
 int i,i_src;
 char *buf,*buf_str;

  buf=TsdBuf[ii][mm];
  buf_str=&TsdStr[ii][pnt_beg_G[mm]];

  TsdStr[ii][pnt_dot_G[mm]]=0x30;

 if(mm<2)
  for(i=0,i_src=0;i<6;i++,i_src++)
  {
   if(buf[i_src]=='.')
    {
     TsdStr[ii][pnt_dot_G[mm]]=0x36-i_src;
     i_src++;
    }
   buf_str[i]=buf[i_src];
  }
 else
  for(i=0,i_src=2;i<4;i++,i_src++)
  {
   if(buf[i_src]=='.')
    {
     TsdStr[ii][pnt_dot_G[mm]]=0x36-i_src;
     i_src++;
    }
   buf_str[i]=buf[i_src];
  }
}
//----------------------------
void f_print_TSD(int ii)
{ // отображает значения float на дисплее

 int nn;
 int mm;
 char buf_tmp[20];
 float ftmp;

 if( f_timer(TSD_time_pool,TSD_period) == 0) return;
    TSD_time_pool=TimeStamp;

 if(TSD[ii].status != 0)
   if(TSD_fn[ii] != 0) return;

 /*-------------------------*/
 // Сравнение буфера экрана и вывод в TSD при наличии расхождений

  ftmp=fabs(*TSD_value[ii][0]);
  if( ftmp != TSD_val_c[ii][0])
  {
   TSD_val_c[ii][0]=ftmp;
   f_prep_TSD(n_dot[ii][0],TsdBuf[ii][0],ftmp);
   f_prep_StrG(ii,0);
   TSD_fn[ii]=TSD_WR_S;
  }

  ftmp=fabs(*TSD_value[ii][1]);
  if( ftmp != TSD_val_c[ii][1])
  {
   TSD_val_c[ii][1]=ftmp;
   f_prep_TSD(n_dot[ii][1],TsdBuf[ii][1],ftmp);
   f_prep_StrG(ii,1);
   TSD_fn[ii]=TSD_WR_S;
  }

  ftmp=fabs(*TSD_value[ii][2]);
  if( ftmp != TSD_val_c[ii][2])
  {
   TSD_val_c[ii][2]=ftmp;
   f_prep_TSD(n_dot[ii][2],TsdBuf[ii][2],ftmp);
   f_prep_StrG(ii,2);
   TSD_fn[ii]=TSD_WR_S;
  }
}
//----------------------------
char buf_space[]="      ";
void  f_prep_TSD(int dot,char *buf,float f_value)
{
int itmp;
itmp=dot;
  if( f_value< 0)
  {
m_err:
   strcpy(buf,buf_space);
   goto m_end;
  }

  if(itmp>3)
  {
/*
    if(f_value <= 9.99999)
    {
     itmp=5;
     goto m_sw;
    }
    if(f_value <= 99.9999)
    {
     itmp=4;
     goto m_sw;
    }
*/
    if(f_value <= 999.999)
    {
     itmp=3;
     goto m_sw;
    }
    if(f_value <= 9999.99)
    {
     itmp=2;
     goto m_sw;
    }

    if(f_value <= 99999.9)
    {
     itmp=1;
     goto m_sw;
    }
    if(f_value <= 999999)
    {
     itmp=0;
     goto m_sw;
    }
    goto m_err;
  }

m_sw:
  switch( itmp)
  {
  case 0:
     if(f_value > 999999) goto m_err;
     sprintf(buf,"%6.0f",f_value);
     break;

  case 1:
     if(f_value > 99999.9) goto m_err;
     sprintf(buf,"%7.1f",f_value);
     break;
  case 2:
     if(f_value > 9999.99) goto m_err;
     sprintf(buf,"%7.2f",f_value);
     break;
  case 3:
     if(f_value > 999.999) goto m_err;
     sprintf(buf,"%7.3f",f_value);
     break;
/*
  case 4:
     if(f_value > 99.9999) goto m_err;
     sprintf(buf,"%7.4f",f_value);
     break;

  case 5:
     if(f_value > 9.99999) goto m_err;
     sprintf(buf,"%7.5f",f_value);
     break;
*/
   default: goto m_err;

  }
m_end:
//printf("\n\r---%s---",buf);

}
//----------------------------

