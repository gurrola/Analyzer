##############################################################
# Author: Andres Florez, Universidad de los Andes, Colombia. #
##############################################################

ObjSuf = o
SrcSuf = cc
ExeSuf =
DllSuf = so
OutPutOpt = -o
HeadSuf = h

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)

# Linux with egcs
DEFINES =

CXX = g++
CXXFLAGS += $(ROOTCFLAGS) -I./

LD = g++
LDFLAGS += $(ROOTLIBS)

SOFLAGS = -shared
LIBS =

#------------------------------------------------------------------------------
SOURCES = $(wildcard src/*.cc)
OBJECTS = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
#------------------------------------------------------------------------------

all: BSM3G_TNT_Analyzer

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

%: %.o
	$(LD) $(LDFLAGS) -o $@ $< $(LIBS)

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS)

.SUFFIXES: .$(SrcSuf) .cc .o .so
