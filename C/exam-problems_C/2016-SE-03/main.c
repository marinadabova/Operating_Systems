#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>


int comparator(const void* p1, const void* p2){
        uint32_t n1 = *(const uint32_t*) p1;
        uint32_t n2 = *(const uint32_t*) p2;

        return n1 - n2;
}
int main(int argc, char* argv[]){
	if (argc != 2){
		errx(1,"Wrong number of arguments");
	}
	int fd= open(argv[1],O_RDWR);
	if( fd < 0){
		err(2,"Error while opening");
	}

	struct stat st;
	if( stat(fd, &st) < 0){
		err(3,"Error whith stat");		
	}
	if(st.st_size % sizeof(uint32_t) != 0){
		errx(4, "File not in our format");
    }
	uint32_t maxN = 100000000;
    if(st.st_size > maxN * sizeof(uint32_t)){	
    	errx(5, "File must have only 100 000 000 numbers");
    }
	uint32_t first_part_size = (st.st_size / sizeof(uint32_t)) / 2;
    uint32_t second_part_size = (st.st_size / sizeof(uint32_t)) - first_part_size;
	uint32_t* buf = malloc(first_part_size * sizeof(uint32_t));
	
	if(!buf){
    	err(6, "Error malloc");
    }

    if (read(fd, &buf, (first_part_size * sizeof(uint32_t))) < 0){      
    	err(7, "Error reading");
    }

    qsort(&buf, first_part_size, sizeof(uint32_t), comparator);

	int t1 = open("temp1", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if(t1 < 0){
        err(4, "Error opening");
    }
    if(write(t1, buf, (first_part_size * sizeof(uint32_t))) < 0){
    	err(7, "Error writing");
    }

    free(buf);
buf = malloc(second_part_size * sizeof(uint32_t));
        if(!buf){
                err(5, "Error malloc");
        }

        if( read(fd, buf, (second_part_size * sizeof(uint32_t))) < 0){
                err(6, "Error reading");
        }

        qsort(buf, second_part_size, sizeof(uint32_t), comparator);

        int t2 = open("temp2", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR, S_IWUSR);
        if(t2 < 0){
                err(4, "Error openig");
        }

        if(write(t2, buf, (second_part_size * sizeof(uint32_t))) < 0){
                err(7, "Error writing");
        }

        free(buf);

        if(lseek(fd, 0, SEEK_SET) < 0){
                err(8, "Error lseek");
        }

        uint32_t cnt1 = 0;
        uint32_t cnt2 = 0;

        uint32_t num1;
        uint32_t num2;

        while(cnt1 < first_part_size && cnt2 < second_part_size){
                if(lseek(t1, (cnt1 * sizeof(uint32_t)), SEEK_SET) < 0){
                        err(8, "Error lseek");
                }

                if(lseek(t2, (cnt2 * sizeof(uint32_t)), SEEK_SET) < 0){
                        err(8, "Error lseek");
                }

                if(read(t1, &num1, sizeof(num1)) < 0){
                        err(6, "Error reading");
                }

                if(read(t2, &num2, sizeof(num2)) < 0){
                        err(6, "Error reading");
                }

                if(comparator(&num1, &num2) < 0){
                        if(write(fd, &num1, sizeof(num1)) <= 0){
                                err(7, "Error writing");
                        }

                        cnt1++;
                }
                else{
                        if(write(fd, &num2, sizeof(num2)) <= 0){
                                err(7, "Error writing");
                        }

                        cnt2++;
                }
        }

        while(cnt1 < first_part_size){
                if(read(t1, &num1, sizeof(num1)) < 0){
                        err(6, "Error reading");
                }

                if(write(fd, &num1, sizeof(num1)) <= 0){
                        err(7, "Error writing");
                }

                cnt1++;
        }

        while(cnt2 < second_part_size){
                if(read(t2, &num2, sizeof(num2)) < 0){
                        err(6, "Error reading");
                }

                if(write(fd, &num2, sizeof(num2)) <= 0){
                        err(7, "Error writing");
                }

                cnt2++;
        }

        close(fd);
        close(t1);
        close(t2);
	return 0;
}

/*Зад. 60 2016-SE-03 Напишете програма на C приемаща параметър – име на (двоичен) файл с uint32_t
числа. Програмата трябва да сортира файла. Ограничения:
• Числата биха могли да са максимум 100 000 000 на брой.
• Програмата трябва да работи на машина със същия endianness, както машината, която е
създала файла.
• Програмата трябва да работи на машина с 256 MB RAM и 8 GB свободно дисково простран-
ство. */
