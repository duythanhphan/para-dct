# pixeltoaster makefile for apple

# use the default SDK
sdk =

# Uncomment the next line if you want to build your binary on 10.5 Leopard, but want to run it on 10.4 Tiger
#sdk = -mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.4u.sdk

# build 32-bit native only
archs = 

# to build universal binaries use:
#archs = -arch i386 -arch ppc

# when building for 10.5 Leopard you can use the following instead to build 4-way universal binaries
#archs = -arch i386 -arch x86_64 -arch ppc -arch ppc64

CC = clang++

flags   = -O3 -Wall $(sdk) $(archs)
#ldflags = -framework CoreServices -framework Cocoa -framework OpenGL
ldflags =
exec := frequency
corefiles := frequency.cpp
coreobjs := frequency.o
lib := -lpt -ltbb
headdir := -Ipt-headers -I/opt/local/include/
libdir := -Llibs/ -L/opt/local/lib/

all : $(coreobjs)
	$(CC) ${coreobjs} ${flags} ${headdir} $(libdir) $(lib) -o $(exec)

%.o : %.cpp
	$(CC) $< ${flags} ${ldflags} ${headdir}  -c

clean:
	rm -rf $(exec) $(coreobjs)
