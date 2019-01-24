#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include    <sys/malloc.h>
/*#include  <malloc.h>*/

#include "getprm_.h"
#include "getprm.h"
#include "html.h"
#include "cfg.h"
#include "cgi.h"

unsigned long FileSize(FILE *fh);

int GetINI(char *szCfgFileName)
{
  unsigned int i,j;
  char *szCfgData;
  int ch;
  unsigned char blComment;
  char *szTmp, *szTmp1, *szTmp2;
  
  FILE    *fh;
  unsigned long lFileSize, l;

  
  fh = fopen(szCfgFileName, "r");
  if(fh==NULL)
  {
    fprintf(stderr,"GetINI Error: not open config file");
    return FALSE;
  }
  lFileSize=FileSize(fh);

  szCfgData=malloc(sizeof(char)*(lFileSize+1));
  if(szCfgData==NULL)
  {
    fprintf(stderr,"GetINI Error: no memory for reading config");
    return FALSE;
  }
  i=0;
  blComment=FALSE;
  do
  {
    ch=fgetc(fh);
    if(blComment==FALSE)
    {
      if(ch!='#')
        szCfgData[i++]=ch;
      else
        blComment=TRUE;
    }
    else
      if(ch=='\n')
        blComment=FALSE;
  }
  while(ch!=EOF);
  szCfgData[i-1]='\0';
  fclose(fh);

  szTmp=GetCfgValue(szCfgData,"QntOutRec");
  if(szTmp!=NULL)
  {
    uigQntOutRec=atoi(szTmp);
    free(szTmp);  
  }
  else
    uigQntOutRec=10;

  szTmp=GetCfgValue(szCfgData,"MaxRqWord");
  if(szTmp!=NULL)
  {
    uigMaxRqWord=atoi(szTmp);
    free(szTmp);  
  }
  else
    uigMaxRqWord=10;

  szgFileHTMLHead=GetCfgValue(szCfgData,"FileHTMLHead");
  if(szgFileHTMLHead==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read FileHTMLHead value");
    return FALSE;
  }
  fh = fopen(szgFileHTMLHead, "r");
  if(fh==NULL)
  {
    fprintf(stderr,"GetINI Error: not open HTML header file");
    return FALSE;
  }
  lFileSize=FileSize(fh);

  szgHTMLHeader=malloc(sizeof(char)*(lFileSize+1));
  if(szgHTMLHeader==NULL)
  {
    fprintf(stderr,"GetINI Error: no memory for reading HTML header");
    fclose(fh);
    return FALSE;
  }
  i=0;
  do
  {
    ch=fgetc(fh);
    szgHTMLHeader[i++]=ch;
  }
  while(ch!=EOF);
  szgHTMLHeader[i-1]='\0';
  fclose(fh);

  szgFileHTMLFoot=GetCfgValue(szCfgData,"FileHTMLFoot");
  if(szgFileHTMLFoot==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read FileHTMLFoot value");
    return FALSE;
  }
  fh = fopen(szgFileHTMLFoot, "r");
  if(fh==NULL)
  {
    fprintf(stderr,"GetINI Error: not open HTML footer file");

    return FALSE;
  }
  lFileSize=FileSize(fh);

  szgHTMLFooter=malloc(sizeof(char)*(lFileSize+1));
  if(szgHTMLFooter==NULL)
  {
    fprintf(stderr,"GetINI Error: no memory for reading footer");
    fclose(fh);
    return FALSE;
  }
  i=0;
  do
  {
    ch=fgetc(fh);
    szgHTMLFooter[i++]=ch;
  }
  while(ch!=EOF);
  szgHTMLFooter[i-1]='\0';
  fclose(fh);

  szgCGILog=GetCfgValue(szCfgData,"CGILog");
  if(szgCGILog==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read CGILog value");
    return FALSE;
  }
  szgPortKOI=GetCfgValue(szCfgData,"PortKOI");
  if(szgPortKOI==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read PortKOI value");
    return FALSE;
  }
  szgPortDOS=GetCfgValue(szCfgData,"PortDOS");
  if(szgPortDOS==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read PortDOS value");
    return FALSE;
  }    
  szgPortWIN=GetCfgValue(szCfgData,"PortWIN");
  if(szgPortWIN==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read PortWIN value");
    return FALSE;
  }
  szgPortALT=GetCfgValue(szCfgData,"PortALT");
  if(szgPortALT==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read PortALT value");
    return FALSE;
  }  
  szgHelpAddr=GetCfgValue(szCfgData,"HelpAddr");
  if(szgHelpAddr==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read HelpAddr value");
    return FALSE;
  }

  szTmp=GetCfgValue(szCfgData,"QntSrchFld");
  if(szTmp!=NULL)
  {
    uigSrchFld=atoi(szTmp);
    free(szTmp);  
  }
  else
    uigSrchFld=0;

  szgSrchFld=malloc(sizeof(char *)*uigSrchFld);
  if(szgSrchFld==NULL)
  {
    fprintf(stderr,"GetINI Error: no memory for set Search Fields");
    return FALSE;
  }  
  for(i=0;i<uigSrchFld;i++)
  {
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set Search Fields - szTmp1");
      return FALSE;
    }
    szTmp=malloc(sizeof(char)*(strlen("SrchFld")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set Search Fields - szTmp");
      free(szTmp1);
      return FALSE;
    }
    strcpy(szTmp,"SrchFld");
    strcat(szTmp,szTmp1);
    szgSrchFld[i]=GetCfgValue(szCfgData,szTmp);
    if(szgSrchFld[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value in SearchField",szTmp);
      return FALSE;
    }
    free(szTmp1);
    free(szTmp);
  }
  szTmp=GetCfgValue(szCfgData,"QntIndFld");
  if(szTmp!=NULL)
  {
    uigIndFld=atoi(szTmp);
    free(szTmp);
  }
  else
    uigIndFld=0;

  szgIndFld=malloc(sizeof(char **)*uigSrchFld);
  if(szgIndFld==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set Indexing Fields");
     return FALSE;
  }
  for(i=0;i<uigSrchFld;i++)
  {
    szgIndFld[i]=malloc(sizeof(char *)*uigIndFld);
    if(szgIndFld[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set Indexing sub Fields");
      return FALSE;
    }
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set Indexing Fields - szTmp1");
      return FALSE;
    }
    for(j=0;j<uigIndFld;j++)
    {
      szTmp2=NumToStr(j);
      if(szTmp2==NULL)
      {
        fprintf(stderr,"GetINI Error: no memory for set Indexing Fields - szTmp2");
        return FALSE;
      }
      szTmp=malloc(sizeof(char)*(strlen("IndFld")+strlen(szTmp1)+strlen("_")+strlen(szTmp2)+1));
      if(szTmp==NULL)
      {
        fprintf(stderr,"GetINI Error: no memory for set Indexing Fields - szTmp");
        return FALSE;
      }
      strcpy(szTmp,"IndFld");
      strcat(szTmp,szTmp1);
      strcat(szTmp,"_");
      strcat(szTmp,szTmp2);
      szgIndFld[i][j]=GetCfgValue(szCfgData,szTmp);
      if(j==0)
      {
        if(szgIndFld[i][j]==NULL)
        {
          fprintf(stderr,"GetINI Error: Not read %s value in Indexing Field",szTmp);
          return FALSE;
        }
      }
      free(szTmp2);
      free(szTmp);
    }
    free(szTmp1);
  }

  szgSrchHTML=malloc(sizeof(char *)*uigSrchFld);
  if(szgSrchHTML==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set SrchHTML");
     return FALSE;
  }
  for(i=0;i<uigSrchFld;i++)
  {
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set SrchHTML - szTmp1");
      return FALSE;
    }
    szTmp=malloc(sizeof(char)*(strlen("SrchHTML")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set SrchHTML - szTmp");
      return FALSE;
    }
    strcpy(szTmp,"SrchHTML");
    strcat(szTmp,szTmp1);
    szgSrchHTML[i]=GetCfgValue(szCfgData,szTmp);
    if(szgSrchHTML[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value in SrchHTML",szTmp);
      return FALSE;
    }
    free(szTmp1);
    free(szTmp);
  }  
  
  szgSrchSelHTML=malloc(sizeof(char *)*uigSrchFld);
  if(szgSrchSelHTML==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set SrchSelHTML");
     return FALSE;
  }
  for(i=0;i<uigSrchFld;i++)
  {
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set SrchSelHTML - szTmp1");
      return FALSE;
    }
    szTmp=malloc(sizeof(char)*(strlen("SrchSelHTML")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set SrchSelHTML - szTmp");
      return FALSE;
    }
    strcpy(szTmp,"SrchSelHTML");
    strcat(szTmp,szTmp1);
    szgSrchSelHTML[i]=GetCfgValue(szCfgData,szTmp);
    if(szgSrchSelHTML[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value SrchSelHTML",szTmp);
      return FALSE;
    }
    free(szTmp1);
    free(szTmp);
  }    

  szgNameSrchHTML=malloc(sizeof(char *)*uigSrchFld);
  if(szgNameSrchHTML==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set NameSrchHTML");
     return FALSE;
  }
  for(i=0;i<uigSrchFld;i++)
  {
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set NameSrchHTML - szTmp1");
      return FALSE;
    }
    szTmp=malloc(sizeof(char)*(strlen("NameSrchHTML")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set NameSrchHTML - szTmp");
      return FALSE;
    }
    strcpy(szTmp,"NameSrchHTML");
    strcat(szTmp,szTmp1);
    szgNameSrchHTML[i]=GetCfgValue(szCfgData,szTmp);
    if(szgNameSrchHTML[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value NameSrchHTML",szTmp);
      return FALSE;
    }
    free(szTmp1);
    free(szTmp);
  }

  szTmp=GetCfgValue(szCfgData,"QntVwFld");
  if(szTmp!=NULL)
  {
    uigVwFld=atoi(szTmp);
    free(szTmp);  
  }
  else
    uigVwFld=0;

  szgVwFld=malloc(sizeof(char *)*uigVwFld);
  if(szgVwFld==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set VwFld");
     return FALSE;
  }
  for(i=0;i<uigVwFld;i++)
  {
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set VwFld - szTmp1");
      return FALSE;
    }
    szTmp=malloc(sizeof(char)*(strlen("VwFld")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set VwFld - szTmp");
      return FALSE;
    }
    strcpy(szTmp,"VwFld");
    strcat(szTmp,szTmp1);
    szgVwFld[i]=GetCfgValue(szCfgData,szTmp);
    if(szgVwFld[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value VwFld",szTmp);
      return FALSE;
    }
    free(szTmp1);
    free(szTmp);
  }

  szgNameVwHTML=malloc(sizeof(char *)*uigVwFld);
  if(szgNameVwHTML==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set VwHTML");
     return FALSE;
  }
  for(i=0;i<uigVwFld;i++)
  {
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set VwHTML - szTmp1");
      return FALSE;
    }
    szTmp=malloc(sizeof(char)*(strlen("NameVwHTML")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set VwHTML - szTmp");
      return FALSE;
    }
    strcpy(szTmp,"NameVwHTML");
    strcat(szTmp,szTmp1);
    szgNameVwHTML[i]=GetCfgValue(szCfgData,szTmp);
    if(szgNameVwHTML[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value VwHML",szTmp);
      return FALSE;
    }
    free(szTmp1);
    free(szTmp);
  }

  szTmp=GetCfgValue(szCfgData,"QntBases");
  if(szTmp!=NULL)
  {
    uigQntBases=atoi(szTmp);
    free(szTmp);  
  }
  else
    uigQntBases=1;
 
  szgBaseName=malloc(sizeof(char *)*uigQntBases);
  if(szgBaseName==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set BaseName");
     return FALSE;
  }

  szgBaseSelNm=malloc(sizeof(char *)*uigQntBases);
  if(szgBaseSelNm==NULL)
  {
     fprintf(stderr,"GetINI Error: no memory for set BaseSelNm");
     return FALSE;
  }  
  szgPath=GetCfgValue(szCfgData,"Path");
  if(szgPath==NULL)
  {
    fprintf(stderr,"GetINI Error: no memory for set base Path");
    return FALSE;
  }
  
  szgPathCreate=GetCfgValue(szCfgData,"PathCreate");
  if(szgPathCreate==NULL)
  {
    fprintf(stderr,"GetINI Error: no memory for set base CreatePath");
    return FALSE;
  }  
  
  for(i=0;i<uigQntBases;i++)
  {
    szTmp1=NumToStr(i);
    if(szTmp1==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set BaseName - szTmp1");
      return FALSE;
    }
    szTmp=malloc(sizeof(char)*(strlen("BaseName")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set BaseName - szTmp");
      return FALSE;
    }
    strcpy(szTmp,"BaseName");
    strcat(szTmp,szTmp1);
    szgBaseName[i]=GetCfgValue(szCfgData,szTmp);
    if(szgBaseName[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value BaseName",szTmp);
      return FALSE;
    }
    free(szTmp);

    szTmp=malloc(sizeof(char)*(strlen("BaseSelName")+strlen(szTmp1)+1));
    if(szTmp==NULL)
    {
      fprintf(stderr,"GetINI Error: no memory for set BaseSelName - szTmp");
      return FALSE;
    }
    strcpy(szTmp,"BaseSelName");
    strcat(szTmp,szTmp1);
    szgBaseSelNm[i]=GetCfgValue(szCfgData,szTmp);
    if(szgBaseSelNm[i]==NULL)
    {
      fprintf(stderr,"GetINI Error: Not read %s value BaseSelName",szTmp);
      return FALSE;
    }    
    free(szTmp);
    free(szTmp1);
  }

  szgBaseHTML=GetCfgValue(szCfgData,"BaseHTML");
  if(szgBaseHTML==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read BaseHTML value");
    return FALSE;
  }    
  szgORecNum=GetCfgValue(szCfgData,"ORecNum");
  if(szgORecNum==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read ORecNum value");
    return FALSE;
  }    
  szgAndWord=GetCfgValue(szCfgData,"AndWord");
  if(szgAndWord==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read AndWord value");
    return FALSE;
  }    
  szgAllInOne=GetCfgValue(szCfgData,"AllInOne");
  if(szgAllInOne==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read AllInOne value");
    return FALSE;
  }
  szgAllInOneSel=GetCfgValue(szCfgData,"AllInOneSel");
  if(szgAllInOneSel==NULL)
  {
    fprintf(stderr,"GetINI Error: Not read AllInOneSel value");
    return FALSE;
  }    

  free(szCfgData);
  return TRUE;
}

unsigned long FileSize(FILE *fh)
{
  unsigned long lCurFilePos, lFileSize;

  lCurFilePos=ftell(fh);
  fseek(fh,0L,SEEK_END);
  lFileSize=ftell(fh);
  fseek(fh,lCurFilePos,SEEK_SET);
  return lFileSize;
}
