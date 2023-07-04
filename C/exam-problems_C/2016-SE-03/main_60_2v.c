#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>

int compareUint32(const void* a, const void* b) 
{
    uint32_t num1 = *(const uint32_t*)a;
    uint32_t num2 = *(const uint32_t*)b;

    if (num1 < num2) return -1;
    if (num1 > num2) return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "Need to be one argument");
    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1)
    {
        err(2, "Cannot open file");
    }

    struct stat st;
    if(fstat(fd, &st) == -1)
    {
        err(3, "Error with fstat");
    }
    
    if (st.st_size % sizeof(uint32_t) != 0)
    {
        errx(4, "Error in file format");
    }

    off_t fileSize = st.st_size;
    off_t numCount = fileSize / sizeof(uint32_t);
    int maxN = 100000000;
    if (numCount > maxN) // ili moga da napisha bez 44 red i tuk v if-a if(fileSize > maxN*sizeof(uint32_t))
    {
        errx(5, "Input has more numbers than it should");
    }

    uint32_t* numbers = malloc(numCount * sizeof(uint32_t));
    if (numbers == NULL) 
    {
        err(6, "Memory allocation failed");
    }

    ssize_t read_bytes = read(fd, numbers, fileSize);
    if (read_bytes == -1) 
    {
        err(7, "Error while reading");
    }
 
    qsort(numbers, numCount, sizeof(uint32_t), compareUint32);

    int output = open("output", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output == -1) 
    {
        err(2, "Cannot open file");
    }

    ssize_t write_bytes = write(output, numbers, fileSize);
    if (write_bytes == -1) 
    {
        err(8, "Error while writing");
    }

    close(fd);
    close(output);
    free(numbers);
    exit(0);
}
