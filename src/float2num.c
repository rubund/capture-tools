#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)

{
	if (argc != 3 && argc != 5)
	{
		fprintf(stderr,"Usage: %s <filein> <fileout>\n",argv[0]);
		exit(-1);
	}

	int from,to;
	from = -1;
	to = -1;
	if(argc > 3){
		from = atoi(argv[3]);
		to = atoi(argv[4]);
	}
	FILE *in = fopen(argv[1],"r");
	FILE *ut = fopen(argv[2],"w");

	float tmp;
	int i=0;
	i=0;
	while(fread(&tmp,4,1,in) && (to == -1 || (i < to)))
	{
		if (i >= from)
			fprintf(ut,"%f\n",tmp);
		i++;
	}
	fclose(in);
	fclose(ut);


}
