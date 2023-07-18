
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
          if(argc < 3){
                errx(1, "We need at least 2 arguments");
        }

        if(strcmp(argv[1], "-c") == 0){
                int n, m;
                if(strlen(argv[2]) == 1){
                        n = argv[2][0] - '0';
                        m = n;
                }
                else if(strlen(argv[2]) == 3){
                        n = argv[2][0] - '0';
                        m = argv[2][2] - '0';

                        if(n > m){
                                errx(3, "The second must be bigger");
                        }
                }
                else{
                        errx(4, "Wrong format of argument");
                }

                ssize_t bytes_read;
                ssize_t cnt = 0;
                char buf;

                while((bytes_read = read(0, &buf, sizeof(buf))) > 0){
                        cnt += sizeof(buf);

                        if(buf == '\n'){
                                cnt = 0;
                                if(write(1, &buf, sizeof(buf)) != sizeof(buf)){
                                        err(6, "Error writing");
                                }

                                continue;
                        }

                        if(cnt >=n && cnt <= m){
                                if(write(1, &buf, sizeof(buf)) != sizeof(buf)){
                                        err(6, "Error writing");
                                }
                        }
                }

                if(bytes_read == -1){
                        err(5, "Error reading");
                }
        }
        else if(strcmp(argv[1], "-d") == 0){
                if(argc != 5){
                        errx(7, "Not enough arguments for -d");
                }

                if(strcmp(argv[3], "-f") != 0){
                        errx(8, "Wrong format for -d");
                }

                char delim = argv[2][0];
                int n, m;
                if(strlen(argv[4]) == 1){
                        n = argv[4][0] - '0';
                        m = n;
                }
                else if(strlen(argv[4]) == 3){
                        n = argv[4][0] - '0';
                        m = argv[4][2] - '0';

                        if(n > m){
                                errx(3, "The second must be bigger");
                        }

                }
                else{
                        errx(4, "Wrong format of argument");
                }

                int col = 1;
                char buf;
                ssize_t bytes_read;

                while((bytes_read = read(0, &buf, sizeof(buf))) > 0){
                        if(buf == delim){
                                col++;

                                if(col > n && col <= m){
                                        if(write(1, &buf, sizeof(buf)) != sizeof(buf)){
                                                err(6, "Error writing");
                                        }
                                }
                        }
                        else if(buf == '\n'){
                                col = 1;
                                if(write(1, &buf, sizeof(buf)) != sizeof(buf)){
                                        err(6, "Error writing");
                                }
                        }
                        else{
                                if(col >= n && col <= m){
                                        if(write(1, &buf, sizeof(buf)) != sizeof(buf)){
                                                err(6, "Error writing");
                                        }
                                }
                        }
                }

                if(bytes_read == -1){
                        err(5, "Error reading");
                }

        }
        else{
                errx(2, "Not in correct format");
        }


    return 0;
}

/* Зад. 68 2018-SE-03 Напишете програма на C, която да работи подобно на командата cut, реализирайки
само следната функционалност:
• програмата трябва да чете текст от стандартния вход и да извежда избраната част от всеки
ред на стандартния изход;
• ако първият параметър на програмата е низът -c, тогава вторият параметър е или едноцифре-
но число (от 1 до 9), или две едноцифрени числа N и M (N ≤ M), разделени с тире (напр. 3-5).
В този случай програмата трябва да изведе избраният/избраните символи от реда: или само
символа на указаната позиция, или няколко последователни символи на посочените позиции.
• ако първият параметър на програмата е низът -d, тогава вторият параметър е низ, от който е
важен само първият символ; той се използва като разделител между полета на реда. Третият
параметър трябва да бъде низът -f, а четвъртият - или едноцифрено число (от 1 до 9),
или две едноцифрени числа N и M (N ≤ M), разделени с тире (напр. 3-5). В този случай
програмата трябва да разглежда реда като съставен от няколко полета (може би празни),
разделени от указания символ (първият символ от низа, следващ парметъра -d), и да изведе
или само указаното поле, или няколко последователни полета на указаните позиции, разделени
от същия разделител.
• ако някой ред няма достатъчно символи/полета, за него програмата трябва да изведе каквото
(докъдето) е възможно (или дори празен ред). */
