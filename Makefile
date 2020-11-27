$(CC) = gcc
final:
	$(CC) aux_myprompt.c mypro.h  myprompt.c -o final

Clean:
	rm *.o final