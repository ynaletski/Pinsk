

#define START_FWD_SK  0x183
#define STOP_FWD_SK   0x181

#define START_REV_SK  0x189
#define STOP_REV_SK   0x181

#define MSKCH_RUN_SK 0x7
#define MSK_RUN_SK 0x3
#define MSK_STP_SK 0x7

// бит "Привод исправен" в слове состояния
#define SK_OK 0x01

// бит "Нулевая скорость" в слове состояния
#define STP_SK 0x04

// бит "Ниже задания скорости" в слове состояния
#define V_LESS  0x10

// бит "На скорости" в слове состояния
#define V_OK  0x20

// бит "Выше задания скорости" в слове состояния
#define V_MORE  0x40


// бит "Сторожевой таймер" в слове управления
#define SK_WDT 0x4000
#define MSK_SK_WDT 0xbfff


  // функции драйвера:
  // установка управления приводом через управляющее слово
#define SK_SET_CTR  1

  // сброс привода (ошибок )
#define SK_RES_ERR  2

 // установка инверсии входа разрешения работы
#define SK_INV_RDY  3

 // чтение последней причины отключения (ошибки)
#define SK_RD_ERR   4


#define SK_SV_OUT   5
#define SK_SA_OUT   6
#define SK_SD_OUT   7
#define SK_CTRL_OUT 8
#define SK_SET_MODE 9

#define SK_RD   16

#define ToutAnsSK_rtu 100

#define ComSK_max_out_err 10
#define ComSK_max_inp_err 10

void f_ComSK(int ii);
void f_ComSK_rd(int ii);
void f_init_ComSK(int ii);
void  f_sk_wd( int ii);
void f_ComSK_WD_rd(int ii);

extern struct s_icp_dev ComSK[];
extern struct COM_rqst ComSK_Rqst[];

extern int ComSK_State[];  // слово состояния
extern int ComSK_Ctrl[];   // упраляющее слово
extern int ComSK_Ctrl_c[]; // текущее состояние упраляющего слово
extern int ComSK_SV[];     // значение частоты вращения
extern int ComSK_SVc[];    // текущее состояние  частоты вращения
extern int ComSK_Err_cod[];// ошибка последнего отключения

extern int ComSK_SA[];  // текущее состояние  ускорения 2 сек на 100Гц
extern int ComSK_SAc[];  // текущее состояние  ускорения

extern int ComSK_SD[];  // текущее состояние  замедления 2 сек на 100Гц
extern int ComSK_SDc[]; // текущее состояние замедления
extern int ComSK_fn[];     // регистр функций драйвера

extern int ComSK_fl[];
extern int sk_wd[];

extern long int sk_pool_period[];
extern struct COM_rqst ComSK_WD_Rqst[];
extern int ComSK_current[];

