#include "img_pro.h"
#include "my_header.h"

int main(int argc, char **argv){
unsigned char **img, **img2;
char *pgm_file;
int i,j,NR,NC,size, number_of_filters;
char name[] = "0size0times___\0\0\0\0\0\0\0\0";
if(argc!=4) {printf("\n Usage: median_app size number_of_filters cat5.pgm \n"); exit(-1); }
// take command line arguments
size = atoi(argv[1]);
number_of_filters = atoi(argv[2]);
pgm_file = argv[3];
// create img from pgm
img = pgm_file_to_img(pgm_file, &NC, &NR);
// apply filter for wanted number of times
for(i=0; i<number_of_filters; i++){
	img = median_filter(img, NC, NR, size);
}
// create a custom name for filter operation which includes the filtering parameters
name[0] = size + 48; 
name[5] = number_of_filters + 48;
i = 0; j = 11;
while (pgm_file[i] != '\0') {
    name[j] = argv[3][i];
    i++; j++;
}
name[j] = '\0';
// generate pgm file and show with irfanview  
img_to_pgm_file(img, name, NC, NR);
show_pgm_file(name);

// free images
free_img(img);

return(1);
}

