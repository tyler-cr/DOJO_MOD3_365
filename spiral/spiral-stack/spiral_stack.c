#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define WIDTH 69
#define HEIGHT 29

#define RESET  "\033[0m"
#define CYAN   "\033[36m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define RED    "\033[31m"
#define CLEAR  "\033[2J\033[H"

char *EDGE   = "X~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~X";

char *MENU1  = "{  @           @@@  @   @ @@@@@ @@@@  @   @ @@@@  @@@ @@@@@ @@@@@        @@@         @@@@  @@@@@ @@@@@       @@@@         @@@@@ @   @ @@@ @@@@@ }";
char *MENU2 =  "{ @@          @   @ @   @ @     @   @ @   @ @   @  @    @   @           @   @        @   @ @       @             @        @      @ @   @    @   }";
char *MENU3 =  "{  @          @   @ @   @ @@@@  @@@@  @ @ @ @@@@   @    @   @@@@           @         @@@@  @@@@    @          @@@         @@@@    @    @    @   }";
char *MENU4 =  "{  @    @     @   @  @ @  @     @  @  @@ @@ @  @   @    @   @             @    @     @  @  @       @             @  @     @      @ @   @    @   }";
char *MENU5 =  "{ @@@   @      @@@    @   @@@@@ @   @ @   @ @   @ @@@   @   @@@@@       @@@@@  @     @   @ @@@@@   @         @@@@   @     @@@@@ @   @ @@@   @   }";

void win(){
	int fd_1 = open("/flag", O_RDONLY);
        char *flag = calloc(0x50, 1);
        size_t len = read(fd_1, flag, 0x50);

	puts(flag);
}


long get_fib_num(size_t index){

	long first = 0;
	long second = 1;
	long third = first + second;

	if (index == first) return first;
	else if (index == second) return second;	
		
	for (size_t i = 1; i < index; i++){
		third = first + second;
		first = second;
		second = third;
	}

        return third;
}


void print_intro() {
    char grid[HEIGHT][WIDTH];
    const char arms[] = {'R', 'G', 'B'};
    const char *colors[] = {
        "\x1b[0m", "\x1b[31m", "\x1b[32m", "\x1b[34m", "\x1b[33m"
    };
    const double pi = acos(-1.0);
    const double phi = (1.0 + sqrt(5.0)) / 2.0;
    const int cx = WIDTH / 2;
    const int cy = HEIGHT / 2;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[y][x] = ' ';
        }
    }

    for (int arm = 0; arm < 3; arm++) {
        double offset = arm * 2.0 * pi / 3.0;
        for (int step = 0; step < 9000; step++) {
            double theta = (double)step / 8999.0 * 3.15 * pi;
            double radius = 0.65 * pow(phi, 2.0 * theta / pi);
            int x = (int)lround(cx + 1.68 * radius * cos(theta + offset));
            int y = (int)lround(cy - 0.78 * radius * sin(theta + offset));
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                grid[y][x] = arms[arm];
            }
        }
    }

    for (int arm = 0; arm < 3; arm++) {
        double theta = 2.3 * pi;
        double radius = 0.65 * pow(phi, 2.0 * theta / pi);
        double offset = arm * 2.0 * pi / 3.0;
        int x = (int)lround(cx + 1.68 * radius * cos(theta + offset));
        int y = (int)lround(cy - 0.78 * radius * sin(theta + offset));
        if (x > 0 && x < WIDTH - 1 && y >= 0 && y < HEIGHT) {
            grid[y][x - 1] = '(';
            grid[y][x] = 'o';
            grid[y][x + 1] = ')';
        }
    }

    grid[cy][cx - 1] = '(';
    grid[cy][cx] = '@';
    grid[cy][cx + 1] = ')';

    int first = HEIGHT;
    int last = -1;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[y][x] != ' ') {
                if (y < first) first = y;
                if (y > last) last = y;
            }
        }
    }

    for (int y = first; y <= last; y++) {
        int end = WIDTH - 1;
        while (end >= 0 && grid[y][end] == ' ') end--;
        int previous = 0;
        for (int x = 0; x <= end; x++) {
            char ch = grid[y][x];
            int color = ch == 'R' ? 1 : ch == 'G' ? 2 : ch == 'B' ? 3 : ch == ' ' ? 0 : 4;
            if (color != previous) {
                fputs(colors[color], stdout);
                previous = color;
            }
            putchar(ch);
        }
        fputs(colors[0], stdout);
        putchar('\n');
    }
    printf(RESET);
}

void print_menu() {
    printf(CLEAR);

    print_intro();

    printf("\n" GREEN);
    puts(EDGE);
    puts(MENU1);
    puts(MENU2);
    puts(MENU3);
    puts(MENU4);
    puts(MENU5);
    puts(EDGE);
    printf(RESET);
}


void challenge(){
	size_t offset;
	unsigned int value;
	unsigned char selection = 0;
	long fib_index;
	long fib_number;
	char overwrite_char;
	char buffer[0x30] = {0};
	

	print_menu();
	while (1){

		printf("\nThe Spiral awaits your choice: ");
		fflush(stdout);
		
		if (scanf("%hhu", &selection) != 1) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			puts(RED "Invalid input. The Spiral demands a number." RESET);
			continue;
		}
		
		switch (selection) {
			case 1:
				puts("You have selected to overwrite.");
				puts("The Spiral awaits your index: ");

				if (scanf("%ld", &fib_index) != 1){
					int c;
					while((c = getchar()) != '\n' && c != EOF);
					puts(RED "Invalid input. The Spiral demands a number." RESET);
					continue;
				}
				

				fib_number = get_fib_num(fib_index) % 0x6a;

				puts("The Spiral awaits your char you: ");

				if (read(STDIN_FILENO, &overwrite_char, 1) != 1){
					puts(RED "Failed to read byte." RESET);
					exit(1);
				}

				buffer[fib_number] = overwrite_char;

				break;
			case 2:
				printf("The Spiral awaits your return.\n");
				return;
				break;
			case 3:
				printf("The Spiral judges you...\n");
				exit(0);
				break;

		}
	
	}
}

void main(){
	challenge();
}


