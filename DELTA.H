  // функции драйвера:

#define START_FWD_DELTA  0x12
#define STOP_FWD_DELTA   0x11

#define START_REV_DELTA  0x22
#define STOP_REV_DELTA   0x21

#define MSK_RUN_DELTA   0x03
#define RUN_DELTA       0x03
#define ACS_DELTA       0x01
#define STP_DELTA       0x00

  // установка управления приводом через управляющее слово
#define Dlt_SET_CTR  1

  // сброс привода (ошибок )
#define Dlt_RES_ERR  2

 // чтение последней причины отключения (ошибки)
#define Dlt_RD_ERR   4


#define Dlt_SV_OUT   5
#define Dlt_SA_OUT   6
#define Dlt_SD_OUT   7
#define Dlt_CTRL_OUT 8
#define Dlt_SET_MODE 9

#define Dlt_RD   16

// бит "Привод исправен" в слове состояния
#define Dlt_OK 0x01

// бит "Сторожевой таймер" в слове управления
#define Dlt_WDT 0x4000
#define MDlt_Dlt_WDT 0xbfff

#define ToutAnsDlt_rtu 100

#define Delta_max_out_err 10
#define Delta_max_inp_err 10

void f_Delta(int ii);
void f_Delta_rd(int ii);
void f_init_Delta(int ii);
void  f_Dlt_wd( int ii);
void f_Delta_WD_rd(int ii);

extern struct s_icp_dev Delta[];
extern struct COM_rqst Delta_Rqst[];

extern int Delta_State[];  // слово состояния
extern int Delta_Ctrl[];   // упраляющее слово
extern int Delta_Ctrl_c[]; // текущее состояние упраляющего слова
extern int Delta_SV[];     // значение частоты вращения
extern int Delta_SVc[];    // текущее состояние  частоты вращения
extern int Delta_Err_cod[];// ошибка последнего отключения

extern int Delta_SA[];  // текущее состояние  ускорения 2 сек на 100Гц
extern int Delta_SAc[];  // текущее состояние  ускорения

extern int Delta_SD[];  // текущее состояние  замедления 2 сек на 100Гц
extern int Delta_SDc[]; // текущее состояние замедления
extern int Delta_fn[];     // регистр функций драйвера

extern int Delta_fl[];
extern int Dlt_wd[];

extern long int Dlt_pool_period[];
extern struct COM_rqst Delta_WD_Rqst[];

extern int Delta_current[];


