#include "img_pro.h"

unsigned char **median_filter(unsigned char **img, int NC, int NR, int size){
unsigned char **img_out;
int i, j, k, l, maxval, maxindex, padding;
// create an array for filter memory
int *arr; arr = (int*)malloc(size*size*sizeof(int));
// create an output img
img_out = alloc_img(NC,NR);
// define a padding which prevents segmentation faults
padding = size/2;
// for loops and image coordinates includes padding for dynamic size operations
for(i=padding; i<NR-padding; i++){
	for(j=padding; j<NC-padding; j++){
		// fill the array from pixel and its neighbors
		for(k=0; k<size; k++){
			for(l=0; l<size; l++){
				arr[size*k+l] = img[i-padding+k][j-padding+l];				
			}
		} 
		// to the middle term, zero the max values
		for(k=0; k<(size*size)/2+1; k++){
			// find max value
			maxval = -1; maxindex = 0;
			for(l=0; l<size*size; l++){
				if(arr[l] > maxval){
					maxval = arr[l];
					maxindex = l;
				}
			}
			arr[maxindex] = 0;
		}
		// resulting pixel value
		img_out[i][j] = maxval;
	}
}
free(arr);	       
return(img_out);
}  

