EXTRAS ?= ../build/extras
GOBS = numbers.gob objects.gob rabbit.gob
DATAFILES = bump.mod calib.dat death.smp fly.smp font.gob jump.mod \
	jump.smp levelmap.txt level.pcx mask.pcx menu.pcx \
	menumask.pcx $(GOBS) scores.mod splash.smp spring.smp

all: jumpbump.dat

font.gob:
	$(EXTRAS)/gobpack font

rabbit.gob:
	$(EXTRAS)/gobpack rabbit

numbers.gob:
	$(EXTRAS)/gobpack numbers

objects.gob:
	$(EXTRAS)/gobpack objects

jumpbump.dat: $(DATAFILES)
	$(EXTRAS)/jnbpack -o jumpbump.dat $(DATAFILES)

clean:
	rm -f jumpbump.dat $(GOBS)
