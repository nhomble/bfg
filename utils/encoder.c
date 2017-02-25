#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char *prog){
	fprintf(stderr, "usage: %s [message to encode]\n", prog);
}

void encode_char(char c){
	int i;
	for(i = 0; i < (int) c; i++)
		printf("+");
	printf(".");
}

void encode_string(char *message){
	int length, i;
	char c;
	// verbose bf file until I optimize TODO
	length = strlen(message);
	for(i = 0; i < length; i++){
		c = message[i];
		encode_char(c);
		printf(">");
	}
	encode_char('\n');
}

int main(int argc, char**argv){
	char *message;
	if(argc != 2){
		usage(argv[0]);
		return 0;
	}
	message = argv[1];
	encode_string(message);
	return 0;
}
