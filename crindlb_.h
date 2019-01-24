#if !defined __CRINDLB__H
#define __CRINDLB__H

int QuickSort(unsigned long LenIndex, unsigned int MaxLenFld, char *A, int iLo, int iHi);

void DelRec(unsigned long NumRec, unsigned long QntRec, unsigned int LenRec, char *ptrIndex);

unsigned long DelDublRec(unsigned long LenIndex, unsigned int MaxLenFld, char *ptrIndex);

int SortIndex(unsigned long LenIndex, unsigned int MaxLenFld, char *ptrIndex);

ParamIndex FindParamField(FILE* fh, char SubfldCode[4]);

int SaveIndexHeader(FILE *fhIndex, ParamIndex PInd);

#endif