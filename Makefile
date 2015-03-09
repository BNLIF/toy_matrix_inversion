# A Generic Makefile for compiling ROOT programs
# R. Michaels, rom@jlab.org, Aug 2001  See also README !!
# Version of this release
# 
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)
CXX           = g++
CXXFLAGS      = -fopenmp -Wall -frtti -fexceptions -Wl,--no-as-needed -fPIC \
                   -DLINUXVERS -I$(ROOTSYS)/include -O3 -g



# Linux with g++
INCLUDES      = -I$(ROOTSYS)/include  -I$(ROOTSYS)/math/minuit2/inc
CXX           = g++-4.6
LD            = g++-4.6
LDFLAGS       =

#ROOTSYS = /home/xqian/root/root

LIBS          = -L$(ROOTSYS)/lib -lCore -lMinuit2 -lRIO -lNet -lTree -lRint -lThread -pthread -lm -ldl -rdynamic -lPhysics -lHist -lMatrix -lMathCore
#LIBS          = -L/w/work5602/transversity/xqian/5.20-00/lib -lCore -lTree -lMinuit2
#GLIBS         = $(ROOTGLIBS) -L/usr/X11R6/lib -lXpm -lX11
GLIBS         = -L/usr/X11R6/lib -lXpm -lX11
ALL_LIBS =  $(GLIBS) $(LIBS)

# The following sources comprise the package of scaler classes by R. Michaels.
SRC = $(O).C

HEAD = $(SRC:.C=.h)
DEPS = $(SRC:.C=.d)
SCALER_OBJS = $(SRC:.C=.o)

# Test code executibles
PROGS = $(O)

$(O): $(O).o $(O).C
	rm -f $@
	$(CXX) $(CXXFLAGS) -o $@ $(O).o $(ALL_LIBS)

         
clean:
	rm -f *.o core *~ *.d *.tar $(PROGS)

realclean:  clean
	rm -f *.d

###

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .C .o .d

%.o:	%.C
	$(CXX) $(CXXFLAGS) -c $<

%.d:	%.C
	@echo Creating dependencies for $<
	@$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) -c $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

-include $(DEPS)








