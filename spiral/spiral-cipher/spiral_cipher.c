#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>

char *generate_fake_flag(size_t len)
{
    static const char prefix[] = "pwn.college{";
    static const char chars[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_";

    const size_t prefix_len = sizeof prefix - 1;

    char *flag = malloc(prefix_len + len + 2);
    if (flag == NULL) return NULL;

    memcpy(flag, prefix, prefix_len);
    for (size_t i = 0; i < len; i++)
        flag[prefix_len + i] = chars[rand() % (sizeof chars - 1)];

    flag[prefix_len + len] = '}';
    flag[prefix_len + len + 1] = '\0';
    return flag;
}

size_t *get_fib_array(size_t size){
	if (size < 2) return NULL;

	size_t *array = calloc(size, sizeof *array);	
	array[1] = 1;	
	for (size_t i = 2; i < size; i++){
		array[i] = array[i-1] + array[i-2];
	}

	return array;
}

void main(){
	srand((unsigned)time(NULL));
	
	int fd_1 = open("/flag", O_RDONLY);
	char *flag = calloc(0x50, 1);

	size_t len = read(fd_1, flag, 0x50);

	while (len && (flag[len - 1] == '\n' || flag[len - 1] == '\r')) len--;

	size_t *fib_array = get_fib_array(len);

	char *spiral = calloc(0x1000, 1);

	size_t filled = 0;
	size_t fib_index = 0;
	size_t spiral_index;

	while (filled < 0x1000){
		spiral_index = fib_array[fib_index] % 0x1000;

		while(spiral[spiral_index]) { spiral_index = (spiral_index + 1) % 0x1000;}
		
		spiral[spiral_index] = flag[fib_index];	

		fib_index++;
		filled++;

		if (fib_index == len){
			flag = generate_fake_flag(len - 13);
			fib_index = 0;
		}	
	}


	fwrite(spiral, 1, 0x1000, stdout);
}
