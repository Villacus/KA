/*  Konputagailuen Arkitektura - Informatika Ingeniaritza (IF - EHU)
    OpenMP laborategia - PROIEKTUA

    defineth.h
    multzohitz eta funth fitxategietan erabiltzen diren konstanteak eta datu-egiturak
**********************************************************************************/


// konstanteak

#define EMAX        200000	// osagai kopuru (hitz) maximoa
#define MULTZOKOPMAX 200	// multzo kopuru maximoa
#define ALDAKOP     50		// hitzu bakoitzaren aldagai kopurua
#define DELTA1      0.01	// konbergentzia: zentroideen aldaketa minimoa
#define DELTA2      0.0001	// konbergentzia: sailkapena (cvi)
#define ITMAX       10000	// konbergentzia: iterazio kopuru maximoa
#define ALORRA      24		// Zientzia eta Teknologiako Alorren kpurura
#define MEDKNN      10		// Medoide bakoitzaren gertueneko hitz kopurua. Inprimatzeko 

extern int  multzokop;		// simulazioaren iterazio bakoitzeko multzo kopurua


// datu-egiturak

struct multzoinfo 		// multzoei buruzko informazioa
{
  int  osagaiak[EMAX]; 	// osagaiak
  int  kop;       		// osagai kopurua
};


struct analisia   		// alorren analisiaren emaitzak
{
  float  mmax, mmin;           	// medianaren maximoa eta minimoa alor bakoitzerako
  int    multzomax, multzomin;  	// maximoa eta minimoa duten multzoak
};

struct medoide {
	double dist;		// medoidearekiko distantzia
	int pos;		// Sarrerako datu-basean zenbagarren hitza den
};

struct unesco {
    int lerroa;		// Sarrerako datu-basean zenbagarren hitza den
    char izena[32];		// Sarrerako datu-basean ageri den hitz zehatza
};
