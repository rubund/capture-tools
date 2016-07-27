#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
	if (argc != 3 && argc != 4 && argc != 5)
	{
		fprintf(stderr,"Usage: %s <filein> <fileout> [relation] [keepafter]\n",argv[0]);
		exit(-1);
	}

	FILE *in;
	FILE *out;

	int keepafter;
	float tmp, tmp2;
	float mag2;
	float max;
	int counter;
	float relation;
	char filename[100];
	int read=0;
	relation = 0.1;

	if (argc == 4){
		relation = atof(argv[3]);
	}
	printf("Relation: %f\n", relation);
 	keepafter = 1000;
	if (argc == 5){
		keepafter = atoi(argv[4]);
	}
	printf("Keepafter: %d\n", keepafter);

	in = fopen(argv[1],"r");
	max = 0;
	while(fread(&tmp, 4, 1, in)){
		//fread(&tmp2, 4, 1, in);
		mag2 = sqrt(pow(tmp,2) + pow(tmp2,2));
		if (tmp > max) {
			max = tmp;
		}
		read++;
	}
	fclose(in);
	printf("Max: %f\n", max);

	float keepabove = max * relation;
	char keeping = 0;
	int written = 0;
	int filenum = 0;
	counter = 0;
	int lcounter;

	in = fopen(argv[1],"r");
	max = 0;
	while(fread(&tmp, 4, 1, in)){
		fread(&tmp2, 4, 1, in);
		mag2 = sqrt(pow(tmp,2) + pow(tmp2,2));
		if (mag2 > keepabove) {
			if(!keeping){
				snprintf(filename,99,"%s%03d",argv[2],filenum);
				printf("%s\n",filename);
				filenum++;
				out = fopen(filename,"w");
				lcounter = 0;
			}
			keeping = 1;
			counter = keepafter;
		}
		else if (counter == 0){
			if(keeping) {
				fclose(out);
				printf("  %d\n", lcounter);
			}
			keeping = 0;
		}
		else {
			counter--;
		}
		if (keeping){
			fwrite(&tmp, 4, 1, out);
			fwrite(&tmp2, 4, 1, out);
			written++;
			lcounter++;
		}
	}
	fclose(in);
	printf("Read:    %d\n", read);
	printf("Written: %d\n", written);

}
