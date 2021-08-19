
// Pressure in MPa for:
// flowmeter: 2 tubes in parallel, diameter=1mm
// flow 1 kg/hour
// Density = 1 kg/m3

#define   ConstBern  (0.015635985)
   // MPa
  s_MVD[0].PressB= FctBern*s_MVD[0].FlowM*s_MVD[0].FlowM/s_MVD[0].Dens;

void f_prep_bern()
{
 if(R_MVD>=0)
   FctBern=ConstBern/(R_MVD*R_MVD*R_MVD*R_MVD);
 else
   FctBern=0;
}
