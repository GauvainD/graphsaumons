cpp = g++
opt = --std=c++11 -g
dst = build
src = src
prologD = prolog
plc = gplc
plOpt = -c

libs = graph.hpp invariants.hpp transformations.hpp nauty.h nauty.a nautyInterface.hpp graph6.hpp

# the order is REALLY IMPORTANT don't ask why
prologLibs = /usr/share/gprolog/lib/top_level.o /usr/share/gprolog/lib/libengine_pl.a /usr/share/gprolog/lib/all_pl_bips.o /usr/share/gprolog/lib/libbips_pl.a /usr/share/gprolog/lib/liblinedit.a

prologC = $(wildcard $(prologD)/*.o)

prologSRC = $(wildcard $(prologD)/*.pl)

exec = test.cpp

test = testsBoost.cpp

default : prepare test

prepare :
	test -d $(dst) || mkdir $(dst)

build :
	$(cpp) $(opt) -o $(dst)/$(basename $(exec)) $(src)/$(exec) $(addprefix $(src)/, $(libs))

test :
	$(cpp) $(opt) -o $(dst)/$(basename $(test)) $(src)/$(test) $(addprefix $(src)/, $(libs))

nauty :
	$(cpp) $(opt) -o $(dst)/$(basename $(nauty)) $(src)/$(nauty) $(addprefix $(src)/, $(libs))

libProlog : cplProlog
	$(cpp) $(opt) -o $(dst)/IProlog.o -c $(src)/IProlog.cpp

runProlog : libProlog
	$(cpp) $(opt) -o $(dst)/prologW $(src)/prologW.cpp $(dst)/IProlog.o $(prologC) $(prologLibs)

clean:
	rm -rf $(dst)

cplProlog :
	$(plc) $(plOpt) $(prologSRC)

