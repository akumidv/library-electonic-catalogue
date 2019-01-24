#if !defined __GETPRM__H
#define __GETPRM__H

/* Field/subfield codes: */
unsigned int uigQntBases; /* Количество баз */
unsigned int uigSrchFld; /* Количество полей для поиска */
unsigned int uigVwFld; /* Количество выдаваемых полей */
unsigned int uigIndFld; /* Количество дополнительно индексируемых полей */

char *szgPath;
char *szgPathCreate;
char *szgBaseHTML; /* Поле HTML для переменной Base */
char **szgBaseSelNm;/* Названия баз для выбора */
char **szgBaseName; /* Названия баз */
char **szgSrchFld; /* Поля для поиска */
char **szgSrchHTML; /* Поля HTML для поиска */
char **szgSrchSelHTML; /* Поля HTML Sel для поиска */
char **szgNameSrchHTML; /* Названия полей HTML для поиска */
char **szgVwFld; /* Поля для просмотра */
char **szgNameVwHTML; /* Названия полей для просмтотра */
char ***szgIndFld;/* Названия полей для индекса */

char *szgORecNum; /* Поле HTML для переменной ORecNum */
char *szgAndWord; /* Поле HTML для переменной AndWord */
char *szgAllInOne; /* Поле HTML для переменной AllInOne */
char *szgAllInOneSel; /* Поле HTML для переменной AllInOneSel */

char *szgFileHTMLHead;
char *szgFileHTMLFoot;

unsigned int uigQntOutRec;
unsigned int uigMaxRqWord;

char *szgCGILog;
char *szgPortKOI;
char *szgPortDOS;
char *szgPortWIN;
char *szgPortALT;
char *szgHelpAddr;

char *szgHTMLHeader;
char *szgHTMLFooter;

#endif