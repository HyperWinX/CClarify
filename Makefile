all:
	@gcc test.c -o test
	@./test
	@rm test
