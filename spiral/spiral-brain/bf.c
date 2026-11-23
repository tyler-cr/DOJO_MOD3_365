#include "fcntl.h"
#include "unistd.h"
#include "stdio.h"

#define CODE_CAP  4096
#define IN_CAP    0x4000
#define TAPE_SIZE 30000
#define OUT_CAP   1024
#define FILE_CAP  0x18000

static char code[CODE_CAP + 1];
static int  code_len;
static unsigned char input[IN_CAP];
static int  input_len, input_pos;
static unsigned char tape[TAPE_SIZE];
static int  dp;
static char outbuf[OUT_CAP];
static int  out_pos;
static char filebuf[FILE_CAP];

static void flush_out(void)
{
  if (out_pos == 0)
    return;
  outbuf[out_pos] = '\0';
  printf(outbuf);
  out_pos = 0;
}

static void emit(unsigned char c)
{
  outbuf[out_pos++] = (char) c;
  if (c == '\n' || out_pos >= OUT_CAP - 1)
    flush_out();
}

static int load_program(void)
{
  int fd = open("/usr/prog", O_RDONLY);
  if (fd < 0)
    return -1;
  int total = 0, r;
  while (total < FILE_CAP && (r = read(fd, filebuf + total, FILE_CAP - total)) > 0)
    total += r;
  close(fd);

  int i = 0;
  while (i < total && filebuf[i] != '\n' && code_len < CODE_CAP)
    code[code_len++] = filebuf[i++];
  code[code_len] = '\0';
  while (i < total && filebuf[i] != '\n')
    i++;
  if (i < total)
    i++;
  while (i < total && filebuf[i] != '\n' && input_len < IN_CAP)
    input[input_len++] = (unsigned char) filebuf[i++];
  return 0;
}

static int match_bracket(int pc)
{
  int depth = 0;
  if (code[pc] == '[')
  {
    for (int i = pc; i < code_len; i++)
    {
      if (code[i] == '[') depth++;
      else if (code[i] == ']' && --depth == 0) return i;
    }
  }
  else
  {
    for (int i = pc; i >= 0; i--)
    {
      if (code[i] == ']') depth++;
      else if (code[i] == '[' && --depth == 0) return i;
    }
  }
  return -1;
}

static void run(void)
{
  for (int pc = 0; pc < code_len; pc++)
  {
    switch (code[pc])
    {
      case '>': if (dp < TAPE_SIZE - 1) dp++; break;
      case '<': if (dp > 0) dp--; break;
      case '+': tape[dp]++; break;
      case '-': tape[dp]--; break;
      case '.': emit(tape[dp]); break;
      case ',': tape[dp] = (input_pos < input_len) ? input[input_pos++] : 0; break;
      case '[':
        if (tape[dp] == 0) { int m = match_bracket(pc); if (m < 0) return; pc = m; }
        break;
      case ']':
        if (tape[dp] != 0) { int m = match_bracket(pc); if (m < 0) return; pc = m; }
        break;
      default: break;
    }
  }
}

int main(void)
{
  if (load_program() < 0)
    return 1;
  run();
  flush_out();
  return 0;
}
