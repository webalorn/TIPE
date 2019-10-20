PROGNAME=csv2latex
PREFIX=/usr
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/share/man/man1
CFLAGS=-Wall -Wextra -pedantic -Os
all:
	${CC} $(CFLAGS) $(PROGNAME).c -o $(PROGNAME)
	docbook-to-man $(PROGNAME).sgml > $(PROGNAME).1
	gzip $(PROGNAME).1
install:
	strip $(PROGNAME)
	install $(PROGNAME) ${DESTDIR}$(BINDIR)/$(PROGNAME)
	install $(PROGNAME).1.gz ${DESTDIR}$(MANDIR)/$(PROGNAME).1.gz
uninstall:
	$(RM) ${DESTDIR}$(BINDIR)/$(PROGNAME)
	$(RM) ${DESTDIR}$(MANDIR)/$(PROGNAME).1.gz
clean:
	$(RM) $(PROGNAME)
	$(RM) $(PROGNAME).1.gz
