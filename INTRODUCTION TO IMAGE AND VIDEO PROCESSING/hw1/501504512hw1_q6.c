#include "img_pro.h"
#include "my_header.h"

int main(int argc, char **argv)
{
unsigned char **img, **img2;
char *pgm_file;
int i,j,k,l,NR,NC;
float gama;

/*---------->>> Start assigning variables from command line arguments   */
if(argc!=3) {
  printf("\n Usage: q6 gama [Image file (*.pgm)] \n");
  printf("\n E.g.   q6 20 cathedral.pgm \n");
  exit(-1); }
  
gama = atof(argv[1]); if(gama<-250 || gama> 250) {printf("\n gama should be between 0 and 255\n"); exit(0);} 
pgm_file = argv[2];
/*----------<<< End assigning valiables from command line arguments  */

img = pgm_file_to_img(pgm_file, &NC, &NR);/*  read img and its size from file */
// show_pgm_file(pgm_file);

for(i=0;i<NR;i++) 
  for(j=0;j<NC;j++) 
    img[i][j] = 255 * (pow(img[i][j]/255.0,1.0/gama)); 
    
img_to_pgm_file(img,"deneme.pgm", NC, NR);
show_pgm_file("deneme.pgm");

// free images
free_img(img);

return(1);
}

