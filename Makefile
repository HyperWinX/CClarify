all:
	@gcc test.c -o test -std=c17 -g
	@./test
	@rm test
