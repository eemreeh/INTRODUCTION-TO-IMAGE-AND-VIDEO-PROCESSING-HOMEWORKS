// type gcc -o tilt_shiftB partb.c img_pro.c -lm
//./tilt_shiftB 200 150 20 [.ppm file]
#include "img_pro.h"

int is_blurred(int r_center, int c_center, int radius, int iter, int i, int j);
int find_iter(int NR, int NC, int r_center, int c_center, int radius);

int main(int argc, char **argv){
color **img, **timg;
char *ppm_file;
int i,j,k,NR,NC,num_of_iter,r_center, c_center, radius;
int gaussian_row[5] = {1,4,6,4,1};

if(argc!=5) {printf("\n Usage: tilt_shiftB 200 150 20 [ppm file]\n"); exit(-1); }
// take command line arguments
r_center = atoi(argv[1]);
c_center = atoi(argv[2]);
radius = atoi(argv[3]);
ppm_file = argv[4];
// create img from ppm
img = (color **)ppm_file_to_img(ppm_file, &NC, &NR);
timg = (color **)ppm_file_to_img(ppm_file, &NC, &NR);

num_of_iter = find_iter(NR, NC, r_center, c_center, radius);

// this part is similar gaussian filter from homework 3 part a without flags and 5x5 gaussian instead of 3x3
for(k=0;k<num_of_iter;k++){
	for(i=2; i<NR-2; i++){
		for(j=2; j<NC-2; j++){
			if(is_blurred(r_center, c_center, radius, k+1, i, j)){
				//Gaussian row applied.
				timg[i][j].r = (img[i][j-2].r*gaussian_row[0]+img[i][j-1].r*gaussian_row[1]+img[i][j].r*gaussian_row[2]+img[i][j+1].r*gaussian_row[3]+img[i][j+2].r*gaussian_row[4])/16;
				timg[i][j].g = (img[i][j-2].g*gaussian_row[0]+img[i][j-1].g*gaussian_row[1]+img[i][j].g*gaussian_row[2]+img[i][j+1].g*gaussian_row[3]+img[i][j+2].g*gaussian_row[4])/16;
				timg[i][j].b = (img[i][j-2].b*gaussian_row[0]+img[i][j-1].b*gaussian_row[1]+img[i][j].b*gaussian_row[2]+img[i][j+1].b*gaussian_row[3]+img[i][j+2].b*gaussian_row[4])/16;	
			}
		}
	}
	for(i=2; i<NR-2; i++){
		for(j=2; j<NC-2; j++){
			if(is_blurred(r_center, c_center, radius, k+1, i, j)){
				//Gaussian column applied.
				img[i][j].r = (timg[i-2][j].r*gaussian_row[0]+timg[i-1][j].r*gaussian_row[1]+timg[i][j].r*gaussian_row[2]+timg[i+1][j].r*gaussian_row[3]+timg[i+2][j].r*gaussian_row[4])/16;
				img[i][j].g = (timg[i-2][j].g*gaussian_row[0]+timg[i-1][j].g*gaussian_row[1]+timg[i][j].g*gaussian_row[2]+timg[i+1][j].g*gaussian_row[3]+timg[i+2][j].g*gaussian_row[4])/16;
				img[i][j].b = (timg[i-2][j].b*gaussian_row[0]+timg[i-1][j].b*gaussian_row[1]+timg[i][j].b*gaussian_row[2]+timg[i+1][j].b*gaussian_row[3]+timg[i+2][j].b*gaussian_row[4])/16;	
			}
		}
	}
}

img_to_ppm_file(img,"partb.ppm",NC,NR);/*  write to file */
free_color_img(img); /* free array */

show_pgm_file("partb.ppm");

return(1);
}


int is_blurred(int r_center, int c_center, int radius, int iter, int i, int j){
	int distance = sqrt((j-r_center)*(j-r_center) + (i-c_center)*(i-c_center));
	if(distance < radius ) return 0;
	if(distance >= radius*iter ) return 1;
  return 0;
}

int find_iter(int NR, int NC, int r_center, int c_center, int radius){
	int t_dist[4];	
	int max,i;
	t_dist[0] = sqrt((0 - c_center)*(0 - c_center) + (0 - r_center)*(0 - r_center))/radius;
	t_dist[1] = sqrt((0 - c_center)*(0 - c_center) + (NR - r_center)*(NR - r_center))/radius;
	t_dist[2] = sqrt((NC - c_center)*(NC - c_center) + (0 - r_center)*(0 - r_center))/radius;
	t_dist[3] = sqrt((NC - c_center)*(NC - c_center) + (NR - r_center)*(NR - r_center))/radius;
	
	max = t_dist[0];
	for(i = 1; i < 4; i++){
		if (t_dist[i] > max) max = t_dist[i];
	}
	return max;
}
