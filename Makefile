
#
# Files and folders configuration
#

INCDIR = include
SRCDIR = src
TESTDIR = test

CPPFLAGS += -I../libcan/include

# Compile bootloader-client executable
CLIENTNAME = bootloader-client
CLIENTSRCS = $(wildcard $(SRCDIR)/*.cpp)
CLIENTOBJS = $(CLIENTSRCS:.cpp=.o)

# Compile the library
LIBNAME = cvrabootloader
LIBSRCS := $(wildcard $(SRCDIR)/*.cpp)
LIBSRCS := $(filter-out $(CLIENTNAME).cpp, $(LIBSRCS))
LIBOBJS = $(LIBSRCS:.cpp=.o)

LIBS = ../libcan/libcan.so

# Compile library test program(s)
TESTNAME = datagram
TESTSRCS = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS = $(TESTSRCS:.cpp=.o)

#
# Toolchain setup
#
CPP = g++
CPPFLAGS += -fdiagnostics-color=auto
CPPFLAGS += -std=gnu++14
CPPFLAGS += -Wall -Wextra -pedantic
CPPFLAGS += -g
CPPFLAGS += -O3

# Required for linking to shared library:
CPPFLAGS += -fPIC

CPPFLAGS += -I$(INCDIR)
CPPFLAGS += -L.
CPPFLAGS += -lpthread
CPPFLAGS += -lz

RM = rm -f


#
# Targets
#

all: client lib $(TESTDIR)/$(TESTNAME).elf

$(LIBS):
	# Don't attempt to compile shared library dependencies.

# Client
client: $(CLIENTNAME).elf

run: $(CLIENTNAME).elf
	@./$<

$(CLIENTNAME).elf: $(CLIENTOBJS) $(LIBS)
	@$(RM) $@
	$(CPP) $(CPPFLAGS) $^ -o $@

# Library
lib: lib$(LIBNAME).so

lib$(LIBNAME).so: $(LIBOBJS)
	@$(RM) $@
	$(CPP) $(CPPFLAGS) -shared $^ -o $@

# Test(s)
$(TESTDIR)/$(TESTNAME).elf: $(TESTOBJS) lib$(LIBNAME).so $(LIBS)
	@$(RM) $@
	$(CPP) $(CPPFLAGS) $^ -o $@

test: $(TESTDIR)/$(TESTNAME).elf
	@./$<

# Generic
%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	@$(RM) $(CLIENTNAME).elf $(SRCDIR)/*.o lib$(LIBNAME).so $(TESTDIR)/*.o $(TESTDIR)/$(TESTNAME).elf
