Search1.cgi:	cgimain.c cgi.o indlib.o marclib.o getprm.h indlib.h getprm.o cfg.o
		cc -o Search1.cgi cgimain.c cgi.o indlib.o marclib.o getprm.o cfg.o

cgi.o:		cgi.c cgi.h cgitbl.h getprm.h
		cc -c cgi.c

cfg.o:		cfg.c cfg.h cfg_.h
		cc -c cfg.c

indlib.o:	indlib.c  indlib_.h indlib.h getprm.h
		cc -c indlib.c

marclib.o:	marclib.c marclib_.h marclib.h getprm.h
		cc -c marclib.c

getprm.o:	getprm.c getprm_.h getprm.h
		cc -c getprm.c
