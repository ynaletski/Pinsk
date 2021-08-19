

#define OFP_max_inp_err 10

#define OFP_max_t_err 3


#define OFP_WR_I        16

void f_OFP(int ii);
void f_OFP_rd(int ii);
void f_OFP_rd_t(int ii);

int f_OFP_WR(int ii,int addr,int i_val);

extern int OFP_fl[];
extern struct s_icp_dev OFP[];
extern struct COM_rqst OFP_Rqst[];

extern unsigned int INP_OFP;

