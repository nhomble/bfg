#include <stdio.h>
#include <stdlib.h>

// close enough to INF for a bf program
#define MEM_LENGTH 	(2 << 8)

char *load_file(char *path, long *out_length){
	FILE *f;
	long length;
	char *buffer;

	f = fopen(path, "rb");
	if(!f)
		return NULL;
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	if(length <= 0){
		fclose(f);
		return NULL;
	}
	buffer = malloc(sizeof(char) * length);
	if(!buffer){
		fclose(f);
		return NULL;
	}
	fseek(f, 0, SEEK_SET);
	if(!fread(buffer, 1, length, f)){
		free(buffer);
		fclose(f);
		return NULL;
	}
	fclose(f);
	*out_length = length;
	return buffer;
}

void usage(char* prog){
	printf("usage: %s [path to *.bf]\n", prog);
}

int loop(char *bf, long length, char inc, char dec, int dir, int index){
	int ip, inner_loop;

	inner_loop = 0;
	for(ip = index + dir; ip >= 0 && ip < length; ip += dir){
		if(bf[ip] == dec && !inner_loop) 	break;
		if(bf[ip] == inc) 			inner_loop++;
		if(bf[ip] == dec) 			inner_loop--;
	}
	return ip;
}

void interpret(char *bf, long length){
	unsigned char memory[MEM_LENGTH] = {0};
	int ip;
	unsigned int ptr;
	char input;

	ptr = 0;
	for(ip = 0; ip < length; ip++){
		input = bf[ip];
		switch(input){
			case '+': memory[ptr]++; 		break;
			case '-': memory[ptr]--; 		break;
			case '>': ptr = (ptr + 1) % MEM_LENGTH; 	break;
			case '<': ptr = (ptr - 1 + MEM_LENGTH) % MEM_LENGTH; break;
			case ',': memory[ptr] = getchar(); 	break;
			case '.': putchar(memory[ptr]); 	break;
			case '[': if(!memory[ptr]) ip = loop(bf, length, '[', ']',  1, ip) - 1;	break;
			case ']': if(memory[ptr])  ip = loop(bf, length, '[', ']', -1, ip) - 1;	break;
			case '{': ip = loop(bf, length, '{', '}',  1, ip) - 1;			break;
			case '}': ip = loop(bf, length, '{', '}', -1, ip) - 1;			break;
			default: continue;
		}
	}
}

int main(int argc, char** argv){
	char *path, *buffer;
	long length;

	if(argc != 2){
		usage(argv[0]);
		return 0;
	}

	path = argv[1];
	buffer = load_file(path, &length);
	if(!buffer)
		return -1;
	
	interpret(buffer, length);
	free(buffer);
	return 0;
}
