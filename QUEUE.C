
// очереди доступа к COM портам

struct COM_q COM_que[que_wdth];

//----------------------------
void f_queue_init()
{  // init после включения питания

//printf("\n f_queue_init");
 _fmemset( &COM_que[0].busy_ptr,0, sizeof( COM_que) );
}
//----------------------------
unsigned long int TimeClear=0;

void f_queue_clr( int port)
{  // очистка очереди одного порта
   // устройства должны будут заново стать в очередь
   // обнаружив аварийное завершение запроса
int i;
  struct s_icp_dev *ICPl_DEV;

  for(i=COM_que[port].empt_ptr;i != COM_que[port].busy_ptr;)
  {
    if(COM_que[port].request[i] != NULL)
     {
        // вызов функций аварийного завершения запроса
       (*COM_que[port].request[i]->answ_flt)(port);

        // установка статуса аварии в обработке запроса
        COM_que[port].request[i]->status=Req_Flt;
     }

    i=(i+1) & que_lgth1;
  }
 _fmemset( &COM_que[port].busy_ptr,0, sizeof( struct COM_q) );
    for(i=0;i<5;i++)
     last_out[i]=-1;

    for(i=0;i<icp_lst_max;i++)
    {
      ICPl_DEV=ICP_dd[i];
      ICPl_DEV->n_transaction =0;
      ICPl_DEV->n_success     =0;
      ICPl_DEV->n_timeout_error =0;
      ICPl_DEV->n_CRC_error     =0;
    }
  TimeClear=TimeStamp;
}

//==========================================================
int f_queue_put(int port, struct COM_rqst *request)
{ // постановка в очередь запросов на обращение к COM
int itmp;
//printf("\n f_queue_put");
  itmp= (COM_que[port].busy_ptr+1) & que_lgth1;

  if( itmp == COM_que[port].empt_ptr ) return 1;// очередь переполнена

   if(request->status==Req_Que) return 1;
   request->status=Req_Que;
   COM_que[port].request[COM_que[port].busy_ptr]=request;
   COM_que[port].busy_ptr=itmp;

   return 0;
}
//==========================================================
int f_queue_chk(int port )
{  // проверка  очереди на переполнение
   // если есть место для ввода  - возвращает 1
   // иначе 0

  int itmp;


// printf("\n f_queue_chk");
  itmp= (COM_que[port].busy_ptr+1) & que_lgth1;

  if( itmp == COM_que[port].empt_ptr ) return 0;// очередь переполнена
  else
    return 1;
}
//==========================================================

int f_queue_free(int port, struct COM_rqst *request)
{  // запрос обслужен, продвижение очереди

//printf("\n f_queue_free");

  if( COM_que[port].request[COM_que[port].empt_ptr] == request)
  {
//    COM_que[port].request[COM_que[port].empt_ptr] = NULL;
    COM_que[port].empt_ptr = (COM_que[port].empt_ptr+1) & que_lgth1;

    if(request != NULL) request->status =0;
    return 0;
  }
  else return 1;
}
//==========================================================

int f_queue_Flt(int port)
{  // запрос обслужен аварийно, продвижение очереди

    struct COM_rqst *request;

//printf("\n f_queue_Flt");

    request=COM_que[port].request[COM_que[port].empt_ptr];
//    COM_que[port].request[COM_que[port].empt_ptr] = NULL;
    COM_que[port].empt_ptr = (COM_que[port].empt_ptr+1) & que_lgth1;
    if(request != NULL) request->status =10;

    return 0;
}

//==========================================================

