/*  Konputagailuen Arkitektura - Informatika Ingeniaritza (IF - EHU)
    OpenMP laborategia - PROIEKTUA

    multzohitz_s.c     SERIEKO BERTSIOA

    Hitzen bektoreak prozesatzea zientzia eta teknologia alorrei buruzko informazioa lortzeko.
    ALDAKOP aldagaiko hitzak, "multzokop" multzotan sailkatzeko, "distantzien" arabera

    Sarrera: hitzbektoreak.dat  hitz-bektoreak dituen fitxategia
             ztalor.dat      zientzia eta teknologiako alorrei buruzko informazioa duen fitxategia
    Irteera: emaitzak_s.out zentroideak, multzo kopurua eta haien tamaina eta trinkotasuna
                    	    zt alorrei buruzko informazioa

    funth_p.c moduluarekin konpilatu;  -lm aukera gehitu
*****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "defineth.h"           	// konstante eta datu-egituren definizioak
#include "funth.h"              	// programan deitzen diren funtzioak

char  *hitz_izen[EMAX];                // datu-baseko hitzak, string eran

float             hitz[EMAX][ALDAKOP];  // prozesatu behar diren hitzak (hitzbektoreak.dat fitxategian)
struct multzoinfo kideak[MULTZOKOPMAX];  // multzo bakoitzeko kideen zerrenda

float            alor[EMAX][ALORRA];    // hitzen alorrei buruzko datuak (ztalor.dat fitxategian)
struct analisia  alordist[ALORRA];       // alorren probabilitateak: medianen maximoa, minimoa...

struct medoide **medoideak;            // medoideak eta berarengandik gertuen diren bizilagunak, kluster bakoitzerako

struct unesco unesco_kodeak[ALORRA]; // UNESCO-ren sailkapeneko arloen izenak

int  multzokop = 35;			// hasierako multzo kopurua



// ================================
//       PROGRAMA NAGUSIA
// ================================

void main (int argc, char *argv[])
{
  float   zent[MULTZOKOPMAX][ALDAKOP], zentberri[MULTZOKOPMAX][ALDAKOP];   // zentroideak (multzoak)
  float   multzo_trinko[MULTZOKOPMAX];	// multzo bakoitzeko trinkotasuna

  int     i, j, hitzkop, multzoa, zenb;
  int     sailka[EMAX];			// hitz bakoitzeko multzoa
  int     bukatu = 0, iterkop = 0, amaitu_sailkapena;
  double  cvi, cvi_zaharra, dif;

  FILE    *f1, *f2;
  struct timespec  t1, t2, t3, t4, t5, t6;
  double  t_irak, t_sailka, t_alor, t_idatzi, t_med;
  struct timespec   t_tg1, t_tg2, t_zb1, t_zb2, t_kid1, t_kid2, t_bal1, t_bal2;
  double  t_tg = 0.0, t_zb =0.0, t_kid = 0.0, t_bal = 0.0;

  hasieratu_unesco_kodeak(unesco_kodeak);

  if ((argc < 3) || (argc > 4)) {
    printf ("ADI:  progr - f1 (hitz) - f2 (alor) - [hitz kop])\n");
    exit (-1);
  }

  printf ("\n >> Exekuzioa seriean\n");
  clock_gettime (CLOCK_REALTIME, &t1);

  // Irakurri datuak sarrea-fitxategietatik
  // ======================================

  // Hitzen adierazpideak (hitzbektoreak) >> hitz[i][j]

  f1 = fopen (argv[1], "r");
  if (f1 == NULL) {
    printf ("Errorea %s fitxategia irekitzean\n", argv[1]);
    exit (-1);
  }

  hitzkop = EMAX;
  if (argc == 4) hitzkop = atoi (argv[3]);	// 4. parametroa = prozesatu behar diren hitzen kopurua
  hitzkop = hitzkop > EMAX ? EMAX : hitzkop;

  for (i=0; i<hitzkop; i++){
    hitz_izen[i] = (char *)malloc(100*sizeof(char));
    fscanf (f1, "%s ", hitz_izen[i]);
    for (j=0; j<ALDAKOP; j++){
      fscanf (f1, "%f", &(hitz[i][j]));		// hitzen zerrenda, ALDAKOP aldagaikoak
    }
  }
  fclose (f1);

  // Alorrei buruzko informazioa (ztalor) >> alor[i][j]
  f1 = fopen (argv[2], "r");
  if (f1 == NULL) {
    printf ("Errorea %s fitxategia irekitzean\n", argv[2]);
    exit (-1);
  }

  for (i=0; i<hitzkop; i++)
  for (j=0; j<ALORRA; j++)
    fscanf (f1, "%f", &(alor[i][j]));		// alorrei buruzko informazioa

  fclose (f1);

  clock_gettime (CLOCK_REALTIME, &t2);


  // 1. FASEA: prozesu iteratiboa hitzak multzokop multzoetan sailkatzeko
  // CVI indizean DELTA2 balioa baino diferentzia txikiagoa lortu arte.
  // =====================================================================

  amaitu_sailkapena = 0;
  cvi_zaharra = -1;

  while (multzokop < MULTZOKOPMAX && amaitu_sailkapena == 0)
  {
    // Aukeratu lehen zentroideak, ausaz
    hasierako_zentroideak (zent);

    // A. Sailkatze-prozesua, multzokop multzotan
    // ========================================
    iterkop = 0;
    bukatu = 0;
    while ((bukatu == 0) && (iterkop < ITMAX))
    {
      // Kalkulatu multzo gertuena
      /** OSATZEKO funth_s.c fitxategian **/
      multzo_gertuena (hitzkop, hitz, zent, sailka);

      // Kalkulatu multzoetako zentroide berriak: kideen dimentsio bakoitzaren batezbestekoa
      // Aldaketa zentroideetan DELTA1 baino txikiagoa bada, bukatu = 1
      /** OSATZEKO funth_s.c fitxategian **/
      bukatu = zentroide_berriak (hitz, zent, sailka, hitzkop);

      iterkop ++;
    }
    printf ("   Multzo kopurua: %d -- Iterazio kopurua: %d\n", multzokop, iterkop);

    // B. Sailkatzearen "kalitatea"
    // ===========================
    for (i=0; i<multzokop; i++)  kideak[i].kop = 0;

    // Multzo bakoitzeko hitzak (osagaiak) eta kopurua
    for (i=0; i<hitzkop; i++)
    {
      multzoa = sailka[i];
      kideak[multzoa].osagaiak[kideak[multzoa].kop] = i;
      kideak[multzoa].kop ++;
    }

    // Sailkatzearen balidazioa eta konbergentzia
    /** OSATZEKO funth_s.c fitxategian **/
    cvi = balidazioa (hitz, kideak, zent, multzo_trinko);

    dif = cvi - cvi_zaharra;
    if (dif < DELTA2 || (multzokop+10)>MULTZOKOPMAX) amaitu_sailkapena = 1;
    else {
      multzokop += 10;
      cvi_zaharra = cvi;
    }
  }
  clock_gettime (CLOCK_REALTIME, &t3);
  
 // Kalkulatu multzo bakoitzerako medoidea eta haren MEDKNN hitz gertukoenak

  medoideak = (struct medoide **)malloc(multzokop*sizeof(struct medoide *));
  for(i=0; i<multzokop; i++){
	medoideak[i] = (struct medoide *)malloc(MEDKNN*sizeof(struct medoide));
  }
  medoideak_kalkulatu(hitz, kideak, medoideak);

  clock_gettime (CLOCK_REALTIME, &t4);

  // 2. FASEA: analizatu alorrak
  // ==============================

  // Multzoen analisia, alorrak: medianen maximoa, minimoa...
  /** OSATZEKO funth_s.c fitxategian **/
  ztalorren_analisia (kideak, alor, alordist);

  clock_gettime (CLOCK_REALTIME, &t5);

  // Idatzi emaitzak emaitzak_s.out fitxategian
  // ==========================================

  f2 = fopen ("../emaitzak/emaitzak_s.out", "w");
  if (f2 == NULL) {
    printf ("Errorea emaitzak_s.out fitxategia irekitzean\n");
    exit (-1);
  }

  fprintf (f2, " >> Multzoen zentroideak\n");
  for (i=0; i<multzokop; i++) {
    for (j=0; j<ALDAKOP; j++) fprintf (f2, "%7.3f", zent[i][j]);
    fprintf (f2,"\n");
  }

  fprintf (f2, "\n >> Multzoen tamaina: %d multzo\n\n", multzokop);
  for (i=0; i<multzokop/10; i++) {
    for (j=0; j<10; j++) fprintf (f2, "%9d", kideak[10*i+j].kop);
    fprintf (f2, "\n");
  }
  for (i=i*10; i<multzokop; i++) fprintf (f2, "%9d", kideak[i].kop);
  fprintf (f2, "\n");

  fprintf (f2, "\n >> Multzoen trinkotasuna \n\n");
  for (i=0; i<multzokop/10; i++) {
    for (j=0; j<10; j++) fprintf (f2, "%9.2f", multzo_trinko[10*i+j]);
    fprintf (f2, "\n");
  }
  for (i=i*10; i<multzokop; i++) fprintf (f2, "%9.2f", multzo_trinko[i]);
  fprintf (f2, "\n");

  fprintf (f2, "\n >> Alorren analisia (medianak)\n");
  fprintf (f2, "\n Alor.  M_max - Tald   M_min - Tald");
  fprintf (f2, "\n ==================================\n");
  for (i=0; i<ALORRA; i++)
    fprintf (f2, "  %2d     %4.2f - %2d      %4.2f - %2d\n", i,
                alordist[i].mmax, alordist[i].multzomax, 
                alordist[i].mmin, alordist[i].multzomin);

  fclose (f2);

  clock_gettime (CLOCK_REALTIME, &t6);


  // Pantailaratu emaitza batzuk
  // ===========================

  t_irak   = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / (double)1e9;
  t_sailka = (t3.tv_sec - t2.tv_sec) + (t3.tv_nsec - t2.tv_nsec) / (double)1e9;
  t_med    = (t4.tv_sec - t3.tv_sec) + (t4.tv_nsec - t3.tv_nsec) / (double)1e9;
  t_alor    = (t5.tv_sec - t4.tv_sec) + (t5.tv_nsec - t4.tv_nsec) / (double)1e9;
  t_idatzi = (t6.tv_sec - t5.tv_sec) + (t6.tv_nsec - t5.tv_nsec) / (double)1e9;


  printf ("\n   Exekuzio-denborak\n");
  printf ("\n   T_irakurtzea: %7.3f s", t_irak);
  printf ("\n   T_sailkatzea: %7.3f s", t_sailka);
  printf ("\n   T_alorrak:    %7.3f s", t_alor);
  printf ("\n   T_idaztea:    %7.3f s", t_idatzi);
  printf ("\n   ===================");
  printf ("\n        T_osoa:  %7.3f s\n\n", t_irak + t_sailka + t_alor + t_idatzi);

  // Idatzi emaitza batzuk pantailan
  // ==========================================
  printf ("\n >> 0, 20, 40... zentroideak \n");
  for (i=0; i<multzokop; i+=20) {
		printf ("\n  zent%2d -- ", i);
		for (j=0; j<ALDAKOP; j++) printf ("%7.3f", zent[i][j]);
		printf("\n");
	}

  printf ("\n >> Multzoen tamaina: %d multzo \n\n", multzokop);
  for (i=0; i<multzokop/10; i++) {
    for (j=0; j<10; j++) printf ("%9d", kideak[10*i+j].kop);
    printf("\n");
  }
  for (i=i*10; i<multzokop; i++) printf ("%9d", kideak[i].kop);
  printf ("\n");

  printf ("\n >> Multzoen trinkotasuna \n\n");
  for (i=0; i<multzokop/10; i++) {
    for (j=0; j<10; j++) printf ("%9.2f", multzo_trinko[10*i+j]);
    printf("\n");
  }
  for (i=i*10; i<multzokop; i++) printf ("%9.2f", multzo_trinko[i]);
  printf ("\n");

  printf ("\n >> Alorren analisia (medianak)\n");
  printf ("\n Alor.  M_max - Tald   M_min - Tald");
  printf ("\n ==================================\n");
  for (i=0; i<ALORRA; i++){
    printf ("  Arloa: %2d (%-12s)   %4.2f - %3d (%-12s)   %4.2f - %3d (%-12s)\n", 
      i, unesco_kodeak[i].izena,
      alordist[i].mmax, alordist[i].multzomax, hitz_izen[medoideak[alordist[i].multzomax][0].pos], 
      alordist[i].mmin, alordist[i].multzomin, hitz_izen[medoideak[alordist[i].multzomin][0].pos]);
  }
  printf("\n\n");

  printf("\n>> Multzoen medoideak:\n\n");
	for(i=0; i<multzokop; i++){
		printf("Multzoa: %3d; Medoidea:", i);
		for(j=0; j<MEDKNN; j++){
			if(medoideak[i][j].pos!=-1){
				printf(" %s", hitz_izen[medoideak[i][j].pos]);
			} else {
				break;
			}
    }
		printf("\n");
	}

	// free hitz_izen
	for (i=0; i<hitzkop; i++){
		free(hitz_izen[i]);
	}

	// free medoides
	for(i=0; i<multzokop; i++){ free(medoideak[i]); }
	free(medoideak);
}
