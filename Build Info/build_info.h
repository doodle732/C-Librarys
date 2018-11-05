/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> Build Infos
* Version           -> 1.0.0.1231
* Author            -> Jan Homann @ Workstadion.: XXXJANIIIX3XXX
* Build Date        -> 31.12.2017 11:19:49
* Description       -> Building infos from Preprocessor
*
*
*	Versions - String.:
*
*	Hauptversion(Major) | Nebenversion(Minior) | Fehler Korrekturen | Stunden & Minuten | Tag & Monat
*
*
*	Hauptversion.:			Gibt an wie oft die Firmware komplett neu erstellt wurde
*	Nebenversion.:			Gibt an ob es eine neue Subroutine gibt
*	Fehler Korrekturen.:	Sprechen f�r sich!
*	Stunden & Minuten.:		Zeit der letzten �bersetzung
*	Tag & Monat.:			Datum der letzten �bersetzung
*/

#ifndef _BUILD_INFO_H_
#define _BUILD_INFO_H_

/* buildVer
* @para             -> none
* @return           -> pointer @ string address
* @description      -> build info of project
*/
char *buildVer(void);


uint8_t *buildVerBytes(void);

#endif // _BUILD_INFO_H_
