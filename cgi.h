/* (C) Mr. Ka 1999*/
#if !defined __CGI_H
#define __CGI_H

#include "html.h"
#define ENV_INCREM 128 /* Увеличение размера выделяемого для переменной 64*/
#define MAX_PARAM 512 /* Длина предаваемого параметра 512*/

#define DATE_SEPARATOR '.' /* Разделитель в формате даты */
#define TIME_SEPARATOR ':' /* Разделитель в формате времени */
#define REQUEST_CREATER "WHO YOUR AUTHOR?"

#define WIN     0             /* Номер кодировки Windows в таблице перекодировок */
#define DOS     1             /* Номер кодировки Windows в таблице перекодировок */
#define KOI     2             /* Номер кодировки Windows в таблице перекодировок */
#define NUM_CH  3             /* Количество кодировок */

#define CGI_CONTENT "Content-type: text/html\r\n\n"

int IdentUser(char *szHost, char *szIP, char *szUser, char *szPasswd);

char *GetParamStr(void);/* Получить строку параметров */
char *UnCode(char *strData);
/* Вернуть значение запрашиваемого поля из переменной окружения QUERY_STRING (Author - например)*/
char *GetQueryValue(char *StrData, char *Field);

/* Определение количества знаков в числе */
int DefQvantDig(long lNum);
/* Преобразование числа в стоку заданной длины */
char *NumToFStr(long lNum, int iQvantDig, char *szNum);
/* Преобразование числа в стоку */
char *NumToStr(long lNum);
char *GetCurDate(char *szDateArr);
char *GetCurTime(char *szTimeArr);

int CodeStringDig(char *ptrTemp);/* Однозначно кодирует символы строки */
int StrRusToEng(char *Str);/* Переводит русские символы в их английские аналоги DOS*/
int StrEngToRus(char *Str);/* Переводит английские аналоги русск. симв. в рус. код. DOS*/
int QSToCharSet(char *QrChSet, char *Str);/* Переводит строку при CGI запросе в указанную кодировку */
int ReCodeCharSet(char *CurChSet, char *QrChSt, char *Str);/* Изменение набора символов */


void UpAllCaps(char *Text);/* Перевод всех символов DOS в заглавные */
char UpSymCase(char ch);

#endif