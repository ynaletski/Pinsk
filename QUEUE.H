
#define que_lgth  32
#define que_wdth  5

#define que_lgth1  que_lgth-1
#define n_buf_queue   120


#define Req_OK   0
#define Req_Que  1
#define Req_Snd  2
#define Req_Flt  10

// структура с запросом на доступ к COM порту
struct COM_rqst
{

 int status;  // состояние запроса:
              // 0 - нет обслуживания ( обслуживание завершено успешно)
              // 1 - запрос поставлен в очередь
              // 2 - команда передана в канал связи
              // 10 - запрос завершился аварийно - таймаут в ответе или
              // очередь очищена

 int  (*gets_com)(int ii);  // Указатель на функцию , которая примет команду
                            // в буфер команды согласно протоколу адресуемого
                            //  устройства.
                            // Если команда не подразумевает ответа,
                            // Функция должна завершаться без приема символов.


 void (*answ_com)(int ii);  //  Указатель на функцию , которая  обработает
                            //  принятую команду в соответствии с синтаксисом
                            //  и обработает  принятые данные

 void (*answ_flt)(int ii);  // указатель на функцию с действиями в случае
                            //  таймаута по приему команды

 char name[10];             // имя устройства
 int n_dev;                 // номер устройства в группе однотипных устройств
 struct s_icp_dev *ICP_dd;  // указатель на структуру устройства
 int function;              // текущая функция

 unsigned long int timeout;       // допустимое время ответа на команду,мс
 unsigned long int time_stamp;    // момент постановки запроса в очередь
 int  cmd_lgth;                   // длина передаваемой команды
 int CRC_flag;                    // флаг наличия контрольной суммы
 char Cmd[n_buf_queue];           // тело передаваемой команды
};

 // очередь запросов на доступ к COM порту
struct COM_q
{
 int busy_ptr; // Индекс в кольцевом буфере запросов (очереди).
               // Указывает на свободное место в кольцевом буфере запросов.
               // Ппосле занесения запроса в буфер, инкрементирется

 int empt_ptr; // Указывает на запрос подлежащий обработке.
               // После обработки запроса  инкрементирется.
               // если  busy_ptr==empt_ptr - буфер пуст.

struct COM_rqst  *request[que_lgth]; //массив указателей на структуры с запросом
};

extern struct COM_q COM_que[];

void f_queue_init();
void f_queue_clr( int port);
int f_queue_put(int port, struct COM_rqst *request);
int f_queue_chk(int port );
int f_queue_free(int port, struct COM_rqst *request);
int f_queue_Flt(int port);

extern unsigned long int TimeClear;
