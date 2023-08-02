MODULES = main globals utils engine3d render logic3d
LIBS = -lSDL2 -lSDL2_image -lm
DBGFLGS = -Wall -g
OPTFLGS = -Ofast
SRCPATH = ./src
BINPATH = ./bin
OBJPATH = ./bin/obj
DBGOPRX = DBG_
OPTOPRX = OPT_
DBGBIN = $(BINPATH)/debug
OPTBIN = $(BINPATH)/optimized

HEADERS = $(addprefix $(SRCPATH)/, $(addsuffix .h, $(MODULES)))
SOURCES = $(addprefix $(SRCPATH)/, $(addsuffix .c, $(MODULES)))
DBGOBJS = $(addprefix $(OBJPATH)/$(DBGOPRX), $(addsuffix .o, $(MODULES)))
OPTOBJS = $(addprefix $(OBJPATH)/$(OPTOPRX), $(addsuffix .o, $(MODULES)))

.PHONY: debug optimized clean

$(OBJPATH)/$(DBGOPRX)%.o: $(SRCPATH)/%.c $(SRCPATH)/%.h
	gcc -c $(DBGFLGS) $(LIBS) -o $@ $<

$(OBJPATH)/$(OPTOPRX)%.o: $(SRCPATH)/%.c $(SRCPATH)/%.h
	gcc -c $(OPTFLGS) $(LIBS) -o $@ $<

debug: $(DBGOBJS)
	gcc $(DBGFLGS) $(LIBS) -o $(DBGBIN) $^
	$(DBGBIN)

optimized: $(OPTOBJS)
	gcc $(OPTFLGS) $(LIBS) -o $(OPTBIN) $^
	$(OPTBIN)

clean:
	rm -rf $(BINPATH)/*
	mkdir $(OBJPATH)

install:
	mkdir $(BINPATH)
	mkdir $(OBJPATH)