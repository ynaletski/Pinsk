
//---------------------------------
#define n_coil_b   256
#define n_char_b   256

int flag_AD=0;
double V_ref=0.;

/*----------------------------*/
struct s_MVD_dev
{
unsigned int r_addr; // address of MODBUS register
int n;            // number of data - for read coils and ascii
int type;         // data type
int i;            // int,coil value for write
int exc;          // exception
long int  l;      // long
float f;          // float

float FlowM;          // float
float FlowV;          // float
float MassI;          // float
float MassT;          // float
float VolI;          // float
float VolT;          // float
float VolTd;          // float
float Temp;          // float
float TempR;         // float
float Dens;          // float
float Press;         // float
float Compos;       // float
float PressG;       // float
float PressB;       // float
float PressD;       // float
float PressGN;      // float
float FlowMc;        // float
float Volt_DC;       // float

char c[n_coil_b]; // coil(s) value(s) read from MVD
char a[n_char_b]; // ascii
int reg_err;
};

/*----------------------------*/
struct s_MVD_dev s_MVD[2]=
{
0 , //int r_addr;       // address of MODBUS register
0,  //int n;            // number of data - for read coils and ascii
0,  //int type;         // data type
0,  //int i;            // int,coil value for write
0,  //int exc;          // exception
0,  //long int  l;      // long
0,    //float f;          // float

-1.0, //float FlowM;         // float
-1.0, //float FlowV;         // float
-1.0, //float MassI;        // float
-1.0, //float MassT;         // float
-1.0,  //float VolI;          // float
-1.0, //float VolT;          // float
-1.0, //float VolTd;         // float
-1.0, //float Temp;          // float
-1.0, //float TempR;         // float
-1.0, //float Dens;          // float
-1.0, //float Press;         // float
-1.0, //float Compos;        // float
 0.0, //float PressG;        // float
-1.0, //float PressB;        // float
-1.0, //float PressD;        // float
-1.0, //float PressGN;       // float
-1.0, //float Volt_A         // float
-1.0, //float Volt_DC        // float

"",   //char c[n_coil_b]; // coil(s) value(s) read from MVD
"",   //char a[n_char_b]; // ascii
0,    //int reg_err;
};
//--------------------------------
float f_get_VCF(float Temp,float D_ref);
float f_get_T(float D1,float D_ref, float T_ref);
float f_get_ref_Dns(float D_obs, float Temp,float Press, float T_Ref);
extern int type_liq;  // 2 Fuel Oils   - диэтопливо

int mode_temp=0;
int mode_refD=0;

#define EXT (1)



int flag_Slv=0;
void f_SlaveRTU()
{
}


void  f_ctrl_v2()
{
}


    float D_ref=820, D_obs=800, Temp=23, T_Ref=15,Press=0.44;
    float D1=200,Mass,Vol,T1,VCF;

void main()
{

m1:
    printf( "\nEnter Density, Temp ,Press MPa, Temp_ref: " );
    if(scanf ("%f %f %f %f", &D_obs, &Temp, &Press, &T_Ref ))
    {
      D_ref=f_get_ref_Dns( D_obs, Temp,Press,T_Ref);
      printf("\n D_ref(%f)=%f, \( t=%f,Press=%f\) \n",T_Ref,D_ref,Temp,Press);
    }
    else exit(0);
m2:
    printf( "\nEnter D1: " );
    if( scanf("%f",&D1 ) )
    {
  //      D1=Mass*1000./Vol;
      T1=f_get_T( D1,D_ref, T_Ref);
      printf("\n D1=%f kg/m3,T1=%f",D1,T1);
    }
    else exit(0);

    goto m2;
}



//---------------------------------
