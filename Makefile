CC=g++
LNFLAGS=-lffi -ldl
SRCDIR=./src
SRCDIRS=$(shell find $(SRCDIR) -type d)
SRC=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

ifndef DEBUG
	CXXFLAGS=-std=c++11 -MMD -MP -O3
	OBJDIR=release
	EXECUTABLE=release/sky
else
	CXXFLAGS=-std=c++11 -g -MMD -MP -D _DEBUG
	OBJDIR=debug
	EXECUTABLE=debug/sky
endif

OBJDIRS=$(pathsubst $(SRCDIRS)/%,$(OBJDIRS)/%,$(SRCDIRS))
_OBJ=$(SRC:.cpp=.o)
OBJ=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(_OBJ))
DEPS = ${OBJ:.o=.d}

sky: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $^ $(CXXFLAGS) $(LNFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $< $(CXXFLAGS) -c -o $@

install: sky
	install release/sky /usr/local/bin/sky
	install -d /usr/local/lib/sky
	cp -r sky /usr/local/lib/sky

install-home: sky
	install release/sky ~/bin/sky
	install -d ~/.sky/lib/
	cp -r sky ~/.sky/lib

clean:
	rm -rf release debug

-include ${DEPS}