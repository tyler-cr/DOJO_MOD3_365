#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define RESET  "\033[0m"
#define CYAN   "\033[36m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define RED    "\033[31m"
#define CLEAR  "\033[2J\033[H"

char *EDGE   = "X~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~X";
char *INTRO1 = "{        @   @ @@@@@ @      @@@   @@@  @   @ @@@@@    @@@@@  @@@      @@@@ @@@@  @@@ @@@@   @@@  @         @@@@ @@@@@  @@@   @@@  @   @         }";
char *INTRO2 = "{        @   @ @     @     @     @   @ @@ @@ @          @   @   @    @     @   @  @  @   @ @   @ @        @       @   @   @ @     @  @          }";
char *INTRO3 = "{        @ @ @ @@@@  @     @     @   @ @ @ @ @@@@       @   @   @     @@@  @@@@   @  @@@@  @@@@@ @         @@@    @   @@@@@ @     @@@           }";
char *INTRO4 = "{        @@ @@ @     @     @     @   @ @   @ @          @   @   @        @ @      @  @  @  @   @ @            @   @   @   @ @     @  @          }";
char *INTRO5 = "{        @   @ @@@@@ @@@@@  @@@   @@@  @   @ @@@@@      @    @@@     @@@@  @     @@@ @   @ @   @ @@@@@    @@@@    @   @   @  @@@  @   @         }";

char *MENU1  = "{  @           @@@  @   @ @@@@@ @@@@  @   @ @@@@  @@@ @@@@@ @@@@@        @@@         @@@@  @@@@@ @@@@@       @@@@         @@@@@ @   @ @@@ @@@@@ }";
char *MENU2 =  "{ @@          @   @ @   @ @     @   @ @   @ @   @  @    @   @           @   @        @   @ @       @             @        @      @ @   @    @   }";
char *MENU3 =  "{  @          @   @ @   @ @@@@  @@@@  @ @ @ @@@@   @    @   @@@@           @         @@@@  @@@@    @          @@@         @@@@    @    @    @   }";
char *MENU4 =  "{  @    @     @   @  @ @  @     @  @  @@ @@ @  @   @    @   @             @    @     @  @  @       @             @  @     @      @ @   @    @   }";
char *MENU5 =  "{ @@@   @      @@@    @   @@@@@ @   @ @   @ @   @ @@@   @   @@@@@       @@@@@  @     @   @ @@@@@   @         @@@@   @     @@@@@ @   @ @@@   @   }";

void win(){
	int fd_1 = open("/flag", O_RDONLY);
        char *flag = calloc(0x50, 1);
        size_t len = read(fd_1, flag, 0x50);

	printf("CONGRATULATIONS! Here is your flag! \n");
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
    printf(GREEN);
    puts(EDGE);
    puts(INTRO1);
    puts(INTRO2);
    puts(INTRO3);
    puts(INTRO4);
    puts(INTRO5);
    puts(EDGE);
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

		printf("\nPlease select your option: ");
		fflush(stdout);
		
		if (scanf("%hhu", &selection) != 1) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			puts(RED "Invalid input. Please enter a number." RESET);
			continue;
		}
		
		switch (selection) {
			case 1:
				puts("You have selected to overwrite.");
				puts("Please select your fibonacci index: ");

				if (scanf("%ld", &fib_index) != 1){
					int c;
					while((c = getchar()) != '\n' && c != EOF);
					puts(RED "Invalid input. Please enter an integer." RESET);
					continue;
				}
				

				fib_number = get_fib_num(fib_index) % 0x6a;

				puts("Please select the char you would like to overwrite with: ");

				if (read(STDIN_FILENO, &overwrite_char, 1) != 1){
					puts(RED "Failed to read byte." RESET);
					exit(1);
				}

				buffer[fib_number] = overwrite_char;

				break;
			case 2:
				printf("You have selected to return. Come back soon!\n");
				return;
				break;
			case 3:
				printf("You have selected to exit. Goodbye!\n");
				exit(0);
				break;

		}
	
	}
}

void main(){
	challenge();
}


