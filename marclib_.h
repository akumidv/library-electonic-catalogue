#if !defined(__MARCLIB__H)
#define __MARCLIB__H

/*   Standard delimiters:     */
#define   EOSUB   0x1f  /* Идентификатор подполя  */
#define   EOFLD   0x1e  /* Разделитель поля */
#define   EOREC   0x1d  /* Конец записи */
#define   ANYCODE '+'

#define   MAX_BUFF  0x0800 /* Максимальная длина буфера */

/*   24byte Header Comes First...  */
typedef struct  tagMarcHdr
{
  char  RecLen[5];  /* Including EOREC*/
  char  Status;   /* 'n' */
  char  Type; /* 'a' */
  char  Level;    /* 'm' */
  char  Resvd[2]; /* Spaces */
  char  IndCnt;   /* Must be 2 */
  char  SubCnt;   /* 2 */
  char  BasePtr[5]; /* 24+12*NumOfIdxEntries+1  *//* Базовый адресс записи */
  char  CodingLevel;  /* 7 */
  char  DescrForm;  /* Space */
  char  Link; /* Space */
  char  IdxFldLen;  /* 4 */
  char  IdxFldStart;  /* 5 */
  char  IdxFldUser; /* 0 */
  char  IdxResvd; /* 0 */
} MarcHdr;

/*      *
 *   Index entries follows the header.  *
 *      */
typedef struct  tagMarcIdx
{
  char  Label[3];
  char  FldLen[4];
  char  FldStart[5];
} MarcIdx;

#endif  /* __MARCLIB__H */
