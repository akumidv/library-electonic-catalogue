/* (C) Mr. Ka 1999*/
/*#include <malloc.h> // For DOS only*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cfg.h"
#include "cfg_.h"
#include "html.h"

/* Вернуть значение параметра из массива данных файла конфигурации */
char *GetCfgValue(char *StrData, char *szNameValue)
{
  char *Dst, *szTmp;
  int iSizeBuff, i;
  unsigned char blQuat;

  iSizeBuff=MAX_PARAM;
  Dst=malloc(iSizeBuff);
  if(Dst==NULL)
  {
    fprintf(stderr,"CGF Error:GetCfgValue, Value %s. No memory for Dst.\n", szNameValue);
    return FALSE;
  }
  do
  {
    if((!strncmp(StrData, szNameValue, strlen(szNameValue))) && (StrData[strlen(szNameValue)] == ' '))
    {
      /* Parameter found! */
      StrData += strlen(szNameValue) + 1;

      while(*StrData==' ' || *StrData=='\t')
        StrData++;
      blQuat=FALSE;
      if(*StrData=='"')
      {
        blQuat=TRUE;
	StrData++;
      }
      i=0;
      while((StrData[i] != '\n') && (StrData[i] != '\0') && (StrData[i] != '"'))
      {
        if((StrData[i]==' '|| *StrData=='\t') && blQuat==FALSE)
	  break;
        Dst[i] = StrData[i];
        if(i==(iSizeBuff-2))
        {
          iSizeBuff+=ENV_INCREM;
          szTmp=malloc(iSizeBuff);
	  if(!szTmp)
	  {
            fprintf(stderr,"CFG Error:GetCfgValue, Value %s. No memory for resize Dst.\n", szNameValue);
            return FALSE;
	  }
	  strcpy(szTmp,Dst);
          free(Dst);
	  Dst=szTmp;
        }
        i++;
      }
      Dst[i] = '\0';
      if(i>0)
        return Dst;
    }
    /* Next parameter */
    while((*StrData != '\n') && (*StrData != '\0'))
      StrData++;
  }
  while(*(StrData++) != '\0');
  free(Dst);

/*  fprintf(stderr,"%sCFG Error: GetCfgValue. Not Value %s",HTML_NXTSTR, szNameValue); /* For Debug*/
  return NULL;
}

