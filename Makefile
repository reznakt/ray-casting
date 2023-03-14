BUILDDIR = build


all: build


.PHONY: clean
.NOTPARALLEL: force


build: *.c *.h
	cmake -B $(BUILDDIR)
	cmake --build $(BUILDDIR) --parallel


run: build
	./"`find $(BUILDDIR) -maxdepth 1 -type f -executable`"


clean:
	rm -rf $(BUILDDIR)


force: clean all

