
        else if (key=='-')
        {
          if(PageD==7)
          { // включить симуляцию OFP
             f_OFP_WR(0,7,1);
             INP_OFP=0xc0ff;
          }
          break;
        }
        else if (key=='0')
        {
          if(PageD==7)
          { // выключить симуляцию OFP
             f_OFP_WR(0,7,2);
             INP_OFP=0x0;
          }
          break;
        }

//--------------------------------

#define LVL_IN_L   f_ik(num_in10)

//---------------------------

int  iv_msk[KOL_D_INP+2]  ;
int  inv_msk[KOL_D_INP+2] ;
int int_fict;
//---------------------------

void *p_ik[]=
{
&int_fict,
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  1  2  3  4
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  5  6  7  8
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  9 10 11 12
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  // 13 14 15 16
&INP_OFP ,&INP_OFP ,&INP_OFP ,&INP_OFP , // 17 18 19 20
&INP_OFP ,&INP_OFP ,&INP_OFP ,&INP_OFP , // 21 22 23 24
};

int msk_ik[]=
{
 0,
   1    ,2    ,4    ,8,  0x10   ,0x20,  0x40  ,0x80,         //  1  2  3  4  5  6  7  8
 0x100,0x200,0x400,0x800,0x1000, 0x2000,0x4000,0x8000,       //  9 10 11 12 13 14 15 16
   1    ,2    ,4    ,8,  0x10   ,0x20,  0x40  ,0x80,         // 17 18 19 20 21 22 23 24
};
//---------------------------
int f_ik(int i)
{
   return( ( ( *(int *)p_ik[i]) ^ iv_msk[i]  ) & msk_ik[i] );
}

//---------------------------

// #define    mski_inp1     inv_msk[1]
void f_init_mki()
{
int i;

  for(i=1;i<=KOL_D_INP;i++)
    {
     if(inv_msk[i] != 0.) iv_msk[i] = 0xffff;
     else   iv_msk[i] = 0;
    }
}
//-------------------------------
avt_ctrl.h

#define KOL_INP (24)
#define KOL_D_INP (12)


#define ES_IN    f_ik(num_in1)
#define UZA_IN   f_ik(num_in2)
#define LVL_IN   f_ik(num_in3)
#define STRT_IN  f_ik(num_in4)
#define STOP_IN  f_ik(num_in5)
#define CONS_IN  f_ik(num_in6)
#define TRAP_IN  f_ik(num_in7)

#define ES_IN_L   f_ik(num_in8)
#define UZA_IN_L   f_ik(num_in9)
#define LVL_IN_L   f_ik(num_in10)

#define STOP_IN_L  f_ik(num_in11)
#define STRT_IN_L  f_ik(num_in12)

#define    mski_inp1     inv_msk[1]
#define    mski_inp2     inv_msk[2]
#define    mski_inp3     inv_msk[3]
#define    mski_inp4     inv_msk[4]
#define    mski_inp5     inv_msk[5]
#define    mski_inp6     inv_msk[6]
#define    mski_inp7     inv_msk[7]
#define    mski_inp8     inv_msk[8]
#define    mski_inp9     inv_msk[9]
#define    mski_inp10    inv_msk[10]
#define    mski_inp11    inv_msk[11]
#define    mski_inp12    inv_msk[12]
#define    mski_inp13    inv_msk[13]
#define    mski_inp14    inv_msk[14]
#define    mski_inp15    inv_msk[15]
#define    mski_inp16    inv_msk[16]
#define    mski_inp17    inv_msk[17]
#define    mski_inp18    inv_msk[18]


