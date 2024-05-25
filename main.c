#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void print_usage(char *program) {
	printf("Usage: %s <filename>\n", program);
	printf("\n");
}

int main(int argc, char **argv) {
	// If no arguments are provided, print usage
	if (argc < 2) {
		print_usage(argv[0]);
		return 1;
	}
	
	// If filename is provided, try to open file
	if (argc > 1) {
		FILE *fp = fopen(argv[1], "rb");
		
		// Verify file could be opened
		if (fp == 0) {
			perror("Error");
			exit(1);
		}
				
		int file_len = 0;
		// Get length of file
		if (fseek(fp, 0, SEEK_END) == 0) {
			// If fseek() succeeds, get position of file pointer
			file_len = ftell(fp);
			
			// Return file pointer to beginning of file
			fseek(fp, 0, SEEK_SET);
		}
		else {
			// If fseek() fails, throw error and exit
			fprintf(stderr, "Error: Could not seek to end of file\n");
			exit(1);
		}
		
		// Handle optional arguments
		if (argc > 2) {
			for (int i=2; i<argc; i++) {
				if (!strcmp("--length", argv[i]) || !strcmp("-l", argv[i])) {
					printf("File is %d bytes long\n", file_len);
					exit(0);
				}
				else if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
					print_usage(argv[0]);
					exit(0);
				}
				else {
					fprintf(stderr, "Error unknown argument '%s'\n", argv[i]);
					print_usage(argv[0]);
					exit(1);
				}
			}
		}
		
		// Buffer to store current byte
		unsigned char rd;
		
		// Buffer to store current word
		unsigned char rd16[17];
		int word_len;
		
		int line_bytes = 0;
		int file_idx = 0;
		
		int num_hex_digits = ceil((log(file_len) / log(2)) / 4.0);
		
		while ((line_bytes = fread(&rd16, sizeof(char), 16, fp)) > 0) {
			// Print address of first byte in line
			char hex_addr_format[32] = {0};
			sprintf(hex_addr_format, "0x%%0%dX: ", num_hex_digits);
			printf(hex_addr_format, ftell(fp) - line_bytes);
			
			for (int i=0; i<line_bytes; i++) {
				printf("%02X ", rd16[i]);
			}
			
			if (line_bytes < 16) {
				for (int i=line_bytes; i<16; i++) {
					printf("   ");
				}
			}
			
			for (int i=0; i<line_bytes; i++) {
				if (rd16[i] > 31 && rd16[i] < 127) {
					printf("%c", rd16[i]);
				}
				else {
					printf(".");
				}
			}
			
			printf("\n");
		}
		return 0;
	}
}
