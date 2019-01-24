/* (C) Mr. Ka 1999*/
/*#include <malloc.h> // For DOS only*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "html.h"
#include "cgitbl.h"

/* Функция идентификации пользователя по хосту, ip, имени пользователя и 
пароля. Не реализована */
int IdentUser(char *szHost, char *szIP, char *szUser, char *szPasswd)
{
  return 1;
}

/* Получение строки параметров для CGI */
char *GetParamStr(void)
{
  int ch;
  long i,lCL;
  char *strTemp, *szQS, *szRM, *szCL;

  strTemp=NULL;
  
  szRM=getenv("REQUEST_METHOD");
  if(!szRM)
  {
    fprintf(stderr,"CGI lib: Not define Request Method\n");
    return NULL;
  }
  
  if(!strcmp("GET",szRM))
  {
    szQS=getenv("QUERY_STRING");
    if(!szQS)
    {
      fprintf(stderr,"CGI lib: Method GET, not Query_String\n");
      return NULL;
    }    
    strTemp=malloc(strlen(szQS)+1);
    strcpy(strTemp,szQS);
  }
  else
    if(!strcmp("POST",szRM))
    {
      szCL=getenv("CONTENT_LENGTH");
      if(!szCL)
      {
        fprintf(stderr,"CGI lib: Method POST not Content_lenght \n");
        return NULL;
      }
      lCL=atol(szCL);
      strTemp=malloc(lCL+1);
      i=0;
      ch=fgetc(stdin);
      while(!feof(stdin))
      {
        strTemp[i++]=ch;
        ch=fgetc(stdin);	
       }
      strTemp[i]='\0';
    }
    
  return strTemp;
}

/* Переводим коды символов в сами символы */
char *UnCode(char *strData)
{
  int i, j, k;
  long len;
  char arTmp[2];
  char *szTmp;

  len=strlen(strData);
  j=0;
  for(j=0,i=0;i<len;i++,j++)
  {
    if(strData[i]=='%')/* Закодированный символ */
    {
      i++;
      arTmp[0]=strData[i];
      i++;
      arTmp[1]=strData[i];
      for(k=0;k<2;k++)
      {
        if(arTmp[k]<0x40)
          arTmp[k]=arTmp[k]-0x30;
        else
        {
          if(arTmp[k]<0x47)
            arTmp[k]=arTmp[k]-0x37;
          else
            if(arTmp[k]<0x67) arTmp[k]=arTmp[k]-0x57;
        }
      }
      strData[j]=arTmp[0]*0x10+arTmp[1];
    }
    else
    {
      if(strData[i]=='+')/* Заменяем + на пробел */
        strData[j]=' ';
      else
        strData[j]=strData[i];/* Если не закодированный символ скопировать */
    }
  }
  strData[j]='\0';
  
  len=strlen(strData);
  szTmp=malloc(sizeof(char)*len+1);
  if(szTmp)
  {
    strcpy(szTmp,strData);
    free(strData);
    return szTmp;
  }
  fprintf(stderr,"CGI Lib: Not memort for UnCode \n");
  return NULL;
}

/* Вернуть значение запрашиваемого поля из переменной окружения QUERY_STRING (Author - например)*/
char *GetQueryValue(char *StrData, char *Field)
{
  char *Dst, *szTmp;
  int iSizeBuff, i;

  iSizeBuff=MAX_PARAM;
  Dst=malloc(iSizeBuff);
  if(Dst==NULL)
  {
    fprintf(stderr,"CGI lib:GetQueryValue, Field %s. No memory for Dst.\n", Field);
    return NULL;
  }
  do
  {
    if((!strncmp(StrData, Field, strlen(Field))) && (StrData[strlen(Field)] == '='))
    {
      /* Parameter found! */
      StrData += strlen(Field) + 1;
      i=0;
      while((StrData[i] != '&') && (StrData[i] != '\0'))
      {
        Dst[i] = StrData[i];
        if(i==(iSizeBuff-2))
        {
          iSizeBuff+=ENV_INCREM;
          szTmp=malloc(iSizeBuff);
	  if(!szTmp)
	  {
            fprintf(stderr,"CGI Error:GetQueryValue, Field %s. No memory for resize Dst.\n", Field);
            return NULL;
	  }
	  strcpy(szTmp,Dst);
          free(Dst);
	  Dst=szTmp;
        }
        i++;
      }
      Dst[i] = '\0';
      return Dst;
    }
    /* Next parameter */
    while((*StrData != '&') && (*StrData != '\0'))
      StrData++;
  }
  while(*(StrData++) != '\0');
  free(Dst);

/*  fprintf(stderr,"%sCGI Error: GetValue. Not Field %s",HTML_NXTSTR, Field); /* For Debug*/
  return "";
}

/* Воззведение 10 в степень */
long Pow10(long x)
{
  long i;
  long y=1;
  for(i=0;i<x;i++)
  {
     y*=10;
  }
  return y;
}

/* Определение количества знаков в числе */
int DefQvantDig(long lNum)
{
  int i;

  i=1;
  while((lNum=lNum/10)>0)
   i++;
  return i;
}

/* Преобразование числа в стоку заданной длины */
char *NumToFStr(long lNum, int iQvantDig, char *szNum)
{
  long lTmp;
  int i,j;

  for(i=0,j=iQvantDig-1;i<iQvantDig;i++,j--)
  {
    lTmp=lNum;
    lNum=lNum/Pow10(i+1);
    lNum*=Pow10(i+1);
    szNum[j]=(lTmp-lNum)/Pow10(i)+0x30;
  }
  szNum[iQvantDig]='\0';
  return szNum;
}

/* Преобразование числа в стоку */
char *NumToStr(long lNum)
{
  char *szNum;

  szNum=malloc(DefQvantDig(lNum)+1);
  if(szNum==NULL)
  {
    fprintf(stderr,"CGI Lib: no memory for NumToStr\n");
    return NULL;
  }
  return NumToFStr(lNum, DefQvantDig(lNum), szNum);
}

char *GetCurDate(char *szDateArr)
{
  time_t timer;
  struct tm *tblock;

  /* gets time of day */
  timer=time(NULL);
  tblock=localtime(&timer);

  tblock->tm_year+=1900;/* Исправление ошибки 2000 */
  NumToFStr(tblock->tm_mday,2,szDateArr);
  szDateArr[2]=DATE_SEPARATOR;
  NumToFStr(tblock->tm_mon,2,(szDateArr+3));
  szDateArr[5]=DATE_SEPARATOR;
  NumToFStr(tblock->tm_year,4,(szDateArr+6));
  return szDateArr;
}

char *GetCurTime(char *szTimeArr)
{
  time_t timer;
  struct tm *tblock;

  /* gets time of day */
  timer=time(NULL);
  tblock=localtime(&timer);

  NumToFStr(tblock->tm_hour,2,szTimeArr);
  szTimeArr[2]=TIME_SEPARATOR;
  NumToFStr(tblock->tm_min,2,(szTimeArr+3));
  szTimeArr[5]=TIME_SEPARATOR;
  NumToFStr(tblock->tm_sec,2,(szTimeArr+6));
  return szTimeArr;
}

int CodeStringDig(char *ptrTemp)
{
  char ch;
  char *szTemp;
  int len, i, j;

  szTemp=ptrTemp;
  while(*ptrTemp!='\0')
  {
    switch (*ptrTemp)
    {
      case '0': *ptrTemp='s'; break;
      case '1': *ptrTemp='e'; break;
      case '2': *ptrTemp='j'; break;
      case '3': *ptrTemp='q'; break;
      case '4': *ptrTemp='p'; break;
      case '5': *ptrTemp='n'; break;
      case '6': *ptrTemp='z'; break;
      case '7': *ptrTemp='f'; break;
      case '8': *ptrTemp='d'; break;
      case '9': *ptrTemp='c'; break;
    }
    ptrTemp++;
  }
  len=strlen(szTemp);
  for(i=0,j=len-1;i<len/2;i++,j--)
  {
    ch=szTemp[i];
    szTemp[i]=szTemp[j];
    szTemp[j]=ch;
  }
  for(i=0,j=(len-1)/2;i<(len-1)/2;i++,j--)
  {
    ch=szTemp[i];
    szTemp[i]=szTemp[j];
    szTemp[j]=ch;
  }
  for(i=len/2,j=len-1;i<(len-1);i++,j--)
  {
    ch=szTemp[i];
    szTemp[i]=szTemp[j];
    szTemp[j]=ch;
  }
  return 1;
}

int StrEngToRus(char *Str)
{
/* Не работает, при встерче символов x и др. будет затирать не выделенную область памяти */

  while(*Str!='\0')
  {
    switch (*Str)
    {
      case 'a': *Str='а'; break;
      case 'b': *Str='б'; break;
      case 'c': *Str='ц'; break;
      case 'd': *Str='д'; break;
      case 'e': *Str='е'; break;
      case 'f': *Str='ф'; break;
      case 'g': *Str='г'; break;
      case 'h': *Str='х'; break;
      case 'i': *Str='ш'; break;
      case 'j': *Str='ж'; break;
      case 'k': *Str='к'; break;
      case 'l': *Str='л'; break;
      case 'm': *Str='м'; break;
      case 'n': *Str='н'; break;
      case 'o': *Str='о'; break;
      case 'p': *Str='п'; break;
      case 'q': *Str='к'; break;
      case 'r': *Str='р'; break;
      case 's': *Str='с'; break;
      case 't': *Str='т'; break;
      case 'u': *Str='у'; break;
      case 'v': *Str='в'; break;
      case 'w': *Str='в'; break;
      case 'x': *Str='к'; Str++; *Str='с'; break;
      case 'y': *Str='й'; break;
      case 'z': *Str='з'; break;
      case 'A': *Str='А'; break;
      case 'B': *Str='Б'; break;
      case 'C': *Str='Ц'; break;
      case 'D': *Str='Д'; break;
      case 'E': *Str='Е'; break;
      case 'F': *Str='Ф'; break;
      case 'G': *Str='Г'; break;
      case 'H': *Str='Х'; break;
      case 'I': *Str='Ш'; break;
      case 'J': *Str='Ж'; break;
      case 'K': *Str='К'; break;
      case 'L': *Str='Л'; break;
      case 'M': *Str='М'; break;
      case 'N': *Str='Н'; break;
      case 'O': *Str='О'; break;
      case 'P': *Str='П'; break;
      case 'Q': *Str='К'; break;
      case 'R': *Str='Р'; break;
      case 'S': *Str='С'; break;
      case 'T': *Str='Т'; break;
      case 'U': *Str='У'; break;
      case 'V': *Str='В'; break;
      case 'W': *Str='В'; break;
      case 'X': *Str='К'; Str++; *Str='C'; break;
      case 'Y': *Str='Й'; break;
      case 'Z': *Str='З'; break;
    }
    Str++;
  }
  return 0;
}

int StrRusToEng(char *Str)
{
/* Не работает, при встерче символов ш,щ и др. будет затирать не выделенную область памяти */

  while(*Str!='\0')
  {
    switch (*Str)
    {
      case 'а': *Str='a'; break;
      case 'б': *Str='b'; break;
      case 'в': *Str='v'; break;
      case 'г': *Str='g'; break;
      case 'д': *Str='d'; break;
      case 'е': *Str='e'; break;
      case 'ё': *Str='e'; break;
      case 'ж': *Str='j'; break;
      case 'з': *Str='z'; break;
      case 'и': *Str='i'; break;
      case 'й': *Str='y'; break;
      case 'к': *Str='k'; break;
      case 'л': *Str='l'; break;
      case 'м': *Str='m'; break;
      case 'н': *Str='n'; break;
      case 'о': *Str='o'; break;
      case 'п': *Str='p'; break;
      case 'р': *Str='r'; break;
      case 'с': *Str='s'; break;
      case 'т': *Str='t'; break;
      case 'у': *Str='u'; break;
      case 'ф': *Str='f'; break;
      case 'х': *Str='h'; break;
      case 'ц': *Str='c'; break;
      case 'ч': *Str='c'; Str++; *Str='h'; break;
      case 'ш': *Str='s'; Str++; *Str='h'; break;
      case 'щ': *Str='t'; Str++; *Str='c'; Str++; *Str='h'; break;
      case 'ъ': break;
      case 'ы': *Str='i'; break;
      case 'ь': break;
      case 'э': *Str='e'; break;
      case 'ю': *Str='y'; Str++; *Str='u'; break;
      case 'я': *Str='y'; Str++; *Str='a'; break;
      case 'А': *Str='A'; break;
      case 'Б': *Str='B'; break;
      case 'В': *Str='V'; break;
      case 'Г': *Str='G'; break;
      case 'Д': *Str='D'; break;
      case 'Е': *Str='E'; break;
      case 'Ё': *Str='E'; break;
      case 'Ж': *Str='J'; break;
      case 'З': *Str='Z'; break;
      case 'И': *Str='I'; break;
      case 'Й': *Str='Y'; break;
      case 'К': *Str='K'; break;
      case 'Л': *Str='L'; break;
      case 'М': *Str='M'; break;
      case 'Н': *Str='N'; break;
      case 'О': *Str='O'; break;
      case 'П': *Str='P'; break;
      case 'Р': *Str='R'; break;
      case 'С': *Str='S'; break;
      case 'Т': *Str='T'; break;
      case 'У': *Str='U'; break;
      case 'Ф': *Str='F'; break;
      case 'Х': *Str='H'; break;
      case 'Ц': *Str='C'; break;
      case 'Ч': *Str='C'; Str++; *Str='h'; break;
      case 'Ш': *Str='S'; Str++; *Str='h'; break;
      case 'Щ': *Str='T'; Str++; *Str='c'; Str++; *Str='h'; break;
      case 'Ъ': break;
      case 'Ы': *Str='I'; break;
      case 'Ь': break;
      case 'Э': *Str='E'; break;
      case 'Ю': *Str='Y'; Str++; *Str='u'; break;
      case 'Я': *Str='Y'; Str++; *Str='a'; break;
    }
    Str++;
  }
  return 0;
}


/* Переводит строку при CGI запросе в указанную кодировку */
int QSToCharSet(char *QrChSet, char *Str)
{
  char *CurChSet;/* Текущий набор символо*/

  CurChSet=getenv("CHARSET");
  if(CurChSet)
    return ReCodeCharSet(CurChSet, QrChSet, Str);
  return 1;
}

int ReCodeCharSet(char *CurChSet, char *QrChSet, char *Str)/* Изменение набора символов */
{
  int NmRecCol;/* Номер столбца перекодировки*/

  NmRecCol=0; /* На случай ошибки кодировка по умолчанию 0 */
  if(!strcmp(CurChSet, "windows-1251"))
    NmRecCol=WIN*NUM_CH;
  else
    if(!strcmp(CurChSet, "ibm866"))
      NmRecCol=DOS*NUM_CH;
    else
      if(!strcmp(CurChSet, "koi8-r"))
        NmRecCol=KOI*NUM_CH;
      else
        return 1;

  if(!strcmp(QrChSet, "windows-1251"))
    NmRecCol+=WIN;
  else
    if(!strcmp(QrChSet, "ibm866"))
      NmRecCol+=DOS;
    else
      if(!strcmp(QrChSet, "koi8-r"))
        NmRecCol+=KOI;
      else
        return 1;


  while(*Str!='\0')
  {
    *Str=ChTable[(unsigned char)*Str][NmRecCol];
    Str++;
  }
  return 0;
}


/* Перевод всех символов DOS в заглавные */
void UpAllCaps(char *Text)
{
  int Cnt;

  for(Cnt = 0; Text[Cnt] != '\0'; Cnt++)
    Text[Cnt]=UpSymCase(Text[Cnt]);
}

char UpSymCase(char ch)
{
  if(((ch >= 'a') && (ch <= 'z')) || ((ch >= 'а') && (ch <= 'п')))
    ch -= 32;
  else
    if((ch >= 'р') && (ch <= 'я'))
      ch -= 80;
    else
      if(ch == 'ё')
         ch--;
  return ch;
}