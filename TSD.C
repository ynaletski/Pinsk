
// Трех-строчный дисплей, 6 символов в строке

#define ICP_TSD

//---------------
int TSD_vn[max_TSD][3]={
23,1,9,           // заданное зн.дозы, объем , масса
23,1,9,
};
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
"TSD/1",          // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
0,                // protocol
1,                // CRC_flag
500,              // pool_time
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
"TSD/2",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
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
/*----------------------------*/
struct COM_rqst TSD_Rqst[max_TSD]={
//----------------------
// device 1
//--------------------------
0,                   //  состояние запроса
ffgets_com_ICP,      //  gets_com
f_TSD_rd,            //  answ_com
f_TSD_flt,        //  answ_flt
"TSD/1",             //  name
0,                   //  n_dev номер устройства в группе однотипных устройств
&TSD[0],             //  *ICP_dd
0,                   //  текущая функция
ToutAnsTSD,           //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                   //  состояние запроса
ffgets_com_rtu,      //  gets_com
f_TSD_rd,            //  answ_com
f_TSD_flt,        //  answ_flt
"TSD/2",             //  name
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

char TsdStr[max_TSD][10];
int tsd_sw[max_TSD]={0,0};
int TsdStrN[max_TSD];
int fl_tsd_new[max_TSD]={1,1};
int fl_tsd_new1[max_TSD]={0,0};

//----------------------

void f_TSD(int ii)
{
 // ii - номер устройства
 //
int fn,sw;
unsigned char buf_tmp[20];
unsigned char *buf1_tmp;
int n_str;

 if(TSD[ii].status == 0) return;

//  f_print_TSD(ii);
/////  f_chk_new_TSD(ii);

  if((TSD_Rqst[ii].status == Req_Flt) ||
     (TSD_Rqst[ii].status == Req_OK));
  else return;

 if(f_queue_chk(TSD[ii].port)== 0) return;// очередь переполнена

 if((TSD_fn[ii] != 0) && (TSD_fn[ii] < F_PROGRESS))
      fn=TSD_fn[ii];
 else if( f_timer(TSD[ii].time_stamp_pool,TSD[ii].pool_time ))
      fn=TSD_WR_P;
 else return;

      switch(fn)
      {
          case TSD_WR_S:  // вывод строки
              buf1_tmp=TsdStr[ii];
              n_str=TsdStrN[ii];

              TSD_fn[ii]=F_PROGRESS;
              break;

          case TSD_WR_P:  // циклическая запись
          // формирует команды записи
             TSD_sw[ii]=(TSD_sw[ii]+1) & 0xf;
             sw=TSD_seq[TSD_sw[ii]];

             TSD_fn[ii]=0;

             switch (sw)
             {
              case 0:
              case 1:
              case 2:
               buf1_tmp=TsdBuf[ii][sw];
               n_str=sw+1;
               break;
              default:
               return;
             }
             fn+=sw;
             break;

          default:
          TSD_fn[ii]=0;
          return;
      }
// n_str - номер строки вывода
// buf1_tmp[] - содержание строки

    sprintf(buf_tmp,"~%02XT%01X%s",TSD[ii].addr,n_str,buf1_tmp);

           TSD_Rqst[ii].function = fn;
           f_prepare7000(buf_tmp,TSD[ii].CRC_flag);
//   printf("\n\r==%s==",buf_tmp);
           strcpy(TSD_Rqst[ii].Cmd,buf_tmp);
           TSD_Rqst[ii].cmd_lgth=strlen(buf_tmp);
           TSD_Rqst[ii].time_stamp=TimeStamp;
           TSD[ii].time_stamp_pool=TimeStamp;
           TSD[ii].n_transaction++;
           f_queue_put(TSD[ii].port, &TSD_Rqst[ii]) ;
}
/*----------------------------*/
void f_TSD_rd(int ii)
{
// ii - номер порта

 struct COM_rqst *request;
 int i,nn,fn;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства  (0,1..)

  fn=request->function;
  if((fn==TSD_WR_S) || ((fn & 0xf8)== TSD_WR_P ) )
  { // ответ на команду записи
       if(cb_COM[ii][0]=='!')
       {
           TSD[nn].n_success++;
           TSD_inp_err[nn]=0;
           f_queue_free(ii,&TSD_Rqst[nn]);
           TSD_fn[nn]=0;
           return;
       }
  }

m_err:
         TSD[nn].n_CRC_error++;
         TSD_inp_err[nn]++;
         if(TSD_inp_err[nn]>=TSD_max_inp_err)
         {
           TSD_inp_err[nn]=TSD_max_inp_err+1;
           f_icp_error(&TSD[nn],RD_ERR );
         }

         if(fn==TSD_WR_S)
         {
             TSD_fn[nn]=fn;
         }
         else
             TSD_fn[nn]=0;
         f_queue_free(ii,&TSD_Rqst[nn]);
}
/*-------------------------------------------*/
void f_TSD_flt(int ii)
{
 struct COM_rqst *request;
 int nn;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];

 nn=request->n_dev; // номер устройства (0,1..)

 TSD[nn].time_stamp_rcv=0;
 TSD[nn].n_timeout_error++;

 TSD_inp_err[nn]++;
 if(TSD_inp_err[nn]>=TSD_max_inp_err)
 {
   TSD_inp_err[nn]=TSD_max_inp_err;
   f_icp_error(&TSD[nn],RD_ERR );
 }
 f_queue_Flt(ii);
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
long int TSD_time_pool=0;
long int TSD_period=50;  // ms
/*================
void f_chk_new_TSD(int ii)
{  // отображает содержание TsdBuf на дисплей
 char *buf,*buf_c;

  if( fl_tsd_new1[ii]==0)
    if( f_timer(TSD_time_pool,TSD_period) == 0) return;

 if(TSD_fn[ii] != 0) return;

 if((fl_tsd_new[ii] != 0) && (fl_tsd_new1[ii]==0) )
 {
 // найти и отобразить изменения  с самого начала буфера
   fl_tsd_new1[ii]=1;
   fl_tsd_new[ii]=0;
   TsdStrN[ii]=1;
   tsd_sw[ii]=0;
 }
 if(fl_tsd_new1[ii]==0) return;;

 TSD_time_pool=TimeStamp;

 switch(tsd_sw[ii])
 {
 //-------------------------
 // Сравнение буфера экрана и вывод в TSD при наличии расхождений

  case 0:
mm1:
   buf=TsdBuf[ii][TsdStrN[ii]];
   buf_c=TsdBuf_c[ii][TsdStrN[ii]];

   if( _fmemcmp( buf, buf_c , 7) != 0)
   { //  найдены различия

    _fmemcpy(buf_c,buf,(long int)7);
    _fmemcpy(TsdStr[ii],buf,(long int)7);
    TSD_fn[ii]=TSD_WR_S;
    mmi_sw=1;
    break;
   }
 //-------------------------
  case 1:
    if(++TsdStrN[ii] > 3)
    {
     fl_tsd_new1[ii]=0;
     TsdStrN[ii]=1;
     break;
    }
    goto  mm1;

  default:
       break;
 }
}

==========================*/
//----------------------
float fict_TSD=0;

float TSD_val_c[max_TSD][3]={
 -1,-1,-1,
 -1,-1,-1
};

int  tsd_swf[max_TSD]={0,0};
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

 nn=0;
 switch(tsd_swf[ii])
 {
 /*-------------------------*/
 // Сравнение буфера экрана и вывод в TSD при наличии расхождений

   case 0:
  mm1:
  tsd_swf[ii]++;
  ftmp=fabs(*TSD_value[ii][0]);
  if( ftmp == TSD_val_c[ii][0])
  {
   if((nn++) > 2)
   {
    TSD_time_pool=TimeStamp;
    return;
   }
  }
  else
  {
   TsdStrN[ii]=1;
   mm=0;
   TSD_val_c[ii][0]=ftmp;
mm_end:
   TSD_val_c[ii][mm]=ftmp;
   f_prep_TSD(n_dot[ii][mm],buf_tmp,ftmp);
   strcpy(TsdBuf[ii][mm],buf_tmp);
   strcpy(TsdBuf_c[ii][mm],buf_tmp);
   strcpy(TsdStr[ii],buf_tmp);

   TSD_fn[ii]=TSD_WR_S;
   break;
  }

  case 1:
   ftmp=fabs(*TSD_value[ii][1]);
   tsd_swf[ii]++;
  if( ftmp == TSD_val_c[ii][1])
  {
   if((nn++) > 2)
   {
    TSD_time_pool=TimeStamp;
    return;
   }
  }
  else
  {
   TsdStrN[ii]=2;
   mm=1;
   goto mm_end;
  }

  case 2:
  ftmp=fabs(*TSD_value[ii][2]);
  tsd_swf[ii]=0;
  if( ftmp == TSD_val_c[ii][2])
  {
   if((nn++) > 2)
   {
    TSD_time_pool=TimeStamp;
    return;
   }
   else goto mm1;
  }
  else
  {
   TsdStrN[ii]=3;
   mm=2;
   goto mm_end;
  }
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