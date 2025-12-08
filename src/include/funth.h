/*  Konputagailuen Arkitektura - Informatika Ingeniaritza (IF - EHU)
    OpenMP laborategia - PROIEKTUA

    funth.h
    taldegen fitxategian erabiltzen diren funtzioen goiburukoak
*******************************************************************/

extern void   hasierako_zentroideak 
              	(float zent[][ALDAKOP]);

extern int    zentroide_berriak     
		(float hitz[][ALDAKOP], 
		 float zent[][ALDAKOP], 
		 int   *sailka, 
		 int   hitzkop
		);

extern double hitzen_distantzia  
		(float *hitz1, 
		 float *hitz2
		);

extern void   multzo_gertuena
	        (int   hitzkop, 
	         float hitz[][ALDAKOP], 
	         float zent[][ALDAKOP], 
	         int   *sailka
	        );

extern double balidazioa
            	(float  hitz[][ALDAKOP], 
 	         struct multzoinfo *kideak, 
		 float  zent[][ALDAKOP], 
		 float  *multzo_trinko
		);

extern void   ztalorren_analisia 
        	(struct multzoinfo *kideak, 
		 float  alor[][ALORRA], 
		 struct analisia *alordist
		);

extern void medoideak_kalkulatu(
	float hitz[][ALDAKOP], 
	struct multzoinfo *kideak, 
	struct medoide **medoideak
);

extern void hasieratu_unesco_kodeak(
	struct unesco unesko_kodeak[]
);