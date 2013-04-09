#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

typedef enum FileType {
    kFileTypeUnknown = 0,
    kFileTypeNonexistent,       // i.e. ENOENT
    kFileTypeRegular,
    kFileTypeDirectory,
    kFileTypeCharDev,
    kFileTypeBlockDev,
    kFileTypeFifo,
    kFileTypeSymlink,
    kFileTypeSocket,
} FileType;

FileType getFileType(const char* fileName)
{
    struct stat sb;

    if (stat(fileName, &sb) < 0) {
        if (errno == ENOENT || errno == ENOTDIR)
            return kFileTypeNonexistent;
        else {
            fprintf(stderr, "getFileType got errno=%d on '%s'\n",
                errno, fileName);
            return kFileTypeUnknown;
        }
    } else {
        if (S_ISREG(sb.st_mode))
            return kFileTypeRegular;
        else if (S_ISDIR(sb.st_mode))
            return kFileTypeDirectory;
        else if (S_ISCHR(sb.st_mode))
            return kFileTypeCharDev;
        else if (S_ISBLK(sb.st_mode))
            return kFileTypeBlockDev;
        else if (S_ISFIFO(sb.st_mode))
            return kFileTypeFifo;
#ifdef HAVE_SYMLINKS            
        else if (S_ISLNK(sb.st_mode))
            return kFileTypeSymlink;
        else if (S_ISSOCK(sb.st_mode))
            return kFileTypeSocket;
#endif            
        else
            return kFileTypeUnknown;
    }
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("No filename specified");
		return -1;
	}

	printf("FileType: %d\n", getFileType(argv[1]));

	return 0;
}
