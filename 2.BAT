SET PATH=C:\TC\BIN;C:\TC\INCLUDE;C:\PE2;
dos_win dos_win.c dos_winw.c
dos_win AVT_CTRL.C AVT_CTRw.C
tcc -ml -M -Ic:\tc\include -Lc:\tc\lib nfd.C MOD1.C MOD2.C 7188xal.lib x607.lib  > 1.RPT
rem del *.obj
pe2.bat 1.rpt


