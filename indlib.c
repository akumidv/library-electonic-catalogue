#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include    <sys/malloc.h>
/*#include  <malloc.h> */

#include "getprm.h"
#include "crindlb.h"
#include "indlib.h"
#include "indlib_.h"
#include "marclib_.h"

#include "cgi.h"


int IsSym(unsigned int chCod)
{
  if( (chCod>0x2F && chCod<0x3A) ||
      (chCod>0x40 && chCod<0x5B) ||
      (chCod>0x60 && chCod<0x7B) ||
       chCod>0x7E
    )
  {
    return TRUE;
  }
  return FALSE;
}

int IsSeparator(unsigned int chCod)
{
  if(  chCod<=0x2F       ||
      (chCod>=0x3A && chCod<=0x40) ||
      (chCod>=0x5B && chCod<=0x60) ||
      (chCod>=0x7B && chCod<=0x7E)
    )
  {
    return TRUE;
  }
  return FALSE;
}

ResSearch FindFstAndLst(FILE *fhIndex, char* Data, ParamIndex PrmInd, unsigned char JustWord)
{
  ResSearch RS;

  RS.FstRec=0;
  RS.LstRec=0;

  if(FindFirstIndexRec(fhIndex, Data, PrmInd, JustWord))
  {
    RS.FstRec=ftell(fhIndex)/(PrmInd.MaxLen+4);
    if(FindLastIndexRec(fhIndex, Data, PrmInd, JustWord))
      RS.LstRec=ftell(fhIndex)/(PrmInd.MaxLen+4);
  }
  return RS;
}


int SearchInIndex(Request RQ)
{
  FILE *fhIndex, *fhBase;
  char *Tmp, *Rec;
  char *Data, *szTmp;
  char *szFileName;
  unsigned int i, j, k,z,x, LenRec, w, uiQntRqsWord;

  unsigned long QntFnd=0, Hits=0, l, q,ulFndRec;
  unsigned long *ptrL;

  unsigned int QntWord;
  unsigned char ucFind;
  ParamIndex *PrmInd;
  ResSearch **ResSrch;
  ParamRequest *PReq;
  AllSearch ASearch;
  FindingRec FndRec;

  char **Files;

  ASearch.QFRec=malloc(sizeof(unsigned long *)*uigSrchFld);
  if(ASearch.QFRec==NULL)
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). No memory for ASearch.QFRec\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return 0;
  }
  ASearch.FRec=malloc(sizeof(unsigned long **)*uigSrchFld);
  if(ASearch.FRec==NULL)
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). No memory for ASearch.FRec\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return 0;
  }  
  for(i=0;i<uigSrchFld;i++)
  {
    ASearch.QFRec[i]=malloc(sizeof(unsigned long)*uigMaxRqWord);
    if(ASearch.QFRec[i]==NULL)
    {
      fprintf(stderr,"Search Lib: SearchInIndex(). No memory for ASearch.QFRec[]\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      return 0;
    }
    ASearch.FRec[i]=malloc(sizeof(unsigned long *)*uigMaxRqWord);
    if(ASearch.FRec[i]==NULL)
    {
      fprintf(stderr,"Search Lib: SearchInIndex(). No memory for ASearch.FRec[]\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      return 0;
    }    
  }

  PrmInd=malloc(sizeof(ParamIndex)*uigSrchFld);
  if(PrmInd==NULL)
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). No memory for PrmInd\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return 0;
  }
  PReq=malloc(sizeof(ParamRequest)*uigSrchFld);
  if(PReq==NULL)
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). No memory for PReq\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return 0;
  }
  ResSrch=malloc(sizeof(ResSearch *)*uigSrchFld);
  if(ResSrch==NULL)
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). No memory for ResSrch\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return 0;
  }
  for(i=0;i<uigSrchFld;i++)
  {
    ResSrch[i]=malloc(sizeof(ResSearch)*uigMaxRqWord);
    if(ResSrch[i]==NULL)
    {
      fprintf(stderr,"Search Lib: SearchInIndex(). No memory for ResSrch[]\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      return 0;
    }
  }

  Files=malloc(sizeof(char *)*uigSrchFld);
  if(Files==NULL)
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). No memory for Files\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return 0;
  }
  for(i=0;i<uigSrchFld;i++)
  {
    Files[i]=malloc(sizeof(char)*(strlen(szgPath)+strlen(szgBaseName[RQ.uiBase])+strlen(szgSrchFld[i])+sizeof("/")+4+1));
    if(Files[i]==NULL)
    {
      fprintf(stderr,"Search Lib: SearchInIndex(). No memory for Files[]\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      return 0;
    }
    strcpy(Files[i],szgPath);
    strcat(Files[i],szgBaseName[RQ.uiBase]);
    strcat(Files[i],"/");
    strcat(Files[i],szgSrchFld[i]);
    strcat(Files[i],".idx");
  }

  uiQntRqsWord=0;
  for(j=0;j<uigSrchFld;j++)
  {
    PrmInd[j].MaxLen=0;
    PrmInd[j].QntRec=0;
    ResSrch[j][i].FstRec=0;
    ResSrch[j][i].LstRec=0;
    if((fhIndex = fopen(Files[j], "rb")) == NULL)
      fprintf(stderr,"Search Lib: SearchInIndex(). Can't open database %s for search\n", Files[j]);
    else
    {
/* Получаем длину записи в индексе */
      Tmp=&PrmInd[j].MaxLen;
      Tmp[0]=fgetc(fhIndex);
      Tmp[1]=fgetc(fhIndex);
/* Получаем количество записей в индексе */
      Tmp=&PrmInd[j].QntRec;
      for(i=0;i<4;i++)
      {
        Tmp[i]=fgetc(fhIndex);
      }
/* Устанавливаем длину найденной записи равной длине максимальной 
записи в индексе + 4 байта адреса записи в индексе */
      PReq[j].LenRec=PrmInd[j].MaxLen+4;

/* Если в этом индексе нет записей ищем в следующем индексе */
      if(PrmInd[j].QntRec==0)
      {
        fclose(fhIndex);
        continue;
      }
      PReq[j]=GetRqstWord(PReq[j], RQ.ReqData[j],RQ.PrmReq[j]);
      uiQntRqsWord+=PReq[j].QntWord;
      for(i=0;i<PReq[j].QntWord;i++)
        ResSrch[j][i]=FindFstAndLst(fhIndex, PReq[j].ReqWord[i], PrmInd[j], PReq[j].PrmWord[i]);
      fclose(fhIndex);
    }
  }

/* Нашли первые и последние записи, для каждого слова и поля в запросе.
   Осуществляем выдачу */

/* Открываем файл с базой данных */
  szFileName=malloc(sizeof(char)*(strlen(szgPath)+sizeof("base/")+strlen(szgBaseName[RQ.uiBase])+1));
  if(szFileName==NULL) 
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). No memory for szFileName\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return FALSE;
  }
  strcpy(szFileName,szgPath);
  strcat(szFileName,"base/");
  strcat(szFileName,szgBaseName[RQ.uiBase]);
  if((fhBase = fopen(szFileName, "rb")) == NULL)
  {
    fprintf(stderr,"Search Lib: SearchInIndex(). Can't open database LDB %s",szFileName);
    free(szFileName);
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return 0;
  }
  free(szFileName);

  /* Обнуляем массив адресов найденный слов */
  for(j=0;j<uigSrchFld;j++)
    for(i=0;i<uigMaxRqWord;i++)
    {
      ASearch.FRec[j][i]=0;
      ASearch.QFRec[j][i]=0;
    }
  FndRec.Size=0;

  for(j=0;j<uigSrchFld;j++)
  {
    if((fhIndex = fopen(Files[j], "rb")) == NULL)
      fprintf(stderr,"Search Lib: SearchInIndex(). Can't open database %s for output\n", Files[j]);
    else
    {
      for(i=0;i<PReq[j].QntWord;i++)
      {
        if(ResSrch[j][i].FstRec && ResSrch[j][i].LstRec)
        {
          ASearch.QFRec[j][i]=ResSrch[j][i].LstRec+1-ResSrch[j][i].FstRec;
          FndRec.Size+=ASearch.QFRec[j][i];
          ASearch.FRec[j][i]=malloc(ASearch.QFRec[j][i]*sizeof(long));
          if(ASearch.FRec[j][i]==NULL)
          {
            fprintf(stderr,"Search Lib: SearchInIndex(). No memory for ASearch.Frec[][]\n");
            printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
            return 0;
          }
          SetToRec(fhIndex, PReq[j].LenRec, ResSrch[j][i].FstRec);
          Rec=malloc(sizeof(char)*PReq[j].LenRec);
          if(Rec==NULL)
          {
            fprintf(stderr,"Search Lib: SearchInIndex(). No memory for Rec\n");
            printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
            return 0;
          }
          for(l=0;l<ASearch.QFRec[j][i];l++)
          {
            GetIndexRec(fhIndex, Rec, PReq[j].LenRec);
            fseek(fhIndex,PrmInd[j].MaxLen+4,SEEK_CUR);/* Следующая запись */
            Tmp=&ASearch.FRec[j][i][l];
            for(k=0;k<4;k++)
            {
              Tmp[k]=Rec[PReq[j].LenRec-4+k];
            }
          }
          free(Rec);
        }
      }
      fclose(fhIndex);
    }
  }

  FndRec.QFRec=0;
  Hits=0;
  QntFnd=RQ.ORecNum;

  if(FndRec.Size)
  {
    FndRec.IndRec=malloc(FndRec.Size*sizeof(long));
    if(FndRec.IndRec==NULL)
    {
      fprintf(stderr,"Search Lib: SearchInIndex(). No memory for FndRec.IndRec\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      return 0;
    }
    FndRec.Cost=malloc(FndRec.Size*sizeof(int));
    if(FndRec.Cost==NULL)
    {
      free(FndRec.IndRec);
      fprintf(stderr,"Search Lib: SearchInIndex(). No memory for FndRec.Cost\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      return 0;
    }

    if(RQ.AndWord==TRUE)
    {
      if(RQ.AllInOne==FALSE)
      {
        ucFind=FALSE;
        for(k=0;k<uigSrchFld;k++)
        {
          if(PReq[k].QntWord>0)
  	  {
            ucFind=TRUE;
	    break;
	  }
        }
        if(ucFind==TRUE)
        {
          for(l=0;l<ASearch.QFRec[k][0];l++)
          { 
            ulFndRec=ASearch.FRec[k][0][l];
            QntWord=0;
            ucFind=FALSE;
            for(j=k;j<uigSrchFld;j++)
            {
              for(i=0;i<PReq[j].QntWord;i++)
              {
                if(FindIndRec(ASearch.FRec[j][i],ASearch.QFRec[j][i],ulFndRec))
	          QntWord++;
 	      }
 	    }    
            if(QntWord>=uiQntRqsWord)
            {
              FndRec.IndRec[FndRec.QFRec]=ulFndRec;
              FndRec.Cost[FndRec.QFRec]=1;
              FndRec.QFRec++;
            }
          }
        }
        for(j=0;j<uigSrchFld;j++)
	{
	  for(i=0;i<PReq[j].QntWord;i++)
	  {
              free(ASearch.FRec[j][i]);
	  }
	  free(ASearch.FRec[j]);
	}
	free(ASearch.FRec);
 
      }
    }  
    else
    {
      QntWord=0;
      ucFind=FALSE;
      for(j=0;j<uigSrchFld;j++)
      {
        for(i=0;i<PReq[j].QntWord;i++)
        {
          QntWord++;
          for(l=0;l<ASearch.QFRec[j][i];l++)
          {
            if(!FindFoundIndRec(&FndRec,ASearch.FRec[j][i][l]))
            {
              FndRec.IndRec[FndRec.QFRec]=ASearch.FRec[j][i][l];
              FndRec.Cost[FndRec.QFRec]=1;
              FndRec.QFRec++;
            }
          }
        }
      }
      for(j=0;j<uigSrchFld;j++)
      {
	for(i=0;i<PReq[j].QntWord;i++)
	{
          free(ASearch.FRec[j][i]);
	}
        free(ASearch.FRec[j]);
      }
      free(ASearch.FRec);	    
      CostSortFoundRec(FndRec.IndRec, FndRec.Cost, 0, (FndRec.QFRec-1));
    }

    for(l=RQ.ORecNum;l<FndRec.QFRec;l++)
    {
      QntFnd++;
      if(Hits>=uigQntOutRec) continue;
      fseek(fhBase, FndRec.IndRec[l],SEEK_SET);
      printf("\n<br><hr>%ld<br>\n", l+1);

      for(k=0;k<uigVwFld;k++)
      {
        Data=GetSubfld(fhBase, szgVwFld[k]);
        if(Data==NULL)
	  continue;
        if(Data[0]=='\0')
	  continue;
        printf("%s%s", szgNameVwHTML[k] ,Data);
	free(Data);
      }
      Hits++;
    }
    free(FndRec.IndRec);
    free(FndRec.Cost);
  }
  else
  {
    printf("По Вашему запросу ничего не найдено!!!<br>");
  }
  fclose(fhBase);
  printf("\n<br>\n<br>\n<br>Найдено записей: %ld<br>\nОтвечающих запросу: %ld", FndRec.QFRec, QntFnd);
  for(i=0;i<uigSrchFld;i++)
  {
    free(Files[i]);
  }
  free(Files);
  for(i=0;i<uigSrchFld;i++)
  {
    free(ResSrch[i]);
  }
  free(ResSrch);
  free(PrmInd);
  free(PReq);

  return QntFnd;
}

ParamRequest GetRqstWord(ParamRequest PReq, char *szRqst, unsigned char ForAllRqst)
{
  char *Data, *ForFreeData;
  char NewWord;
  unsigned int uiLenStr;
  unsigned int i;

  PReq.QntWord=0;
/* Узнаем длину запроса в этом поле спосотавленному с индексом  */
  uiLenStr=strlen(szRqst);
/* Если для этого поля нет запроса следующее поле и индекс*/
  if(uiLenStr==0)
    return PReq;    
/*      PReq[j]=GetRqstWord(PReq[j], AllReqData[j]);*/
/* Выделяем память для работы со строкой */
  Data=malloc(sizeof(char)*uiLenStr+1);
  if(Data!=NULL)
  {
    ForFreeData=Data;
    strcpy(Data, szRqst);
/* Отделяем слова в запросе, каждое слово - отдельная строка */
    for(i=0;i<uiLenStr;i++)
    {
      if(IsSym(Data[i]))
        NewWord=FALSE;
      else
        if(Data[i]!='$' && Data[i]!='#')
          Data[i]='\0';
/* Проверяем текущий символ разделитель, если да и 
предыдущий символ не был буквой, то увеличиваем счетчик слов */
      if(IsSeparator(Data[i+1]) && NewWord==FALSE)
      {
        NewWord=TRUE;
        PReq.QntWord++;/* Счетчик слов в запросе для данного поля*/
      }
    }
    PReq.ReqWord=malloc(sizeof(char *)*PReq.QntWord);
    if(PReq.ReqWord==NULL)
    {
      fprintf(stderr,"Search Lib: GetRqstWord(). No memory for PReq.ReqWord\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      PReq.QntWord=0;
      return PReq;
    }
    PReq.PrmWord=malloc(sizeof(unsigned char)*PReq.QntWord);
    if(PReq.PrmWord==NULL)
    {
      fprintf(stderr,"Search Lib: GetRqstWord(). No memory for PReq.PrmWord\n");
      printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
      PReq.QntWord=0;
      return PReq;
    }

    for(i=0;i<PReq.QntWord;i++)
    {
      uiLenStr=strlen(Data);
      PReq.PrmWord[i]=ForAllRqst;
      if(Data[uiLenStr-1]=='$')
      {
        PReq.PrmWord[i]=0;/* Изменить на анализ каждого слова */
        /*Удаляем службный символ */
	uiLenStr--;
	Data[uiLenStr]='\0';
      }
      if(Data[uiLenStr-1]=='#')
      {
        PReq.PrmWord[i]=1;/* Изменить на анализ каждого слова */
        /*Удаляем службный символ */
	uiLenStr--;
	Data[uiLenStr]='\0';
      }

      PReq.ReqWord[i]=malloc(sizeof(char)*uiLenStr+1);
      if(PReq.ReqWord[i]==NULL)
      {
        fprintf(stderr,"Search Lib: GetRqstWord(). No memory for PReq.ReqWord[]\n");
        printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
        PReq.QntWord=i;/* Если предыдущие слова были записаны */
        return PReq;
      }
      strcpy(PReq.ReqWord[i],Data);
      if((i+1)<PReq.QntWord)
      {
        while(*Data!='\0')
          Data++;
        while(*Data=='\0')
          Data++;
      }
    }
    free(ForFreeData);
  }
  return PReq;
}


int CostSort(unsigned long *IndRec, unsigned int *Cost, unsigned long QntRec)
{
  unsigned int iSwap;
  unsigned long i, j, lSwap;

  for(i=0;i<QntRec;i++)
  {
    for(j=i;j<QntRec;j++)
    {
      if(Cost[i]<Cost[j])
      {
        /*Обениваем записи*/
        iSwap=Cost[i];
        Cost[i]=Cost[j];
        Cost[j]=iSwap;
        /*Обениваем смещения*/
        lSwap=IndRec[i];
        IndRec[i]=IndRec[j];
        IndRec[j]=lSwap;
      }
    }
  }
}

/* Not Worked, but it's were quickly. Needed repair ;-) */
int CostSortFoundRec(unsigned long *IndRec, unsigned int *Cost, int iLo, int iHi)
{
  int Lo, Hi, Mid;

  unsigned int i, iSwap;
  unsigned long lSwap;

  if(iLo>iHi)
    return TRUE;

  Lo = iLo;
  Hi = iHi;

  Mid=(Lo+Hi)/2;
  do
  {
    while(Cost[Lo]>Cost[Mid])
      Lo++;
    while (Cost[Hi]<Cost[Mid])
      Hi--;
    if (Lo <= Hi)
    {
      iSwap=Cost[Lo];
      Cost[Lo]=Cost[Hi];
      Cost[Hi]=iSwap;
      lSwap=IndRec[Lo];
      IndRec[Lo]=IndRec[Hi];
      IndRec[Hi]=lSwap;
      Lo++;
      Hi--;
    }
  }
  while(Lo<=Hi);
  if((Hi-iLo) < (iHi-Lo))
  {
    CostSortFoundRec(IndRec, Cost, iLo, Hi);
    CostSortFoundRec(IndRec, Cost, Lo, iHi);
  }
  else
  {
    CostSortFoundRec(IndRec, Cost, Lo, iHi);
    CostSortFoundRec(IndRec, Cost, iLo, Hi);
  }
  return TRUE;
}

int FindFoundIndRec(FindingRec *FndRec, unsigned long Val)
{
  long i;

  for(i=0; i<(FndRec->QFRec); i++)
  {
    if(FndRec->IndRec[i]==Val)
    {
      FndRec->Cost[i]++;
      return TRUE;
    }
  }
  return FALSE;
}

int FindIndRec(unsigned long *ulArr, unsigned long ulLen, unsigned long Val)
{
  long i;

  for(i=0; i<ulLen; i++)
  {
    if(ulArr[i]==Val)
      return TRUE;
  }
  return FALSE;
}


int SetToRec(FILE *fhIndex, unsigned int LenRec, unsigned long RecNum)
{
/* Вставить что если выход за границу возврат 0*/
  return fseek(fhIndex, LenRec*RecNum, SEEK_SET);
}

int GetIndexRec(FILE *fhIndex, char *Rec, unsigned int LenRec)
{
  unsigned int i;
  unsigned long OldPos;

  OldPos=ftell(fhIndex);
  for(i=0;i<LenRec;i++)
  {
    Rec[i]=fgetc(fhIndex);
  }
  fseek(fhIndex, OldPos, SEEK_SET);
  return TRUE;
}

int FindFirstIndexRec(FILE *fhIndex, char *Data, ParamIndex PrmInd, unsigned char JustWord)
{
  char *Rec;
  unsigned int LenStr, LenRec, i;
  unsigned long Fst,Lst,Midl, NumFindRec=0;

  LenRec=PrmInd.MaxLen+4;
  Rec=malloc(sizeof(char)*LenRec);
  if(Rec==NULL)
  {
    fprintf(stderr,"Search Lib: FindFirstIndexRec(). No memory for Rec\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return FALSE;
  }

  FirstRec(fhIndex);
  Fst=0;
  Lst=PrmInd.QntRec+1;/*Так как 1-я запись служебная, и она не
    занесена как запись с данными*/
  while(Midl!=(Lst-1) && Midl!=(Fst+1))
  {
    Midl=(Lst+Fst)/2;
    SetToRec(fhIndex,LenRec,Midl);
    GetIndexRec(fhIndex, Rec, LenRec);
    Rec[LenRec-4]='\0';/* Затирам первый байт адреса записи индекса концом 
                       строки, чтобы можно было работать как со строкой*/
    if(!JustWord)
    {
      LenStr=Minimum(strlen(Rec),strlen(Data));
      if(strncmp(Rec, Data, LenStr)<0)
      {
        Fst=Midl;
        continue;
      }
      if(strncmp(Rec, Data, LenStr)>0)
      {
        Lst=Midl;
        continue;
      }
      if(strlen(Rec)>=strlen(Data))
      {
        Lst=Midl;
        NumFindRec=Midl;
      }
      else
        Fst=Midl;
    }
    else
    {
      if(strcmp(Rec, Data)<0)
      {
        Fst=Midl;
        continue;
      }
      if(strcmp(Rec, Data)>0)
      {
        Lst=Midl;
        continue;
      }
      if(strlen(Rec)>=strlen(Data))
      {
        Lst=Midl;
        NumFindRec=Midl;
      }
      else
        Fst=Midl;
    }
  }
  free(Rec);
  if(NumFindRec)
  {
    SetToRec(fhIndex,LenRec,NumFindRec);
    return TRUE;
  }
  return FALSE;
}


int FindLastIndexRec(FILE *fhIndex, char *Data, ParamIndex PrmInd, unsigned char JustWord)
{
  char *Rec;
  unsigned int LenStr, LenRec, i;
  unsigned long Fst,Lst,Midl, NumFindRec=0;

  LenRec=PrmInd.MaxLen+4;
  Rec=malloc(sizeof(char)*LenRec);
  if(Rec==NULL)
  {
    fprintf(stderr,"Search Lib: FindFirstIndexRec(). No memory for Rec\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return FALSE;
  }

  FirstRec(fhIndex);
  Fst=0;
  Lst=PrmInd.QntRec+1;/*Так как 1-я запись служебная, и она не
    занесена как запись с данными*/
  while(Midl!=(Lst-1) && Midl!=(Fst+1))
  {
    Midl=(Lst+Fst)/2;
    SetToRec(fhIndex,LenRec,Midl);
    GetIndexRec(fhIndex, Rec, LenRec);
    Rec[LenRec-4]='\0';/* Затирам первый байт адреса записи индекса концом 
                       строки, чтобы можно было работать как со строкой*/
    if(!JustWord)
    {
      LenStr=Minimum(strlen(Rec),strlen(Data));
      if(strncmp(Rec, Data, LenStr)<=0)
      {
        Fst=Midl;
        NumFindRec=Midl;
        continue;
      }
      if(strncmp(Rec, Data, LenStr)>0)
      {
        Lst=Midl;
        continue;
      }
    }
    else
    {
      if(strcmp(Rec, Data)<=0)
      {
        Fst=Midl;
        NumFindRec=Midl;
        continue;
      }
      if(strcmp(Rec, Data)>0)
      {
        Lst=Midl;
        continue;
      }
    }
  }
  free(Rec);
  if(NumFindRec)
  {
    SetToRec(fhIndex,LenRec,NumFindRec);
    return TRUE;
  }
  return FALSE;
}

unsigned int Minimum(unsigned int First, unsigned int Second)
{
  if(First>Second) return Second;
  return First;
}

/* Следующая запись из индексного, если вышла за границы поиска вернуть 0 */
/*char *NextIndexRec(FILE *fhIndex, char *Data, ParamIndex PrmInd)*/
int NextIndexRec(FILE *fhIndex, char *Data, ParamIndex PrmInd)
{
  char *Rec;
  unsigned int  LenRec, i;

  LenRec=PrmInd.MaxLen+4;
  Rec=malloc(sizeof(char)*LenRec);
  if(Rec==NULL)
  {
    fprintf(stderr,"Search Lib: FindFirstIndexRec(). No memory for Rec\n");
    printf("Error! Send mail witch date&time to webmaster@.ru. Thanks.\n");
    return FALSE;
  }

  fseek(fhIndex,LenRec,SEEK_CUR);
  GetIndexRec(fhIndex, Rec, LenRec);
  Rec[LenRec-4]='\0';/* Затирам первый байт адреса записи индекса концом 
                       строки, чтобы можно было работать как со строкой*/
  if(strncmp(Rec,Data,Minimum(strlen(Rec),strlen(Data)))==0)
  {
    free(Rec);
    return TRUE;
  }
  free(Rec);
  return FALSE;
}

