
#define ICP_7060

// Поддерживает устройства:
// I-7041 ,  I-7060,I-7063,I-7044,I-7050,I-7065,I-7066,I-7067,I-7042,I-7043
//  значение 'channels' сдедует установить:
//    0   ,     4     4       8       8      8      8      8    16    16
// значение 'name' следует установить соглано типу модуля

// значения i7060_seq установить согласно используемым функциям

#define max_7060 2
//---------------
struct s_icp_dev I7060[max_7060]=
{
// device 1
0,                // status
1,                // port
5,                // addr
"7044",         // name[8]
06,               // baudrate_cod
0x40,             // type
0x41,             // format
4,                // channels for output: 4 - 7060,8 - 7044 , 12-7042, 16 - 7043
                  //   0 : 7041
"B2.6",           // firmware
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
&I7060_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
1,                // port
9,                // addr
"7044/2",         // name[8]
6,                // baudrate_cod
0x40,             // type
0x41,             // format
4,                // channels for output: 4 - 7060,8 - 7044 , 12-7042, 16 - 7043
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
&I7060_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst I7060_Rqst[max_7060]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_7060_rd,             //  answ_com
f_7060_Flt,            //  answ_flt
"7060/1",              //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&I7060[0],             //  *ICP_dd
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
f_7060_rd,             //  answ_com
f_7060_Flt,            //  answ_flt
"7060/2",              //  name
1,                     // n_dev номер устройства в группе однотипных устройств
&I7060[1],             //  *ICP_dd
RD_STATUS,             //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

int i7060_out[max_7060];  // требуемое состояние выходов 7060
int i7060_inp[max_7060];  // состояние входов

int i7060_inpl[max_7060]; // состояние защелок 1->0
int i7060_inph[max_7060]; // состояние защелок 0->1
int i7060_wdf[max_7060];  // флаг срабатывания WD  04 - сработал

int i7060_fn[max_7060]={0,0};  // регистры функций драйвера

//C_LATCH   0x30       очистка защелок
//C_WDOG    0x40       очистка флага сброса по WD
//S_WDOG    0x50       установка WD с парам. I7060_wd_E[],I7060_wd_VV[]

// Если записать значение функции в регистр функции , например:
//  i7060_fn[0]= C_WDOG;  драйвер выполнит заданную функцию - в данном случае
// пошлет в устройство 7060/1 команду очистки флага срабатывания Watch Dog.
// После постановки запроса с установленной функцией в очередь на вывод в COM,
// регистр функции очищается драйвером.

int I7060_wd_E[max_7060] ={1,1};
int I7060_wd_VV[max_7060]={20,20}; // 2 сек

int i7060_sw[max_7060]={0,0};

int i7060_outc[max_7060]; // текущее состояние выходов в 7060
int OUT_VAR_c_r=0;

unsigned int i7060_out_err[max_7060]={0,0};  // счетчик ошибок вывода
unsigned int i7060_inp_err[max_7060]={0,0};  // счетчик ошибок ввода

int i7060_seq[max_7060][16]=
/*
{2,0,1,0,2,0,3,0,1,0,2,0,3,0,1,0,    // последовательность функций опроса Dev1
 2,0,1,0,2,0,3,0,1,0,2,0,3,0,1,0,};  // последовательность функций опроса Dev2
*/

{0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,    // последовательность функций опроса Dev1
 0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,};  // последовательность функций опроса Dev2

         //    0:  // чтение входов-выходов
         //    1:  // чтение защелки L  ,не использовать для  7042,7043,7066,7067
         //    2:  // чтение защелки H  ,не использовать для  7042,7043,7066,7067
         //    3:  // чтение флага срабатывания WD ,не использовать для  7041

int I7060_fl[max_7060]={0,0};

void f_7060(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных из устройства ICP 7060
  //  ( '@aa') . Период посылки I7060[ii].pool_time мс.
  // В ответ будет передана информация о  4-х входных сигналах
  // и 4 выходных сигналах
  //  Если состояние выходных сигналов в устройстве 7060 (i7060_outc[ii])
  //  отличается от требуемого состояния i7060_out[ii] , будет сформирована
  //  команда записи.
  //   Если в i7060_fn[] записана функция != 0 - функция выполняется
  //  в первую очередь.
  //
  //

int i,fn,sw;
char buf_tmp[20];

 if(I7060[ii].status == 0) return;

 if( I7060_fl[ii] >= 0 )
            f_init_7060(ii);

 if(i7060_fn[ii] != 0)
    // C_LATCH S_WDOG C_WDOG
      fn=i7060_fn[ii];
 else if(i7060_out[ii] != i7060_outc[ii])
      fn=W_OUT;
 else if( f_timer(I7060[ii].time_stamp_pool,I7060[ii].pool_time ))
      fn=RD_STATUS;
 else return;

  if((I7060_Rqst[ii].status == Req_Flt) ||
     (I7060_Rqst[ii].status == Req_OK)) goto m1;
//  else if( f_timer(I7060[ii].time_stamp_pool,(I7060[ii].pool_time+I7060[ii].pool_time) ))
//           goto m1;
  return;

m1:
          if(f_queue_chk(I7060[ii].port)== 0) return;// очередь переполнена

          if(fn  == C_LATCH)
          {
            sprintf(buf_tmp,"$%02XC",I7060[ii].addr);
            i7060_fn[ii]=0;
            i7060_inpl[ii]=0;
            i7060_inph[ii]=0;
          }
          else if(fn  == C_WDOG)
          {
            sprintf(buf_tmp,"~%02X1",I7060[ii].addr);
            i7060_fn[ii]=0;
            i7060_wdf[ii]=0;
          }
          else if(fn  == S_WDOG)
          {
            sprintf(buf_tmp,"~%02X3%1X%2X",I7060[ii].addr,I7060_wd_E[ii],I7060_wd_VV[ii]);
            i7060_fn[ii]=0;
          }
          else if(fn == W_OUT) // вывод
          {

           if(I7060[ii].channels == 8)
            sprintf(buf_tmp,"@%02X%02X",I7060[ii].addr,i7060_out[ii] & 0xff);
           else  if(I7060[ii].channels == 16)
            sprintf(buf_tmp,"@%02X%04X",I7060[ii].addr,i7060_out[ii]);
           else // if(I7060[ii].channels == 4)
            sprintf(buf_tmp,"@%02X%01X",I7060[ii].addr,i7060_out[ii] & 0xf);

             i7060_outc[ii]=i7060_out[ii];
          }
          else if(fn == RD_STATUS)  // чтение входов-выходов
          {// формирует команды чтения входов/выходов или защелок H,L
             i7060_sw[ii]=(i7060_sw[ii]+1) & 0xf;
             sw=i7060_seq[ii][i7060_sw[ii]];
             switch (sw)
             {
              case 0:  // чтение входов-выходов
           //   printf( "\n\r W,%d",ii );
                sprintf(buf_tmp,"@%02X",I7060[ii].addr);
                break;
              case 1:  // чтение защелки L
                sprintf(buf_tmp,"$%02XL0",I7060[ii].addr);
                break;
              case 2:  // чтение защелки H
                sprintf(buf_tmp,"$%02XL1",I7060[ii].addr);
                break;
              case 3:  // чтение флага срабатывания WD
                sprintf(buf_tmp,"~%02X0",I7060[ii].addr);
                break;
              default:
                return;
             }
             fn+=sw;
          }
          else return;

           I7060_Rqst[ii].function = fn;
           if(f_prepare7000(buf_tmp,I7060[ii].CRC_flag)) return;
           strcpy(I7060_Rqst[ii].Cmd,buf_tmp);
           I7060_Rqst[ii].cmd_lgth=strlen(buf_tmp);
           I7060_Rqst[ii].time_stamp=TimeStamp;
           I7060[ii].time_stamp_pool=TimeStamp;
           I7060[ii].n_transaction++;
           f_queue_put(I7060[ii].port, &I7060_Rqst[ii]) ;
}
/*----------------------------*/
#define  sw_7060  I7060_fl
unsigned long int i7060it[max_7060];
void f_init_7060(int ii)
 {
   switch ( sw_7060[ii])
   {
    case 0:
       if(i7060_fn[ii] != 0) break;
       i7060_fn[ii]=C_WDOG;
       sw_7060[ii]=1;
       break;
    case 1:
       if(i7060_fn[ii] != 0) break;
       i7060_fn[ii]=S_WDOG;
       sw_7060[ii]=2;
       break;

    case 2:
       if(i7060_fn[ii] != 0) break;
       i7060it[ii]=TimeStamp;
       sw_7060[ii]=3;
       break;
    case 3:
 if( f_timer(i7060it[ii],300L) == 0) break;
       flag_fst_gun=0;
       I7060[ii].n_transaction    = 0;
       I7060[ii].n_success        = 0;
       I7060[ii].n_timeout_error  = 0;
       I7060[ii].n_CRC_error      = 0;

       sw_7060[ii]=-1;
       break;

    case -1:
       break;
    default:break;
   }
 }
int itmp7060;

unsigned int msk4[4]  ={0x0f,0xf0,0xf00,0xf000};
unsigned int n_sht4[4]={0,4,8,12};

unsigned int msk8[2]  ={0xff,0xff00};
unsigned int n_sht8[2]={0,8};




void f_7060_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные     -  i7060_inp []
//                                             i7060_inpl[]
//                                             i7060_inph[]
//
 struct COM_rqst *request;
 int i,nn,fn,itmp;
 int *dat_in;
 int *dat_out_c;
 int *dat_out;
 int dat_i[8];

// long int count;
// count=n_bcom[ii]-2;  // длина имени

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства 7060 (0,1..)

  fn=request->function;
  if(( fn & 0xf0)==RD_STATUS) // ответ на команду опроса состояния
  {  // ввод состояний входов-выходов
     if(I7060[nn].CRC_flag==0) goto m1;
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

 //   printf( "\n\r W,%d,CHAN=%d,",nn,I7060[nn].channels );
         if(I7060[nn].channels == 4)
         {

   //    printf("\n\r %2x,%d",i7060_inp[nn],nn   );
           sscanf(cb_COM[ii],">%2x%2x", &i7060_outc[nn],&i7060_inp[nn]);

           itmp=i7060_outc[nn];

           OUT_VAR_c_r &= ~msk8[nn];
           OUT_VAR_c_r |= (itmp << n_sht8[nn]) & msk8[nn];

//         INP_VAR &= ~msk4[nn];
//         INP_VAR |= (i7060_inp[nn] << n_sht4[nn]) & msk4[nn];
//         INP_VARi = INP_VAR ^ INP_inv;
         }
         else if(I7060[nn].channels == 8)
         {
//         sscanf(cb_COM[ii],">%2x%2x", &i7060_outc[nn],&i7060_inp[nn]);
//         INP_VAR = i7060_inp[0];
//         INP_VARi = INP_VAR ^ INP_inv;
           sscanf(cb_COM[ii],">%2x%2x", &i7060_outc[nn],&i7060_inp[nn]);

           itmp=i7060_outc[nn];

           OUT_VAR_c_r &= ~msk8[nn];
           OUT_VAR_c_r |= (itmp << n_sht8[nn]) & msk8[nn];

//         INP_VAR &= ~msk4[nn];
//         INP_VAR |= (i7060_inp[nn] << n_sht4[nn]) & msk4[nn];
//         INP_VARi = INP_VAR ^ INP_inv;
         }
         else if(I7060[nn].channels == 16)
         {
           sscanf(cb_COM[ii],">%4x", &i7060_outc[nn]);

           OUT_VAR_c_r=i7060_outc[0];

           INP_VAR = i7060_inp[0];
//           INP_VARi = INP_VAR ^ INP_inv;
         }
         if(I7060[nn].channels == 0)
           sscanf(cb_COM[ii],">%4x", &i7060_inp[nn]);

         I7060[nn].n_success++;
         i7060_inp_err[nn]=0;
         break;
      case 1:    // чтение защелки L
    //в ответ на $AAL0  приходит:
    //  ! 0F 0F 00 6D
         if(cb_COM[ii][0] != '!') goto m_err;

         if(I7060[nn].channels == 0)
           sscanf(cb_COM[ii],"!%4x", &i7060_inpl[nn]);
         else
           sscanf(cb_COM[ii],"!%2x%2x", &itmp7060,&i7060_inpl[nn]);

         I7060[nn].n_success++;
         i7060_inp_err[nn]=0;
         break;

      case 2:    // чтение защелки H
    //в ответ на $AAL1  приходит:
    //  ! 0F 0F 00 6D
         if(cb_COM[ii][0] != '!') goto m_err;
         if(I7060[ii].channels == 0)
           sscanf(cb_COM[ii],"!%4x", &i7060_inph[nn]);
         else
         sscanf(cb_COM[ii],"!%2x%2x", &itmp7060,&i7060_inph[nn]);
         I7060[nn].n_success++;
         i7060_inp_err[nn]=0;
         break;
      case 3:    // чтение флага срабатывания WD
    //в ответ на ~AA0  приходит:
    //  ! AA 04 00 6D   , 04 - сработал WD
    //  ! 09 04 EE
         if(cb_COM[ii][0] != '!') goto m_err;
         sscanf(cb_COM[ii],"!%2x%2x", &itmp7060,&i7060_wdf[nn]);
         if(i7060_wdf[nn] & 0x4)
            f_icp_error(&I7060[nn],WD_ERR );
         I7060[nn].n_success++;
         i7060_inp_err[nn]=0;
         break;
      }
     }
     else
     {
m_err:
       I7060[nn].n_CRC_error++;
       i7060_inp_err[nn]++;
         if(i7060_inp_err[nn]>=I7060_max_inp_err)
         {
           i7060_inp_err[nn]=I7060_max_inp_err;
           f_icp_error(&I7060[nn],RD_ERR );
         }
     }
  }
  else if(fn==W_OUT)
  { // ответ на команду записи
       if(cb_COM[ii][0]=='>')
       {
           I7060[nn].n_success++;
           i7060_out_err[nn]=0;
       }
       else
       {
         I7060[nn].n_CRC_error++;
         i7060_out_err[nn]++;
         if(i7060_out_err[nn]>=I7060_max_out_err)
         {
           i7060_out_err[nn]=I7060_max_out_err+1;
           f_icp_error(&I7060[nn],WR_ERR );
         }
       }
  }
  else if(fn == C_LATCH)
  { // ответ на команду очистки защелки
       if(cb_COM[ii][0]=='!')
           I7060[nn].n_success++;
       else
         I7060[nn].n_CRC_error++;
  }
  else
         I7060[nn].n_CRC_error++;
    f_queue_free(ii,&I7060_Rqst[nn]);
}
/*-------------------------------------------*/
void f_7060_Flt(int ii)
{
 struct COM_rqst *request;
 int nn;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];

 nn=request->n_dev; // номер устройства 7060 (0,1..)

// if(flag_prn)
// {
//   flag_prn=0;
//   puts(request->Cmd );
//   printf("\n n=%d\n",request->cmd_lgth);
// }

 I7060[nn].time_stamp_rcv=0;
 I7060[nn].n_timeout_error++;

 i7060_inp_err[nn]++;
 if(i7060_inp_err[nn]>=I7060_max_inp_err)
 {
   i7060_inp_err[nn]=I7060_max_inp_err;
   f_icp_error(&I7060[nn],RD_ERR );
 }
// i7060_outc[nn]=FAULT_COD;
 f_queue_Flt(ii);
}
/* ---------------------------------------- */
