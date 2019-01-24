#if !defined(__MARCLIB_H)
#define __MARCLIB_H

long  FirstRec(FILE *fh); /* Первая запись */
long  NextRec(FILE *fh);  /* Указатель на следующую запись */
char  *GetSubfld(FILE *fh, char SubfldCode[4]); /* Получить подполе */
void  PrintFld(char *Fld);

#endif  /* __MARCLIB_H */
