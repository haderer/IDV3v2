#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "tag.h"

int read_u8(int fd, u8 *val){

	return read(fd,val,sizeof(u8));
}

int read_u16(int fd, u16 *val){
	
	int r = read(fd,val,sizeof(u16));
	if (r == -1){
	  perror("Error u16 read");	
	  return 0;
	}
	
	*val = *val >> 8 | *val << 8;
	return 1;
}

int read_u32(int fd, u32 *val){

	int r = read(fd,val,sizeof(u32));
	if (r == -1){
	  perror("Error u32 read");	
	  return 0;
	}
		
	*val = ((*val ) << 24)
                        | ((*val & 0xFF00) << 8)
                        | ((*val >> 8) & 0xFF00)
                        | (*val >> 24);
	return 1;
}

u32 convert_size(u32 size){
	int b1,b2,b3,b4;
    
    b1 = (size & 0x000000FF);
    b2 = (size & 0x0000FF00) >> 8;
    b3 = (size & 0x00FF0000) >> 16;
    b4 = (size & 0xFF000000) >> 24;

	return ((b4 & 0x7F) << (7 * 3)) | 
		 ((b3 & 0x7F) << (7 * 2)) |
		 ((b2 & 0x7F) << (7 * 1)) |
		 ((b1 & 0x7F) << (7 * 0));
}


int tag_read_id3_header(int fd, id3v2_header_t *header){

	if (-1 == read(fd,&header->ID3, 3 * sizeof(char))){
		perror("Cannot read ID3 string in header tag");
		return -1;
	}

	/*TODO check IDV3 */

	if (-1 == read_u16(fd,&header->version)){
		perror("Cannot read ID3 version in header tag");
		return -1;
	}

	/*TODO check version */

	if (-1 == read_u8(fd,&header->flags)){
		perror("Cannot read ID3 flag in header tag");
		return -1;
	}

	if (-1 == read_u32(fd,&header->size)) {
		perror("Cannot read ID3 flag in header tag");
		return -1;
	}

	header->size = convert_size(header->size);
	
	return  0;
}
