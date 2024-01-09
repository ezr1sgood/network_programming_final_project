# sudo apt-get install graphviz
# sudo apt-get install makefile2graph
# makefile2graph | dot -Tpng -o makefile_graph.png
PROGRAM		= server
CLIENT		= client

SDIR		:= source
IDIR		:= include

CXX			?= g++
INCLUDES	:= -I $(IDIR)
C_FLAGS		:= -Wall -Wextra -Wshadow -std=c++17
L_FLAGS		:= -lncursesw
RC_FLAGS	:= -O2
DC_FLAGS	:= -g -DDEBUG -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG_PEDANTIC_ASSERT

ifeq ($(DEBUG), 1)
	CXXFLAGS := $(CXXFLAGS) $(C_FLAGS) $(DC_FLAGS)
	ODIR := obj/debug
	BDIR := bin/debug
else
	CXXFLAGS := $(CXXFLAGS) $(C_FLAGS) $(RC_FLAGS)
	ODIR := obj/release
	BDIR := bin/release
endif

EXE			:= $(BDIR)/$(PROGRAM)
CLIEXE			:= $(BDIR)/$(CLIENT)
SRCS		:= $(wildcard $(SDIR)/*.cpp)
SRCS		:= $(filter-out $(SDIR)/client.cpp,$(SRCS))
OBJS		:= $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))
DEPS		:= $(SRCS:$(SDIR)/%.cpp=$(ODIR)/%.d)

all: build

build: $(EXE)

server: $(EXE)
	$<

# client: $(CLIEXE)
#	$<

$(EXE): dirs $(OBJS)
	$(CXX) $(OBJS) -o $@
	$(CXX) $(ODIR)/client.o -o $(BDIR)/client $(L_FLAGS)
	
-include $(DEPS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $(SDIR)/client.cpp -o $(ODIR)/client.o


dirs:
	@mkdir -p $(BDIR) $(ODIR)

clean:
	$(RM) -r obj bin

show:
	@echo 'EXE      :' $(EXE)
	@echo 'CXX      :' $(CXX)
	@echo 'CXXFLAGS :' $(CXXFLAGS)
	@echo 'BDIR     :' $(BDIR)
	@echo 'SDIR     :' $(SDIR)
	@echo 'ODIR     :' $(ODIR)
	@echo 'SRCS     :' $(SRCS)
	@echo 'OBJS     :' $(OBJS)
	@echo 'DESP     :' $(DEPS)
