CC := gcc
CFLAGS := 				\
	-O0					\
	-g					\
	-Wall				\
	-Wextra				\
	-Werror				\
	-ansi				\
	-pedantic			\
	-pedantic-errors	\


BUILD_DIRECTORY = $(shell pwd)/build
DIRECTORY_GUARD=@mkdir -p $(@D)

SRC = $(wildcard *.c)
OBJECTS = $(patsubst %.c, $(BUILD_DIRECTORY)/%.o, $(SRC))
BINARY = riscv-emulator

$(BINARY): $(OBJECTS)
	$(DIRECTORY_GUARD)
	$(CC) $(CFLAGS) -g $^ -o $@ 

$(BUILD_DIRECTORY)/%.o: %.c
	$(DIRECTORY_GUARD)
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c -o $@ $<


indent:
	@sh ./indent.sh 

clean:
	@rm riscv-emulator 
	@rm -r build 

.phony: indent 
.phony: clean
