
int fict_npn=0;
int *num_pnt[KOL_D_INP+1]={&fict_npn,
&num_in1,&num_in2,&num_in3,&num_in4,&num_in5,&num_in6,&num_in7,&num_in8,
&num_in9,&num_in10,&num_in11,&num_in12,&num_in13,&num_in14,&num_in15,&num_in16,
&num_in17,&num_in18,
};
void f_init_mki()
{
int i;

  for(i=1;i<=KOL_D_INP;i++)
    {
     if(inv_msk[i] != 0.) iv_msk[  *num_pnt[i]  ] = 0xffff;
     else   iv_msk[*num_pnt[i]] = 0;
    }

/*
  INP_inv=0;
  if(mski_inp1 != 0) INP_inv|=msk_int[num_in1];
  if(mski_inp2 != 0) INP_inv|=msk_int[num_in2];
  if(mski_inp3 != 0) INP_inv|=msk_int[num_in3];
  if(mski_inp4 != 0) INP_inv|=msk_int[num_in4];
  if(mski_inp5 != 0) INP_inv|=msk_int[num_in5];
  if(mski_inp6 != 0) INP_inv|=msk_int[num_in6];
  if(mski_inp7 != 0) INP_inv|=msk_int[num_in7];
  if(mski_inp8 != 0) INP_inv|=msk_int[num_in8];
  if(mski_inp9 != 0) INP_inv|=msk_int[num_in9];
  if(mski_inp10 != 0) INP_inv|=msk_int[num_in10];
  if(mski_inp11 != 0) INP_inv|=msk_int[num_in11];
  if(mski_inp12 != 0) INP_inv|=msk_int[num_in12];
  if(mski_inp13 != 0) INP_inv|=msk_int[num_in13];
  if(mski_inp14 != 0) INP_inv|=msk_int[num_in14];
  if(mski_inp15 != 0) INP_inv|=msk_int[num_in15];
  if(mski_inp16 != 0) INP_inv|=msk_int[num_in16];
  if(mski_inp17 != 0) INP_inv|=msk_int[num_in17];
  if(mski_inp18 != 0) INP_inv|=msk_int[num_in18];
*/

//  INP_MSK_OK = ES_IN | UZA_IN | LVL_IN | TRAP_IN | CONS_IN ;

}
//-------------------------------
