PRG=gnu.exe
# web script will insert definition of PRG here 
# it will include student's login (security)
GCC=g++-5 #required for online testing
MSC=cl  #optional - mostly to help students who prefer VC

GCCFLAGS=-std=c++1z -Wall -Wextra -Werror -pedantic
MSCFLAGS=/EHa /W4 /WX /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0" /O2i /GL

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

OBJECTS0=functional-list.h
DRIVER0=test.cpp

gcc0:
	$(GCC) $(DRIVER0) $(GCCFLAGS) $(CYGWIN) -o $(PRG) #PRG will be defined by web script
msc0:
	$(MSC) -Fe$@.exe $(DRIVER0) $(OBJECTS0) $(MSCFLAGS) 
1 2 3 4:
	echo "running test$@"
	@echo "should run in less than 300 ms"
	./$(PRG) >studentout$@
	diff output.txt studentout$@ $(DIFF_OPTIONS) > difference$@	
mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9 mem10 mem11 mem12 mem13 mem14 mem15 mem16 mem17 mem18 mem19 mem20 mem21 mem22 mem23 mem24:
	echo "running memory test $@"
	@echo "should run in less than 1000 ms"
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
	@echo "lines after this are memory errors"; cat difference$@
clean:
	rm -f *.exe *.o *.obj *.tds *.exe.manifest student* difference*
