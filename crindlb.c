#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include    <sys/malloc.h>
/*#include  <malloc.h>*/

#include "crindlb.h"
#include "crindlb_.h"
#include "getprm.h"

int QuickSort(unsigned long LenIndex, unsigned int MaxLenFld, char *A, int iLo, int iHi)
{
  int Lo, Hi;

  char *ForSwap, *HiRec, *LoRec, *MidRec, OffsetSwp[4];
  unsigned int i, LenRec;

  Lo = iLo;
  Hi = iHi;
  LenRec=MaxLenFld+4;
  MidRec = malloc(MaxLenFld);
  if(MidRec==NULL)
    return FALSE;  
  ForSwap = malloc(MaxLenFld);
  if(ForSwap==NULL)
  {
    free(MidRec);
    return FALSE;
  }
  strcpy(MidRec, (A+(Lo+Hi)/2*LenRec));
  do
  {
    LoRec=A+Lo*LenRec;
    while (strcmp(LoRec, MidRec)<0)
    {
        Lo++;
        LoRec+=LenRec;
    }
    HiRec=A+Hi*LenRec;
    while (strcmp(HiRec, MidRec)>0)
    {
        Hi--;
        HiRec-=LenRec;
    }
    if (Lo <= Hi)
    {
      strcpy(ForSwap, LoRec);
      memset(LoRec,'\0',MaxLenFld);
      strcpy(LoRec, HiRec);
      memset(HiRec,'\0',MaxLenFld);
      strcpy(HiRec, ForSwap);
      for(i=0;i<4;i++)
      {
        OffsetSwp[i]=*(LoRec+MaxLenFld+i);
        *(LoRec+MaxLenFld+i)=*(HiRec+MaxLenFld+i);
        *(HiRec+MaxLenFld+i)=OffsetSwp[i];
      }
      Lo++;
      Hi--;
    }
  }
  while (Lo <= Hi);
  free(MidRec);
  free(ForSwap);
  if (Hi > iLo)
    QuickSort(LenIndex, MaxLenFld, A, iLo, Hi);
  if (Lo < iHi)
    QuickSort(LenIndex, MaxLenFld, A, Lo, iHi);

  return TRUE;
}

void DelRec(unsigned long NumRec, unsigned long QntRec, unsigned int LenRec, char *ptrIndex)
{
  unsigned long j, i, Tmp;

  Tmp=NumRec*LenRec;
  for(i=Tmp;i<Tmp+LenRec;i++)
    ptrIndex[i]=0xFF;
}

unsigned long DelDublRec(unsigned long LenIndex, unsigned int MaxLenFld, char *ptrIndex)
{
  char *TopRec, *CurRec;
  unsigned int LenRec;
  unsigned long i, j, l, Top, QntRec;
  int ResCmp;

  Top=1;
  LenRec=MaxLenFld+4;
  QntRec=LenIndex/LenRec;
  TopRec=ptrIndex;

  l=0;
  /* Убиваем пустые записи */
  while(strlen(TopRec)==0)
  {
    DelRec(l, QntRec, LenRec, ptrIndex);
    QntRec--;
    l++;
    TopRec+=LenRec;
  }

  /* Удаляем дублетные записи */
  for(i=l;i<QntRec;i++)
  {
    CurRec=TopRec;
    for(j=Top;j<QntRec;j++)
    {
      CurRec+=LenRec;
      ResCmp=strcmp(TopRec, CurRec);
      if(ResCmp==0 && (*((unsigned long*)(TopRec+MaxLenFld)))==(*((unsigned long*)(CurRec+MaxLenFld))))
      {
        DelRec(i, QntRec, LenRec, ptrIndex);
        QntRec--;
      }
      else
        if(ResCmp<0)
          break;
    }
    TopRec+=LenRec;
    Top++;
  }
 
  QuickSort(LenIndex, MaxLenFld, ptrIndex, 0, (LenIndex/LenRec-1));

  return QntRec*LenRec;
}

int SortIndex(unsigned long LenIndex, unsigned int MaxLenFld, char *ptrIndex)
{
  char *ForSwap, *TopRec, *CurRec, OffsetSwp[4];
  unsigned int k, LenTopRec, LenCurRec, LenRec;
  unsigned long i, j, Top, QntRec;
  int ResCmp;

  ForSwap=malloc(MaxLenFld);
  if(ForSwap)
  {
    Top=1;
    LenRec=MaxLenFld+4;
    QntRec=LenIndex/LenRec;
    TopRec=ptrIndex;

    for(i=0;i<QntRec;i++)
    {
      LenTopRec=strlen(TopRec);
      CurRec=TopRec;
      for(j=Top;j<QntRec;j++)
      {
        CurRec+=LenRec;
        LenCurRec=strlen(CurRec);
        ResCmp=strncmp(TopRec, CurRec, Minimum(LenTopRec,LenCurRec));
        if(ResCmp>0)
        {
          /*Обениваем записи*/
          strcpy(ForSwap,CurRec);
          strcpy(CurRec,TopRec);
          strcpy(TopRec,ForSwap);
          LenTopRec=LenCurRec;
          /*Обениваем смещения*/
          for(k=0;k<4;k++)
          {
            OffsetSwp[k]=*(CurRec+MaxLenFld+k);
            *(CurRec+MaxLenFld+k)=*(TopRec+MaxLenFld+k);
            *(TopRec+MaxLenFld+k)=OffsetSwp[k];
          }
        }
        else
          if((ResCmp==0) && (LenCurRec<LenTopRec))
          {
            /*Обениваем записи*/
            strcpy(ForSwap,CurRec);
            strcpy(CurRec,TopRec);
            strcpy(TopRec,ForSwap);
            LenTopRec=LenCurRec;
            /*Обениваем смещения*/
            for(k=0;k<4;k++)
            {
              OffsetSwp[k]=*(CurRec+MaxLenFld+k);
              *(CurRec+MaxLenFld+k)=*(TopRec+MaxLenFld+k);
              *(TopRec+MaxLenFld+k)=OffsetSwp[k];
            }
          }
      }
      /* Необходимо для удаления мусора */
      strcpy(ForSwap,TopRec);
      memset(TopRec,'\0',MaxLenFld);
      strcpy(TopRec,ForSwap);
      TopRec+=LenRec;
      Top++;
    }
    free(ForSwap);
  }
  else
    return FALSE;
  return TRUE;
}


ParamIndex FindParamField(FILE* fh, char SubfldCode[4])
{
  char *Field;
  unsigned int Len=0, i, chCod;
  char NewRec;
  ParamIndex PrmI;

  PrmI.QntRec=0;
  PrmI.MaxLen=0;
  do
  {
    Len=0;
    NewRec=FALSE;
    Field=GetSubfld(fh, SubfldCode);
    if(Field==NULL) continue;
    for(i=0;i<strlen(Field);i++)
    {
      if(IsSym(Field[i]))
      {
        Len++;
        NewRec=FALSE;
      }
      if(IsSeparator(Field[i+1]) && !NewRec )
      {
        PrmI.QntRec++;
        if(PrmI.MaxLen<Len)
        {
          PrmI.MaxLen=Len;
          printf("\n%dМакс. поле %d - %s", Len, Field);
        }
        Len=0;
        NewRec=TRUE;
      }
    }
    free(Field);

  }
  while(NextRec(fh) != -1);

  PrmI.MaxLen++;
  return PrmI;
}

int SaveIndexHeader(FILE *fhIndex, ParamIndex PInd)
{
  char *Tmp;
  unsigned int i;

  Tmp=&PInd.MaxLen;
  fputc(Tmp[0], fhIndex);
  fputc(Tmp[1], fhIndex);

  Tmp=&PInd.QntRec;
  for(i=0;i<4;i++)
  {
    fputc(Tmp[i],fhIndex);
  }
  for(i=6;i<PInd.MaxLen+4;i++)
  {
    fputc('\0',fhIndex);
  }
  return TRUE;
}


unsigned long CreateIndexFile(FILE *fh, FILE *fhIndex,  char **NameField, unsigned int uiQntFld)
{
  char *arIndex, *Field, *Tmp;
  unsigned long Pos;
  unsigned int k, Len, chCod, l;
  int chTmp;
  unsigned long i, j, SizeIndex;
  FILE *fhTmp;
  ParamIndex PrmInd;
  ParamIndex TmpPrmInd;
  char NewRec;

  printf("\nПоиск параметров индексного файла");
    
  TmpPrmInd.MaxLen=0;
  PrmInd.MaxLen=0;
  PrmInd.QntRec=0;
  for(l=0;l<uiQntFld;l++)
  {
    if(NameField[l]==NULL) continue;
    FirstRec(fh);
    printf("\nПоиск параметров поля %s", NameField[l]);
    TmpPrmInd=FindParamField(fh, NameField[l]);
    if(TmpPrmInd.MaxLen<2) TmpPrmInd.MaxLen=2;
    PrmInd.QntRec+=TmpPrmInd.QntRec;
    if(TmpPrmInd.MaxLen>PrmInd.MaxLen) PrmInd.MaxLen=TmpPrmInd.MaxLen;
  }
  printf("\n\tДлина записи - %d\tКол-во записей: %ld", PrmInd.MaxLen+4, PrmInd.QntRec);
  if(PrmInd.QntRec==0)
  {
    SaveIndexHeader(fhIndex, PrmInd);
    return PrmInd.QntRec;
  }
  SizeIndex=(PrmInd.MaxLen+4)*PrmInd.QntRec;

  printf("\nСоздание не сортированного индекса размером %ld", SizeIndex);

  arIndex=malloc(SizeIndex);
  if(arIndex)
  {
    Tmp=&Pos;

    j=0;
    for(l=0;l<uiQntFld;l++)
    {
      if(NameField[l]==NULL) continue;
      printf("\nСчитывание данных поля %s", NameField[l]);
      printf("\n");    
      FirstRec(fh);
      do
      {
        Pos=ftell(fh);
        Field=GetSubfld(fh, NameField[l]);
        if(Field==NULL) continue;
        Len=0;
        for(i=0;i<strlen(Field);i++)
        {
          if(IsSym(Field[i]))
          {
            arIndex[j++]=UpSymCase(Field[i]);
            Len++;
            NewRec=FALSE;
          }
          if(IsSeparator(Field[i+1]) && !NewRec)
          {
            for(k=0;k<(PrmInd.MaxLen-Len);k++)
            {
              arIndex[j++]='\0';
            }
            for(k=0;k<4;k++)
            {
              arIndex[j++]=Tmp[k];
            }
            Len=0;
            NewRec=TRUE;
          }
        }
        free(Field);
      }
      while(NextRec(fh) != EOF);
    }

    printf("\nСортировка");
    if(QuickSort(SizeIndex, PrmInd.MaxLen, arIndex, 0, (SizeIndex/(PrmInd.MaxLen+4))-1))
    {
      PrmInd.QntRec=SizeIndex/(PrmInd.MaxLen+4);
      printf("\nСохранение");
      SaveIndexHeader(fhIndex, PrmInd);
      for(i=0;i<SizeIndex;i++)
        fputc(arIndex[i],fhIndex);
    }
    else
      fprintf(stderr,"\n!!! Ошибка при создании индекса !!!");
    free(arIndex);
  }
  else
    fprintf(stderr,"\n!!! Не хватает памяти !!!");
  return PrmInd.QntRec;
}

