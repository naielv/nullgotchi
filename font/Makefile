INPUT:=font.png
OUTPUT:=$(INPUT:.png=.h)

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT)

.PHONY: all clean

%.h: %.dat
	xxd -i "$<" | sed -e 's/unsigned char/const PROGMEM byte/' >"$@"

%.dat: %.png
	./dump.py "$<"
