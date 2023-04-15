#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

void reverse(char* start, int size)
{
	char temp;
	int i = 0;
	int j = size - 1;
	
	while(i < j)
	{
		temp = start[i];
		start[i] = start[j];
		start[j] = temp;
		i++;
		j--;
	}
}

void getNewPath(char* path)
{
	char current_char = '0';
	int i = 0;
	int j = 0;
	while(current_char != '\0')
	{
		current_char = path[i++];
		if(current_char == '/')
		{
			j = i;
		}
	}
	char name[1000];
	strcpy(name, &path[j]);
	reverse(name, strlen(name));
	strcpy(&path[j], name);
}

int getByteNumber(int fd)
{
	int i = 0;
	char temp;
	while(read(fd, &temp, 1) == 1)
	{
		i++;
	}	
	lseek(fd, 0, SEEK_SET);
	return i;
}

void createNewReversedFile(char* fileName, char* path, char* startPath)
{
	char pathToOrig[1000];
	char pathToNew[1000];
	strcpy(pathToOrig, startPath);
	strcpy(pathToOrig + strlen(startPath), "/");
	strcpy(pathToOrig + strlen(startPath) + 1, fileName);
	
	reverse(fileName, strlen(fileName));
	
	strcpy(pathToNew, path);
	strcpy(pathToNew + strlen(path), "/");
	strcpy(pathToNew + strlen(path) + 1, fileName);
	
	
	int fd1 = open(pathToOrig, O_RDONLY);
	int fd2 = open(pathToNew, O_RDWR | O_CREAT, S_IRWXU);
	
	char temp;
	int i = 0;

	int byteNumber = getByteNumber(fd1);
	
	lseek(fd1, -1, SEEK_END);
	
	while(i++ != byteNumber)
	{
		read(fd1, &temp, 1);
		lseek(fd1, -2, SEEK_CUR);
		write(fd2, &temp, 1);
	}
	close(fd1);
	close(fd2);
}

int main()
{
	char path[1000];
	scanf("%s", path);
	char startPath[1000];
	strcpy(startPath, path);
	
	
	getNewPath(path);
	int isCreated = mkdir(path, 0700);
	
	DIR *d;
	struct dirent *dir;
	d = opendir(startPath);
	while((dir = readdir(d)) != NULL)
	{
		if(dir -> d_type != DT_DIR)
		{
			createNewReversedFile(dir->d_name, path, startPath);
		}
	}
	closedir(d);
	
	return 0;
}
