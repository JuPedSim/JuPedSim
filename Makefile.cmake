MKDIR_P = mkdir -p

.PHONY: release debug clean clean-release clean-debug install

DIRS:= bin bin/release bin/debug

all: $(DIRS) release


$(DIRS):
	${MKDIR_P} $(DIRS)


release:
	( cd bin/release && cmake -DCMAKE_BUILD_TYPE=release  -DBUILD_TESTING=OFF ../.. && $(MAKE) --no-print-directory )

debug:
	( cd bin/debug && cmake -DCMAKE_BUILD_TYPE=debug  -DBUILD_TESTING=ON ../.. && $(MAKE) --no-print-directory )

clean: clean-release clean-debug

clean-release:
	( cd bin/release && $(MAKE) --no-print-directory clean )

clean-debug:
	( cd bin/debug && $(MAKE) --no-print-directory clean )


