#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int binarySearch(int *arr, int bottom, int top, int s){
	if(top < bottom)
		return -1;
	int mid = (bottom + top)/2;
	if(arr[mid] == s)	return mid;
	else if (s > arr[mid])	return binarySearch(arr, mid+1, top, s);
	else if (s < arr[mid])	return binarySearch(arr, bottom, mid-1, s);
}

int main(int argc, char **argv){

	if(argc != 4){
		printf("Insufficient Arguments. Exiting. \n");
		exit(-1);
	}
	
	char *modeStr = argv[1];
	char *inFile1 = argv[2];
	char *inFile2 = argv[3];
	
	int mode;
	
	if(strcmp(modeStr, "--mem-lin") == 0){
		printf("in-memory sequential search\n");
		mode = 1;
	} else if(strcmp(modeStr, "--mem-bin") == 0){
		printf("in-memory binary search\n");
		mode = 2;
	} else if(strcmp(modeStr, "--disk-lin") == 0){
		printf("on-disk sequential search\n");
		mode = 3;
	} else if(strcmp(modeStr, "--disk-bin") == 0){
		printf("on-disk binary search\n");
		mode = 4;
	}
	
	FILE *ip1, *ip2;
	
	ip1 = fopen(inFile1, "rb");
	ip2 = fopen(inFile2, "rb");
	
	int i, N=0;
	int k;
	
	int SSize, KSize;

	int *K;
	K = NULL;

	int *S;
	S==NULL;


	int *hit;
	hit = NULL;
	
	if (mode == 1 || mode == 2){
	
	/*
	*
	*
	*	IN MEMORY SEARCH
	*
	*
	*/
		//----reading K----------------
		for(i=0;;i++){
			fseek(ip1, i*sizeof(int), SEEK_SET);
			fread(&k, sizeof(int), 1, ip1);
			if(feof(ip1) == 0){
				N++;
			} else break;
		}
		K = (int *) malloc(N * sizeof(int));
		for(i=0;;i++){
			fseek(ip1, i*sizeof(int), SEEK_SET);
			fread(&k, sizeof(int), 1, ip1);
			if(feof(ip1) == 0){
				K[i] = k;
			} else break;
		}
		//-----------------------------
		KSize = N;
		
		N=0;
		//----reading S----------------
		for(i=0;;i++){
			fseek(ip2, i*sizeof(int), SEEK_SET);
			fread(&k, sizeof(int), 1, ip2);
			if(feof(ip2) == 0){
				N++;
			} else break;
		}
		S=(int *) malloc(N * sizeof(int));
		for(i=0;;i++){
			fseek(ip2, i*sizeof(int), SEEK_SET);
			fread(&k, sizeof(int), 1, ip2);
			if(feof(ip2) == 0){
				S[i] = k;
			} else break;
		}		
		//-----------------------------
		SSize = N;
		
		
		hit = (int *) malloc(SSize * sizeof(int));
		int seek, j, pos;
		
		//-------- linear search -----------
		if(mode == 1){
			for(i=0; i<SSize; i++){
				seek = S[i];
				for(j=0; j<KSize; j++){
					if(K[j] == seek){
						hit[i] = 1;
						break;	
					} else{
						hit[i] = 0;
					}
				}
			}
				
		} 
		//----------------------------------
		
		//------- binary search ------------
		else if(mode == 2){
			for(i=0; i<SSize; i++){
				seek = S[i];
				pos = binarySearch(K, 0, KSize-1, seek);
				if(pos == -1){
					hit[i] = 0;
				} else {
					hit[i] = 1;
				}
			}
		}
		//----------------------------------
		
		
	}
	
	
	/*
	*
	*	DISPLAY RESULTS
	*
	*
	*/
	
	for (i=0; i<SSize; i++){
		if(hit[i] == 1){
			printf( "%12d: Yes\n", S[i] );
		} else if(hit[i] == 0){
			printf( "%12d: No\n", S[i] );
		}
	}
	
}
