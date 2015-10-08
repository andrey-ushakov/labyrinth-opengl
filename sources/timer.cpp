

#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"



/****************************************************************************\
*                                                                            *
*                            Variables EXTERNES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                            Variables GLOBALES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/




/**************************************************************************\
*                                                                          *
*  doit être appelé au début du programme, avant la classe configuration   *
*                                                                          *
\**************************************************************************/
TIMER::TIMER()
{
	tick_frequency = 0;
	base_tick      = 0;
	high_res_timer = false;

	LARGE_INTEGER t;

	if ( QueryPerformanceCounter(&t) == TRUE && QueryPerformanceFrequency(&t) == TRUE )
	{
		high_res_timer = true;
		QueryPerformanceFrequency(&t);
		tick_frequency = t.LowPart;
		QueryPerformanceCounter  (&t);
		base_tick      = t.LowPart;
	}
	else
	{
		high_res_timer  = false;
	}


	srand( (unsigned)time( NULL ) );
	reset_horloge();					// après l'initialisation !
}


/**************************************************************************\
*                                                                          *
*                                                                          *
*                                                                          *
\**************************************************************************/
TIMER::~TIMER()
{
	// rien pour le moment
}


/**************************************************************************\
*                                                                          *
* Nombre de ms depuis que le système a démarré                             *
*                                                                          *
\**************************************************************************/
float TIMER::timer_get_ms()
{
	LARGE_INTEGER t;

	if ( high_res_timer )             // précision : < 1 ms
	{
		QueryPerformanceCounter(&t);
		return (float)(1000.0f*(t.QuadPart - base_tick))/(float)tick_frequency;
	}

	return (float) timeGetTime();   // précision : environ 10 ms
}


/**************************************************************************\
*                                                                          *
*  renvoie l'heure (0-23)                                                  *
*                                                                          *
\**************************************************************************/
int TIMER::get_heure()
{
	time_t tps_sys;
	tm     *newtime;

	time(&tps_sys);
	newtime = localtime(&tps_sys);

	return newtime->tm_hour;
}


/**************************************************************************\
*                                                                          *
*  renvoie les minutes (0-59)                                              *
*                                                                          *
\**************************************************************************/
int TIMER::get_minute()
{
	time_t tps_sys;
	tm     *newtime;

	time(&tps_sys);
	newtime = localtime(&tps_sys);

	return newtime->tm_min;
}


/**************************************************************************\
*                                                                          *
*  renvoie les secondes (0-59)                                             *
*                                                                          *
\**************************************************************************/
int TIMER::get_seconde()
{
	time_t tps_sys;
	tm     *newtime;

	time(&tps_sys);
	newtime = localtime(&tps_sys);

	return newtime->tm_sec;
}


/**************************************************************************\
*                                                                          *
*  renvoie le jour (1-31)                                                  *
*                                                                          *
\**************************************************************************/
int TIMER::get_jour()
{
	time_t tps_sys;
	tm     *newtime;

	time(&tps_sys);
	newtime = localtime(&tps_sys);

	return newtime->tm_mday;
}


/**************************************************************************\
*                                                                          *
*  renvoie le mois (1-12)                                                  *
*                                                                          *
\**************************************************************************/
int TIMER::get_mois()
{
	time_t tps_sys;
	tm     *newtime;

	time(&tps_sys);
	newtime = localtime(&tps_sys);

	return newtime->tm_mon + 1;
}


/**************************************************************************\
*                                                                          *
*  renvoie l'année (-1990)                                                 *
*                                                                          *
\**************************************************************************/
int TIMER::get_annee()
{
	time_t tps_sys;
	tm     *newtime;

	time(&tps_sys);
	newtime = localtime(&tps_sys);

	return newtime->tm_year;
}


/**************************************************************************\
*                                                                          *
*  Fait une pause de ms millisecondes                                      *
*                                                                          *
\**************************************************************************/
void TIMER::pause_ms(dword ms)
{
	if (ms != 0) Sleep(ms);
}


/**************************************************************************\
*                                                                          *
* Nombre de us depuis que le système a démarré                             *
*                                                                          *
* à vérifier : pb potentiel de rebouclage à cause de 32 bits au lieu de 64 *
*                                                                          *
\**************************************************************************/
float TIMER::timer_get_us()
{
	LARGE_INTEGER t;

	if (high_res_timer)             // précision : < 1 ms
	{
		QueryPerformanceCounter(&t);
		return (float)(1000000.0f*(t.QuadPart - base_tick))/(float)tick_frequency;
	}

	return (float) timeGetTime() * 1000.0f;   // précision : environ 10 ms : pas cool !!!
}


/**************************************************************************\
*                                                                          *
*  Mets les horloges globales à false                                      *
*                                                                          *
\**************************************************************************/
void TIMER::reset_horloge()
{
	global_timer_40_Hz = false;  // 0,025  s
	global_timer_25_Hz = false;  // 0.04   s
	global_timer_20_Hz = false;  // 0.05   s
	global_timer_16_Hz = false;  // 0.0625 s
	global_timer_10_Hz = false;  // 0.10   s
	global_timer_05_Hz = false;  // 0.20   s
	global_timer_02_Hz = false;  // 0.50   s
	global_timer_01s   = false;
	global_timer_02s   = false;
	global_timer_10s   = false;
	global_timer_30s   = false;
	global_timer_01m   = false;
	global_timer_02m   = false;
	global_timer_05m   = false;
	global_timer_10m   = false;
	global_timer_01h   = false;

	global_timer_fps   = 0;
	last_global_timer  = timer_get_ms();

	local_timer_40_Hz = 0.0f;
	local_timer_25_Hz = 0.0f;
	local_timer_20_Hz = 0.0f;
	local_timer_16_Hz = 0.0f;
	local_timer_10_Hz = 0.0f;
	local_timer_05_Hz = 0.0f;
	local_timer_02_Hz = 0.0f;
	local_timer_01s   = 0.0f;
	local_timer_02s   = 0.0f;
	local_timer_10s   = 0.0f;
	local_timer_30s   = 0.0f;
	local_timer_01m   = 0.0f;
	local_timer_02m   = 0.0f;
	local_timer_05m   = 0.0f;
	local_timer_10m   = 0.0f;
	local_timer_01h   = 0.0f;
}


/**************************************************************************\
*                                                                          *
* Gère les horloges globales. A APPELER A CHAQUE FRAME: au moins 25 fois/s *
* si possible.                                                             *
*                                                                          *
\**************************************************************************/
void TIMER::update_horloge()
{
	float temps_passe,tmp;

	tmp               = timer_get_ms();
	temps_passe       = tmp - last_global_timer;
	last_global_timer = tmp;


	local_timer_40_Hz += temps_passe;
	local_timer_25_Hz += temps_passe;
	local_timer_20_Hz += temps_passe;
	local_timer_16_Hz += temps_passe;
	local_timer_10_Hz += temps_passe;
	local_timer_05_Hz += temps_passe;
	local_timer_02_Hz += temps_passe;
	local_timer_01s   += temps_passe;
	local_timer_02s   += temps_passe;
	local_timer_10s   += temps_passe;
	local_timer_30s   += temps_passe;
	local_timer_01m   += temps_passe;
	local_timer_02m   += temps_passe;
	local_timer_05m   += temps_passe;
	local_timer_10m   += temps_passe;
	local_timer_01h   += temps_passe;

	global_timer_40_Hz = false;  // 0.025  s
	global_timer_25_Hz = false;  // 0.04   s
	global_timer_20_Hz = false;  // 0.05   s
	global_timer_16_Hz = false;  // 0.0625 s
	global_timer_10_Hz = false;  // 0.10   s
	global_timer_05_Hz = false;  // 0.20   s
	global_timer_02_Hz = false;  // 0.50   s
	global_timer_01s   = false;
	global_timer_02s   = false;
	global_timer_10s   = false;
	global_timer_30s   = false;
	global_timer_01m   = false;
	global_timer_02m   = false;
	global_timer_05m   = false;
	global_timer_10m   = false;
	global_timer_01h   = false;

	// le while pour éviter l'effet cumul en cas de frame rate trop bas
	while (local_timer_40_Hz >=      25.0f) { local_timer_40_Hz -=      25.0f; global_timer_40_Hz = true; };
	while (local_timer_25_Hz >=      40.0f) { local_timer_25_Hz -=      40.0f; global_timer_25_Hz = true; };
	while (local_timer_20_Hz >=      50.0f) { local_timer_20_Hz -=      50.0f; global_timer_20_Hz = true; };
	while (local_timer_16_Hz >=      62.5f) { local_timer_16_Hz -=      62.5f; global_timer_16_Hz = true; };
	while (local_timer_10_Hz >=     100.0f) { local_timer_10_Hz -=     100.0f; global_timer_10_Hz = true; };
	while (local_timer_05_Hz >=     200.0f) { local_timer_05_Hz -=     200.0f; global_timer_05_Hz = true; };
	while (local_timer_02_Hz >=     500.0f) { local_timer_02_Hz -=     500.0f; global_timer_02_Hz = true; };
	while (local_timer_01s   >=    1000.0f) { local_timer_01s   -=    1000.0f; global_timer_01s   = true; };
	while (local_timer_02s   >=    2000.0f) { local_timer_02s   -=    2000.0f; global_timer_02s   = true; };
	while (local_timer_10s   >=   10000.0f) { local_timer_10s   -=   10000.0f; global_timer_10s   = true; };
	while (local_timer_30s   >=   30000.0f) { local_timer_30s   -=   30000.0f; global_timer_30s   = true; };
	while (local_timer_01m   >=   60000.0f) { local_timer_01m   -=   60000.0f; global_timer_01m   = true; };
	while (local_timer_02m   >=  120000.0f) { local_timer_02m   -=  120000.0f; global_timer_02m   = true; };
	while (local_timer_05m   >=  300000.0f) { local_timer_05m   -=  300000.0f; global_timer_05m   = true; };
	while (local_timer_10m   >=  600000.0f) { local_timer_10m   -=  600000.0f; global_timer_10m   = true; };
	while (local_timer_01h   >= 3600000.0f) { local_timer_01h   -= 3600000.0f; global_timer_01h   = true; };

	global_timer_fps++;

}

