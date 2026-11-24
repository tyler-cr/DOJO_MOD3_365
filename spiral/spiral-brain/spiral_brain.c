#include "fcntl.h"
#include "unistd.h"
#include "stdio.h"
#include "stdint.h"
#include <sys/syscall.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define CODE_CAP 0x100
#define TAPE_SIZE 0x1000
#define REGISTER_SIZE 0x07

static char code[CODE_CAP+1];
static unsigned char tape[TAPE_SIZE];
static unsigned int fib[TAPE_SIZE];

static uintptr_t regs[REGISTER_SIZE];

int code_index, true_tape_index, tape_index;
int code_len;

void print_spiral(void) {
    enum { W = 101, H = 43 };
    int grid[H][W] = {0};

    const char *colors[] = {
        "\033[38;2;255;30;30m",
        "\033[38;2;30;255;70m",
        "\033[38;2;40;90;255m"
    };

    double pi = acos(-1);
    double phi = (1 + sqrt(5)) / 2;
    double end = 6 * pi;

    for (int arm = 0; arm < 6; arm++) {
        for (double t = 0; t <= end; t += 0.002) {
            double r = 18 * pow(phi, 2 * (t - end) / pi);
            double a = t + arm * pi / 3;

            int x = W / 2 + lround(2.4 * r * cos(a));
            int y = H / 2 + lround(r * sin(a));

            if (x >= 0 && x < W && y >= 0 && y < H)
                grid[y][x] = arm % 3 + 1;
        }
    }

    const char *eye[] = {
        "\\\\\\\\\\                       /////",
        " \\\\\\\\\\                     ///// ",
        "  \\\\\\\\\\___________________/////  ",
        "   \\                         /   ",
        "    \\    ####   |   ####    /    ",
        "     \\   ####  |||  ####   /     ",
        "      \\_______|||||_______/      ",
        "              |||||              ",
        "               |||               ",
        "                |                "
    };

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            double dx = (x - W / 2) / 2.4;
            double dy = y - H / 2;
            double r = hypot(dx, dy);
            double a = atan2(dy, dx);

            int ey = y - H / 2 + 5;

            if (ey >= 0 && ey < 10) {
                int len = strlen(eye[ey]);
                int ex = x - (W - len) / 2;

                if (ex >= 0 && ex < len) {
                    printf("\033[1;38;2;255;80;0m%c",
                           eye[ey][ex]);
                    continue;
                }
            }

            if (grid[y][x]) {
                int c = grid[y][x] - 1;
                printf("%s%c", colors[c], "RGB"[c]);
                continue;
            }

            double flame = 17 + 2 * sin(19 * a)
                              + sin(31 * a);

            if (r > 15 && r < flame) {
                int heat = (int)(180 + 75 * sin(r * 2 + a * 7));

                printf("\033[38;2;255;%d;0m%c",
                       heat, "^~*:"[(x + y) % 4]);
            } else {
                putchar(' ');
            }
        }

        printf("\033[0m\n");
    }

    printf("\033[1;38;2;255;0;0m");
    printf("\n");
    printf("                         I DEMAND YOUR INVOCATION.\n");
    printf("\033[0m\n");
}


long calc_fib(int index){
	unsigned long first = 0;
	unsigned long second = 1;
	unsigned long third;
	
	for (int i = 0; i < index; i++){
		third = (second + first) % TAPE_SIZE;
		first = second;
		second = third;
	}

	return first;
}

void set_register(char reg, uintptr_t value){
	reg -= 0x61;
	regs[reg] = value;
}

void do_syscall(){
	syscall(regs[0], regs[1], regs[2], regs[3], regs[4], regs[5], regs[6]);
}

int match_bracket(int code_index){
	int depth = 0;
	if (code[code_index] == '['){
		for (int i = code_index; i < code_len; i++){
			if (code[i] == '[') depth++;
			else if (code[i] == ']' && --depth == 0) return i;
		}
	}
	else{
		for (int i = code_index; i >= 0; i--){
			if (code[i] == ']') depth++;
			else if (code[i] == '[' && --depth == 0) return i;
		}
	}

	return -1;
}

void run(){
	while(code[code_index] != 0){
		char cur_inst = code[code_index];
		switch (cur_inst){
			case '>':
				true_tape_index = (true_tape_index + 1) % TAPE_SIZE;
				tape_index = calc_fib(true_tape_index) % 0x1000;
				break;
			case '<':
				true_tape_index = (true_tape_index - 1 + TAPE_SIZE) % TAPE_SIZE;
				tape_index = calc_fib(true_tape_index) % 0x1000;
				break;
			case '+':
				fib[tape_index]++;
				tape[tape_index] = calc_fib(fib[tape_index]);
				break;
			case '-':
				fib[tape_index]--;
				tape[tape_index] = calc_fib(fib[tape_index]);
				break;
			case '.':
				putchar(tape[tape_index]);
				break;
			case ',':
				read(0,&tape[tape_index], 1);
				fib[tape_index] = tape[tape_index];
				tape[tape_index] = calc_fib(tape[tape_index]);
				break;
			case '[':
				if (tape[tape_index] == 0){
					int m = match_bracket(code_index);
					if (m < 0) return;
					code_index = m;
				}
				break;
			case ']':
				if (tape[tape_index] != 0){
					int m = match_bracket(code_index);
					if (m < 0) return;
					code_index = m;
				}
				break;
			case '!':
				tape[tape_index] = fib[tape_index];
				break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
				set_register(cur_inst, tape[tape_index]);
				break;
			case '&':
				code_index++;
				char reg = code[code_index];
				if (reg < 'a' || reg > 'g') {
					puts("Even The Spiral demands correct syntax");
					exit(5);
				}

				set_register(reg, (uintptr_t)&tape[tape_index]);
				break;
			case 's':
				do_syscall();
				break;
			default:
				puts("The Spiral demands perfect scripture");
				exit(3);
		}
		code_index++;
	}
}

int main(int argc, char *argv[]){
	print_spiral();
	
	if (argc != 2) {
		puts("The Spiral demands you to speak in the forbidden tongue");
		return 2;
	}

	int fd = open(argv[1],0,0);
	if (fd < 0){
		puts("The Spiral cares not for this scripture");
		return 1;
	}

	code_len = read(fd, code, CODE_CAP);
	close(fd);
	if (code_len < 0){
		puts("The Spiral demands that you speak");
		return 1;
	}
	
	run();
	return 0;

}

