VERSION := 0.0.1

PACKAGE := accuforcev2tool

all: compile

compile: $(PACKAGE)

distclean: clean
clean:
	$(RM) *.o *.swp $(PACKAGE) *.orig *.rej map *~

accuforcev2tool: accuforcev2tool.c
	gcc $(CFLAGS) $(CPPFLAGS) -funsigned-char $^ $(LDFLAGS) -lm -lhidapi-hidraw -o $@


install:
	install -d $(DESTDIR)$(PREFIX)/bin
	install $(PACKAGE) $(DESTDIR)$(PREFIX)/bin

dist: clean
	rm -rf $(PACKAGE)
	mkdir $(PACKAGE)


.PHONY: all clean distclean compile install dist
