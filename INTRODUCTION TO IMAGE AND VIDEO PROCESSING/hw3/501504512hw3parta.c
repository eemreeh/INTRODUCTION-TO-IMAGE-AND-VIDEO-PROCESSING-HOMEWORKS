// type gcc -o rgbgauss 501504512hw3parta.c img_pro.c -lm
#include "img_pro.h"

int main(int argc, char **argv){
color **img, **timg;
char *ppm_file;
int i,j,k,NR,NC,Rflag,Gflag,Bflag,num_of_iter;
int gaussian_row[3] = {1,2,1};

if(argc!=6) {printf("\n Usage: rgb_gauss [red_flag] [green_flag] [blue_flag] [iteration_number] [ppm file]\n"); exit(-1); }
// take command line arguments
Rflag = atoi(argv[1]);
Gflag = atoi(argv[2]);
Bflag = atoi(argv[3]);
num_of_iter = atoi(argv[4]);
ppm_file = argv[5];
// create img from ppm
img = (color **)ppm_file_to_img(ppm_file, &NC, &NR);
timg = (color **)alloc_color_img(NC, NR);

for(k=0;k<num_of_iter;k++){
	for(i=1; i<NR-1; i++){
		for(j=1; j<NC-1; j++){
			//Gaussian row applied.
			if(Rflag==1){
				timg[i][j].r = (img[i][j-1].r*gaussian_row[0]+img[i][j].r*gaussian_row[1]+img[i][j+1].r*gaussian_row[2])/4;
			}
			if(Gflag==1){
				timg[i][j].g = (img[i][j-1].g*gaussian_row[0]+img[i][j].g*gaussian_row[1]+img[i][j+1].g*gaussian_row[2])/4;
			}
			if(Bflag==1){
				timg[i][j].b = (img[i][j-1].b*gaussian_row[0]+img[i][j].b*gaussian_row[1]+img[i][j+1].b*gaussian_row[2])/4;
			}
		}
	}
	for(i=1; i<NR-1; i++){
		for(j=1; j<NC-1; j++){
			//Gaussian column applied.
			if(Rflag==1){
				img[i][j].r = (timg[i-1][j].r*gaussian_row[0]+timg[i][j].r*gaussian_row[1]+timg[i+1][j].r*gaussian_row[2])/4;
			}
			if(Gflag==1){
				img[i][j].g = (timg[i-1][j].g*gaussian_row[0]+timg[i][j].g*gaussian_row[1]+timg[i+1][j].g*gaussian_row[2])/4;
			}
			if(Bflag==1){
				img[i][j].b = (timg[i-1][j].b*gaussian_row[0]+timg[i][j].b*gaussian_row[1]+timg[i+1][j].b*gaussian_row[2])/4;
			}
		}
	}
}
		
img_to_ppm_file(img,"parta.ppm",NC,NR);/*  write to file */
free_color_img(img); /* free array */

show_pgm_file("parta.ppm");

return(1);
}
