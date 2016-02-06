INPUT:=font.png
OUT_DIR:=../firmware
OUTPUT:=$(OUT_DIR)/$(INPUT:.png=.h)

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT)

.PHONY: all clean

$(OUT_DIR)/%.h: %.dat
	xxd -i "$<" | sed -e 's/unsigned char/const PROGMEM byte/' >"$@"

%.dat: %.png
	./dump.py "$<"
