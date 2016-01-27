#CPPFLAGS=-m64 -Ofast -march=native
CXXFLAGS=-m64 -g3 -O0 -std=c++11 -fPIC -DPIC
INCLUDE=-I/usr/local/mysql/include \
	-I/usr/local/libxml2/include/libxml2 \
	-I.
CXXLIBFLAGS=-Wl,-rpath /usr/local/libxml2/lib \
	-Wl,-rpath /usr/local/mysql/lib \
	-lxml2 \
	-lboost_filesystem \
	-lboost_system \
	-lmysqlclient \
  -lstdc++ \
  -fPIC \
  -DPIC \
  -shared


.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

libcrawl.so: crawler.o importer.o
	$(CXX) $(CXXLIBFLAGS) -shared -Wl,-soname -Wl,$@.0 -o $@.1.0.0 $<
	ln -s $@.1.0.0 $@.0
	ln -s $@.1.0.0 $@

clean:
	rm -fv *.o *.so*
