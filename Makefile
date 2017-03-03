CC = g++
CFLAGS = -Wall -Werror
EXE = rshell

BDIR = bin
SDIR = src
ODIR = obj

SRCS = $(wildcard $(SDIR)/*.cpp)
DEPS = $(wildcard $(SDIR)/*.h)
OBJS = $(patsubst $(SDIR)/%,$(ODIR)/%,$(patsubst %.cpp,%.o,$(SRCS)))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(FLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/$(EXE)