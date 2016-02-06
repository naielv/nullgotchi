PNGS:=$(wildcard *.png)
INCLUDES:=$(PNGS:.png=.rle.h)
OUT_DIR:=../firmware
OUTPUT:=$(OUT_DIR)/icons.h

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) $(INCLUDES)

.PHONY: all clean

$(OUTPUT): $(INCLUDES)
	cat $(INCLUDES) >"$@"

%.rle.h: %.rle
	xxd -i "$<" | sed -e 's/unsigned char/const PROGMEM byte/' >"$@"

%.rle: %.png
	./rle.py "$<"
