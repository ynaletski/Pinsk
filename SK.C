
// электропривод "Commander SK"

//    пример включения
// freq=500; // 50 Hz
// ComSK_SV[0]=freq; // задание скорости
// ComSK_Ctrl[0]=START_FWD_SK;
// while((ComSK_State[0] & MSKCH_RUN_SK ) != MSK_RUN_SK )
//  {
//    if(FL_err != 0) {...;break;}
//  }
//  ....
//
// ComSK_Ctrl[0]=STOP_FWD_SK;
// while((ComSK_State[0] & MSKCH_RUN_SK ) != MSK_STP_SK )
//  {
//    if(FL_err != 0) {...;break;}
//  }
//

// #define RTU_ComSK  // см. device.h

#define ComSK_State     DriveState
#define ComSK_SV        DriveSV
#define ComSK_SA        DriveSA
#define ComSK_SD        DriveSD
#define ComSK_Err_cod   DriveError
#define ComSK_current   DriveCurrent

void f_ComSK_Run()
{
 ComSK_Ctrl[0]=START_FWD_SK;
}
//------------------------
void f_ComSK_Stop()
{
 ComSK_Ctrl[0]=STOP_FWD_SK;
}
//------------------------
int f_ComSK_St_chk()
{
// if(FL_err != 0) return -1;
 if((ComSK_State[0] & MSKCH_RUN_SK ) == MSK_STP_SK ) return 1;
 else return 0;
}
//------------------------
int f_ComSK_Rn_chk()
{
// if(FL_err != 0) return -1;
 if((ComSK_State[0] & MSKCH_RUN_SK ) == MSK_RUN_SK ) return 1;
 else return 0;
}
//------------------------

#define max_SK 2
//---------------
struct s_icp_dev ComSK[max_SK]=
{
// device 1
0,                // status
2,                // port
3,                // addr
"ComSK/1",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&ComSK_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
4,                // addr
"ComSK/2",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&ComSK_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst ComSK_Rqst[max_SK]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_ComSK_rd,            //  answ_com
f_ComSK_rd,            //  answ_flt
"ComSK/1",              //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&ComSK[0],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsSK_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_ComSK_rd,            //  answ_com
f_ComSK_rd,            //  answ_flt
"ComSK/2",              //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&ComSK[1],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsSK_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/
struct COM_rqst ComSK_WD_Rqst[max_SK]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_ComSK_WD_rd,         //  answ_com
f_ComSK_WD_rd,         //  answ_flt
"SK_WD/1",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&ComSK[0],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsSK_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_ComSK_WD_rd,         //  answ_com
f_ComSK_WD_rd,         //  answ_flt
"SK_WD/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&ComSK[1],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsSK_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

int ComSK_Ctrl[max_SK]={0,0};   // упраляющее слово
int ComSK_Ctrl_c[max_SK]={0,0}; // текущее состояние упраляющего слово

/*
int ComSK_State[max_SK]={0,0};   // слово состояния
int ComSK_SV[max_SK]={0,0};     // значение частоты вращения в 0.1 Гц
int ComSK_SA[max_SK]={20,20};  // текущее состояние  ускорения 2 сек на 100Гц
int ComSK_SD[max_SK]={20,20};  // текущее состояние  замедления 2 сек на 100Гц
int ComSK_current[max_SK]={0,0};
int ComSK_Err_cod[max_SK]={0,0};// ошибка последнего отключения
*/

int ComSK_SVc[max_SK]={-1,-1};  // текущее состояние  частоты вращения

int ComSK_SAc[max_SK]={-1,-1};  // текущее состояние  ускорения

int ComSK_SDc[max_SK]={-1,-1}; // текущее состояние замедления

int ComSK_fn[max_SK]={0,0};    // регистр функций драйвера

//  Если записать значение функции в регистр функции , например:
//  ComSK_fn[0]= RES_SK_ERR;  драйвер выполнит заданную функцию - в данном случае
// пошлет в устройство ComSK команду сброса привода.
//  После постановки запроса с установленной функцией в очередь на вывод в COM,
// регистр функции очищается драйвером.

int ComSK_sw[max_SK]={0,0};

//unsigned int ComSK_out_err[max_SK]={0,0};  // счетчик ошибок вывода
unsigned int ComSK_inp_err[max_SK]={0,0};  // счетчик ошибок ввода

int ComSK_seq[16]={0,3,2,3,0,1,3,2,0,3,1,0,3,1,2,3};  // последовательность функций опроса
         //      0  чтение слова состояния
         //      1  чтение слова управления
         //      2  чтение текущего SV
         //      3  чтение текущего тока

int ComSK_fl[max_SK]={0,0};

// 11.23 - адрес устройства
//
// 0x00d2 - адрес регистра ускорения  2.11
// 0x00dc - адрес регистра замедления 2.21
// 0x0078 - адрес регистра предустановленной скорости вращения 1.21
// 0x0281 - адрес регистра управляющего слова 6.42
// 0x2000 - бит сброса устройства
// 0x0282 - адрес регистра включения управляющего слова 6.43
// 0x032B - адрес регистра инверсии входа разрешения 8.12
// 0x03fb - адрес регистра последнего отключения 10.20
// 0x040f - адрес регистра слова состояния 10.40
// 0x0190 - адрес регистра текущего тока   4.01

unsigned char ComResSK[]={1,6,0x02,0x81,0x20,0x00};
//                        | |    |    |    | |
//                        | |    |    |    | младший байт данных
//                        | |    |    | старший байт данных
//                        | |    | младший байт адреса
//                        | |  старший байт адреса
//                        | код функции (однократная запись)
//                        адрес

unsigned char SK_Set_ctr[]={1,6,0x02,0x82,0x00,0x01};
unsigned char SK_Set_mode[]={1,6,0x04,0x66,0x00,0x03};
unsigned char SK_Set_irdy[]={1,6,0x03,0x2b,0x00,0x01};
unsigned char SK_Rd_state[]={1,3,0x04,0x0f,0x00,0x01};
unsigned char SK_Rd_ctrl[]={1,3,0x02,0x81,0x00,0x01};
unsigned char SK_Rd_sv[]={1,3,0x00,0x78,0x00,0x01};
unsigned char SK_Rd_cur[]={1,3,0x01,0x90,0x00,0x01};
unsigned char SK_Rd_err[]={1,3,0x03,0xfb,0x00,0x01};

void f_ComSK(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных слова состояния из
  // устройства ComSK( '01 03 0281 0001'),
  // а также текущего состояния слова управления и предустановки скорости.
  //   Период посылки ComSK[ii].pool_time мс.
  //
  //   Если состояние уставки частоты вращения (ComSK_SVc[ii])
  //  отличается от требуемого состояния ComSK_SV[ii] , будет сформирована
  //  команда записи в регистр предустановки частоты вращения.
  //   Если состояние слова управления (ComSK_Ctrl_c[ii])
  //  отличается от требуемого состояния ComSK_Ctrl[ii] , будет сформирована
  //  команда записи в регистр управляющего слова.
  //    Если в  регистр функции != 0 - функция выполняется
  //  в первую очередь.
  //

int i,fn,sw,i2,itmp;
long int i1;
unsigned char buf_tmp[20];

 if(ComSK[ii].status == 0) return;

 if( ComSK_fl[ii] >= 0 )
            f_init_ComSK(ii);
 else
     f_sk_wd(ii);

 if(ComSK_fn[ii] != 0)
      fn=ComSK_fn[ii];
 else if( (ComSK_Ctrl[ii] ^ ComSK_Ctrl_c[ii] ) & MSK_SK_WDT)
      fn=SK_CTRL_OUT;
 else if(ComSK_SV[ii] != ComSK_SVc[ii])
      fn=SK_SV_OUT;
 else if(ComSK_SA[ii] != ComSK_SAc[ii])
      fn=SK_SA_OUT;
 else if(ComSK_SD[ii] != ComSK_SDc[ii])
      fn=SK_SD_OUT;
 else if( f_timer(ComSK[ii].time_stamp_pool,ComSK[ii].pool_time ))
      fn=SK_RD;
 else return;

  if((ComSK_Rqst[ii].status == Req_Flt) ||
     (ComSK_Rqst[ii].status == Req_OK)) goto m1;
//  else if( f_timer(IComSK[ii].time_stamp_pool,(IComSK[ii].pool_time+IComSK[ii].pool_time) ))
//           goto m1;
  return;

m1:
         if(f_queue_chk(ComSK[ii].port)== 0) return;// очередь переполнена
         if(fn  == SK_SET_CTR)
          {  // включение работы через управляющее слово
            i1=6;
            _fmemcpy(buf_tmp,SK_Set_ctr,i1);
            buf_tmp[0]=ComSK[ii].addr;
            ComSK_fn[ii]=0;
          }
         else if(fn  == SK_SET_MODE)
          {  // включение работы через управляющее слово
            i1=6;
            _fmemcpy(buf_tmp,SK_Set_mode,i1);
            buf_tmp[0]=ComSK[ii].addr;
            ComSK_fn[ii]=0;
          }
          else if(fn  == SK_RES_ERR)
          {
            i1=6;
            _fmemcpy(buf_tmp,ComResSK,i1);
            buf_tmp[0]=ComSK[ii].addr;
            ComSK_fn[ii]=0;
            ComSK_Ctrl[ii]=0;
          }
          else if(fn  == SK_INV_RDY)
          {  // задание инверсии дискретного входа разрешения работы
            i1=6;
            _fmemcpy(buf_tmp,SK_Set_irdy,i1);
            buf_tmp[0]=ComSK[ii].addr;
            ComSK_fn[ii]=0;
          }
          else if(fn  == SK_RD_ERR)
          { // после получения ответа в ComSK_Err_cod[] будет записан код последней ошибки
            i1=6;
            _fmemcpy(buf_tmp,SK_Rd_err,i1);
            buf_tmp[0]=ComSK[ii].addr;
            ComSK_fn[ii]=0;
            ComSK_Err_cod[ii]=-1;
          }
          else if(fn == SK_SV_OUT) // вывод уставки скорости вращения
          {
            i1=6;
            buf_tmp[0]=ComSK[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=00;   // 0x0078 - адрес регистра предустановленной скорости вращения 1.21
            buf_tmp[3]=0x78; //
            buf_tmp[4]=(ComSK_SV[ii]>>8)&0xff;
            buf_tmp[5]=ComSK_SV[ii] & 0xff;
            ComSK_SVc[ii]=ComSK_SV[ii];
          }
          else if(fn == SK_SA_OUT) // вывод уставки ускорения
          {
            i1=6;
            buf_tmp[0]=ComSK[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=00;   // 0x0078 - адрес регистра предустановленной скорости вращения 1.21
            buf_tmp[3]=0xd2; //
            buf_tmp[4]=(ComSK_SA[ii]>>8)&0xff;
            buf_tmp[5]=ComSK_SA[ii] & 0xff;
            ComSK_SAc[ii]=ComSK_SA[ii];
          }
          else if(fn == SK_SD_OUT) // вывод уставки замедления
          {
            i1=6;
            buf_tmp[0]=ComSK[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=00;   // 0x0078 - адрес регистра предустановленной скорости вращения 1.21
            buf_tmp[3]=0xdc; //
            buf_tmp[4]=(ComSK_SD[ii]>>8)&0xff;
            buf_tmp[5]=ComSK_SD[ii] & 0xff;
            ComSK_SDc[ii]=ComSK_SD[ii];
          }
          else if(fn == SK_CTRL_OUT) // вывод управляющего слова
          {
            i1=6;

            itmp=ComSK_Ctrl[ii] | sk_wd[ii];

            buf_tmp[0]=ComSK[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=0x02; // 0x0281 - адрес регистра управляющего слова 6.42
            buf_tmp[3]=0x81; //
            buf_tmp[4]=(itmp>>8)&0xff;
            buf_tmp[5]= itmp & 0xff;
            ComSK_Ctrl_c[ii]=ComSK_Ctrl[ii];
          }
          else if(fn == SK_RD)  // чтение состояния
          {// формирует команды чтения
             ComSK_sw[ii]=(ComSK_sw[ii]+1) & 0xf;
             sw=ComSK_seq[ComSK_sw[ii]];
             switch (sw)
             {
              case 0:  // чтение слова состояния
            i1=6;
            _fmemcpy(buf_tmp,SK_Rd_state,i1);
                break;
              case 1:  // чтение слова управления
            i1=6;
            _fmemcpy(buf_tmp,SK_Rd_ctrl,i1);
                break;
              case 2:  // чтение текущего SV
            i1=6;
            _fmemcpy(buf_tmp,SK_Rd_sv,i1);
                break;
              case 3:  // чтение текущего тока
            i1=6;
            _fmemcpy(buf_tmp,SK_Rd_cur,i1);
                break;

              default:
                return;
             }
             buf_tmp[0]=ComSK[ii].addr;
             fn+=sw;
          }
          else return;

           ComSK_Rqst[ii].function = fn;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(ComSK_Rqst[ii].Cmd,buf_tmp,(long)i2);
           ComSK_Rqst[ii].cmd_lgth=i2;
           ComSK_Rqst[ii].time_stamp=TimeStamp;
           ComSK[ii].time_stamp_pool=TimeStamp;
           ComSK[ii].n_transaction++;
           f_queue_put(ComSK[ii].port, &ComSK_Rqst[ii]) ;
}
/*----------------------------*/
#define  sw_SK  ComSK_fl
unsigned long int ComSKit[max_SK];
void f_init_ComSK(int ii)
 {
   switch (sw_SK[ii])
   {
    case 0:
       if(ComSK_fn[ii] != 0) break;
       ComSK_fn[ii]=SK_RES_ERR;
       sw_SK[ii]=1;
       break;
    case 1:
       if(ComSK_fn[ii] != 0) break;
       ComSK_fn[ii]=SK_SET_CTR;
       sw_SK[ii]=2;
       break;

    case 2:
       if(ComSK_fn[ii] != 0) break;
       ComSK_fn[ii]=SK_INV_RDY;
       sw_SK[ii]=3;
       break;

    case 3:
       if(ComSK_fn[ii] != 0) break;
       ComSK_fn[ii]=SK_SET_MODE;
       sw_SK[ii]=10;
       break;

    case 10:
       if(ComSK_fn[ii] != 0) break;
       ComSKit[ii]=TimeStamp;
       sw_SK[ii]=11;
       break;

    case 11:
 if( f_timer(ComSKit[ii],100L) == 0) break;

       ComSK[ii].n_transaction    = 0;
       ComSK[ii].n_success        = 0;
       ComSK[ii].n_timeout_error  = 0;
       ComSK[ii].n_CRC_error      = 0;
       ComSK_Err_cod[ii] = 0;

       DriveStatus=&ComSK[0].status;

       f_Drive_St_chk= f_ComSK_St_chk;
       f_Drive_Rn_chk= f_ComSK_Rn_chk;
       f_Drive_Stop  = f_ComSK_Stop;
       f_Drive_Run   = f_ComSK_Run;

       sw_SK[ii]=-1;
       break;

    case -1:
       break;
    default:break;
   }
 }
//-----------------------------------------
int itmpComSK;
void f_ComSK_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn,fn;
 int count;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства ComSK (0,1..)
  fn=request->function;

  count=n_bcom[ii];  // длина команды
  if(count < 3)
   {
      goto m_tout_err;
   }
  if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
  if(( fn & 0xf0)==RD_STATUS) // ответ на команду чтения регистра
  {
      if(cb_COM[ii][1] != 3)
      {
        goto m_crc_err;
      }
      fn &= 0xf;
      switch(  fn )
      {
      case 0:  // чтение слова состояния
    //в ответ на 01 03 04 0f 00 01 приходит:
    //  01 03 02 00 05 78 47

         ComSK_State[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8) &0xff00)  ;

       if( ComSK_fl[nn] < 0 )
         if((ComSK_State[nn] & SK_OK)== 0)
         {
           if( f_get_icp_error(&ComSK[nn]) & Drive_ERR) break;
           ComSK_fn[nn]=SK_RD_ERR;
         }
         break;
      case 1:     // чтение слова управления
         ComSK_Ctrl_c[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         break;

      case 2:    // чтение текущего SV
         ComSK_SVc[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         break;
      case 3:    // чтение текущего тока
         ComSK_current[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         f_reg_I();
         break;
      default: goto m_crc_err;
      }
  }
  else if(fn  == SK_RD_ERR)
  {
    if(cb_COM[ii][1] != 3)
    {
       goto m_crc_err;
    }
    ComSK_Err_cod[nn]=(cb_COM[ii][4] & 0x00ff) | ( ((unsigned int)cb_COM[ii][3] << 8)  & 0xff00)  ;
    f_icp_error(&ComSK[nn],Drive_ERR | ((ComSK_Err_cod[nn] << 8)& 0xff00) );
  }
  else
  { // ответ на команду записи
    if(cb_COM[ii][1] != 6)
    {
      goto m_crc_err;
    }
  }
  ComSK[nn].n_success++;
  ComSK_inp_err[nn]=0;
  f_queue_free(ii,&ComSK_Rqst[nn]);
  return;

m_crc_err:
 ComSK[nn].time_stamp_rcv=0;
 ComSK[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 ComSK[nn].time_stamp_rcv=0;
 ComSK[nn].n_timeout_error++;

m_err:
 ComSK_inp_err[nn]++;

 switch (fn)
 {
  case SK_SV_OUT  :
       ComSK_SVc[nn]=-1;
       break;
  case SK_SA_OUT  :
       ComSK_SAc[nn]=-1;
       break;
  case SK_SD_OUT  :
       ComSK_SDc[nn]=-1;
       break;
  case SK_CTRL_OUT  :
       ComSK_Ctrl_c[nn]=-1;
       break;
  case SK_INV_RDY  :
  case SK_RES_ERR  :
  case SK_SET_MODE :
  case SK_SET_CTR :
       ComSK_fn[nn]=fn;
       break;
 }
 if(ComSK_inp_err[nn]>=ComSK_max_inp_err)
 {
   ComSK_inp_err[nn]=ComSK_max_inp_err;
   f_icp_error(&ComSK[nn],RD_ERR );
 }
 f_queue_free(ii,&ComSK_Rqst[nn]);
}
/* ---------------------------------------- */
void f_ComSK_WD_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn;
 int count;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства ComSK (0,1..)

  count=n_bcom[ii];  // длина команды
  if(count < 3)
   {
      goto m_tout_err;
   }
  if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
  // ответ на команду записи
  if(cb_COM[ii][1] != 6)
  {
    goto m_crc_err;
  }

  ComSK[nn].n_success++;
  ComSK_inp_err[nn]=0;
  f_queue_free(ii,&ComSK_WD_Rqst[nn]);
  return;

m_crc_err:
 ComSK[nn].time_stamp_rcv=0;
 ComSK[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 ComSK[nn].time_stamp_rcv=0;
 ComSK[nn].n_timeout_error++;

m_err:
 ComSK_inp_err[nn]++;
 if(ComSK_inp_err[nn]>=ComSK_max_inp_err)
 {
   ComSK_inp_err[nn]=ComSK_max_inp_err;
   f_icp_error(&ComSK[nn],RD_ERR );
 }
 f_queue_free(ii,&ComSK_WD_Rqst[nn]);
}
/* ---------------------------------------- */
long int sk_wd_time[max_SK]={0,0};
long int sk_pool_period[max_SK]={200,200};
int sk_wd[max_SK]={0,0};

void  f_sk_wd( int ii)
{
  // Watch Dog для Commander SK
  // ii - номер устройства

unsigned char buf_tmp[20];
int i,i2,itmp;
long int i1;

 if(FL_err) return;
 if( f_timer(sk_wd_time[ii],sk_pool_period[ii] ))
 {
  if((ComSK_WD_Rqst[ii].status == Req_Flt) ||
     (ComSK_WD_Rqst[ii].status == Req_OK)) ;
  else return;

           sk_wd[ii] ^= SK_WDT;

           itmp=ComSK_Ctrl[ii] | sk_wd[ii];
           i1=6;
           buf_tmp[0]=ComSK[ii].addr;
           buf_tmp[1]=6;
           buf_tmp[2]=0x02; // 0x0281 - адрес регистра управляющего слова 6.42
           buf_tmp[3]=0x81; //
           buf_tmp[4]=(itmp>>8)&0xff;
           buf_tmp[5]= itmp & 0xff;
           ComSK_Ctrl_c[ii]=ComSK_Ctrl[ii];

           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(ComSK_WD_Rqst[ii].Cmd,buf_tmp,(long)i2);
           ComSK_WD_Rqst[ii].cmd_lgth=i2;
           ComSK_WD_Rqst[ii].time_stamp=TimeStamp;
           sk_wd_time[ii]=TimeStamp;
           ComSK[ii].n_transaction++;
           f_queue_put(ComSK[ii].port, &ComSK_WD_Rqst[ii]) ;
 }
}
/* ---------------------------------------- */
