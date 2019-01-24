#if !defined(__INDLIB__H)
#define __INDLIB__H

extern unsigned int uigQntOutRec;
extern char *szgPath;
extern char **szgBaseName;
extern unsigned int uigSrchFld;
extern char **szgSrchFld;
extern unsigned int uigVwFld;
extern char **szgVwFld;
extern char **szgNameVwHTML;
extern unsigned int uigMaxRqWord;

typedef struct tagResSearch
{
  unsigned long FstRec;
  unsigned long LstRec;
} ResSearch;

typedef struct tagParamRequest
{
  unsigned int QntWord;/* Количество слов в запросе для заданного поля */
  unsigned int LenRec;/* Длина записи для поля(с адресом) для этого поля */
  char **ReqWord;/* Запрашиваемые слова */
  unsigned char *PrmWord;/* Параметры для каждого запрашиваемого слова */
} ParamRequest;

typedef struct tagAllSearch
{
//  unsigned long QFRec[8][MAXQNTWORD];
//  unsigned long *FRec[8][MAXQNTWORD];/* Массив найденных записей по всем полям по всем словам*/
  unsigned long **QFRec;
  unsigned long ***FRec;/* Массив найденных записей по всем полям по всем словам*/
} AllSearch;

typedef struct tagFindingRec
{
  unsigned long QFRec; /* Кол-во найденных записей */
  unsigned long Size;
  unsigned long *IndRec; /* Объединенный массив всех найденных записей */
  unsigned int *Cost;
} FindingRec;



int IsSym(unsigned int chCod);
int IsSeparator(unsigned int chCod);
unsigned int Minimum(unsigned int First, unsigned int Second);
ResSearch FindFstAndLst(FILE *fhIndex, char* Data, ParamIndex PrmInd, unsigned char JustWord);

int SetToRec(FILE *fhIndex, unsigned int LenRec, unsigned long RecNum);
int GetIndexRec(FILE *fhIndex, char *Rec, unsigned int LenRec);
int FindFirstIndexRec(FILE *fhIndex, char *Data, ParamIndex PrmInd, unsigned char JustWord);
int FindLastIndexRec(FILE *fhIndex, char *Data, ParamIndex PrmInd, unsigned char JustWord);
int NextIndexRec(FILE *fhIndex, char *Data, ParamIndex PrmInd);

int FindFoundIndRec(FindingRec *FndRec, unsigned long Val);
int FindIndRec(unsigned long *ulArr, unsigned long ulLen, unsigned long Val);

int CostSortFoundRec(unsigned long *IndRec, unsigned int *Cost, int iLo, int iHi);
int CostSort(unsigned long *IndRec, unsigned int *Cost, unsigned long QntRec);
ParamRequest GetRqstWord(ParamRequest PReq, char *szReq, unsigned char ForAllRqst);

#endif