# all: main

# main: main.o systemPro.o my_complex.o ex_10_1.o unicode.o csapp.o
# 	gcc $(CXXFLAGS) $^ -o $@  $(LDFLAGS)
# clean:
# 	-rm main *.o *.d
# .PHONY: clean
# CXXFLAGS = -g -Wall
# LDFLAGS = -lm -lpthread

# sources = main.c systemPro.c my_complex.c ex_10_1.c unicode.c csapp.c
# include $(sources:.c=.d)
# %.d: %.c
# 	set -e; rm -f $@; \
# 	$(CC) -MM $(CPPFLAGS) $(CXXFLAGS) $< > $@.$$$$; \
# 	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
# 	rm -f $@.$$$$
	
######################################
#
######################################
#source file
#源文件，自动找所有.c和.cpp文件，并将目标定义为同名.o文件
SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
#target you can change test to what you want
#目标文件名，输入任意你想要的执行文件名
TARGET  := test
  
#compile and lib parameter
#编译参数
CC      := gcc
LIBS    :=
LDFLAGS :=	-lm -lpthread
DEFINES :=
INCLUDE := -I.
CFLAGS  := -g -Wall -O0 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H
  
  
#i think you should do anything here
#下面的基本上不需要做任何改动了
.PHONY : everything objs clean veryclean rebuild
  
everything : $(TARGET)
  
all : $(TARGET)
  
objs : $(OBJS)
  
rebuild: veryclean everything
                
clean :
	-rm -fr *.so
	-rm -fr *.o

veryclean : clean
	-rm -fr $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)