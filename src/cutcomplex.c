#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
	FILE *infile;
	FILE *outfile;

	infile = fopen(argv[1],"r");
	outfile = fopen(argv[2],"w");
	int from,to;
	from = -1;
	to = -1;
	if(argc > 3){
		from = atoi(argv[3]);
		to = atoi(argv[4]);
	}
	float tmp1, tmp2;
	double out;
	float tmpdouble1, tmpdouble2;
	int i;
	i=0;
	while((fread(&tmp1,4,1,infile) != 0) && (fread(&tmp2,4,1,infile)) && (to == -1 || (i < to))){
		if(i >= from){
			fwrite(&tmp1,4,1,outfile);
			fwrite(&tmp2,4,1,outfile);
		}
		i++;
	}
	fclose(infile);
	fclose(outfile);



}

