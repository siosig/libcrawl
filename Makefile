export PKG_CONFIG_PATH := $(PKG_CONFIG_PATH):/usr/local/mysql/lib/pkgconfig:/usr/local/lib/pkgconfig


INCLUDE=-I./include -I/usr/local/include \
	`pkg-config --cflags log4cpp ` \
  `pkg-config --cflags libxml++-3.0` \
	`pkg-config --cflags libcurl` \
  `pkg-config --cflags mysqlclient 2> /dev/null`

INCLUDE := $(INCLUDE)

#CXXFLAGS=-g3 -O0 -fPIC -std=c++11 $(INCLUDE)
CXXFLAGS=$(INCLUDE) -m64 -Ofast -march=native -std=c++11 -fPIC

SRCDIR=src
OBJDIR=obj
DISTDIR=dist
VERSION=1.0.0
PREFIX=/usr/local
#OBJS=$(OBJDIR)/importer.o $(OBJDIR)/tidy.o $(OBJDIR)/db.o $(OBJDIR)/updater.o $(OBJDIR)/http.o $(OBJDIR)/downloader.o $(OBJDIR)/executer.o $(OBJDIR)/downloader.o $(OBJDIR)/htmlparser.o

OBJS=$(OBJDIR)/importer.o $(OBJDIR)/db.o $(OBJDIR)/updater.o $(OBJDIR)/http.o $(OBJDIR)/downloader.o $(OBJDIR)/executer.o $(OBJDIR)/downloader.o $(OBJDIR)/util.o $(OBJDIR)/htmlparser.o

.SUFFIXES: $(SRCDIR)/.cpp $(OBJDIR)/.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $<  -o $@

$(DISTDIR)/libcrawl.so.$(VERSION): $(OBJS)
	$(CXX) -shared -Wl,-soname -Wl,libcrawl.so.1 -o $@ $^

$(DISTDIR)/libcrawl.a: $(OBJS)
	ar r $@ $^

clean:
	rm -fv $(DISTDIR)/*
	rm -fv $(OBJDIR)/*

all:
	$(MAKE) $(DISTDIR)/libcrawl.a
	$(MAKE) $(DISTDIR)/libcrawl.so.$(VERSION)

install: $(DISTDIR)/libcrawl.so.$(VERSION)
	install -v $(DISTDIR)/libcrawl.so.$(VERSION) $(PREFIX)/lib
	(cd $(PREFIX)/lib; ln -sf libcrawl.so.$(VERSION) libcrawl.so.1)
	(cd $(PREFIX)/lib; ln -sf libcrawl.so.1 libcrawl.so)
	(mkdir -p $(PREFIX)/include/crawl)
	(cp -fv include/crawl/*.hpp $(PREFIX)/include/crawl)
	(mkdir -p $(PREFIX)/lib/pkgconfig)
	(cp -fv pkgconfig $(PREFIX)/lib/pkgconfig/libcrawl.pc)
