#CC = g++
CC = gcc

#CFLAGS = -O -DUNIX -DHTML
#CFLAGS = -g -pg -O -Wall -DUNIX -DHTML
#CFLAGS = -O -DDOS
CFLAGS = -g -pg -O -Wall -DOS2 -DHTML -Zexe

ged2www: ged2www.o load.o process.o report.o utils.o parser.o rptutil.o cfgfile.o
	$(CC) $(CFLAGS) -o ged2www ged2www.o parser.o load.o \
	process.o report.o rptutil.o utils.o cfgfile.o

ged2www.o: ged2www.c defs.h load.h process.h report.h global.h utils.h cfgfile.h
	$(CC) $(CFLAGS) -c ged2www.c

parser.o: parser.c defs.h parser.h utils.h global.h
	$(CC) $(CFLAGS) -c parser.c

load.o: load.c load.h defs.h parser.h utils.h global.h
	$(CC) $(CFLAGS) -c load.c

process.o: process.c process.h defs.h utils.h global.h
	$(CC) $(CFLAGS) -c process.c

rptutil.o: rptutil.c rptutil.h defs.h utils.h global.h
	$(CC) $(CFLAGS) -c rptutil.c

report.o: report.c report.h defs.h rptutil.h utils.h global.h
	$(CC) $(CFLAGS) -c report.c

utils.o: utils.c utils.h global.h
	$(CC) $(CFLAGS) -c utils.c

cfgfile.o: cfgfile.c cfgfile.h global.h utils.h
	$(CC) $(CFLAGS) -c cfgfile.c


clean:
	rm *.o

run:
	rm ./html/*
	ged2www

zip: ged2www.c defs.h global.h global.h utils.c utils.h history.txt \
	load.c load.h parser.c parser.h process.c process.h COPYING \
	read_me.txt report.c report.h rptutil.c rptutil.h Makefile \
	cfgfile.c cfgfile.h 
	rm g2w_opts.txt
	zip -l ged2wwws.zip *.h *.c *.txt COPYING Makefile 
