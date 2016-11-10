#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)

{
	if (argc != 4 && argc != 6)
	{
		fprintf(stderr,"Usage: %s <filein> <fileout> <scale>\n",argv[0]);
		exit(-1);
	}

	int from,to;
	float scale;
	from = -1;
	to = -1;
	if(argc > 4){
		from = atoi(argv[4]);
		to = atoi(argv[5]);
	}
	scale = atof(argv[3]);
	FILE *in = fopen(argv[1],"r");
	FILE *ut = fopen(argv[2],"w");

	int tmp, tmp2;
	float tmpf, tmpf2;
	int i=0;
	i=0;
	while((fscanf(in,"%d %d", &tmp, &tmp2) != EOF) && (to == -1 || (i < to)))
	{
		tmpf = ((float)(tmp))*scale;
		tmpf2 = ((float)(tmp2))*scale;
		if (i >= from){
			fwrite(&tmpf, 4, 1, ut);
			fwrite(&tmpf2, 4, 1, ut);
		}
		i++;
	}
	fclose(in);
	fclose(ut);


}
