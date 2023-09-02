COMPILER_FLAGS = gcc -g -Wall -ansi -pedantic -o $@
O_FILES = pre_processor.o macro_table.o utilities.o  files.o first_pass.o second_pass.o macros.o main.o

assembler: $(O_FILES)
	$(COMPILER_FLAGS) $^

%.o: %.c %.h
	$(COMPILER_FLAGS) -c $<

clean:
	rm -rf *.o
