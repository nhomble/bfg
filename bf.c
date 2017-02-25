#include <stdio.h>
#include <stdlib.h>

// close enough to INF for a bf program
#define MEM_LENGTH 	(2 << 8)

long file_size(char *path){
	FILE *f;
	long length;

	f = fopen (path, "rb");
	length = 0;
	if(f){
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fclose(f);
	}
	return length;
}

int load_file(char *buffer, long length, char *path){
	FILE *f;
	f = fopen(path, "rb");
	if(!f)
		return 0;
	fseek(f, 0, SEEK_SET);
	fread(buffer, 1, length, f);
	fclose(f);
	return 1;
}

void usage(char* prog){
	printf("usage: %s [path to brainfuck file]\n", prog);
}

void interpret(char *bf, long length){
	unsigned char memory[MEM_LENGTH] = {0};
	int ip, inner_loop;
	unsigned int ptr;
	char input;

	ptr = 0;
	for(ip = 0; ip < length; ip++){
		input = bf[ip];
		switch(input){
			case '+': memory[ptr]++; break;
			case '-': memory[ptr]--; break;
			case '>': ptr++; break;
			case '<': ptr--; break;
			case ',': memory[ptr] = getchar(); break;
			case '.': putchar(memory[ptr]); break;
			case '[':
				// can we execute the loop
				if(memory[ptr])
					break;	// yes we can
				// nope
				inner_loop = 0;
				ip++;
				for(; ip < length; ip++){
					if(bf[ip] == ']' && !inner_loop)
						break;
					if(bf[ip] == '[')
						inner_loop++;
					if(bf[ip] == ']')
						inner_loop--;
				}
				ip--; // we are going to increment in just a bit
				break;
			case ']':
				// should we loop
				if(!memory[ptr])
					break; // nope
				inner_loop = 0;
				ip--;
				for(; ip >= 0; ip--){
					if(bf[ip] == '[' && inner_loop == 0)
						break;
					if(bf[ip] == ']')
						inner_loop++;
					if(bf[ip] == '[')
						inner_loop--;
				}
				ip--; // we are going to increment in just a bit
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
	length = file_size(path);
	if(!length)
		return -1;

	buffer = malloc(sizeof(char) * length);
	if(!load_file(buffer, length, path))
		return -1;	
	
	interpret(buffer, length);
	free(buffer);
	return 0;
}
