#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

void main(){
	int fd_1 = open("/flag", O_RDONLY);

	char *flag = calloc(0x50, 1);
	
	size_t len = read(fd_1, flag, 0x50);

	unsigned char fir, sec;
	unsigned char top_1, bot_1, top_2, bot_2;
	
	int fd_2 = open("/challenge/flag_cipher.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	char *cipher_flag = calloc(len+2, 1);


	for(size_t i = 0; i < (len+1)/2; i++){
		fir = flag[2*i];
		sec = flag[2*i + 1];

		top_1 = fir << 4;
		top_2 = fir >> 4;
		bot_1 = sec >> 4;
		bot_2 = sec << 4;
		cipher_flag[2*i] = top_1 + bot_1;
		cipher_flag[2*i+1] = top_2 + bot_2;
	}

	write(fd_2, cipher_flag, 2*((len+1) / 2));

	printf("flag_cipher.bin created!\n");

}
