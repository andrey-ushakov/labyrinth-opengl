
#ifndef _TIMER_H_
#define _TIMER_H_

#include "type.h"

class TIMER
{

  private:

    __int64 tick_frequency;      // nb de tick/s si on utilise le compteur high res 
    __int64 base_tick;           // donne une base fixe pour les tick
    bool  high_res_timer;      // dit si on utilise ou non le compteur high res

    float last_global_timer;   // en ms : dernier appel à l'horloge (pour gérer les var. global_timer_***)
    float local_timer_40_Hz;   // -  
    float local_timer_25_Hz;   //   \                                                                        *
    float local_timer_20_Hz;   //    |
    float local_timer_16_Hz;   //    | 
    float local_timer_10_Hz;   //    | 
    float local_timer_05_Hz;   //    |
    float local_timer_02_Hz;   //    |
    float local_timer_01s;     //    |
    float local_timer_02s;     //    |
    float local_timer_10s;     //     >  Compteurs qui somment le temps en ms. Dès que l'un dépasse son 
    float local_timer_30s;     //    |   temps, le booléen associé passe à true, et le compteur est RAZ.
    float local_timer_01m;     //    |
    float local_timer_02m;     //    |
    float local_timer_05m;     //    |
    float local_timer_10m;     //    /
    float local_timer_01h;     //  -


  public:

    bool  global_timer_40_Hz;		// 
    bool  global_timer_25_Hz;		// 
    bool  global_timer_20_Hz;		// 
    bool  global_timer_16_Hz;		// La série de booléens qui suivent sont modifiés par update_horloge().
    bool  global_timer_10_Hz;		// A chaque appel de la fonction, si le temps est supérieur au pas d'une
    bool  global_timer_05_Hz;		// des variables, celle-ci passe à true jusqu'au prochain appel. Cela 
    bool  global_timer_02_Hz;		// permet de regrouper en quelques variables tous les événements de type
    bool  global_timer_01s;			// temps. De plus la synchronisation est "naturelle". Ces booléens se 
    bool  global_timer_02s;			// comporte comme des ticks d'horloge, ils "tickent" en fonction de leur
    bool  global_timer_10s;			// propre fréquence.
    bool  global_timer_30s;			// La fonction update_horloge() doit être appelée à chaque frame pour une
    bool  global_timer_01m;			// bonne précision. Les variables global_timer_25_Hz et global_timer_40_Hz
    bool  global_timer_02m;			// peuvent ne pas être parfaitement précises si le frame rate descend en
    bool  global_timer_05m;			// desous de 25 FPS. La fonction reset_horloge() met les compteurs à false.
    bool  global_timer_10m;			//
    bool  global_timer_01h;			// Attention : global_timer_fps augmente à la vitesse du frame rate.
    dword global_timer_fps;			// 

     TIMER();
    ~TIMER();

    void	update_horloge();		// à apeller à CHAQUE DEBUT DE FRAME !
    void	reset_horloge();
    float	timer_get_ms();
    float	timer_get_us();
    void	pause_ms(dword ms);
    int		get_annee();
    int		get_mois();
    int		get_jour();
    int		get_heure();
    int		get_minute();
    int		get_seconde();

};



#endif

