#include<stdio.h>
#include<dirent.h>

int main()
{
	DIR *dp;
	struct dirent *dirp;
	char dirname[256];

	printf("Please input a directory: ");
	scanf("%s",dirname);
	if((dp = opendir(dirname)) == NULL)
		printf("Can't open %s\n", dirname);

	while((dirp = readdir(dp)) != NULL)
		printf("%s\n", dirp->d_name);

	closedir(dp);
	return 0;
}
