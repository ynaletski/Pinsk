//
// электропривод "DELTA"
//
//    пример включения
// Delta_SV[0]=freq; // задание скорости
// Delta_Ctrl[0]=START_FWD_DELTA;
// while((Delta_State[0] & MSK_RUN_DELTA ) !=RUN_DELTA )
//  {
//    if(FL_err != 0) {...;break;}
//  }
//  ....
//
// Delta_Ctrl[0]=STOP_FWD_DELTA;
// while((Delta_State[0] & MSK_RUN_DELTA ) !=STP_DELTA )
//  {
//    if(FL_err != 0) {...;break;}
//  }
//

// #define RTU_Delta // см. device.h

#define Delta_State     DriveState
#define Delta_SV        DriveSV
#define Delta_SA        DriveSA
#define Delta_SD        DriveSD
#define Delta_Err_cod   DriveError
#define Delta_current   DriveCurrent

//------------------------
void f_Delta_Run()
{
 Delta_Ctrl[0]=START_FWD_DELTA;
}
//------------------------
void f_Delta_Stop()
{
 Delta_Ctrl[0]=STOP_FWD_DELTA;
}
//------------------------
int f_Delta_St_chk()
{
 if(FL_err != 0) return -1;
 if((Delta_State[0] & MSK_RUN_DELTA ) == STP_DELTA ) return 1;
 else return 0;
}
//------------------------
int f_Delta_Rn_chk()
{
 return 1;  //qqq
 /*
 if(FL_err != 0) return -1;
 if((Delta_State[0] & MSK_RUN_DELTA ) == RUN_DELTA ) return 1;
 else return 0;
 */
}
//------------------------

#define max_Dlt 2
//---------------
struct s_icp_dev Delta[max_Dlt]=
{
// device 1
0,                // status
2,                // port
2,                // addr
"DeltC/1",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
600,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Delta_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
1,                // port
3,                // addr
"DeltC/2",        // name[8]
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
&Delta_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst Delta_Rqst[max_Dlt]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Delta_rd,            //  answ_com
f_Delta_rd,            //  answ_flt
"Delta/1",              //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&Delta[0],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsDlt_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Delta_rd,            //  answ_com
f_Delta_rd,            //  answ_flt
"Delta/2",              //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&Delta[1],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsDlt_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/
int Delta_Ctrl[max_Dlt]={0,0};   // упраляющее слово
int Delta_Ctrl_c[max_Dlt]={0,0}; // текущее состояние упраляющего слово

/*
int Delta_State[max_Dlt]={0,0};  // слово состояния
int Delta_SV[max_Dlt]={5000,5000};  // значение частоты вращения
int Delta_SA[max_Dlt]={20,20};     // текущее состояние  ускорения 2 сек
int Delta_SD[max_Dlt]={20,20};  // текущее состояние  замедления 2 сек на 100Гц
int Delta_current[max_Dlt]={0,0};
int Delta_Err_cod[max_Dlt]={0,0};   // ошибка последнего отключения
*/


int Delta_SVc[max_Dlt]={-1,-1};     // текущее состояние  частоты вращения

int Delta_SAc[max_Dlt]={-1,-1};    // текущее состояние  ускорения

int Delta_SDc[max_Dlt]={-1,-1}; // текущее состояние замедления

int Delta_fn[max_Dlt]={0,0};    // регистр функций драйвера

//  Если записать значение функции в регистр функции , например:
//  Delta_fn[0]= RES_Dlt_ERR;  драйвер выполнит заданную функцию - в данном случае
//  пошлет в устройство Delta команду сброса привода.
//  После постановки запроса с установленной функцией в очередь на вывод в COM,
//  регистр функции очищается драйвером.

int Delta_sw[max_Dlt]={0,0};

//unsigned int Delta_out_err[max_Dlt]={0,0};  // счетчик ошибок вывода
unsigned int Delta_inp_err[max_Dlt]={0,0};  // счетчик ошибок ввода

int Delta_seq[16]={1,2,3,4,0,1,2,3,4,0,1,2,3,4,1,0};
            // последовательность функций опроса
               // 0 чтение слова состояния
               // 1 чтение слова управления
               // 2 чтение текущего SV
               // 3 чтение регистра ошибки
               // 4 чтение тока

int Delta_fl[max_Dlt]={0,0};

// 09.00 - адрес устройства
//
// 0x0112 - адрес регистра ускорения  1.12
// 0x0113 - адрес регистра замедления 1.13
// 0x2001 - адрес регистра предустановленной скорости вращения
// 0x2000 - адрес регистра управляющего слова
// 0x2002 - 0x2 - бит сброса устройства
// 0x0021 - адрес регистра включения управляющего слова 00.21
// 0x2100 - адрес регистра ошибки
// 0x2101 - адрес регистра слова состояния

unsigned char ComResDlt[]={1,6,0x20,0x02,0x00,0x02};   // сброс ошибок
//                        |  |   |    |    |   |
//                        |  |   |    |    | младший байт данных
//                        |  |   |    | старший байт данных
//                        |  |   | младший байт адреса
//                        |  |  старший байт адреса
//                        | код функции (однократная запись)
//                        адрес
unsigned char Dlt_Set_mode[6]={1,6,0x00,0x21,0x00,0x02};  // 2 - включение работы через RS485


unsigned char Dlt_Set_ctr[]  ={1,6,0x00,0x20,0x00,0x01};  // включение уставки скорости через RS485

unsigned char Dlt_Rd_sv[]   ={1,3,0x20,0x01,0x00,0x01};  // зад.частота
unsigned char Dlt_Rd_err[]  ={1,3,0x21,0x00,0x00,0x01};  // код ошибки

unsigned char Dlt_Rd_ctrl[] ={1,3,0x20,0x00,0x00,0x01};   // управляющее слово
unsigned char Dlt_Rd_curr[] ={1,3,0x21,0x04,0x00,0x01};   // выходной ток
unsigned char Dlt_Rd_state[]={1,3,0x21,0x01,0x00,0x01};   // ?? состояние ??

void f_Delta(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных слова состояния из
  // устройства Delta( '01 03 0001 0001'),
  // а также текущего состояния слова управления и предустановки скорости.
  //   Период посылки Delta[ii].pool_time мс.
  //
  //   Если состояние уставки частоты вращения (Delta_SVc[ii])
  //  отличается от требуемого состояния Delta_SV[ii] , будет сформирована
  //  команда записи в регистр предустановки частоты вращения.
  //   Если состояние слова управления (Delta_Ctrl_c[ii])
  //  отличается от требуемого состояния Delta_Ctrl[ii] , будет сформирована
  //  команда записи в регистр управляющего слова.
  //    Если в  регистр функции != 0 - функция выполняется
  //  в первую очередь.
  //

int i,fn,sw,i2,itmp;
long int i1;
unsigned char buf_tmp[20];

 if(Delta[ii].status == 0) return;



 if( Delta_fl[ii] >= 0 )
            f_init_Delta(ii);
// else
//     f_Dlt_wd(ii);

 if(Delta_fn[ii] != 0)
      fn=Delta_fn[ii];
 else if(Delta_Ctrl[ii] != Delta_Ctrl_c[ii] )
      fn=Dlt_CTRL_OUT;
 else if(Delta_SV[ii] != Delta_SVc[ii])
      fn=Dlt_SV_OUT;
// else if(Delta_SA[ii] != Delta_SAc[ii])
//      fn=Dlt_SA_OUT;
// else if(Delta_SD[ii] != Delta_SDc[ii])
//      fn=Dlt_SD_OUT;
 else if( f_timer(Delta[ii].time_stamp_pool,Delta[ii].pool_time ))
      fn=Dlt_RD;
 else return;

  if((Delta_Rqst[ii].status == Req_Flt) ||
     (Delta_Rqst[ii].status == Req_OK)) goto m1;
//  else if( f_timer(IDelta[ii].time_stamp_pool,(IDelta[ii].pool_time+IDelta[ii].pool_time) ))
//           goto m1;
  return;

m1:
         if(f_queue_chk(Delta[ii].port)== 0) return;// очередь переполнена
   /*
         if(fn  == Dlt_SET_CTR)
          {  // включение работы через управляющее слово
            i1=6;
            _fmemcpy(buf_tmp,Dlt_Set_ctr,i1);
            buf_tmp[0]=Delta[ii].addr;
            Delta_fn[ii]=0;
          }
         else if(fn  == Dlt_SET_MODE)
          {  // включение работы через управляющее слово
            if(flag_ext_t ==0) Dlt_Set_mode[5]=3;
            else Dlt_Set_mode[5]=1;
            i1=6;
            _fmemcpy(buf_tmp,Dlt_Set_mode,i1);
            buf_tmp[0]=Delta[ii].addr;
            Delta_fn[ii]=0;
          }

     */
          if(fn  == Dlt_RES_ERR)
          {
            i1=6;
            _fmemcpy(buf_tmp,ComResDlt,i1);
            buf_tmp[0]=Delta[ii].addr;
            Delta_fn[ii]=0;
            Delta_Ctrl[ii]=0;
          }
          else if(fn == Dlt_SV_OUT) // вывод уставки скорости вращения
          {
            i1=6;
            buf_tmp[0]=Delta[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=0x20;   // 0x2001 - адрес регистра предустановленной скорости вращения 0x2001
            buf_tmp[3]=0x01; //
            buf_tmp[4]=(Delta_SV[ii]>>8)&0xff;
            buf_tmp[5]=Delta_SV[ii] & 0xff;
            Delta_SVc[ii]=Delta_SV[ii];
          }
          /*
          else if(fn == Dlt_SA_OUT) // вывод уставки ускорения
          {
            i1=6;
            buf_tmp[0]=Delta[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=01;   // 0x010C = 01.12- адрес регистра ускорения 1  1.12
            buf_tmp[3]=0x0C; //
            buf_tmp[4]=(Delta_SA[ii]>>8)&0xff;
            buf_tmp[5]=Delta_SA[ii] & 0xff;
            Delta_SAc[ii]=Delta_SA[ii];
          }
          else if(fn == Dlt_SD_OUT) // вывод уставки замедления
          {
            i1=6;
            buf_tmp[0]=Delta[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=01;   // 0x010D= 01.13- адрес регистра замедления 1 1.13
            buf_tmp[3]=0xD; //
            buf_tmp[4]=(Delta_SD[ii]>>8)&0xff;
            buf_tmp[5]=Delta_SD[ii] & 0xff;
            Delta_SDc[ii]=Delta_SD[ii];
          }
       */
          else if(fn == Dlt_CTRL_OUT) // вывод управляющего слова
          {
            i1=6;

            itmp=Delta_Ctrl[ii];

            buf_tmp[0]=Delta[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=0x20; // 0x2000 - адрес регистра управляющего слова (09.06)
            buf_tmp[3]=0x00; //
            buf_tmp[4]=(itmp>>8)&0xff;
            buf_tmp[5]= itmp & 0xff;
            Delta_Ctrl_c[ii]=Delta_Ctrl[ii];
          }
          else if(fn == Dlt_RD)  // чтение состояния
          {// формирует команды чтения
             Delta_sw[ii]=(Delta_sw[ii]+1) & 0xf;
             sw=Delta_seq[Delta_sw[ii]];
            i1=6;
             switch (sw)
             {
              case 0:  // чтение слова состояния
            _fmemcpy(buf_tmp,Dlt_Rd_state,i1);
                break;
              case 1:  // чтение слова управления
            _fmemcpy(buf_tmp,Dlt_Rd_ctrl,i1);
                break;
              case 2:  // чтение текущего SV
            _fmemcpy(buf_tmp,Dlt_Rd_sv,i1);
                break;
              case 3:  // чтение регистра ошибки
            _fmemcpy(buf_tmp,Dlt_Rd_err,i1);
                break;
              case 4:  // чтение тока
            _fmemcpy(buf_tmp,Dlt_Rd_curr,i1);
                break;
              default:
                return;
             }
             buf_tmp[0]=Delta[ii].addr;
             fn+=sw;
          }
          else
          {
            Delta_fn[ii]=0;
            return;
          }
           Delta_Rqst[ii].function = fn;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(Delta_Rqst[ii].Cmd,buf_tmp,(long)i2);
           Delta_Rqst[ii].cmd_lgth=i2;
           Delta_Rqst[ii].time_stamp=TimeStamp;
           Delta[ii].time_stamp_pool=TimeStamp;
           Delta[ii].n_transaction++;
           f_queue_put(Delta[ii].port, &Delta_Rqst[ii]) ;
}
/*----------------------------*/
#define  sw_Dlt  Delta_fl
unsigned long int Deltait[max_Dlt];
void f_init_Delta(int ii)
 {

 /*
 if(flag_prn)
 {
  printf("\n\r 101");
 }
 */

   switch (sw_Dlt[ii])
   {
    case 0:
       if(Delta_fn[ii] != 0) break;
     // сброс ошибок
       Delta_fn[ii]=Dlt_RES_ERR;
       sw_Dlt[ii]=10;
       break;
/*
    case 1:
       if(Delta_fn[ii] != 0) break;
      // включение уставки скорости через RS485
       Delta_fn[ii]=Dlt_SET_CTR;
       sw_Dlt[ii]=2;
       break;

    case 2:
       if(Delta_fn[ii] != 0) break;
       Delta_fn[ii]=Dlt_SET_MODE;
     // задание источника старт/стоп
       sw_Dlt[ii]=10;
       break;
*/
    case 10:
       if(Delta_fn[ii] != 0) break;
       Deltait[ii]=TimeStamp;
       sw_Dlt[ii]=11;
       break;

    case 11:
 if( f_timer(Deltait[ii],100L) == 0) break;

       Delta[ii].n_transaction    = 0;
       Delta[ii].n_success        = 0;
       Delta[ii].n_timeout_error  = 0;
       Delta[ii].n_CRC_error      = 0;
       Delta_Err_cod[ii] = 0;

       DriveStatus=&Delta[0].status;

       f_Drive_St_chk= f_Delta_St_chk;
       f_Drive_Rn_chk= f_Delta_Rn_chk;
       f_Drive_Stop  = f_Delta_Stop;
       f_Drive_Run   = f_Delta_Run;

       sw_Dlt[ii]=-1;
       break;

    case -1:
       break;
    default:break;
   }

  flag_prn=0;
 }
//-----------------------------------------
int itmpDelta;
void f_Delta_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn,fn;
 int count;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства Delta (0,1..)
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

       Delta_State[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8) &0xff00) ;

         break;
      case 1:     // чтение слова управления
         Delta_Ctrl_c[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         break;

      case 2:    // чтение текущего SV
         Delta_SVc[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         break;
      case 3:    // чтение регистра ошибки
         Delta_Err_cod[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;

         if( Delta_fl[nn] < 0 )
           if(Delta_Err_cod[nn] != 0)
           {
             if( f_get_icp_error(&Delta[nn]) & Drive_ERR) break;
             f_icp_error(&Delta[nn],Drive_ERR | ((Delta_Err_cod[nn] << 8)& 0xff00) );
           }
         break;
      case 4:    // чтение тока
//       Delta_current[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         DriveCurrent[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
//       f_reg_I();
         break;
      default: goto m_crc_err;
      }
  }
  else
  { // ответ на команду записи
    if(cb_COM[ii][1] != 6)
    {
      goto m_crc_err;
    }
  }
  Delta[nn].n_success++;
  Delta_inp_err[nn]=0;
  f_queue_free(ii,&Delta_Rqst[nn]);
  return;

m_crc_err:
 Delta[nn].time_stamp_rcv=0;
 Delta[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 Delta[nn].time_stamp_rcv=0;
 Delta[nn].n_timeout_error++;

m_err:
 Delta_inp_err[nn]++;

 switch (fn)
 {
  case Dlt_SV_OUT  :
       Delta_SVc[nn]=-1;
       break;
  case Dlt_SA_OUT  :
       Delta_SAc[nn]=-1;
       break;
  case Dlt_SD_OUT  :
       Delta_SDc[nn]=-1;
       break;
  case Dlt_CTRL_OUT  :
    // Delta_Ctrl_c[nn]=-1;
       break;
  case Dlt_RES_ERR  :
  case Dlt_SET_MODE :
  case Dlt_SET_CTR :
       Delta_fn[nn]=fn;
       break;
 }
 if(Delta_inp_err[nn]>=Delta_max_inp_err)
 {
   Delta_inp_err[nn]=Delta_max_inp_err;
   f_icp_error(&Delta[nn],RD_ERR );
 }
 f_queue_free(ii,&Delta_Rqst[nn]);
}
/* ---------------------------------------- */
// ----------------------------------------
