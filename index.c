#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include    <sys/malloc.h>
/*#include  <malloc.h>*/
#include <ctype.h>
#include <time.h>

#include "getprm.h"


#define CFGFILENAME "ecatalog.conf"

extern unsigned int uigQntBases;

extern char *szgPathCreate;
extern char **szgBaseName;
extern unsigned int uigSrchFld;
extern unsigned int uigIndFld;
extern char ***szgIndFld;

int main(int argc, char *argv[])
{
  FILE *fh, *fhIndex;
  unsigned long Hits;
  unsigned int MaxLenFld;
  char *NameField;
  unsigned int i, j, l;
  time_t t, t1;
  char **Files;
  char *szFullBaseNm;

  t=time(NULL);
  
  if(argc==2)
  {
    if(GetINI(argv[1])==FALSE)
    {
      fprintf(stderr,"Error CONFIG! Exiting!");
      return 1;
    }
  }
  else
  {
    if(GetINI(CFGFILENAME)==FALSE)
    {
      fprintf(stderr,"Error CONFIG! Exiting!");
      return 1;
    }
  }

  Files=malloc(sizeof(char *)*uigSrchFld);
  if(Files==NULL)
  {
    fprintf(stderr,"No memory for generate output");
    exit(1);
  }  

  for(l=0;l<uigQntBases;l++)
  {
    szFullBaseNm=malloc(sizeof(char)*(strlen(szgPathCreate)+sizeof("base/")+strlen(szgBaseName[l])+1));
    if(szFullBaseNm==NULL)
    {
      fprintf(stderr,"No memory for generate filename index");
      exit(1);
    }
    strcpy(szFullBaseNm,szgPathCreate);
    strcat(szFullBaseNm,"base/");
    strcat(szFullBaseNm,szgBaseName[l]);

    for(i=0;i<uigSrchFld;i++)
    {
      Files[i]=malloc(sizeof(char)*(strlen(szgPathCreate)+sizeof(szgBaseName[l])+sizeof("/")+strlen(szgIndFld[i][0])+4+1));

      if(Files[i]==NULL)
      {
        fprintf(stderr,"No memory for generate filename index");
        exit(1);
      }
      strcpy(Files[i],szgPathCreate);
      strcpy(Files[i],szgBaseName[l]);
      strcat(Files[i],"/");
      strcat(Files[i],szgIndFld[i][0]);
      strcat(Files[i],".idx");
    }

    if((fh = fopen(szFullBaseNm, "rb")) == NULL)
    {
      fprintf(stderr,"Can't open main database %s:-(",szFullBaseNm);
      free(szFullBaseNm);
      continue;
/*      return 1;*/
    }
    free(szFullBaseNm);
    for(i=0;i<uigSrchFld;i++)
    {
      t1=time(NULL);
      if((fhIndex = fopen(Files[i], "wb")) == NULL)
      {
        fprintf(stderr,"Can't create index %s :-(", Files[i]);
        continue;
      }
      printf("\nСоздание индексного файла %s",Files[i]);
      Hits=CreateIndexFile(fh, fhIndex, szgIndFld[i], uigIndFld);
      printf("\nВ индексном файле создано %ld записей за %d секунд\n", Hits, difftime(time(NULL),t1));
      fclose(fhIndex);
      free(Files[i]);      
    }
    fclose(fh);
  }
  free(Files);
  printf("\n\nВремя подготовки : %f секунд", difftime(time(NULL),t));

  return 0;
}
