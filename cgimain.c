#include  <time.h>
#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include    <sys/malloc.h>
/*#include  <malloc.h>*/

#include  "getprm.h"
#include  "cgi.h"
#include  "indlib.h"

#define CFGFILENAME "/etc/www/ecatalog.conf"
#define CFGFILENAME1 "ecatalog.conf"

extern unsigned int uigQntOutRec;
extern char *szgHTMLHeader;
extern char *szgHTMLFooter;
extern char *szgCGILog;
extern char *szgPortKOI;
extern char *szgPortDOS;
extern char *szgPortWIN;
extern char *szgPortALT;
char *szgPort;
extern char *szgHelpAddr;

extern unsigned int uigSrchFld;

extern unsigned int uigQntBases;
extern char *szgBaseHTML;
extern char **szgBaseSelNm;
extern char *szgORecNum;
extern char *szgAndWord;
extern char *szgAllInOne;
extern char *szgAllInOneSel;

extern char **szgSrchFld;
extern char **szgSrchHTML;
extern char **szgSrchSelHTML;
extern char **szgNameSrchHTML;

void FormNxtAll(Request RQ);
void FormNxt(Request RQ);
void FormNewAll(Request RQ);
void FormNew(Request RQ);

int main(int argc, char *argv[], char *envp[])
{
  FILE    *fh;
  int Hits=0,i;
  Request RQ;
  char szTime[11];
  char szDate[11];
  char *szQS;
  char *szTmp, *szTmp1;
  time_t t,t1;
  unsigned long q,l;

  t = time(NULL);

  szgPort="";
/*  putenv("CHARSET=ibm866");
  putenv("REQUEST_METHOD=GET");
  putenv("QUERY_STRING=AUTHOR=");*/

  printf(CGI_CONTENT);

  if(GetINI(CFGFILENAME)==FALSE)
  {
    fprintf(stderr,"CGMain: Error reading CONFIG! Exiting!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    exit(1);
  }
  RQ.ReqData=malloc(sizeof(char *)*uigSrchFld);
  if(RQ.ReqData==NULL)
  {
    fprintf(stderr,"CGMain: Error no memory for RQ.ReqData!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    exit(1);
  }
  RQ.PrmReq=malloc(sizeof(unsigned char)*uigSrchFld);
  if(RQ.PrmReq==NULL)
  {
    fprintf(stderr,"CGMain: Error no memory for RQ.PrmReq!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    exit(1);
  }
  printf("<html>");
  printf(szgHTMLHeader);

  szTmp=getenv("CHARSET");
/*  szgPort=getenv("SERVER_PORT"); /* Don't work. Why?  */
  if(szTmp!=NULL)
  {
    if(!strcmp(szTmp, "ibm866"))
    {
      szgPort=szgPortDOS;
    }
    else
    {
      if(!strcmp(szTmp, "koi8-r"))
      {
        szgPort=szgPortKOI;
      }
  /*    else
      {
        if(!strcmp(szTmp, "translit"))
        {
          szgPort=szgPortALT;
        }
      }  /* Not Released */
    }
  }
  szTmp=getenv("CONTENT_LENGTH");
  if(szTmp==NULL)
  {
    RQ.AndWord=TRUE;
    RQ.ORecNum=0;
    RQ.AllInOne=0;
    RQ.uiBase=0;
    for(i=0;i<uigSrchFld;i++)
    {
      RQ.ReqData[i]="";
      RQ.PrmReq[i]=1;
    }
    FormNewAll(RQ);
    FormNew(RQ);
    printf(szgHTMLFooter);
    return 0;
  }

  szQS=GetParamStr();
  if(!szQS)
  {
    fprintf(stderr,"CGMain: Error no set szQS!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    printf(szgHTMLFooter);
    return 1;
  }
  szQS=UnCode(szQS);
  if(!szQS)
  {
    fprintf(stderr,"CGMain: Error no UnCode(szQS)!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    printf(szgHTMLFooter);
    return 1;
  }
  UpAllCaps(szQS);
  QSToCharSet(szQS,"ibm866");

  szTmp=GetQueryValue(szQS,szgORecNum);
  if(szTmp==NULL)
  {
    fprintf(stderr,"CGMain: Error no GetQueryValue(szQS,szgORecNum)!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    printf(szgHTMLFooter);
    return 1;    
  }
  RQ.ORecNum=atol(szTmp);
  free(szTmp);
  
  szTmp=GetQueryValue(szQS,szgBaseHTML);
  if(szTmp==NULL)
  {
    fprintf(stderr,"CGMain: Error no GetQueryValue(szQS,szgBaseHTML)!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    printf(szgHTMLFooter);
    return 1;    
  }
  RQ.uiBase=atol(szTmp);
  free(szTmp);  

  szTmp=GetQueryValue(szQS,szgAllInOne);
  if(szTmp==NULL)
  {
    fprintf(stderr,"CGMain: Error no GetQueryValue(szQS,szgAllInOne)!\n");
    printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
    printf(szgHTMLFooter);
    return 1;    
  }
  if(szTmp[0]!='\0')
  {
    RQ.AndWord=FALSE;
    RQ.AllInOne=TRUE;
    RQ.ReqData[0]=szTmp;
    l=strlen(RQ.ReqData[0]);
    for(i=1;i<uigSrchFld;i++)
    {
      RQ.ReqData[i]=malloc(sizeof(char)*l+1);
      strcpy(RQ.ReqData[i],RQ.ReqData[0]);
    }
    szTmp=GetQueryValue(szQS,szgAllInOneSel);
    if(szTmp==NULL)
    {
      fprintf(stderr,"CGMain: Error no GetQueryValue(szQS,szgAllInOneSel)!\n");
      printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
      printf(szgHTMLFooter);
      return 1;
    }
    RQ.PrmReq[0]=(unsigned char)atoi(szTmp);
    free(szTmp);
    for(i=1;i<uigSrchFld;i++)
      RQ.PrmReq[i]=RQ.PrmReq[0];
  }
  else
  {
    free(szTmp);
    RQ.AllInOne=FALSE;
    RQ.AndWord=FALSE;
    szTmp=GetQueryValue(szQS,szgAndWord);
    if(szTmp==NULL)
    {
      fprintf(stderr,"CGMain: Error no GetQueryValue(szQS,szgAndWord)!\n");
      printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
      printf(szgHTMLFooter);
      return 1;    
    }
    RQ.AndWord=atol(szTmp);
    free(szTmp);

    for(i=0;i<uigSrchFld;i++)
    {
      RQ.ReqData[i]=GetQueryValue(szQS,szgSrchHTML[i]);
      if(RQ.ReqData[i]==NULL)
      {
        fprintf(stderr,"CGMain: Error no GetQueryValue(szQS,szgSrchHTML)!\n");
        printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
        printf(szgHTMLFooter);
        return 1;    
      }
      szTmp=GetQueryValue(szQS,szgSrchSelHTML[i]);
      if(szTmp==NULL)
      {
        fprintf(stderr,"CGMain: Error no GetQueryValue(szQS,szgSrchSelHTML)!\n");
        printf("Error! Send mail witch date&time to webmaster@fessl.ru. Thanks.\n");
        printf(szgHTMLFooter);
        return 1;    
      }
      RQ.PrmReq[i]=(unsigned char)atoi(szTmp);
      free(szTmp);
    }
  }
  free(szQS);

  if(!strcmp(RQ.ReqData[0],REQUEST_CREATER))
  {
    printf("Программа разработана Mr.Ka в 1999-2000г.");
    printf(szgHTMLFooter);  
    return 0;
  }

  printf("<big><center>Результаты поиска</big><br>");
  printf("<TABLE BORDER CELLSPACING=1 CELLPADDING=7 width=520>\n");
  
  Hits=SearchInIndex(RQ);
  printf("</TABLE></center>\n");
  if((RQ.ORecNum+uigQntOutRec)<Hits)
  {
    if(RQ.AllInOne)
      FormNxtAll(RQ);
    else
      FormNxt(RQ);
  }
  
  if(RQ.AllInOne)
    FormNewAll(RQ);
  else
    FormNew(RQ);
  t1=time(NULL);
  printf("<br>\n<center>Время поиска: %d сек.</center>\n", (unsigned int)difftime(t1,t));

  printf(szgHTMLFooter);
  printf("</html>");

  fh = fopen(szgCGILog, "a");
  if(fh)
  {
    GetCurTime(szTime);
    GetCurDate(szDate);
    szTime[10]='\0';
    szDate[10]='\0';
    fprintf(fh,"%s %s [%s %s]", getenv("REMOTE_HOST"),getenv("REMOTE_ADDR"),szDate, szTime);
    fprintf(fh," \"%s\"",szgBaseSelNm[RQ.uiBase]);
    fprintf(fh," (H:%d T:%d)",Hits, (unsigned int)difftime(t1,t));
    for(i=0;i<uigSrchFld;i++)
    {
      fprintf(fh," \"%s\"", RQ.ReqData[i]);
    }
    fprintf(fh,"\n");
    fclose(fh);
  }
  
/* ******** */
  for(i=0;i<uigSrchFld;i++)
    free(RQ.ReqData[i]);
  free(RQ.PrmReq);
  free(RQ.ReqData);
  return 0;
}

void FormNxtAll(Request RQ)
{
  printf("\n<FORM METHOD=POST ACTION=\"http://%s%s%s\">\n",getenv("SERVER_NAME"),szgPort,getenv("SCRIPT_NAME"));
  printf("<input type=hidden name=%s value=\"%u\">\n",szgBaseHTML,RQ.uiBase);
  printf("<input type=hidden name=%s value=\"%s\">\n",szgAllInOne,RQ.ReqData[0]);
  printf("<input type=hidden name=%s value=\"%u\">\n",szgAllInOneSel,RQ.PrmReq[0]);
  printf("<input type=hidden name=%s value=\"%u\">\n",szgORecNum,(RQ.ORecNum+uigQntOutRec));
  printf("<center><input type=submit checked value=\"Следующие найденные записи\"></center></form><br><hr><br>\n");
}

void FormNxt(Request RQ)
{
  unsigned int i;
  
  printf("\n<FORM METHOD=POST ACTION=\"http://%s%s%s\">\n",getenv("SERVER_NAME"),szgPort,getenv("SCRIPT_NAME"));
  for(i=0;i<uigSrchFld;i++)
  {
    printf("<input type=hidden name=%s value=\"%s\">\n",szgSrchHTML[i], RQ.ReqData[i]);
    printf("<input type=hidden name=%s value=\"%u\">\n",szgSrchSelHTML[i], RQ.PrmReq[i]);
  }
  printf("<input type=hidden name=%s value=\"%u\">\n",szgBaseHTML,RQ.uiBase);
  printf("<input type=hidden name=%s value=\"%u\">\n",szgORecNum,(RQ.ORecNum+uigQntOutRec));
  printf("<input type=hidden name=%s value=%u>\n",szgAndWord,RQ.AndWord);
  printf("<center><input type=submit checked value=\"Следующие найденные записи\"></center></form><br><hr><br>\n");
}

void FormNewAll(Request RQ)
{
  char *szTmp, *szTmp1;
  unsigned i;

  printf("<big><center>Упрощенный поиск (<a href=\"http://%s%s%s\">Помощь</a>)</center></big>",getenv("SERVER_NAME"),szgPort,szgHelpAddr);
  printf("\n<FORM METHOD=POST ACTION=\"http://%s%s%s\">\n",getenv("SERVER_NAME"),szgPort,getenv("SCRIPT_NAME"));
  printf("<center><table border=0 width=520>\n");
  printf("<tr><td>База для поиска</td>\n");
  printf("<td><select name=%s>\n",szgBaseHTML);
  for(i=0;i<uigQntBases;i++)
  {
    if(i==RQ.uiBase)
      printf("<option value=%u SELECTED>%s</option>\n", i,szgBaseSelNm[i]);
    else
      printf("<option value=%u>%s</option>",i, szgBaseSelNm[i]);
  }
  printf("</select></td></tr></table>\n");  
  printf("<center><table border=0 width=520>\n");
  printf("<tr><td>Слова для поиска</td>\n");
  printf("<td><input type=text name=%s value=\"%s\" size=40></td>",szgAllInOne,RQ.ReqData[0]);
  printf("<td><select name=%s>\n",szgAllInOneSel);
  if(RQ.PrmReq[0])
  {
    szTmp="SELECTED";
    szTmp1="";
  }
  else
  {
    szTmp="";
    szTmp1="SELECTED";  
  }
  printf("<option value=1 %s>Совпадает с</option>\n",szTmp);
  printf("<option value=0 %s>Начинается с</option>\n",szTmp1);
  printf("</select></td></tr>\n</table></center>\n");
  printf("<input type=hidden name=%s value=0>\n", szgORecNum);
  printf("<center><input type=submit checked value=\"Новый поиск\"> <input type=reset value=\"Сброс критериев\"></center></form><br>\n");
}

void FormNew(Request RQ)
{
  char *szTmp, *szTmp1;
  unsigned int i;
  
  printf("<big><center>Расширенный поиск (<a href=\"http://%s%s%s\">Помощь</a>)</center></big>",getenv("SERVER_NAME"),szgPort,szgHelpAddr);
  printf("\n<FORM METHOD=POST ACTION=\"http://%s%s%s\">\n",getenv("SERVER_NAME"),szgPort,getenv("SCRIPT_NAME"));
  printf("<center><table border=0 width=520>\n");
  printf("<tr><td>База для поиска</td>\n");
  printf("<td><select name=%s>\n",szgBaseHTML);
  for(i=0;i<uigQntBases;i++)
  {
    if(i==RQ.uiBase)
      printf("<option value=%u SELECTED>%s</option>\n", i,szgBaseSelNm[i]);
    else
      printf("<option value=%u>%s</option>\n",i, szgBaseSelNm[i]);
  }
  printf("</select></td></tr></table>\n");
  printf("<table border=0 width=520>\n");
  for(i=0;i<uigSrchFld;i++)
  {  
    if(RQ.PrmReq[i])
    {
      szTmp="SELECTED";
      szTmp1="";
    }
    else
    {
      szTmp="";
      szTmp1="SELECTED";  
    }
    printf("<tr><td>%s</td>\n<td><input type=text name=%s value=\"%s\" size=40></td>\n", szgNameSrchHTML[i], szgSrchHTML[i], RQ.ReqData[i]);
    printf("<td><select name=%s><option value=1 %s>Совпадает с</option><option value=0 %s>Начинается с</option></select></td></tr>\n",szgSrchSelHTML[i], szTmp, szTmp1);
  }
  printf("</table></center>");
  if(RQ.AndWord)
  {
    szTmp="checked";
    szTmp1="";
  }
  else
  {
    szTmp="";
    szTmp1="checked";  
  }
  printf("<center><input type=radio name=%s value=1 %s>Найти все слова\n",szgAndWord,szTmp);
  printf("<input type=radio name=%s value=0 %s>Сортировка по релевантности<br>\n",szgAndWord,szTmp1);
  printf("<input type=hidden name=%s value=0>\n",szgORecNum);
  printf("<input type=submit checked value=\"Новый поиск\"> <input type=reset value=\"Сброс критериев\"></center></form><br>\n");
}