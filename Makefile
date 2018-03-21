
#
# Files and folders configuration
#

INCDIR = include
SRCDIR = src
TESTDIR = test

CPPFLAGS += -I../libcan/include

# Compile the library
LIBNAME = cvrabootloader
LIBSRCS = $(wildcard $(SRCDIR)/*.cpp)
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

RM = rm -f


#
# Targets
#

all: lib $(TESTDIR)/$(TESTNAME)

$(LIBS):
	# Don't attempt to compile shared library dependencies.

lib: lib$(LIBNAME).so

lib$(LIBNAME).so: $(LIBOBJS)
	@$(RM) $@
	$(CPP) $(CPPFLAGS) -shared $^ -o $@

$(TESTDIR)/$(TESTNAME): $(TESTOBJS) lib$(LIBNAME).so $(LIBS)
	@$(RM) $@
	$(CPP) $(CPPFLAGS) $^ -o $@

test: $(TESTDIR)/$(TESTNAME)
	@./$<

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	@$(RM) $(SRCDIR)/*.o lib$(LIBNAME).so $(TESTDIR)/$(TESTNAME)
