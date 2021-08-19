
#define RTU_OFP

#define max_OFP 2
//---------------
int OFP_fl[max_OFP]={0.0};
//---------------
struct s_icp_dev OFP[max_OFP]=
{
// device 1
0,                // status
2,                // port
20,               // addr
"OFP",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B1.2",           // firmware
0,                // protocol
1,                // CRC_flag
600,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&OFP_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
21,               // addr
"OFP/2",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B1.2",           // firmware
0,                // protocol
1,                // CRC_flag
600,              // pool_time
0,                // time_stamp_pool
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&OFP_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst OFP_Rqst[max_OFP]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_OFP_rd,            //  answ_com
f_OFP_rd,            //  answ_flt
"OFP/1",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&OFP[0],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_OFP_rd,            //  answ_com
f_OFP_rd,            //  answ_flt
"OFP/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&OFP[1],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/
struct COM_rqst OFP_Rqst_T[max_OFP]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_OFP_rd_t,            //  answ_com
f_OFP_rd_t,            //  answ_flt
"OFP/1",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&OFP[0],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_OFP_rd_t,            //  answ_com
f_OFP_rd_t,            //  answ_flt
"OFP/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&OFP[1],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/
unsigned char OFP_RD[] ={1,3,0x00,0x09,0x00,0x01};  // I09 -  1 регистр
void f_OFP(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных из устройства RTU OFP
  //  Период посылки OFP[ii].pool_time мс.
  // В ответ будет передан регистр состояния контроллера переполнения

            // Младший байт
            //  D0...D5 - состояние секций , 1 - ok
            //  D7 - состояние PE , 1 - ok
            //  Старший байт: биты D0...D6  - логическое или
            // состояний входов секций 1...6
                 //
                 //   1 - OK
                 //   2 - Low level
                 //   4 - High level
                 //   8 - Fault
                 //  0x10 - нет PE
                 //  0x20 - нет ID
                 // дополнительно
                 //  0x40 - режим 2-х проводной
                 //  0x80 - режим 5-ти проводной


int i;
char buf_tmp[20];
int i1,i2;

 if(OFP[ii].status == 0) return;

 if( f_timer(OFP[ii].time_stamp_pool,OFP[ii].pool_time ))
 {
  if((OFP_Rqst[ii].status == Req_Flt) ||
     (OFP_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(OFP[ii].time_stamp_pool,(OFP[ii].pool_time+OFP[ii].pool_time) ))
           goto m1;
   return;
m1:
        i1=6;
        _fmemcpy(buf_tmp,OFP_RD,(long int) 6);

           buf_tmp[0]=OFP[ii].addr;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(OFP_Rqst[ii].Cmd,buf_tmp,(long)i2);
           OFP_Rqst[ii].cmd_lgth=i2;
           OFP_Rqst[ii].time_stamp=TimeStamp;
           OFP[ii].time_stamp_pool=TimeStamp;
           OFP[ii].n_transaction++;
           f_queue_put(OFP[ii].port, &OFP_Rqst[ii]);
 }
}
/*----------------------------*/
int i2_OFP=0;
unsigned char buf_tmp_OFP[20];
int f_OFP_WR(int ii,int addr,int i_val)
{
 // записывает значение в регистр OFP
 // ii - номер устройства
int i,sw,itmp;
long int i1;

// if(OFP[ii].status == 0) return 0;

  if((OFP_Rqst_T[ii].status == Req_Flt) ||
     (OFP_Rqst_T[ii].status == Req_OK));
  else return 0;

 if(f_queue_chk(OFP[ii].port)== 0) return 0;// очередь переполнена

           // запись данных Integer
            i1=6;
            buf_tmp_OFP[0]=OFP[ii].addr;
            buf_tmp_OFP[1]=6;
            buf_tmp_OFP[2]=((addr)>> 8) & 0xff;
            buf_tmp_OFP[3]= (addr) & 0xff;
            buf_tmp_OFP[4]=(i_val>> 8) & 0xff;
            buf_tmp_OFP[5]= i_val & 0xff;


           OFP_Rqst_T[ii].function = OFP_WR_I;
           i2_OFP=i1;
           f_prepareRTU(buf_tmp_OFP,&i2_OFP);
           _fmemcpy(OFP_Rqst_T[ii].Cmd,buf_tmp_OFP,(long)i2_OFP);
           OFP_Rqst_T[ii].cmd_lgth=i2_OFP;
           OFP_Rqst_T[ii].time_stamp=TimeStamp;
           OFP[ii].n_transaction++;
// QQQQQQ
           f_queue_put(OFP[ii].port, &OFP_Rqst_T[ii]) ;

           return 1;
}
/*----------------------------*/
unsigned int OFP_inp_err[max_OFP]={0,0};  // счетчик ошибок ввода
unsigned int OFP_t_err[max_OFP]={0,0};  // счетчик ошибок ввода

void f_OFP_rd_t(int ii)
{   // принят ответ на команду записи в MVD
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn,i1,im;
 int count,tmp;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // номер устройства OFP (0,1..)

 count=n_bcom[ii];  // длина команды
 if(count < 3)
   {
      goto m_tout_err;
   }
 if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
 if(cb_COM[ii][1] & 0x80)
   {
     goto m_end;
   }
    // ответ на команду записи
  if ((cb_COM[ii][1] == 6)  || (cb_COM[ii][1] == 16) ||(cb_COM[ii][1] == 5));
  else  goto m_crc_err;
 m_end:
  OFP[nn].n_success++;
  OFP_inp_err[nn]=0;
  f_queue_free(ii,&OFP_Rqst_T[nn]);
  OFP_t_err[nn]=0;
  return;

m_crc_err:
 OFP[nn].time_stamp_rcv=0;
 OFP[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 OFP[nn].time_stamp_rcv=0;
 OFP[nn].n_timeout_error++;

m_err:
 OFP_t_err[nn]++;

 f_queue_free(ii,&OFP_Rqst_T[nn]);
 if(OFP_t_err[nn]>=OFP_max_t_err)
 {
   OFP_t_err[nn]=OFP_max_t_err;
   f_icp_error(&OFP[nn],WR_ERR );
 }
 else
 {
   OFP_Rqst_T[nn].time_stamp=TimeStamp;
   f_queue_put(ii, &OFP_Rqst_T[nn]) ;
   OFP[nn].n_transaction++;

 }

}
/* ---------------------------------------- */
/*----------------------------*/


unsigned int INP_OFP=0;

void f_OFP_rd(int ii)
{
// ii - номер порта
//
 struct COM_rqst *request;
 int i,nn,itmp;

 int count;

 count=n_bcom[ii];  // длина команды
 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // номер устройства OFP (0,1..)

 if(count < 3)
   {
      goto m_tout_err;
   }
 if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
 if(cb_COM[ii][1] & 0x80)
   {
//     OFP[nn].exc=cb_COM[ii][2];
//     OFP_fn[nn]=F_FAULT ;
     goto m_end;
   }

   if(count < 5)
   {
       goto m_tout_err;
   }

  INP_OFP= f_get_int( &cb_COM[ii][0]); ;
//  INP_OFP[nn]= f_get_int( &cb_COM[ii][0]); ;

/*
  if(flag_prn)
  {
    printf("\n\r");
    for(i=0;i<count;i++)
     printf(" %02X",cb_COM[ii][i]&0xff);
  }
*/
 m_end:
  OFP[nn].n_success++;
  OFP_inp_err[nn]=0;
  f_queue_free(ii,&OFP_Rqst[nn]);
  return;

m_crc_err:
 OFP[nn].time_stamp_rcv=0;
 OFP[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 OFP[nn].time_stamp_rcv=0;
 OFP[nn].n_timeout_error++;

m_err:
 OFP_inp_err[nn]++;

 if(OFP_inp_err[nn]>=OFP_max_inp_err)
 {
   OFP_inp_err[nn]=OFP_max_inp_err;
   f_icp_error(&OFP[nn],RD_ERR );
 }
 f_queue_free(ii,&OFP_Rqst[nn]);
}

