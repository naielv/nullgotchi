all:
	$(MAKE) -C font
	$(MAKE) -C icons
	$(MAKE) -C screen-prototype

clean:
	$(MAKE) -C font clean
	$(MAKE) -C icons clean
	$(MAKE) -C screen-prototype clean

.PHONY: all clean
