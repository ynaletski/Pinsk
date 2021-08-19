
#define ind_mDat2w (1)


#define Hst_alarm (0x8000)
#define CMD_BAD   (-1)

#define CMD_ERR  (0x8000)
#define Short_OK (40)

#define CMD_DIS_PMP (0x100)
#define CMD_DIS_RCV (0x1000)
#define Pump_act     1

#define RCV_act     0x10
#define RCV_flch    0x20
#define RCV_prep    0x40
#define RCV_fin     0x80

#define DEN_act     0x100
#define DEN_fin     0x800

#define  ToutAnsLIn_rtu 100

#define mDat1  1
#define mDat2  2

#define mDat1w 3
#define mDat2w 4
#define mDat3w 5
#define mDat4w 6

#define LIn_RD_P   8

#define LIn_max_inp_err   3

struct s_LIn_dev
{
unsigned int r_addr; // address of MODBUS register
int n;               // number of data - for read coils and ascii
int type;            // data type
int exc;             // exception
long int l;          // long
int i1;              // int
int i2;              // int
float f1;            // float
float f2;            // float
float f3;            // float
};

extern struct s_icp_dev LIn[];
extern struct COM_rqst LIn_Rqst[];
extern struct s_LIn_dev s_LIn[];
extern unsigned int LIn_inp_err[];

void f_LIn_interp();
void f_LIn_rd(int ii);
void f_prep_state();
void f_prep_tmp_out();

extern int LInd_max2;
extern int LInd_max1w;
extern int LInd_max2w;
extern int LInd_max3w;


extern int Reg_State;
extern int CMD_IN;
extern int CMD_OUT;

extern  float LIn_val;

struct s_DGM_dev
{
float Prod_lvl; // 0.1 mm
float offset;
float scale;
float Wat_lvl;  // 0.1mm
float Temp;     // 0.1 C
float VolLiq;
float VolGas;
float MassLiq;
float MassGas;
float Mass;
float DensLiq;
float DensGas;
float DensReal;
float TempReal;
};

#define max_DGM  2

extern struct s_DGM_dev s_DGM[];

extern int Tank_n;

void f_inp_Lin(int ii,struct eee_dat *LIn_D,int Lin_N );
void f_rst_slv();
extern int LIn_sw[];

extern float Den_tmp;
extern float Temp_tmp;

