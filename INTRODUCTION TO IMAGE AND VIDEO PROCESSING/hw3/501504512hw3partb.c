#include "img_pro.h"

float **alloc_2Dmat(int ncol,int nrow);
void free_2Dmat(float** mat);
void HSItoRGB(float H, float S, float I, unsigned char *pR, unsigned char *pG, unsigned char *pB);

int main(int argc, char **argv){
color **img, **img2;
int i,j,NR,NC,threshold;
float Rx, Gx, Bx, Ry, Gy, By, gxx, gyy, gxy;
float **F;
float maxF = 0, minF = 256, maxH = 240, minH = 0, hue, intensity, saturation, theta;
unsigned char R,G,B;

// take command line arguments
if(argc!=4) {printf("\n Usage: partb [threshold] [saturation] [intensity]\n"); exit(-1); }
threshold = atoi(argv[1]);
saturation = atof(argv[2]);
intensity = atof(argv[3]);

// create img from ppm
img = (color **)ppm_file_to_img("mandrill.ppm",&NC,&NR);
// create an 2D matrix for hold magnitude values
F = alloc_2Dmat(NC,NR);

// for every pixel
for(i=1; i<NR-1; i++){
	for(j=1; j<NC-1; j++){
		// apply sobel operation to R,G,B for partial derivative
		Rx = (img[i-1][j+1].r+img[i][j+1].r*2+img[i+1][j+1].r)-(img[i-1][j-1].r+img[i][j-1].r*2+img[i+1][j-1].r); Rx = Rx / 8.0;
		Gx = (img[i-1][j+1].g+img[i][j+1].g*2+img[i+1][j+1].g)-(img[i-1][j-1].g+img[i][j-1].g*2+img[i+1][j-1].g); Gx = Gx / 8.0;
		Bx = (img[i-1][j+1].b+img[i][j+1].b*2+img[i+1][j+1].b)-(img[i-1][j-1].b+img[i][j-1].b*2+img[i+1][j-1].b); Bx = Bx / 8.0;
		
		Ry = (img[i-1][j-1].r+img[i-1][j].r*2+img[i-1][j+1].r)-(img[i+1][j-1].r+img[i+1][j].r*2+img[i+1][j+1].r); Ry = Ry / 8.0;
		Gy = (img[i-1][j-1].g+img[i-1][j].g*2+img[i-1][j+1].g)-(img[i+1][j-1].g+img[i+1][j].g*2+img[i+1][j+1].g); Gy = Gy / 8.0;
		By = (img[i-1][j-1].b+img[i-1][j].b*2+img[i-1][j+1].b)-(img[i+1][j-1].b+img[i+1][j].b*2+img[i+1][j+1].b); By = By / 8.0;
		// do calculations of vector approach to find magnitude F(.)
		gxx = Rx*Rx + Gx*Gx + Bx*Bx; gxx = gxx / 3.0;
		gxy = Rx*Ry + Gx*Gy + Bx*By; gxy = gxy / 3.0;
		gyy = Ry*Ry + Gy*Gy + By*By; gyy = gyy / 3.0;
		
		theta = 0.5 * atan(2*gxy / gxx-gyy);
		
		F[i][j] = (sqrt(0.5 * ((gxx+gyy)-(gxx-gyy)*cos(2*theta)+2*gxy*sin(2*theta))));
		
		// apply given threshold value
		if(F[i][j] < threshold) F[i][j] = 0;
		else{
			// update the limits of F(.)
			if(maxF < F[i][j]) maxF = F[i][j];
			if(minF > F[i][j]) minF = F[i][j];
		}		
	} 
}
// use edge magnitude to find RGB variables for every pixel
for(i=0; i<NR; i++){
	for(j=0; j<NC; j++){
		// if the point is on border
		if(i == NR || i == 0) PPM_ASSIGN(img[i][j], 0, 0, 0);
		else if(j == NC || j == 0) PPM_ASSIGN(img[i][j], 0, 0, 0);
		else{
			// if the magnitude is below threshold
			if(F[i][j] == 0) PPM_ASSIGN(img[i][j], 0, 0, 0);
			else{
				// if there is a magnitude, map it to wanted hue value from color gamut
				hue = (F[i][j] - minF) * (maxH - minH) / (maxF - minF) + minH;
				HSItoRGB(hue, saturation, intensity, &R, &G, &B);
				PPM_ASSIGN(img[i][j], R, G, B);
			}
		}
	}
}

// generate pgm file and show with irfanview  
img_to_ppm_file(img, "partb.ppm", NC, NR);
show_pgm_file("partb.ppm");

// free images
free_color_img(img);
free_2Dmat(F);
return(1);
}

// functions from my_header.c
void HSItoRGB(float H, float S, float I, unsigned char *pR, unsigned char *pG, unsigned char *pB){
	float R, G, B, H_hat, Z, C, X, m , H_hat_mod;
	// Calculate color parameters according to formulas
	H_hat = H / 60.0; H_hat_mod = H_hat - 2*((int)H_hat/2);
	Z = 1.0 - fabs(H_hat_mod - 1.0);
	C = (3*I*S) / (1+Z); X = C*Z;
	// Assign R,G,B with hue circle sections
	if(H_hat >= 0 && H_hat <= 1){
		R = C; G = X; B = 0;
	}
	else if(H_hat <= 2){
		R = X; G = C; B = 0;
	}
	else if(H_hat <= 3){
		R = 0; G = C; B = X;
	}
	else if(H_hat <= 4){
		R = 0; G = X; B = C;
	}
	else if(H_hat <= 5){
		R = X; G = 0; B = C;
	}
	else if(H_hat <= 6){
		R = C; G = 0; B = X;
	}
	else{
		R = 0; G = 0; B = 0;
	}
	// Complete the R, G, B
	m = I * (1 - S);
	R = R + m; G = G + m; B = B + m; 
	R *= 256; G *= 256; B *= 256;
	// Return them
	*pR = (unsigned char) R;
	*pG = (unsigned char) G;
	*pB = (unsigned char) B;
}
// 
float **alloc_2Dmat(int ncol,int nrow){
	int i;
	float **mat;
	mat = (float **) malloc((size_t)((nrow+1)*sizeof(float*)));
	mat[0] = (float *) malloc((size_t)((nrow*ncol+1)*sizeof(float)));
	for(i=1;i<=nrow;i++)
	    mat[i]=mat[i-1] + ncol;
	return (mat);
}
void free_2Dmat(float** mat){
    free(mat[0]);
    free(mat);
}
