
#define ICP_7041

// Поддерживает устройства:
// I-7041 ,

// значения i7041_seq установить согласно используемым функциям

#define max_7041 2
//---------------
struct s_icp_dev I7041[max_7041]=
{
// device 1
0,                // status
1,                // port
9,                // addr
"I7041",         // name[8]
06,               // baudrate_cod
0x40,             // type
0x41,             // format
4,                // channels for output: 4 - 7041,8 - 7044 , 12-7042, 16 - 7043
                  //   0 : 7041
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&I7041_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
1,                // port
9,                // addr
"7041/2",         // name[8]
6,                // baudrate_cod
0x40,             // type
0x41,             // format
4,                // channels for output: 4 - 7041,8 - 7044 , 12-7042, 16 - 7043
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
150,              // pool_time
0,                // time_stamp_pool
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&I7041_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst I7041_Rqst[max_7041]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_7041_rd,             //  answ_com
f_7041_Flt,            //  answ_flt
"7041/1",              //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&I7041[0],             //  *ICP_dd
RD_STATUS,             //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_7041_rd,             //  answ_com
f_7041_Flt,            //  answ_flt
"7041/2",              //  name
1,                     // n_dev номер устройства в группе однотипных устройств
&I7041[1],             //  *ICP_dd
RD_STATUS,             //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

int i7041_inp[max_7041];  // состояние входов

int i7041_inpl[max_7041]; // состояние защелок 1->0
int i7041_inph[max_7041]; // состояние защелок 0->1

int i7041_fn[max_7041]={0,0};  // регистры функций драйвера

//C_LATCH   0x30       очистка защелок
//C_WDOG    0x40       очистка флага сброса по WD
//S_WDOG    0x50       установка WD с парам. I7041_wd_E[],I7041_wd_VV[]

// Если записать значение функции в регистр функции , например:
//  i7041_fn[0]= C_WDOG;  драйвер выполнит заданную функцию - в данном случае
// пошлет в устройство 7041/1 команду очистки флага срабатывания Watch Dog.
// После постановки запроса с установленной функцией в очередь на вывод в COM,
// регистр функции очищается драйвером.

int i7041_sw[max_7041]={0,0};

unsigned int i7041_out_err[max_7041]={0,0};  // счетчик ошибок вывода
unsigned int i7041_inp_err[max_7041]={0,0};  // счетчик ошибок ввода

int i7041_seq[max_7041][16]=
{
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // последовательность функций опроса Dev1
// 2,0,1,0,2,0,0,0,1,0,2,0,0,0,1,0,    // последовательность функций опроса Dev1
 2,0,1,0,2,0,0,0,1,0,2,0,0,0,1,0,};  // последовательность функций опроса Dev2
         //    0:  // чтение входов-выходов
         //    1:  // чтение защелки L  ,не использовать для  7042,7043,7066,7067
         //    2:  // чтение защелки H  ,не использовать для  7042,7043,7066,7067

int I7041_fl[max_7041]={0,0};

void f_7041(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных из устройства ICP 7041
  //  ( '@aa') . Период посылки I7041[ii].pool_time мс.
  // В ответ будет передана информация о  4-х входных сигналах
  // и 4 выходных сигналах
  //   Если в i7041_fn[] записана функция != 0 - функция выполняется
  //  в первую очередь.
  //
  //

int i,fn,sw;
char buf_tmp[20];

 if(I7041[ii].status == 0) return;

 if(i7041_fn[ii] != 0)
    // C_LATCH S_WDOG C_WDOG
      fn=i7041_fn[ii];
 else if( f_timer(I7041[ii].time_stamp_pool,I7041[ii].pool_time ))
      fn=RD_STATUS;
 else return;

  if((I7041_Rqst[ii].status == Req_Flt) ||
     (I7041_Rqst[ii].status == Req_OK)) goto m1;
//  else if( f_timer(I7041[ii].time_stamp_pool,(I7041[ii].pool_time+I7041[ii].pool_time) ))
//           goto m1;
  return;

m1:
          if(f_queue_chk(I7041[ii].port)== 0) return;// очередь переполнена

          if(fn  == C_LATCH)
          {
            sprintf(buf_tmp,"$%02XC",I7041[ii].addr);
            i7041_fn[ii]=0;
            i7041_inpl[ii]=0;
            i7041_inph[ii]=0;
          }
          else if(fn == RD_STATUS)  // чтение входов-выходов
          {// формирует команды чтения входов/выходов или защелок H,L
             i7041_sw[ii]=(i7041_sw[ii]+1) & 0xf;
             sw=i7041_seq[ii][i7041_sw[ii]];
             switch (sw)
             {
              case 0:  // чтение входов-выходов
           //   printf( "\n\r W,%d",ii );
                sprintf(buf_tmp,"@%02X",I7041[ii].addr);
                break;
              case 1:  // чтение защелки L
                sprintf(buf_tmp,"$%02XL0",I7041[ii].addr);
                break;
              case 2:  // чтение защелки H
                sprintf(buf_tmp,"$%02XL1",I7041[ii].addr);
                break;
              default:
                return;
             }
             fn+=sw;
          }
          else return;

           I7041_Rqst[ii].function = fn;
           if(f_prepare7000(buf_tmp,I7041[ii].CRC_flag)) return;
           strcpy(I7041_Rqst[ii].Cmd,buf_tmp);
           I7041_Rqst[ii].cmd_lgth=strlen(buf_tmp);
           I7041_Rqst[ii].time_stamp=TimeStamp;
           I7041[ii].time_stamp_pool=TimeStamp;
           I7041[ii].n_transaction++;
           f_queue_put(I7041[ii].port, &I7041_Rqst[ii]) ;
}
/*----------------------------*/
unsigned long int i7041it[max_7041];
int itmp7041;

void f_7041_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные     -  i7041_inp []
//                                             i7041_inpl[]
//                                             i7041_inph[]
//
 struct COM_rqst *request;
 int i,nn,fn;
 int *dat_in;
 int *dat_out_c;
 int *dat_out;
 int dat_i[8];

// long int count;
// count=n_bcom[ii]-2;  // длина имени

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства 7041 (0,1..)

  fn=request->function;
  if(( fn & 0xf0)==RD_STATUS) // ответ на команду опроса состояния
  {  // ввод состояний входов-выходов
     if(I7041[nn].CRC_flag==0) goto m1;
     if(f_check_crc_ICP(cb_COM[ii]))
     {
m1:
      fn &= 0xf;
      switch(  fn )
      {
      case 0:  // чтение входов-выходов
    //в ответ на @AA приходит:
    //  >0F00
         if(cb_COM[ii][0] != '>') goto m_err;

         sscanf(cb_COM[ii],">%4X", &i7041_inp[nn]);
         INP_VAR = i7041_inp[0];
 //        INP_VARi = INP_VAR ^ INP_inv;

         I7041[nn].n_success++;
         i7041_inp_err[nn]=0;
         break;
      case 1:    // чтение защелки L
    //в ответ на $AAL0  приходит:
    //  ! 0F 0F 00 6D
         if(cb_COM[ii][0] != '!') goto m_err;

         sscanf(cb_COM[ii],"!%4X", &i7041_inpl[nn]);

         I7041[nn].n_success++;
         i7041_inp_err[nn]=0;
         break;

      case 2:    // чтение защелки H
    //в ответ на $AAL1  приходит:
    //  ! 0F 0F 00 6D
         if(cb_COM[ii][0] != '!') goto m_err;
         sscanf(cb_COM[ii],"!%4X", &i7041_inph[nn]);
         I7041[nn].n_success++;
         i7041_inp_err[nn]=0;
         break;
      case 3:    // чтение флага срабатывания WD
    //в ответ на ~AA0  приходит:
    //  ! AA 04 00 6D   , 04 - сработал WD
    //  ! 09 04 EE
         if(cb_COM[ii][0] != '!') goto m_err;
         I7041[nn].n_success++;
         i7041_inp_err[nn]=0;
         break;
      }
     }
     else
     {
m_err:
       I7041[nn].n_CRC_error++;
       i7041_inp_err[nn]++;
         if(i7041_inp_err[nn]>=I7041_max_inp_err)
         {
           i7041_inp_err[nn]=I7041_max_inp_err;
           f_icp_error(&I7041[nn],RD_ERR );
         }
     }
  }
  else if(fn == C_LATCH)
  { // ответ на команду очистки защелки
       if(cb_COM[ii][0]=='!')
           I7041[nn].n_success++;
       else
         I7041[nn].n_CRC_error++;
  }
  else
         I7041[nn].n_CRC_error++;
    f_queue_free(ii,&I7041_Rqst[nn]);
}
/*-------------------------------------------*/
void f_7041_Flt(int ii)
{
 struct COM_rqst *request;
 int nn;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];

 nn=request->n_dev; // номер устройства 7041 (0,1..)

 I7041[nn].time_stamp_rcv=0;
 I7041[nn].n_timeout_error++;

 i7041_inp_err[nn]++;
 if(i7041_inp_err[nn]>=I7041_max_inp_err)
 {
   i7041_inp_err[nn]=I7041_max_inp_err;
   f_icp_error(&I7041[nn],RD_ERR );
 }
 f_queue_Flt(ii);
}
/* ---------------------------------------- */
