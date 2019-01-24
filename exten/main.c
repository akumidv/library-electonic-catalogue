#include <stdlib.h>
#include <stdio.h>
#include  "cgi.h"
#include  "str.h"


int main(void)
{
  char *szQS,*Str;

  Str=MarkStr("<b>","</b>","Иванов Иван ","ИВ");
  printf("\n%s",Str);
  return 0;
}
