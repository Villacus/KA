/*  Konputagailuen Arkitektura - Informatika Ingeniaritza (IF - EHU)
    OpenMP laborategia - PROIEKTUA

    funth_s.c		SERIEKO BERTSIOA
    multzohitz_s.c programan erabiltzen diren errutinak
*****************************************************************************************/

#include <stdlib.h>
#include <math.h>
#include <float.h>		// DBL_MAX
#include <string.h>
#include <omp.h>

#include "defineth.h"		// konstante eta datu-egituren definizioak


/* 1 - Bi hitzen arteko distantzia kalkulatzeko funtzioa 
       (kosinu distantzia)

       Sarrera:  ALDAKOP aldagaiko bi hitz (erreferentziaz)
       Irteera:  distantzia (double)
******************************************************************************************/
double hitzen_distantzia(float *hitz1, float *hitz2){
    // EGITEKO
    // Kalkulatu bi elementuren arteko distantzia (kosinu dena)
  double absum = 0;
  double a2sum = 0;
  double b2sum = 0;

  for (int i=0; i<ALDAKOP; i++) {
    absum += hitz1[i]*hitz2[i];
    a2sum += hitz1[i]*hitz1[i];
    b2sum += hitz2[i]*hitz2[i];
  }
  double cos_sim = absum/(sqrt(a2sum)*sqrt(b2sum));

  cos_sim = (cos_sim + 1)/2;

  return 1.0 - cos_sim;
}


/* 2 - Multzo gertuena kalkulatzeko funtzioa (zentroide gertuena)

       Sarrera:  hitzkop  hitz kopurua, int
                 hitz     EMAX x ALDAKOP tamainako matrizea
                 zent     multzokop x ALDAKOP tamainako matrizea
       Irteera:  sailka   EMAX tamainako bektorea, hitz bakoitzari dagokion multzoa
******************************************************************************************/
void multzo_gertuena (int hitzkop, float hitz[][ALDAKOP], float zent[][ALDAKOP], int *sailka){
	// EGITEKO
	// sailka: elementu bakoitzaren zentroide hurbilena, haren "taldea"
  int i, j, gertuena;
  double dist, min_dist;
  for (i=0;i<hitzkop;i++) {
    min_dist = DBL_MAX;
    gertuena = 0;
    for (j=0;j<multzokop;j++) {
      dist = hitzen_distantzia(hitz[i],zent[j]);
      if (min_dist>dist) {
        min_dist = dist;
        gertuena = j;
      }
    }
  sailka[i] = gertuena;
  }

}


/* 3 - Sailkapenaren balidazioa: multzoen trinkotasuna eta zentroideen trinkotasuna
       cvi indizea kalkulatzen da
 
       Sarrera:  hitz     fitxategiko hitzak (EMAX x ALDAKOP tamainako matrizea)
                 kideak   multzoekideen zerrenda (multzokop tamainako struct-bektore bat: hitz eta kop)
                 zent	  multzoeen zentroideak (multzokop x ALDAKOP)
       Irteera:  cvi indizea 
                 multzo_trinko    multzoen trinkotasuna
******************************************************************************************/
double balidazioa (float hitz[][ALDAKOP], struct multzoinfo *kideak, float zent[][ALDAKOP], float *multzo_trinko){
  float   zent_trinko[multzokop];
  // EGITEKO
  // Kalkulatu taldeen trinkotasuna: kideen arteko distantzien batezbestekoa
  // Kalkulatu zentroideen trinkotasuna: zentroide bakoitzeko, besteekiko b.b.-ko distantzia
  // Kalkulatu cvi indizea
  int i, j, k, zenbat, ind_x;
  double batura;

  for (k=0;k<multzokop;k++) {
    batura = 0.0;
    zenbat = 0;
    for (i=0;i<kideak[k].kop;i++) {
      ind_x = kideak[k].osagaiak[i];
      for (j=0;j<kideak[k].kop;j++) {
        if (i!=j) {
          batura += hitzen_distantzia(hitz[ind_x],hitz[kideak[k].osagaiak[j]]);
        }
        zenbat++;
      }
    }
    if (zenbat>1) {
      multzo_trinko[k] = batura/(zenbat*(zenbat-1));
    }
  }

  for (k=0;k<multzokop;k++) {
    batura = 0.0;
    zenbat = 0;
    for (i=0;i<multzokop;i++) {
      if (i!=k) {
        batura += hitzen_distantzia(zent[k],zent[i]);
        zenbat++;
      }
    }
    zent_trinko[k] = batura/(multzokop-1);
  }

  float s[multzokop];
  double baturas = 0.0;
  for (k=0;k<multzokop;k++) {
    s[k] = (zent_trinko[k]-multzo_trinko[k])/fmax(zent_trinko[k],multzo_trinko[k]);
    baturas += s[k];
  }
  return baturas/multzokop;
}

  
/* 4 - Alorrak analizatzeko funtzioa 
 
       Sarrera:  kideak  multzokideen zerrenda (multzokop tamainako struct-bektore bat: hitz eta kop)
                 alor    alorrei buruzko informazioa (EMAX x ALORRA) 
       Irteera:  alordist alorren analisia: medianen maximoa/minimoa, eta multzoak
******************************************************************************************/
void ztalorren_analisia (struct multzoinfo *kideak, float alor[][ALORRA], struct analisia *alordist){
  // EGITEKO
  // Prozesatu UNESCO alorrei buruzko informazioa talde bakoitzeko kideen artean:
  //   UNESCO alor bakoitzarekiko atxikimenduaren neurrien mediana.
  //   UNESCO kode bakoitzarako, medianen maximoa eta minimoa eta zein taldetan.
}


// PROGRAMA NAGUSIAREN BESTE FUNTZIOAK
// ====================================

/* 5 - Medoideak kalkulatu
**************************************************************/

void ordenaturik_gorde(struct medoide *medoidebat, int n, double dist, int pos){
	int i, j;
	for(i=0; i<n; i++){
		if(dist < medoidebat[i].dist){
			for(j=n-2; j>=i; j--){
				medoidebat[j+1].dist = medoidebat[j].dist;
				medoidebat[j+1].pos = medoidebat[j].pos;
			}
			medoidebat[i].dist = dist;
			medoidebat[i].pos = pos;
			break;
		}
	}
}

void medoideak_kalkulatu(float hitz[][ALDAKOP], struct multzoinfo *kideak, struct medoide **medoideak){
	int     i, j, k;
	int     talde_tam, i1, i2;
	double  dist, dist_min;
	int     pos_min;

	// hasieratu egitura
	for(i=0; i<multzokop; i++){
		for(j=0; j<MEDKNN; j++){
			medoideak[i][j].dist = DBL_MAX;
			medoideak[i][j].pos = -1;
		}
	}

	// kalkulatu medoideak eta euren bizilagunak
	for(i=0; i<multzokop; i++){
		// medoidea kalkulatu
		talde_tam = kideak[i].kop;
		dist_min = DBL_MAX;
		pos_min = -1;
		for(j=0; j<talde_tam; j++){
			i1 = kideak[i].osagaiak[j];
			dist = 0.0;
			for(k=0; k<talde_tam; k++){
				i2 = kideak[i].osagaiak[k];
				dist += hitzen_distantzia(hitz[i1], hitz[i2]);
			}
			if(dist < dist_min){
				dist_min = dist;
				pos_min = i1;
			}
		}
		if(pos_min!=-1){
			medoideak[i][0].dist = 0.0;
			medoideak[i][0].pos = pos_min;
		}

		// medoidearen bizilagunak bilatu
		for(j=0; j<kideak[i].kop; j++){
			i2 = kideak[i].osagaiak[j];
			if(i2!=pos_min){
				dist = hitzen_distantzia(hitz[pos_min], hitz[i2]);
				ordenaturik_gorde(medoideak[i]+1, MEDKNN-1, dist, i2);
			}
		}
	}
}


/* 6 - Zentroideen hasierako balioak
**************************************************************/
void hasierako_zentroideak (float zent[][ALDAKOP])
{
  int  i, j;
  float ausa_balioa;
  srand (147);
  for (i=0; i<multzokop; i++)
  for (j=0; j<ALDAKOP/2; j++){
    ausa_balioa = ((rand() % 10000) / 10000.0)*8-4; 
    zent[i][j] = ausa_balioa;
    zent[i][j+ALDAKOP/2] = ausa_balioa;
  }
}



/* 7 - Zentroide berriak; erabaki bukatu behar den edo ez
**************************************************************/

int zentroide_berriak (float hitz[][ALDAKOP], float zent[][ALDAKOP], int *sailka, int hitzkop)
{
    int     i, j, bukatu;
    float   zentberri[multzokop][ALDAKOP];
    double  diszent;
    double  baturak[multzokop][ALDAKOP+1];
    
    bukatu = 1;		// 1: simulazioa bukatu da; 0: jarraitu behar da

    // Zentroide berriak: kideen dimentsio bakoitzeko aldagaien batezbestekoak
    // sailka: hitz bakoitzaren multzoa
    // baturak: multzokideen aldagaien balioak batzeko; azkena kopurua da.

    for (i=0; i<multzokop; i++)
    for (j=0; j<ALDAKOP+1; j++)
      baturak[i][j] = 0.0;

    // Hitz asko prozesatzen dira paraleloan, nahiz eta kalkulu gehiena erreduzioko 
    // aldagai batean egiten den. 
    for (i=0; i<hitzkop; i++){
      for (j=0; j<ALDAKOP; j++)
        baturak[sailka[i]][j] += hitz[i][j];	// batu multzokideen balioak aldagai bakoitzeko

      baturak[sailka[i]][ALDAKOP] ++;   	    // azken osagaia (ALDAKOP) kopuruak gordetzeko
    }

    // Kalkulatu multzoetako zentroide berriak, eta erabaki bukatu behar den (DELTA1)
    for (i=0; i<multzokop; i++)
    {
      if (baturak[i][ALDAKOP] > 0)		// multzoa ez dago hutsik
      {
        // Zentroide berrien "koordenatuak": kideen batezbestekoak
        for (j=0; j<ALDAKOP; j++)
          zentberri[i][j] = baturak[i][j] / baturak[i][ALDAKOP];
  
        // Erabaki bukatu behar den
        diszent = hitzen_distantzia (&zentberri[i][0], &zent[i][0]);
        if (diszent > DELTA1)  bukatu = 0;	// multzoko zentroidea mugitu da DELTA1 baino gehiago; segi simulazioarekin
       
        // Kopiatu zentroide berria
        for (j=0; j<ALDAKOP; j++)
          zent[i][j] = zentberri[i][j];
      }
    }

    return (bukatu);
}


/* 8 - UNESCO kodeak eta euren lerro zenbakiak gorde gero inprimatzeko
**************************************************************/
void hasieratu_unesco_kodeak(struct unesco unesco_kodeak[]){
  unesco_kodeak[0].lerroa = 8015;
  strcpy(unesco_kodeak[0].izena, "logic");
  unesco_kodeak[1].lerroa = 6934;
  strcpy(unesco_kodeak[1].izena, "mathematics");
  unesco_kodeak[2].lerroa = 14452;
  strcpy(unesco_kodeak[2].izena, "astronomy");
  unesco_kodeak[3].lerroa = 5913;
  strcpy(unesco_kodeak[3].izena, "physics");
  unesco_kodeak[4].lerroa = 6298;
  strcpy(unesco_kodeak[4].izena, "chemistry");
  unesco_kodeak[5].lerroa = 210;
  strcpy(unesco_kodeak[5].izena, "life");
  unesco_kodeak[6].lerroa = 1896;
  strcpy(unesco_kodeak[6].izena, "earth"); 
  unesco_kodeak[7].lerroa = 2277;
  strcpy(unesco_kodeak[7].izena, "agriculture");
  unesco_kodeak[8].lerroa = 743;
  strcpy(unesco_kodeak[8].izena, "medical");
  unesco_kodeak[9].lerroa = 766;
  strcpy(unesco_kodeak[9].izena, "technology");
  unesco_kodeak[10].lerroa = 14797;
  strcpy(unesco_kodeak[10].izena, "anthropology");
  unesco_kodeak[11].lerroa = 6288;
  strcpy(unesco_kodeak[11].izena, "demographics");
  unesco_kodeak[12].lerroa = 259;
  strcpy(unesco_kodeak[12].izena, "economic");
  unesco_kodeak[13].lerroa = 4461;
  strcpy(unesco_kodeak[13].izena, "geography");
  unesco_kodeak[14].lerroa = 312;
  strcpy(unesco_kodeak[14].izena, "history");
  unesco_kodeak[15].lerroa = 3737;
  strcpy(unesco_kodeak[15].izena, "judicial");
  unesco_kodeak[16].lerroa = 19675;
  strcpy(unesco_kodeak[16].izena, "linguistics");
  unesco_kodeak[17].lerroa = 37538;
  strcpy(unesco_kodeak[17].izena, "pedagogy");
  unesco_kodeak[18].lerroa = 215;
  strcpy(unesco_kodeak[18].izena, "politic");
  unesco_kodeak[19].lerroa = 6991;
  strcpy(unesco_kodeak[19].izena, "psychology");
  unesco_kodeak[20].lerroa = 731;
  strcpy(unesco_kodeak[20].izena, "art");
  unesco_kodeak[21].lerroa = 12425;
  strcpy(unesco_kodeak[21].izena, "sociology");
  unesco_kodeak[22].lerroa = 5424;
  strcpy(unesco_kodeak[22].izena, "ethics");
  unesco_kodeak[23].lerroa = 4268;
  strcpy(unesco_kodeak[23].izena, "philosophy");
}
