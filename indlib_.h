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
  unsigned int QntWord;/* ������⢮ ᫮� � ����� ��� ��������� ���� */
  unsigned int LenRec;/* ����� ����� ��� ����(� ���ᮬ) ��� �⮣� ���� */
  char **ReqWord;/* ����訢���� ᫮�� */
  unsigned char *PrmWord;/* ��ࠬ���� ��� ������� ����訢������ ᫮�� */
} ParamRequest;

typedef struct tagAllSearch
{
//  unsigned long QFRec[8][MAXQNTWORD];
//  unsigned long *FRec[8][MAXQNTWORD];/* ���ᨢ ��������� ����ᥩ �� �ᥬ ���� �� �ᥬ ᫮���*/
  unsigned long **QFRec;
  unsigned long ***FRec;/* ���ᨢ ��������� ����ᥩ �� �ᥬ ���� �� �ᥬ ᫮���*/
} AllSearch;

typedef struct tagFindingRec
{
  unsigned long QFRec; /* ���-�� ��������� ����ᥩ */
  unsigned long Size;
  unsigned long *IndRec; /* ��ꥤ������ ���ᨢ ��� ��������� ����ᥩ */
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