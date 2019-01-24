#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include    <sys/malloc.h>
/*#include  <malloc.h>*/
#include <ctype.h>

#include "getprm.h"
#include "marclib_.h"
#include "marclib.h"

/* не берет подполя, вернее когда в одном поле несколько значений */

/*                  *
 * The only two functions one need to navigate  *
 *    through MARC file:          *
 *                  */

long FirstRec(FILE* fh)  /* Установить указатель на первую запись */
{
  return fseek(fh, 0, SEEK_SET);
}

long NextRec(FILE* fh)  /* Установить указатель на начало следующей записи */
{
  char  ch = 0x00;
  long  PrvPos;
  int Tmp;

  PrvPos = ftell(fh);
  while((ch != EOREC) && (!feof(fh)))
    ch = getc(fh);
  if((ch == EOREC) && ((Tmp = getc(fh)) != EOF))
  {
    ungetc(Tmp, fh);
    return 0;
  }
  else
  {
    fseek(fh, PrvPos, SEEK_SET);
    return -1;
  }
}

/*                                              *
 *   Very ugly but quite effective:         *
 *            */
char* GetSubfld(FILE* fh, char SubfldCode[4])  /* Получить подполе */
{
  long    PrvPos, IntrPos;
  char    cBasePtr[6] = {'\0','\0','\0','\0','\0','\0',},
    cFldStart[6] = {'\0','\0','\0','\0','\0','\0',};
  static char Buffer[MAX_BUFF];
  char *szTmp;
  int   Cnt, SubCnt = 0;
  char    ch;
  MarcHdr Header;
  MarcIdx Index;

  PrvPos = ftell(fh);
  fread(&Header, sizeof(MarcHdr), 1, fh);/* Считать заголовок */
  strncpy(cBasePtr, Header.BasePtr, 5);
  for(Cnt = 0; Cnt < ((atoi(cBasePtr)-25)/sizeof(MarcIdx)); Cnt++)
  {
    fread(&Index, sizeof(MarcIdx), 1, fh);
    strncpy(cFldStart, Index.FldStart, 5);
    if(!strncmp(Index.Label, SubfldCode, 3))
    {
      /* Begining of data: */
      IntrPos = ftell(fh);
      fseek(fh, PrvPos + atoi(cBasePtr) + atoi(cFldStart), SEEK_SET);
      if(strncmp(SubfldCode, "00", 2)) /* There's a subfield */
      {
        while(getc(fh) != EOSUB); /* First subfield is a dumb "00\0x1f" */
        do
        {
          /* Selecting subfields (or all with '?') */
          if((getc(fh) == SubfldCode[3]) || (SubfldCode[3] == ANYCODE))
            for(; SubCnt < MAX_BUFF; SubCnt++)
            {
              Buffer[SubCnt] = getc(fh);
              if((Buffer[SubCnt] == EOSUB) || (Buffer[SubCnt] == EOFLD))
              {
                ch = Buffer[SubCnt];
                Buffer[SubCnt++] = '\0';
                break;
              }
            }
          else
            while(((ch = getc(fh)) != EOSUB) && (ch != EOFLD));
        }
        while((ch != EOFLD) && (SubCnt < MAX_BUFF));
        Buffer[SubCnt] = '\0';
      }
      else
      {
        /* There'll be no subfields: */
        for(; SubCnt < MAX_BUFF; SubCnt++)
        {
          Buffer[SubCnt] = getc(fh);
          if(Buffer[SubCnt] == EOFLD)
          {
            Buffer[SubCnt++] = '\0';
            break;
          }
        }
        Buffer[SubCnt] = '\0';
      }
      /* Internal cleanup: */
      fseek(fh, IntrPos, SEEK_SET);
    }
  }
  /* Finalize:*/
  if(SubCnt == 0)
    Buffer[0] = '\0';
  fseek(fh, PrvPos, SEEK_SET);
  szTmp=malloc(sizeof(char)*(strlen(Buffer)+1));
  if(szTmp==NULL)
    return NULL;
  strcpy(szTmp,Buffer);
  return szTmp;
}



void  PrintFld(char *Fld)
{
  /* Using double-null terminator: */

  while(*Fld != '\0')
  {
    printf("%s", Fld);
    Fld += strlen(Fld) + 1;
  }
  return;
}

