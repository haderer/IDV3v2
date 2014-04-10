#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../tag.h"


int open_file(){ return open ("resources/OneWayRoad.mp3", O_RDONLY ); }

void prbin(int x);

void test_convert(u32 value, u32 result){
	int r;

	printf("Test U32 Convert %d to %d \n",value,result);
	printf("Binarie representation\t");
	prbin(value);printf("\n");
	printf("Binarie expected\t");
	prbin(result);printf("\n");

	r = convert_size(value);
	printf("Binarie result  \t");
	prbin(r);printf("\n");
	printf("%d\n", r);
	(r == result) ? printf("Test OK\n") : printf("Test ERROR \n");

}

void test_converts(){
	printf("============== TEST U32 CONVERTS ==============\n");
	test_convert(1836543598,230553454);
	test_convert(17782,8950);
	printf("------------------------------------------------\n\n");
}

void test8(){
	int fd;
	u8 * myu8;
	printf("============== TEST READ U8 ====================\n");
	fd = open_file();
	myu8 = malloc(sizeof(u8));
	read_u8(fd,myu8);
	printf("U8 :%x\n",*myu8);
	close(fd);
	printf("------------------------------------------------\n\n");
}

void test16(){
	int fd;
	u16 * myu16;
	printf("============== TEST READ 16 ====================\n");
	fd = open_file();
	myu16 = malloc(sizeof(u16));
	read_u16(fd,myu16);
	printf("U16:%x\n",*myu16);
	close(fd);
	printf("------------------------------------------------\n\n");
}

void test32(){
	int fd;
	u32 * myu32;
	printf("============== TEST READ 32 ====================\n");
	fd = open_file();
	myu32 = malloc(sizeof(u32));
	read_u32(fd,myu32);
	printf("U32 :%x\n",*myu32);
	close(fd);
	printf("------------------------------------------------\n\n");
}


void bin_prnt_byte(int x){
   int n;
   for(n=0; n<8; n++)
   {
      if((x & 0x80) !=0)
      {
         printf("1");
      }
      else
      {
         printf("0");
      }
      if (n==3)
      {
         printf(" "); /* insert a space between nybbles */
      }
      x = x<<1;
   }
}

void prbin(int x){
   int hi, lo;
   hi=(x>>8) & 0xff;
   lo=x&0xff;
   bin_prnt_byte(hi);
   printf(" ");
   bin_prnt_byte(lo);
}

void test_read_header(){
	int fd = open_file();
	id3v2_header_t * header = malloc(sizeof(id3v2_header_t));

	tag_read_id3_header(fd,header);


	close(fd);
}

int main(){

	test8();
	test16();
	test32();
	test_converts();
	test_read_header();
	

	return 0;
}
