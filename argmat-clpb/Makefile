#g++ -std=gnu++11 argmat-clpb-main.cpp -L. -lswipl -lboost_regex -I ../ -I /usr/lib/swi-prolog/include/ -o argmat-clpb.a

# swipl-ld -o clpb.out argmat-clpb-main.cpp ../PlReasoner/argmat-clpb.pl -I../ -cc-options,-std=gnu++11 -lboost_regex

TARGET = argmat-clpb.a
OBJ_PATH = objs
PREFIX_BIN =

CC = g++
INCLUDES =  -I ../ -I /usr/lib/swi-prolog/include/
LIBS =
CFLAGS =-Wall -O2 -std=gnu++11
LINKFLAGS = -L. -lswipl -lboost_regex

SRCDIR = .

#C_SOURCES = $(wildcard *.c)
C_SRCDIR = $(SRCDIR)
C_SOURCES = $(foreach d,$(C_SRCDIR),$(wildcard $(d)/*.c) )
C_OBJS = $(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCES))

CPP_SOURCES = $(wildcard *.cpp)
CPP_SRCDIR = $(SRCDIR)
CPP_SOURCES = $(foreach d,$(CPP_SRCDIR),$(wildcard $(d)/*.cpp) )
CPP_OBJS = $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCES))

default:init compile

$(C_OBJS):$(OBJ_PATH)/%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CPP_OBJS):$(OBJ_PATH)/%.o:%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

init:
	$(foreach d,$(SRCDIR), mkdir -p $(OBJ_PATH)/$(d);)

test:
	@echo "C_SOURCES: $(C_SOURCES)"
	@echo "C_OBJS: $(C_OBJS)"
	@echo "CPP_SOURCES: $(CPP_SOURCES)"
	@echo "CPP_OBJS: $(CPP_OBJS)"

compile:$(C_OBJS) $(CPP_OBJS)
	$(CC)  $^ -o $(TARGET)  $(LINKFLAGS) $(LIBS)

clean:
	rm -rf $(OBJ_PATH)
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(PREFIX_BIN)

uninstall:
	rm -f $(PREFIX_BIN)/$(TARGET)

rebuild: clean init compile
