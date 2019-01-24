#if !defined __INDLIB_H
#define __INDLIB_H

typedef struct tagRequest
{
  char **ReqData;
  unsigned char *PrmReq;
  unsigned char AndWord;
  unsigned long ORecNum;
  unsigned char AllInOne;
  unsigned int uiBase;
} Request;

int SearchInIndex(Request RQ);

#endif