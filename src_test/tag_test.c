#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tag.h"


int TEST_NUMBER = 0;
int TEST_SUCCESSFULL = 0;


/**
 * =================================================================
 * === Small CUnit Librarie 
 * =================================================================
 */

 /* helper to print byte */
 void print_byte(int x){
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

/* helper to print byte */
void print_byte_int(int x){
   int hi, lo;
   hi=(x>>8) & 0xff;
   lo=x&0xff;
   print_byte(hi);
   printf(" ");
   print_byte(lo);
}

void print_pre_test(char * name){

	printf("--------------------------------------------- \n");
	printf("--- Test %d : %s \n",TEST_NUMBER,name);
	printf("--------------------------------------------- \n");
	TEST_NUMBER++;
}

void print_post_test(int result){

	if (result){
		TEST_SUCCESSFULL++;
		printf("State: OK\n");
	}else{
		printf("State: ERROR\n");
	}
	printf("===\t  Global Result : %d/%d \t\t====\n",TEST_SUCCESSFULL,TEST_NUMBER);
	printf("--------------------------------------------\n\n");
}

void assertTrue(char * name, int value){

	print_pre_test(name);
	(value) ? print_post_test(1) : print_post_test(0);
}

void assertTrue_int(char * name, int value, int expectedValue){

	print_pre_test(name);
	printf("Value         :\t%d\n",value);
	printf("Expected Value:\t%d\n",expectedValue);

	(value == expectedValue) ? print_post_test(1) : print_post_test(0);
}

void assertTrue_byte(char * name, int value, int expectedValue){

	print_pre_test(name);

	printf("Value           :\t");print_byte_int(value);printf("\n");
	printf("Expected Value  :\t");print_byte_int(expectedValue);printf("\n");

	(value == expectedValue) ? print_post_test(1) : print_post_test(0);
}


void assertTrue_hexa(char * name, int value, int expectedValue){

	print_pre_test(name);
	printf("Value:\t%x\n",value);
	printf("ExpectedValue:\t%x\n",expectedValue);

	(value == expectedValue) ? print_post_test(1) : print_post_test(0);
}

void assertTrue_char(char * name, char * value, char * expectedValue){

	print_pre_test(name);
	printf("Value        :\t%s\n",value);
	printf("ExpectedValue:\t%s\n",expectedValue);

	(strcmp(value,expectedValue) == 0) ? print_post_test(1) : print_post_test(0);
}

/**
 * =================================================================
 * === Small CUnit Librarie 
 * =================================================================
 */

int open_file(){ return open ("resources/OneWayRoad.mp3", O_RDONLY ); }

void test8(){
	int fd = open_file();
	u8 * myu8 = malloc(sizeof(u8));
	
	read_u8(fd,myu8);

	assertTrue_int("U8 Reader",*myu8, 0x49);
	
	close(fd);
}

void test_converts(){

	assertTrue_byte("Convert Size", convert_size(1836543598),230553454);
	assertTrue_byte("Convert Size", convert_size(17782),8950);
}



void test16(){
	int fd = open_file();
	u16 * myu16 = malloc(sizeof(u16));

	read_u16(fd,myu16);

	assertTrue_hexa("U16 Reader",*myu16,0x4944);
	
	close(fd);
}

void test32(){
	int fd = open_file();
	u32 * myu32 = malloc(sizeof(u32));
	
	read_u32(fd,myu32);
	
	assertTrue_hexa("U32 Reader ",*myu32,0x49443303);
	
	close(fd);
}

void test_read_header(){

	int fd = open_file();
	id3v2_header_t * header = malloc(sizeof(id3v2_header_t));

	assertTrue_int("Header Read",tag_read_id3_header(fd,header),0);
	assertTrue_char("Header format ID3", header->ID3,"ID3");
	assertTrue_hexa("Header version", header->version,0x0300);
	assertTrue_hexa("Header size",header->size,0x1e6fa);

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
