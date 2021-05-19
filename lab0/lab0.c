//NAME: Qinglin Zhang
//ID: 205356739
//EMAIL:qqinglin0327@gmail.com
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void handler()
{
  fprintf(stderr, "Segmentation fault caught\n");
  exit(4);
}

int main(int argc, char* argv[]){
  //http://man7.org/linux/man-pages/man3/getopt.3.html
  struct option options[] = {
    {"input",1,NULL,'i'},
    {"output",1,NULL,'o'},
    {"segfault",0,NULL,'s'},
    {"catch",0,NULL,'c'},
    {0,0,0,0}
  };
  int input = 0;
  int output = 0;
  int segfault = 0;
  int catch = 0;
  char opt;
  char* input_filename;
  char* output_filename;
  while(1){
    opt = getopt_long(argc, argv, "", options, 0);
    if(opt == -1)
      break;
    else if (opt == '?'){
      fprintf(stderr, "Arguments Error: --input=filename --output=filename --segfault --catch\n");
      exit(1); //unrecognized argument
    }
    else if (opt == 'i'){
      input = 1;
      input_filename = optarg;
    }
    else if (opt == 'o'){
      output = 1;
      output_filename = optarg;
    }
    else if (opt == 's'){
      segfault =1;
    }
    else if (opt == 'c'){
      catch = 1;
      
    }
    
  }
  if(input){
    int read = open(input_filename, O_RDONLY);
    if(read == -1){
      fprintf(stderr, "Input File Error: %s -- %s\n", input_filename, strerror(errno));//http://www.cplusplus.com/reference/cstring/strerror/
      exit(2);
    }
    close(0);
    dup(read);
    close(read);}
  if(output){
    int write = creat(output_filename, 0666);
    if(write == -1){
      fprintf(stderr,"Output File Error: %s -- %s\n", output_filename, strerror(errno));
      exit(3);
    }
    close(1);
    dup(write);
    close(write);
}
  if(catch){
    signal(SIGSEGV, handler);
}
  if(segfault){
    char* segfault = NULL;
    *segfault = 'w';
}  
  char c;
  ssize_t status = read(0, &c, 1);
  if (status == -1)
    {
      fprintf(stderr, "Input File Error: %s -- %s\n", input_filename, strerror(errno));
      exit(2);
    }
  while(status > 0){
    status = write(1,&c,1);
    if(status == -1){
      fprintf(stderr,"Output File Error: %s -- %s\n", output_filename, strerror(errno));
      exit(3);
    }
    status = read(0, &c, 1);
    if (status == -1)
      {
	fprintf(stderr, "Input File Error: %s -- %s\n", input_filename, strerror(errno));
	exit(2);
      }
  }
  exit(0);
}
