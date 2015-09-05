CC := g++
UNAME_S := $(shell uname -s | cut -d_ -f1)
UNAME_P := $(shell uname -p)
OUTPUT_BASE := lobots_srv
OUTPUT := $(OUTPUT_BASE)_$(UNAME_S)_$(UNAME_P)
FLAGS := -Wno-write-strings -lpthread

define \n


endef

INTERFACE_XML_FILES := $(shell ls src/interface/*.xml | xargs -n 1 | sed 's/\(.*\)\..*/\1/')
INTERFACE_OBJ := src/interface/*.h
EASYBMP_OBJ := src/widget/support/easybmp/*.h src/widget/support/easybmp/*.cpp
WIDGET_OBJ := $(EASYBMP_OBJ) src/widget/support/*.h src/widget/support/*.cpp src/widget/*.cpp src/widget/*.h
SIMULATOR_OBJ := src/simulator/*.cpp src/simulator/*.h
BLOCKS_OBJ := src/blocks/*.cpp src/blocks/*.h
LOBOTS_SRV_OBJ := $(BLOCKS_OBJ) $(WIDGET_OBJ) $(SIMULATOR_OBJ) src/*.cpp src/*.h src/endian/*.cpp src/endian/*.h

ifeq ($(UNAME_S),Darwin)
	FLAGS := $(FLAGS) -w -framework OpenGL -framework glut
	CC := g++
	ifeq ($(UNAME_P),i386)
		FLAGS := $(FLAGS) -I/opt/X11/include -L/opt/X11/lib
		CC := g++-4.9
	endif
else 
	ifeq ($(UNAME_S),CYGWIN)
		FLAGS := $(FLAGS) -lglut -lGL -lGLU -D __CYGWIN__
	else
		FLAGS := $(FLAGS) -lglut -lGL -lGLU
	endif
endif

all: clean lobots_srv install

interface:
	@echo "Pre-processing UI XML files";
	$(foreach FILE,$(INTERFACE_XML_FILES), xxd -i $(FILE).xml | sed s/}\;/,0x00}\;/ > $(FILE).h ${\n})

lobots_srv: interface $(LOBOTS_SRV_OBJ)  
	$(CC) $(LOBOTS_SRV_OBJ) $(FLAGS) -o $(OUTPUT)
	 
clean:
	rm -f $(OUTPUT_BASE)
	rm -f $(OUTPUT)
	$(foreach FILE,$(INTERFACE_XML_FILES), rm -f $(FILE).h${\n}) 

install:
	ln -s $(OUTPUT) $(OUTPUT_BASE)

debug: FLAGS := $(FLAGS) -g
debug: OUTPUT := $(OUTPUT)_debug
debug: all

distrib:
	mkdir distrib
	cp $(OUTPUT) distrib/
	cp pecas.txt distrib/
	cp pecas_esp.txt distrib/
	

clean-distrib:
	rm -rf distrib
	
