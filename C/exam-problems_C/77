#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

typedef struct header
{
    uint16_t magic;
    uint16_t filetype;
    uint32_t count;

}__attribute__((packed)) Header;

typedef struct list
{
    off_t position;
    off_t value;
    uint16_t other;

}__attribute__((packed)) List;

int main(int argc, char* argv[]){
        if(argc != 4){
                errx(1,"wrong number of arguments");
        }
        int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1){
        err(2, "error while opening");
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1){
        err(2, "error while opening");
    }

    int fd3 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd3 == -1){
        err(2, "error while opening");
    }
        Header h;
    List l;

    struct stat st1;
    struct stat st2;
     if (fstat(fd1, &st1) != 0 || fstat(fd2, &st2) != 0){
        err(3, "Error with fstat");
    }

    if (st1.st_size % sizeof(List) != 0 || st1.st_size % sizeof(uint16_t) != 0 || st2.st_size % sizeof(uint32_t) != 0){
        errx(4, "Error with file format");
    }

        ssize_t read_bytes;

        if ((read_bytes = read(fd1, &h, sizeof(h))) != sizeof(h)){
        err(5, "Error while reading");
    }

    if (h.magic != 0x5A4D){
        errx(6, "Invalid magic value");
    }

    if (h.filetype == 1){
        for (uint32_t i = 0; i < h.count; ++i){
            if ((read_bytes = read(fd1, &l, sizeof(l))) <= 0){
                err(5, "Error reading");
            }

            uint16_t buf;
            if (lseek(fd2, l.position * sizeof(uint32_t), SEEK_SET) == -1){
                err(6, "Error with lseek");
            }

            if (read(fd2, &buf, sizeof(uint16_t)) <= 0){
                err(5, "Error while reading");
            }

            if (buf == 0){
                continue;
            }

            if (lseek(fd3, l.value * sizeof(uint64_t), SEEK_SET) == -1){
                err(6, "Error with lseek");
            }

            if (write(fd3, &buf, sizeof(uint64_t)) != sizeof(uint64_t)){
                err(7, "Error while writing");
            }
        }

        if (read_bytes == -1){
            err(5, "Error while reading");
        }

    } else if (h.filetype == 2){
        if (h.count * sizeof(uint32_t) != st1.st_size){
            errx(8, "Invalid count in header for data.bin");
        }

        for (uint32_t i = 0; i < h.count; ++i){
            uint32_t buf;
            while ((read_bytes = read(fd2, &buf, sizeof(uint32_t))) > 0){
                if (lseek(fd3, buf * sizeof(uint64_t), SEEK_SET) == -1){
                    err(6, "Error with lseek");
                }

                uint64_t out_value = 0;
                if (write(fd3, &out_value, sizeof(uint64_t)) != sizeof(uint64_t)){
                    err(8, "Error while writing");
                }
            }

            if (read_bytes == -1){
                err(5, "Error while reading");
            }
        }
    }
    else if (h.filetype == 3){
        if (h.count * sizeof(uint64_t) != st1.st_size){
            errx(8, "Invalid count in header for out.bin");
        }

        for (uint32_t i = 0; i < h.count; ++i){
            uint64_t buf;
            while ((read_bytes = read(fd2, &buf, sizeof(uint64_t))) > 0){
                if (lseek(fd3, buf * sizeof(uint64_t), SEEK_SET) == -1){
                    err(6, "Error with lseek");
                }

                uint64_t out_value = 0;
                if (write(fd3, &out_value, sizeof(uint64_t)) != sizeof(uint64_t)){
                    err(8, "Error while writing");
                }
            }

            if (read_bytes == -1){
                err(5, "Error while reading");
            }
        }
    }
    else{
        errx(9, "Invalid filetype");
    }

    close(fd1);
    close(fd2);
    close(fd3);
        return 0;
}

/*Зад. 77 2022-IN-01 Вашите колеги от съседната лаборатория работят със специализирана система Hoge,
която съхранява данните си в binary файлове. За съжаление обаче им се налага да работят с две
различни версии на системата (стара и нова) и разбира се, те ползват различни файлови формати.
Вашата задача е да напишете програма на C (./main list.bin data.bin out.bin), която конвер-
тира файлове с данни от стария (list.bin + data.bin) в новия (out.bin) формат.
Всички файлове се състоят от две секции – 8 байтов хедър и данни. Структурата на хедъра е:
• uint16_t, magic – магическа стойност 0x5A4D, която дефинира, че файлът е от системата Hoge
и следва тази спецификация;
• uint16_t, filetype – дефинира какъв тип файл е това, с допустими стойности 1 за list.bin,
2 за data.bin и 3 за out.bin;
• uint32_t, count – дефинира броя на елементите в секцията с данни на файла.
Секцията за данни на всички файлове съдържа елементи – цели числа без знак, от съответните
типове:
• list.bin – uint16_t;
• data.bin – uint32_t;
• out.bin – uint64_t.
Във файлове data.bin и out.bin елементи, чиято стойност е 0, са валидни елементи, но се игнори-
рат от системата Hoge (тяхното наличие във файла не пречи на работата на системата).
Всеки елемент в секцията за данни на list.bin има две характеристики – позиция и стойност,
като позиция е отместването на съответния елемент, докато стойност е неговата стойност. Тези
две числа семантично дефинират отмествания във файловете с данни:
• позиция дефинира отместване в data.bin;
• стойност дефинира отместване в out.bin.
На базата на тези числови характеристики елементите на list.bin дефинират правила от вида:
“елементът на отместване позиция в data.bin трябва да отиде на отместване стойност в out.bin”.
Забележка: Всички отмествания са спрямо N0 в брой елементи.
Програмата трябва да интепретира по ред дефинираните правила и на тяхна база да генерира
изходния файл.
Обърнете внимание на проверките, обработката за грешки и съобщенията към потребителя – искаме
програмата да бъде удобен инструмент.
Забележка: Не е гарантирано, че разполагате с достатъчно памет, в която да заредите всички
елементи на който и да от файловете. */
