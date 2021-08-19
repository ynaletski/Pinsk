
char ScrBuf[9][31];   /* модифицируемое значение буфера экрана */
char ScrBuf_c[9][31]; /* копия состояния экрана */

/* таблица перекодировки клавиш клавиатуры  MMI */
int mmikeycode[]={

//  код вводимый    код из      клавиша
//  по MmiGetch()     MMI     клавиатуры

     ' '  ,       /*   0         ----        */
     '9'  ,       /*   1           9         */
     '\r' ,       /*   2         Enter       */
     '0'  ,       /*   3           0         */
     '@'  ,       /*   4          F1         */
     '#'  ,       /*   5          F2         */
     'Q'  ,       /*   6          ESC        */
     '$'  ,       /*   7          F3         */
     '5'  ,       /*   8           5         */
     '6'  ,       /*   9           6         */
     '8'  ,       /*   A           8         */
     '7'  ,       /*   B           7         */
     '1'  ,       /*   C           1         */
     '2'  ,       /*   D           2         */
     '4'  ,       /*   E           4         */
     '3'  ,       /*   F           3         */

     ' '  ,       /*   10        -----       */
     '-'  ,       /*   11          -         */
     ' '  ,       /*   12       Shif_Enter   */
     'l'  ,       /*   13      стрелка вниз  */
     'W'  ,       /*   14        Shif_F1     */
     'E'  ,       /*   15        Shif_F2     */
     'q'  ,       /*   16        Shif_ESC    */
     'R'  ,       /*   17        Shif_F3     */
     '+'  ,       /*   18          +         */
     '<'  ,       /*   19    стрелка влево   */
     '>'  ,       /*   1A    стрелка вправо  */
     '.'  ,       /*   1B          . (точка) */
     '*'  ,       /*   1C          *         */
     '/'  ,       /*   1D          /         */
     '\b' ,       /*   1E         DEL        */
     '!'  ,       /*   1F     стрелка вверх  */

     ' '  ,       /*   20         -----      */
     '9'  ,       /*   21         -----      */
     '\r' ,       /*   22         -----      */
     '0'  ,       /*   23         -----      */
     '@'  ,       /*   24         -----      */
     '#'  ,       /*   25         -----      */
     '~'  ,       /*   26         -----      */
     '$'  ,       /*   27         -----      */
     '5'  ,       /*   28         -----      */
     '6'  ,       /*   29         -----      */
     '8'  ,       /*   2A         -----      */
     '7'  ,       /*   2B         -----      */
     '1'  ,       /*   2C         -----      */
     '2'  ,       /*   2D         -----      */
     '4'  ,       /*   2E         -----      */
     '3'  ,       /*   2F         -----      */
};

int mmikey[KEY_SIZE];

//--------------------
int MmiKbhit(void)
{
  return ((mmi_key_in-mmi_key_out) & KEY_SIZ1);
}
//--------------------
int MmiGetch(void)
{ int key;
  if(MmiKbhit())
  {
    key=mmikey[mmi_key_out++];
    mmi_key_out &= KEY_SIZ1;
    return key;
  }
  else return -1;
}
//--------------------

int mmi_sw=0;
int fl_mmi_new=1;
int fl_mmi_new1=0;

int page_MMI=6;      /* модифицируемое значение страницы MMI */
int page_MMI_c=-1;   /* копия состояния страницы MMI  */

void f_MMI(void)
{
/*  отображает экранный буфер
 и вводит в буфер клавиатуры нажатые на клавиатуре клавиши.
 Анализ клавиш см. MmiKbhit(),MmiGetch().
*/
 char *buf,*buf_c;
 if( fn_MMI != MMI_KBD) goto m2;

 if(page_MMI != page_MMI_c)
  {
   f_clr_scr_c_MMI(); // заполнение буфера копии пробелами
                      // т.е. фактическое состояние экрана после смены страницы
//printf("\r\n MMI page %d",page_MMI);
    fn_MMI=MMI_PG;
    page_MMI_c=page_MMI;
    fl_mmi_new=1;
    fl_mmi_new1=0;
    goto m2;
  }

 if((fl_mmi_new != 0) && (fl_mmi_new1==0) )
 {
 /* найти и отобразить изменения  с самого начала буфера */
   fl_mmi_new1=1;
   fl_mmi_new=0;
   mmix=0;
   mmiy=0;
   mmi_sw=0;
 }
 if(fl_mmi_new1==0) goto m2;

 switch(mmi_sw)
 {
 /*-------------------------*/
 // Сравнение буфера экрана и вывод в MMI при наличии расхождений

  case 0:
mm1:
   mmix=0;
   buf=&ScrBuf[mmiy][0];
   buf_c=&ScrBuf_c[mmiy][0];
   mmi_sw=1;
   if( _fmemcmp( buf, buf_c , 15) != 0)
   { //  найдены различия

mm_out:
    _fmemcpy(buf_c,buf,(long int)15);
    _fmemcpy(MMI_str,buf,(long int)15);
    MMI_str[15]=0;
    fn_MMI=MMI_ST;
    break;
   }
  /*----------------------------*/
  case 1:

   mmix=15;
   buf=&ScrBuf[mmiy][15];
   buf_c=&ScrBuf_c[mmiy][15];

   if( _fmemcmp( buf, buf_c , 15) != 0)
   { //  найдены различия
    mmix=15;
    mmi_sw=2;
    goto mm_out;
   }
 /*-------------------------*/
  case 2:
    if(++mmiy > 7)
    {
     fl_mmi_new1=0;
     mmiy=0;  mmix=0;
     break;
    }
    goto  mm1;

  default:
       break;
 }
 m2:
 SendToMMI();
}

/*=====================*/
/* ================================================== */
void f_clr_scr_MMI()
{  // очистка буфера экрана
  _fmemset( ScrBuf[0],' ',sizeof(ScrBuf)); // очистка буфера экрана
//_fmemset( ScrBuf[0],' ',248); // очистка буфера экрана
   fl_mmi_new=1;
}
/* ================================================== */
void f_clr_scr_c_MMI()
{  // очистка копии экранного буфера
   // в MMI будут переданы строки отличные от пробелов
  _fmemset( ScrBuf_c[0],' ',sizeof(ScrBuf));
   fl_mmi_new=1;
}
/* ================================================== */
void f_rev_scr_MMI()
{  // обновление экрана (после смены страницы,например)
   // вызывает передачу всего экранного буфера в MMI
  _fmemset( ScrBuf_c[0],1,sizeof(ScrBuf)); // заполнение буфера копии неиспоьзуемым значением
   fl_mmi_new=1;
}
/* ================================================== */
int x_mmi=0,y_mmi=0;
void MoveToXY(int x,int y)
{
 x_mmi=x;y_mmi=y;
}
/* ================================================== */
int mmix=0,mmiy=0;
void MmiShowCursor(void)
{
  MmiPutch('_');
  x_mmi--;
}
/* ================================================== */
void MmiCursorOff(void)
{
  MmiPutch(' ');
  x_mmi--;
}
/* ================================================== */
void MmiGotoxy(int x,int y)
{
  x_mmi=x;
  y_mmi=y;
  MmiShowCursor();
}
/* ================================================== */
void MmiPutch(char ch)
{
   ScrBuf[y_mmi][x_mmi]=ch;
   if(x_mmi++ > 29) x_mmi=29;
   fl_mmi_new=1;
}
/* ================================================== */
void MmiPuts(char *str)
{
int i;
long int itmp;
   i=strlen(str);
   itmp=30 -x_mmi;
   if(i<itmp) itmp=i;
//printf("\n  MmiPuts( %s )",str);
  if(itmp != 0)
   _fmemcpy(&ScrBuf[y_mmi][x_mmi],str, itmp);
   x_mmi+=itmp;
   fl_mmi_new=1;
}
/* ================================================== */
char BufferMmiPrintf[128];
int  MmiPrintf(char *format , ...)
{
  va_list marker;
  va_start( marker, format);
  vsprintf(BufferMmiPrintf, format, marker);
  MmiPuts(BufferMmiPrintf);
  return (strlen(BufferMmiPrintf));
}
/* ================================================== */
void SetDisplayPage(int page)
{
  page_MMI=page;
  page_MMI_c=-1;
}
/* ================================================== */
int fn_MMI=0;
char MMI_str[40];

#define PoolKbd_MMI    (250)
#define ToutAns_MMI    200
//-------------------------------
struct s_icp_dev MMI=
{
// device 1
1,                // status
1,                // port
00,               // addr
"MMIFCT",         // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0,                // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
PoolKbd_MMI,      // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&MMI_Rqst,        // *request
};
//-------------------------------
struct COM_rqst MMI_Rqst={
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com(int ii);
f_MMI_SetAns,          //  answ_com(int ii);
f_MMI_Flt,             //  answ_flt(int ii);
"MMI",                 //  name[10]
0,                     //  n_dev
&MMI,                  //  *ICP_dd
0,                     //  текущая функция
ToutAns_MMI,           //  timeout ответа
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
// величина таймаута в мс для получения ответа на команду выведенную в порт
unsigned long ToutAns[5]= {200,200,200,200,200};
unsigned int flag_fn=0;
int cnt_flt_MMI=0;
void SendToMMI()
/*
 Функция с интервалом PoolKbd_MMI шлет команды чтения  состояния клавиатуры (fn_MMI==1).
 При наличии запроса, устанавливает требуемую страницу дисплея 'page' (fn_MMI==2)
 или выводит символ(ы) из строки  MMI_str в позицию  mmiy, mmix (fn_MMI==3).
*/
{
  int itmp;
  unsigned char Cmd[n_buf_queue];

  if( ((fn_MMI==MMI_PG )|| (fn_MMI==MMI_ST) ||
    f_timer(MMI.time_stamp_pool,MMI.pool_time)  )
     &&  f_queue_chk(MMI.port)  )
  {
 // есть что выводить и есть место в очереди запросов

      if(MMI_Rqst.status == Req_Flt)
      {
      // предыдущая посылка завершилась со сбоем,
      // стать в очередь доступа к COM с повтором команды
      // оставшейся в структуре MMI_Rqst, если это была
      // команда установки страницы или посылки символа(ов)
         if( MMI_Rqst.function == MMI_KBD )
         {

           f_mmi_err();
           goto m_new_send;
         }
         if( cnt_flt_MMI++ < 4)
         { // трехкратный повтор сбойной команды
            MMI_Rqst.time_stamp=TimeStamp;
            MMI.n_transaction++;
            f_queue_put(MMI.port, &MMI_Rqst);
// printf("\r\nN:MMI_Rqst.Cmd=%s",MMI_Rqst.Cmd);
         }
         else
         {
 //    printf("\r\n MMI fault");
             cnt_flt_MMI=0;
             f_mmi_err();
             MMI_Rqst.status = Req_OK;
             return;
         }
      }
      else if(MMI_Rqst.status == Req_OK )
       {
        cnt_flt_MMI=0;
        goto m_new_send;
       }
  }
  return;

m_new_send:
 if(  f_timer(MMI.time_stamp_pool,MMI.pool_time)  )
  {
    if((fn_MMI == MMI_PG) ||(fn_MMI == MMI_ST))
    {
     flag_fn++;
     if(flag_fn & 1) goto m_sw;
    }
    else fn_MMI=MMI_KBD;
    flag_fn=0;
              /* опрос клавиатуры*/
     sprintf(Cmd,"$%02XK",MMI.addr);
     MMI_Rqst.answ_com=f_MMI_KBD_ans;
     MMI_Rqst.function=MMI_KBD;
     MMI.time_stamp_pool=TimeStamp;
     LedToggle();  // мигание светодиодом на процессоре
     goto m11;
  }

m_sw:
 switch(fn_MMI )
 {
  case MMI_PG:

        /* установка страницы дисплея*/
           sprintf(Cmd,"$%02XP%02X",MMI.addr,page_MMI);
           fn_MMI=MMI_KBD;
           MMI_Rqst.answ_com=f_MMI_SetAns;
           MMI_Rqst.function=MMI_PG;
           break;

  case MMI_ST:

      /* вывод строки  MMI_str[] в позицию  mmiy, mmix */
           sprintf(Cmd,"$%02XT%d%02X%s",MMI.addr,mmiy,mmix,MMI_str);
           fn_MMI=MMI_KBD;
           MMI_Rqst.answ_com=f_MMI_SetAns;
           MMI_Rqst.function=MMI_ST;
           break;

   default:
        fn_MMI=MMI_KBD;
    //  MMI_Rqst.function=MMI_KBD;
        return;
 }
m11:
//printf("\r\n0:MMI_Rqst.Cmd=%s",MMI_Rqst.Cmd);
   if(f_prepare7000(Cmd,MMI_Rqst.CRC_flag)) return;
   strcpy(MMI_Rqst.Cmd,Cmd);

   MMI_Rqst.cmd_lgth=strlen(Cmd);
   MMI_Rqst.time_stamp=TimeStamp;
   MMI.n_transaction++;

   f_queue_put(MMI.port, &MMI_Rqst);
   return;
}
/* ================================================== */

void f_mmi_err()
{
  page_MMI_c=-1;
}
/* ---------------------------------------- */
void f_MMI_Flt(int ii)
{
// Произошел таймаут при приеме ответа.
// Очистка очереди и статистика.
 MMI.time_stamp_rcv=0;
 MMI.n_timeout_error++;
 /*
 MMI_inp_err++;
   if(MMI_inp_err>=MMI_max_inp_err)
   {
     MMI_inp_err=MMI_max_inp_err;
 //    f_icp_error(&MMI,RD_ERR );
   }
 */
 // f_mmi_err();
 f_queue_free(ii,&MMI_Rqst);
 MMI_Rqst.status=Req_Flt;
}
/* ---------------------------------------- */
void f_MMI_SetAns(int ii)
{
 // Обработка ответа от MMI на команды изменения страницы
 // и записи символа(ов).Никаких действий не требуется.
 // Освобождение очереди и статистика результата.

 if(cb_COM[ii][0]=='!')
   {
     MMI.n_success++;
     MMI_out_err=0;
     f_queue_free(ii,&MMI_Rqst);
   }
 else
   {
//     f_mmi_err();
     MMI.n_CRC_error++;
/*
     MMI_out_err++;
     if(MMI_out_err>=MMI_max_out_err)
     {
       MMI_out_err=MMI_max_out_err;
//     f_icp_error(&MMI,WR_ERR );
     }
*/
    f_queue_free(ii,&MMI_Rqst);
    MMI_Rqst.status=Req_Flt;
   }
}
/* ---------------------------------------- */

int mmi_key_in=0,mmi_key_out=0;

void f_MMI_KBD_ans(int ii)
{
/* при наличии нажатых клавиш, вводит коды клавиш в кольцевой буфер  mmikey[]
   mmi_key_out - указавает на не использованный символ
   mmi_key_in  - указывает на пустую ячейку для ввода символа
   mmi_key_out==mmi_key_in - буфер пуст
*/

 char *Result;
 int j,key,nextidx;
  Result=cb_COM[ii];        // команда
//  count=n_bcom[ii];  // длина команды

 if(MMI.CRC_flag==0) goto m1;
 if(f_check_crc_ICP(Result))
 {
m1:
   if(Result[0]=='!')
   {
     j=4;
     while( (Result[j+2] & 0xff) != 0)
      {
          key=(ascii_to_hex(Result[j])<<4)+ascii_to_hex(Result[j+1]);
          key=mmikeycode[key];

          nextidx=(mmi_key_in+1) & KEY_SIZ1;
          if(nextidx != mmi_key_out)
          {
            mmikey[mmi_key_in]=key;
            mmi_key_in=nextidx;
          }
          else  break;   // переполнение буфера
          j+=2;
      }
      MMI.n_success++;
      MMI_inp_err=0;
   }
   else  goto m_err;
 }
 else
 {
m_err:
   MMI.n_CRC_error++;
 /*
   MMI_inp_err++;
   if(MMI_inp_err>=MMI_max_inp_err)
   {
     MMI_inp_err=MMI_max_inp_err;
//   f_icp_error(&MMI,RD_ERR );
   }
 */
 }
 f_queue_free(ii,&MMI_Rqst);
}
/* ---------------------------------------- */
int sw_fst=0;
char fst_str[31];
int  fst_n=1;
int  fst_in_n=0;
int fst_beg;
char str_tmp[40];

int fstin()
/* выводит содержимое строки fst_str  на экран MMI  и редактирует ее ,
   результат в той же строке.

   fst_str - строка;
   fst_n длина поля редактирования.

   пример использования :

     sprintf( fst_str,"%f",num_float);
     fst_n=4;
     sw_fst=1;
     MmiGotoxy(4,6);

    ввод завершен , если sw_fst==10;
*/
{
 int i,itmp,itmp1;
 int  key;

 switch(sw_fst)
 {
 case 0: break;
 case fst_OK: break;    //  нажат 'Enter' - ввод данных завершен
 case fst_ESC: break;  // нажата 'ESC' - выход без ввода данных
 case 1:
 m1:
  fst_in_n=0;
//if ( fst_str[0] )
      {
        strcpy(str_tmp,fst_str);
        fst_beg=x_mmi;
        MmiPuts(fst_str);
        MmiShowCursor();
        x_mmi=fst_beg;
      }
      sw_fst=2;

 case 2:

   if( MmiKbhit()) key= MmiGetch();
   else break;

   if( key == ENTER )
   {
    sw_fst=fst_OK;
    break;
   }
   else if( key == ESC )
   {
    sw_fst=fst_ESC;
    break;
   }

 /* стирание до конца строки */

     itmp=strlen(fst_str);
     if(itmp<fst_n) itmp=fst_n;
     for(i=0;i<itmp;i++) MmiPutch(' ');
     x_mmi=fst_beg;
     MmiShowCursor();

     sw_fst=3;
     fst_in_n=0;
     goto m3;
 case 3:
   if( MmiKbhit()) key= MmiGetch();
   else break;
 m3:
   if( isdigit(key)  || (key == '.') || ( ( key == '-') && (fst_in_n==0))  )
   {

    if(fst_in_n <  (fst_n))
    {
     fst_str[fst_in_n]=key;
     MmiPutch(key);
     fst_in_n+=1;
     if(fst_in_n < fst_n)
       MmiShowCursor();
    }
    else
    {
      fst_str[fst_in_n-1]=key;
      x_mmi--;
      MmiPutch(key);
    }
   }
   else if((key == DEL) && (x_mmi > fst_beg)  )
   {
    MmiCursorOff();
    x_mmi--;
    MmiPutch(' ');
    x_mmi--;
    fst_in_n--;
    MmiShowCursor();
   }
   else if(key == ENTER)
   {
    fst_str[fst_in_n]=0;
    sw_fst=fst_OK;
    if(fst_in_n < fst_n) MmiCursorOff();
    break;
   }
   else if(key == ESC)
   {
     strcpy(fst_str,str_tmp);
     itmp=strlen(fst_str);
     if(itmp<fst_n) itmp=fst_n;
     x_mmi=fst_beg;
     for(i=0;i<itmp;i++) MmiPutch(' ');
     x_mmi=fst_beg;
     goto m1;
   }
 }
 return sw_fst;
}
/*================================================================*/

void PrintMmiOp(struct dis_set_MMI *ds_s,int flag_prn)
{

 if(ds_s->descr == NULL) return;
 else
  switch (ds_s->type)
  {
          case T_FLOAT:
    sprintf(fst_str,"%g", *(float*)ds_s->op );
    chg_old=*(float*)ds_s->op;
             break;
          case T_INT:
    sprintf(fst_str,"%d", *(int*)ds_s->op );
    chg_old=*(int*)ds_s->op;
             break;
          case T_INT_L:
    sprintf(fst_str,"%ld", *(long int*)ds_s->op );
    chg_old=*(long int*)ds_s->op;
             break;
          case T_HEX:
    sprintf(fst_str,"%X", *(int*)ds_s->op );
    chg_old=*(int*)ds_s->op;
             break;
          case T_HX2:
    sprintf(fst_str,"%02X", *(int*)ds_s->op );
    chg_old=*(int*)ds_s->op;
             break;
          case T_HX4:
    sprintf(fst_str,"%04X", *(int*)ds_s->op );
    chg_old=*(int*)ds_s->op;
             break;
          case T_STR:
    sprintf(fst_str,"%s", (char*)ds_s->op );
             break;
          case T_DBL:
    sprintf(fst_str,"%lg", *(double*)ds_s->op );
    chg_old=*(double*)ds_s->op;
             break;
  }
  if(flag_prn) MmiPuts(fst_str);
}

/*================================================================*/

void ScanfMmiOp(struct dis_set_MMI *ds_s)
{
int itmp;
long int ltmp;
float ftmp;
double dtmp;

 if(ds_s->descr == NULL) return;
 else
  switch (ds_s->type)
  {
          case T_STR:
     sscanf(fst_str,"%s", (char*)ds_s->op );
     chg_new=*(char*)ds_s->op ;
     return;
          case T_FLOAT:
     sscanf(fst_str,"%g", &ftmp );
      if(ftmp >ds_s->high_l)    *(float*)ds_s->op=ds_s->high_l;
      else if( ftmp <ds_s->low_l) *(float*)ds_s->op=ds_s->low_l;
      else *(float*)ds_s->op=ftmp;
      chg_new=*(float*)ds_s->op;
             break;
          case T_INT:
     sscanf(fst_str,"%d", &itmp );
      if(itmp >ds_s->high_l)     *(int*)ds_s->op=ds_s->high_l;
      else if(itmp <ds_s->low_l) *(int*)ds_s->op=ds_s->low_l;
      else *(int*)ds_s->op=itmp;
      chg_new=*(int*)ds_s->op;
             break;
          case T_INT_L:
     sscanf(fst_str,"%ld", &ltmp );
      if(ltmp >ds_s->high_l)     *(long int*)ds_s->op=ds_s->high_l;
      else if(ltmp <ds_s->low_l) *(long int*)ds_s->op=ds_s->low_l;
      else *(long int*)ds_s->op=ltmp;
      chg_new=*(long int*)ds_s->op;
             break;
          case T_HEX:
     sscanf(fst_str,"%X", &itmp );
      if(itmp >ds_s->high_l)     *(int*)ds_s->op=ds_s->high_l;
      else if(itmp <ds_s->low_l) *(int*)ds_s->op=ds_s->low_l;
      else *(int*)ds_s->op=itmp;
      chg_new=*(int*)ds_s->op;
             break;
          case T_HX2:
     sscanf(fst_str,"%02X", itmp );
      if(itmp >ds_s->high_l)     *(int*)ds_s->op=ds_s->high_l;
      else if(itmp <ds_s->low_l) *(int*)ds_s->op=ds_s->low_l;
      else *(int*)ds_s->op=itmp;
      chg_new=*(int*)ds_s->op;
             break;

          case T_HX4:
     sscanf(fst_str,"%04X", &itmp );
      if(itmp >ds_s->high_l)     *(int*)ds_s->op=ds_s->high_l;
      else if(itmp <ds_s->low_l) *(int*)ds_s->op=ds_s->low_l;
      else *(int*)ds_s->op=itmp;
      chg_new=*(int*)ds_s->op;
             break;
          case T_DBL:
     sscanf(fst_str,"%lg", &dtmp);
      if(dtmp >ds_s->high_l)     *(double*)ds_s->op=ds_s->high_l;
      else if(dtmp <ds_s->low_l) *(double*)ds_s->op=ds_s->low_l;
      else *(double*)ds_s->op=dtmp;
      chg_new=*(double*)ds_s->op;
      break;
  }
}
/*================================================================*/

