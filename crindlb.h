#if !defined __CRINDLB_H
#define __CRINDLB_H

typedef struct  tagParamIndexField
{
  unsigned int MaxLen;
  unsigned long QntRec;
} ParamIndex;

unsigned long CreateIndexFile(FILE *fh, FILE *fhIndex,  char **NameField, unsigned int uiQntFld);

#endif