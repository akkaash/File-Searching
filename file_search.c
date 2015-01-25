#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/time.h>

long mDiffTime(struct timeval end, struct timeval start){
	long ret = 0L;
	ret += (end.tv_sec - start.tv_sec) * 1000000L;
	ret += (end.tv_usec - start.tv_usec);
	
	return ret;
}

int binarySearch(int *arr, int bottom, int top, int s){
	if(top < bottom)
		return -1;
	int mid = (bottom + top)/2;
	if(arr[mid] == s)	return mid;
	else if (s > arr[mid])	return binarySearch(arr, mid+1, top, s);
	else if (s < arr[mid])	return binarySearch(arr, bottom, mid-1, s);
}

int binarySearchOnDisk(FILE *ip, int bottom, int top, int s){
	if(top < bottom)
		return -1;
	
	int mid = (bottom + top)/2;
	int x;
	
	fseek(ip, mid*sizeof(int), SEEK_SET);
	fread(&x, sizeof(int), 1, ip);
	
	if(s == x)		return mid;
	else if(s > x)		return binarySearchOnDisk(ip, mid+1, top, s);
	else if(s < x)		return binarySearchOnDisk(ip, bottom, mid-1, s);
	
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
	long timeDiff = 0;
		
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
		ip2 = fopen(inFile2, "rb");		//OPENING SEEK.DB
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
		
		N=0;
		
		struct timeval t1;
		gettimeofday(&t1, NULL);
		
		ip1 = fopen(inFile1, "rb");		//OPENING KEY.DB
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
		
		struct timeval t2;
		gettimeofday(&t2, NULL);
		timeDiff += mDiffTime(t2, t1);
		
		hit = (int *) malloc(SSize * sizeof(int));
		int seek, j, pos;
		
		//-------- in memory linear search -----------
		if(mode == 1){
			struct timeval t3;
			gettimeofday(&t3, NULL);
			
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
			
			struct timeval t4;
			gettimeofday(&t4, NULL);
			timeDiff += mDiffTime(t4, t3);
				
		} 
		//----------------------------------
		
		//------- in memory binary search ------------
		else if(mode == 2){
			struct timeval t3;
			gettimeofday(&t3, NULL);
			
			for(i=0; i<SSize; i++){
				seek = S[i];
				pos = binarySearch(K, 0, KSize-1, seek);
				if(pos == -1){
					hit[i] = 0;
				} else {
					hit[i] = 1;
				}
			}
			struct timeval t4;
			gettimeofday(&t4, NULL);
			timeDiff += mDiffTime(t4, t3);
			
		}
		//----------------------------------
		
		
	} else if (mode == 3 || mode == 4){
	/*
	*
	*
	*	ON DISK SEARCH
	*
	*
	*/
		ip2 = fopen(inFile2, "rb");		//OPENING SEEK.DB		
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
		int foundFlag = 0;

		if (mode == 3) {
			struct timeval t1;
			gettimeofday(&t1, NULL);
			
			ip1 = fopen(inFile1, "rb");	//OPENING KEY.DB
			
			//--------- ON DISK LINEAR SEARCH ---------------------
			
			for(i=0;i<SSize;i++){
				//printf("i=%d\n", i);
				seek = S[i];
/*				printf("seek=%d ", seek);*/
				for(j=0;;j++){
					fseek(ip1, j*sizeof(int), SEEK_SET);
					fread(&k, sizeof(int), 1, ip1);
					//printf("k=%d ", k);
					if(feof(ip1) == 0){
						if(seek == k){
/*							printf(" found\n");*/
							hit[i] = 1;
							break;
						} else{
							hit[i] = 0;
						}
					} else{
/*						printf("eof\n");*/
						break;
						
					} 
				}
			}
			//---------------------------------------------
			struct timeval t2;
			gettimeofday(&t2, NULL);
			timeDiff += mDiffTime(t2, t1);
			
		} else if (mode == 4){
			struct timeval t1;
			gettimeofday(&t1, NULL);
			
			ip1 = fopen(inFile1, "rb");	//OPENING KEY.DB
			
			//--------- ON DISK BINARY SEARCH ---------------------
			N = 0;
			for(i=0;;i++){
				fseek(ip1, i*sizeof(int), SEEK_SET);
				fread(&k, sizeof(int), 1, ip1);
				if(feof(ip1) == 0){
					N++;
				} else break;
			}
			KSize = N;
			for(i=0;i<SSize;i++){
				seek = S[i];
				pos = binarySearchOnDisk(ip1, 0, KSize-1, seek);
				if(pos == -1){
					hit[i] = 0;
				} else{
					hit[i] = 1;
				}
			}
			struct timeval t2;
			gettimeofday(&t2, NULL);
			timeDiff += mDiffTime(t2, t1);
		}			
			
	
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

	printf("Time: %f\n", (float)timeDiff/1000000);
	
}
