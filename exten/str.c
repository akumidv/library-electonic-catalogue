/* (C) Mr. Ka 1999*/
#include <string.h>
#include "str.h"
#include "cgi.h"

char* MarkStr(char *szMrkLf, char *szMrkRt, char *szStrSrc, char *szStrSrh)
{
  char *szDes,*szCopy;
  char *szTmp,*szTmp1;
  int i, iQnt;
  unsigned int iLenSrc, iLenMrkLf,iLenMrkRt,iLenSrh, iTmp;
  
  iQnt=0;

  iLenSrc=strlen(szStrSrc);
  iLenMrkLf=strlen(szMrkLf);
  iLenMrkRt=strlen(szMrkRt);
  iLenSrh=strlen(szStrSrh);
  szCopy=malloc(iLenSrc+1);
  printf("\n%s",szCopy);
  if(szCopy==NULL)
    return szStrSrc;
  strcpy(szCopy,szStrSrc);
  UpAllCaps(szCopy);
  printf("\n%s",szCopy);
  szTmp=szCopy;
  while((szTmp=strstr(szTmp,szStrSrh))!=NULL)
  {
    szTmp++;
    iQnt++;
  }
  if(iQnt==0)
    return szStrSrc;
  iTmp=iLenSrc+iQnt*iLenMrkLf+iQnt*iLenMrkRt+1;
  szDes=malloc(iTmp);

  if(szDes)
  {
    szTmp1=szCopy;
    szDes[0]='\0';
    for(i=0;i<iQnt;i++)
    {
      szTmp=strstr(szTmp1,szStrSrh);
      strncat(szDes,szStrSrc,(unsigned int)(szTmp-szTmp1));
      printf("\n%s",szDes);
      strcat(szDes,szMrkLf);
      printf("\n%s",szDes);      
      szStrSrc+=(unsigned int)(szTmp-szTmp1);
      strncat(szDes,szStrSrc,iLenSrh);
      printf("\n%s",szDes);
      szStrSrc+=iLenSrh;
      strcat(szDes,szMrkRt);
      szTmp1=szTmp;
      printf("\n%s",szDes);
    }
    return szDes;
  }
  return NULL;
}