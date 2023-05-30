#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

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
	if(lseek(fd, 0, SEEK_SET) == -1)
	{
		printf("Problems setting offset");
		return -1;
	}
	return i;
}

int createNewReversedFile(char* fileName, char* path, char* startPath)
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
	
	if(fd1 == -1 && fd2 == -1)
	{
		printf("Could not open folders\n");
		return -1;
	}
	
	if(fd2 == -1 && fd1 != -1)
	{
		close(fd1);
		printf("Could not open new folder\n");
		return -1;
	}

	if(fd2 != -1 && fd1 == -1)
	{
		close(fd2);
		printf("Could not open original folder\n");
		return -1;
	}

	char temp;
	int i = 0;

	int byteNumber = getByteNumber(fd1);
	if(byteNumber == -1)
	{
		close(fd1);
		close(fd2);
		return -1;
	}
	
	lseek(fd1, -1, SEEK_END);
	
	while(i++ != byteNumber)
	{
		if(read(fd1, &temp, 1) == -1)
		{
			printf("Problems reading");
			close(fd1);
			close(fd2);
			return -1;
		}
		lseek(fd1, -2, SEEK_CUR);
		if(write(fd2, &temp, 1) == -1)
		{
			printf("Problems writing");
			close(fd1);
			close(fd2);
			return -1;
		}
	}
	
	int check = 0;
	if(close(fd1) == -1)
	{
		printf("Problems closing original folder\n");
		check++;
	}
	if(close(fd2) == -1)
	{
		printf("Problems closing new folder\n");
		check++;
	}
	if(check != 0)
	{
		return -1;
	}
	return 0;
}

int main()
{
	char path[1000];
	printf("Enter path to folder:");
	if(scanf("%s", path) <= 0)
	{
		printf("Sorry, troubles with entering path\n");
		return 0;
	}

	char startPath[1000];
	strcpy(startPath, path);
	
	DIR* dirCheck = opendir(path);
	if(dirCheck)
	{
		closedir(dirCheck);
	}
	else if(ENOENT == errno)
	{
		perror("Directory does not exist:");
		printf("\n");
		return 0;
	}
	else
	{
		perror("Failed to open dir:");
		printf("\n");
		return 0;
	}
	
	getNewPath(path);
	
	if(mkdir(path, 0700) != 0)
	{
		printf("Problems with creating new folder\n");
		return 0;
	}
	
	DIR *d;
	struct dirent *dir;
	d = opendir(startPath);

	if(d == NULL)
	{
		printf("Can't open directory.");
		return 0;
	}
	while((dir = readdir(d)) != NULL)
	{
		if(dir -> d_type != DT_DIR)
		{
			if(createNewReversedFile(dir->d_name, path, startPath) == -1)
			{
				return 0;
			}
		}
	}

	if(closedir(d) != 0)
	{
		printf("Can't close directory");
	}
	return 0;
}
