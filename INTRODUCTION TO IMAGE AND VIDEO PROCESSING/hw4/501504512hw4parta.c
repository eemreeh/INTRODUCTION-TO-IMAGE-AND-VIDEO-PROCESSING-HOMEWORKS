// type gcc -o tilt_shiftA parta.c img_pro.c -lm
//./tilt_shiftA 200 20 [.ppm file]
#include "img_pro.h"
int is_blurred(int NR, int center, int distance, int iter, int i){
	if(i > center - distance*iter && i < center + distance*iter) return 0;
	if(i > center + distance*iter && i < NR-1) return 1;
	if(i < center - distance*iter && i > 1) return 1;
	return 0;
}
int main(int argc, char **argv){
color **img, **timg;
char *ppm_file;
int i,j,k,NR,NC,center, distance, iter;
int gaussian_row[5] = {1,4,6,4,1};

if(argc!=4) {printf("\n Usage: tilt_shiftA 200 20 [ppm file]\n"); exit(-1); }
// take command line arguments
center = atoi(argv[1]);
distance = atoi(argv[2]);
ppm_file = argv[3];
// create img from ppm
img = (color **)ppm_file_to_img(ppm_file, &NC, &NR);
timg = (color **)ppm_file_to_img(ppm_file, &NC, &NR);

iter = (NR - center) / distance;

if(center > NR/2)
	iter = center / distance;
else
	iter = (NR - center) / distance;
// gaussian is similar to homework 3 part a without flags and 5x5 gaussian instead of 3x3
for(k=0;k<iter;k++){
	for(i=2; i<NR-2; i++){
	if(is_blurred(NR, center, distance, k+1, i)){
		for(j=2; j<NC-2; j++){
			//Gaussian row applied.
			timg[i][j].r = (img[i][j-2].r*gaussian_row[0]+img[i][j-1].r*gaussian_row[1]+img[i][j].r*gaussian_row[2]+img[i][j+1].r*gaussian_row[3]+img[i][j+2].r*gaussian_row[4])/16;
			timg[i][j].g = (img[i][j-2].g*gaussian_row[0]+img[i][j-1].g*gaussian_row[1]+img[i][j].g*gaussian_row[2]+img[i][j+1].g*gaussian_row[3]+img[i][j+2].g*gaussian_row[4])/16;
			timg[i][j].b = (img[i][j-2].b*gaussian_row[0]+img[i][j-1].b*gaussian_row[1]+img[i][j].b*gaussian_row[2]+img[i][j+1].b*gaussian_row[3]+img[i][j+2].b*gaussian_row[4])/16;
			}
		}
	}
	for(i=2; i<NR-2; i++){
	if(is_blurred(NR, center, distance, k+1, i)){
		for(j=2; j<NC-2; j++){
			//Gaussian column applied.
			img[i][j].r = (timg[i-2][j].r*gaussian_row[0]+timg[i-1][j].r*gaussian_row[1]+timg[i][j].r*gaussian_row[2]+timg[i+1][j].r*gaussian_row[3]+timg[i+2][j].r*gaussian_row[4])/16;
			img[i][j].g = (timg[i-2][j].g*gaussian_row[0]+timg[i-1][j].g*gaussian_row[1]+timg[i][j].g*gaussian_row[2]+timg[i+1][j].g*gaussian_row[3]+timg[i+2][j].g*gaussian_row[4])/16;
			img[i][j].b = (timg[i-2][j].b*gaussian_row[0]+timg[i-1][j].b*gaussian_row[1]+timg[i][j].b*gaussian_row[2]+timg[i+1][j].b*gaussian_row[3]+timg[i+2][j].b*gaussian_row[4])/16;
			}
		}
	}
}

img_to_ppm_file(img,"parta.ppm",NC,NR);/*  write to file */
free_color_img(img); /* free array */

show_pgm_file("parta.ppm");

return(1);
}
