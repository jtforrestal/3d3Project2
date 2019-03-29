CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=seanmurray
CLASSES= 

all: beej_server beej_client beej_server2 DVT DVTEntry initialise

web-server: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

web-client: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM beej_server beej_client beej_server2 DVT DVTEntry initialise *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
