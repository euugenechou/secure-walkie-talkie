SOURCES 	= src
OBJECTS 	= obj
HEADERS 	= inc

CXX     	= g++ -std=c++17
CXXFLAGS 	= -Wall -Wpedantic -Werror -Wextra -I$(HEADERS) -g
LXXFLAGS	= -lgmpxx -lgmp

MODULES 	= audio bigint being ctr database dh protocol rsa sha3 simon socket util
MODSRC 		= $(patsubst %, $(SOURCES)/%.cpp, $(MODULES))
MODOBJ 		= $(patsubst %, $(OBJECTS)/%.o, $(MODULES))

SERVER 		= listener server
SERVERSRC 	= $(patsubst %, $(SOURCES)/%.cpp, $(SERVER))
SERVEROBJ 	= $(patsubst %, $(OBJECTS)/%.o, $(SERVER))

CLIENT 		= talker client
CLIENTSRC 	= $(patsubst %, $(SOURCES)/%.cpp, $(CLIENT))
CLIENTOBJ 	= $(patsubst %, $(OBJECTS)/%.o, $(CLIENT))

.PHONY: all clean

all: talker listener

listener: $(SERVEROBJ) $(MODOBJ)
	$(CXX) -o $@ $^ $(LXXFLAGS)

talker: $(CLIENTOBJ) $(MODOBJ)
	$(CXX) -o $@ $^ $(LXXFLAGS)

$(OBJECTS)/%.o : $(SOURCES)/%.cpp
	@mkdir -p $(OBJECTS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf talker listener $(OBJECTS)
