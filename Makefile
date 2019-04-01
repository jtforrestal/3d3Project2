CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=seanmurray
CLASSES= 

<<<<<<< HEAD
all: beej_client beej_server2 DVT DVTEntry initialise beej_server
=======
all: beej_server beej_client beej_server2 Router_B
>>>>>>> e65cd475c7704510170ddb9a271d172a2b44730d

web-server: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

web-client: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM beej_server beej_client beej_server2 Router_B *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
