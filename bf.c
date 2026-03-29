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

long loop(char *bf, long length, char inc, char dec, int dir, long index){
	long ip;
	int inner_loop;

	inner_loop = 0;
	for(ip = index + dir; ip >= 0 && ip < length; ip += dir){
		if(bf[ip] == dec && !inner_loop) 	break;
		if(bf[ip] == inc) 			inner_loop++;
		if(bf[ip] == dec) 			inner_loop--;
	}
	if(ip < 0 || ip >= length){
		fprintf(stderr, "error: unmatched '%c' at position %ld\n", bf[index], index);
		return -1;
	}
	return ip;
}

int validate(char *bf, long length){
	long ip;
	char *stack;
	int top;

	stack = malloc(sizeof(char) * (length / 2 + 1));
	if(!stack)
		return 0;
	top = 0;
	for(ip = 0; ip < length; ip++){
		if(bf[ip] == '[' || bf[ip] == '{'){
			stack[top++] = bf[ip];
		} else if(bf[ip] == ']' || bf[ip] == '}'){
			if(top <= 0){
				fprintf(stderr, "error: unmatched '%c' at position %ld\n", bf[ip], ip);
				free(stack);
				return 0;
			}
			char expected = (bf[ip] == ']') ? '[' : '{';
			if(stack[--top] != expected){
				fprintf(stderr, "error: mismatched '%c' at position %ld\n", bf[ip], ip);
				free(stack);
				return 0;
			}
		}
	}
	if(top > 0){
		fprintf(stderr, "error: %d unmatched opening bracket(s)\n", top);
		free(stack);
		return 0;
	}
	free(stack);
	return 1;
}

void interpret(char *bf, long length){
	unsigned char memory[MEM_LENGTH] = {0};
	long ip, jump;
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
			case '[': if(!memory[ptr]){ jump = loop(bf, length, '[', ']',  1, ip); if(jump < 0) return; ip = jump - 1; } break;
			case ']': if(memory[ptr]){  jump = loop(bf, length, ']', '[', -1, ip); if(jump < 0) return; ip = jump - 1; } break;
			case '{': jump = loop(bf, length, '{', '}',  1, ip); if(jump < 0) return; ip = jump - 1; break;
			case '}': jump = loop(bf, length, '}', '{', -1, ip); if(jump < 0) return; ip = jump - 1; break;
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
	
	if(!validate(buffer, length)){
		free(buffer);
		return -1;
	}
	interpret(buffer, length);
	free(buffer);
	return 0;
}
