#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

//./main 5 sleep 3
//./main 1 sleep 2

int time_cond(time_t t_start, time_t t_end, int seconds);

int time_cond(time_t t_start, time_t t_end, int seconds) {
    return seconds > (t_end - t_start);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        errx(1, "We need at least 3 arguments");
    }

    int seconds = atoi(argv[1]);
    char* Q = argv[2];
    char** args = &argv[2];

    int condition = 0;

    int fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        err(2, "Cannot open file");
    }

    while (1) {
        pid_t pid = fork();
        if (pid == -1) {
            err(3, "Error with fork");
        }

        if (pid == 0) {
            if (execvp(Q, args) == -1) {
                err(4, "Error with exec");
            }
        } else {
            int status;
            time_t t_start = time(NULL);
            if (wait(&status) == -1) {
                err(5, "Error with wait");
            }
            time_t t_end = time(NULL);

            int exit_code;
            if (WIFEXITED(status)) {
                exit_code = WEXITSTATUS(status);
            } else {
                exit_code = 129;
            }

            dprintf(fd, "%ld %ld %d\n", t_start, t_end, exit_code);

            if (exit_code != 0 && time_cond(t_start, t_end, seconds)) {
                if (condition == 1) {
                    break;
                } else {
                    condition = 1;
                }
            } else {
                condition = 0;
            }
        }
    }

    close(fd);
    return 0;
}
/* Зад. 85 2019-SE-01 Напишете програма-наблюдател P, която изпълнява друга програма Q и я рестар-
тира, когато Q завърши изпълнението си. На командния ред на P се подават следните параметри:
• праг за продължителност в секунди – едноцифрено число от 1 до 9
• Q
• незадължителни параметри на Q
P работи по следния алгоритъм:
• стартира Q с подадените параметри
• изчаква я да завърши изпълнението си
• записва в текстов файл run.log един ред с три полета - цели числа (разделени с интервал):
– момент на стартиране на Q (Unix time)
– момент на завършване на Q (Unix time)
– код за грешка, с който Q е завършила (exit code)
• проверява дали е изпълнено условието за спиране и ако не е, преминава отново към старти-
рането на Q
Условие за спиране: Ако наблюдателят P установи, че при две последователни изпълнения на Q са
били изпълнени и двете условия:
1. кодът за грешка на Q е бил различен от 0;
2. разликата между момента на завършване и момента на стартиране на Q е била по-малка от
подадения като първи параметър на P праг;
то P спира цикъла от изпълняване на Q и сам завършва изпълнението си.
Текущото време във формат Unix time (секунди от 1 януари 1970 г.) можете да вземете с извикване
41
на системната функция time() с параметър NULL; функцията е дефинирана в time.h. Ако изпълне-
ната програма е била прекъсната от подаден сигнал, това се приема за завършване с код за грешка
129.*/
