#include <sys/stat.h>
#include <stdio.h>

int	main(void)
{
	struct stat	buf;
	printf("%d\n", fstat(-1, &buf));
	printf("%d\n", buf.st_mode & S_IFMT);
	printf("%d\n", buf.st_mode & S_IFIFO);
	printf("%d\n", buf.st_mode & S_IFCHR);
	printf("%d\n", buf.st_mode & S_IFDIR);
	printf("%d\n", buf.st_mode & S_IFBLK);
	printf("%d\n", buf.st_mode & S_IFREG);
	printf("%d\n", buf.st_mode & S_IFLNK);
	printf("%d\n", buf.st_mode & S_IFSOCK);
	printf("%d\n", buf.st_mode & S_IFWHT);
	printf("%d\n", buf.st_mode & S_ISUID);
	printf("%d\n", buf.st_mode & S_ISGID);
	printf("%d\n", buf.st_mode & S_ISVTX);
	printf("%d\n", buf.st_mode & S_IRUSR);
	printf("%d\n", buf.st_mode & S_IWUSR);
	printf("%d\n", buf.st_mode & S_IXUSR);
	return (0);
}
