// Зад. 87 2021-SE-01 Ваши колеги - асистенти по ОС се оплакват, че студентите упорито ползват командата sudo, въпреки че им е обяснено да не я ползват. Вашата задача е да подготвите фалшиво sudo, което:
// • добавя ред в текстови файл foo.log с командата, която потребителят се е опитал да изпълни във формат ДАТА ПОТРЕБИТЕЛ ДАННИ, където:
// – ДАТА е във формат YYYY-MM-DD HH:MM:SS.UUUUUU (UUUUUU са микросекунди)
// – ПОТРЕБИТЕЛ е login name (username) на потребителя
// – ДАННИ е всичко, което потребителят е написал, например ./main ls -l /root
// – горните полета са разделени с интервал
// – пример: 2021-05-10 14:38:17.584344 s99999 ./main ls -l /root
// • заключва акаунта, който е изпълнил програмата
// • приключва изпълнението на всички процеси, стартирани от този потребител
// За да може да изпълни тези неща, фалшивото sudo ще има нужда от root права, а в същото време трябва да знае кой акаунт изпълнява командата. Традиционно това може да се реши със SUID, 
// което обаче не важи за shell скриптове. За да заобиколите този проблем, напишете програма на C (main), която за заключването на акаунта и терминирането на процесите извиква външни shell команди през pipe().
// Полезни man страници:
// • gettimeofday(2)
// • localtime(3)
// • strftime(3)
// • getuid(2)
// • getpwuid(3)
// • passwd(1)
// Опишете в doc.txt как бихте настроили компилираната вече програма така, че студентите да ползват нея, вместо оригиналното sudo.
// Естествено, по време на разработка на програмата можете да я тествате само с вашият акаунт и нямате root права. За да може все пак да тествате, подменете в кода финалните команди, 
//които програмата изпълнява (тези, които наистина изискват root права), като им добавите едно echo. Например, ако трябва да изпълните командата foo -m pesho 
// вместо това в кода ви трябва да е дефинирано изпълнение на echo foo -m pesho

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char *argv[]) {
    // Check if the command-line arguments are provided
    if (argc < 2) {
        errx(1, "Usage: %s <command>", argv[0]);
    }

    // Obtain the current time
    struct timeval current_time;
    if (gettimeofday(&current_time, NULL) == -1) {
        err(1, "Error with gettimeofday");
    }

    // Obtain the local time
    struct tm *local_tm = localtime(&current_time.tv_sec);
    if (local_tm == NULL) {
        err(1, "Error with localtime");
    }

    // Format the time string
    char time_string[30];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", local_tm);

    // Obtain the user ID
    uid_t user_id = getuid();

    // Obtain the user name
    struct passwd *user_pw = getpwuid(user_id);
    if (user_pw == NULL) {
        err(1, "Error with getpwuid");
    }

    // Construct the log entry
    char log_entry[256];
    snprintf(log_entry, sizeof(log_entry), "%s %s %s\n", time_string, user_pw->pw_name, argv[1]);

    // Open foo.log in append mode
    int log_fd = open("foo.log", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (log_fd == -1) {
        err(1, "Error opening foo.log");
    }

    // Write the log entry to foo.log
    if (write(log_fd, log_entry, strlen(log_entry)) == -1) {
        close(log_fd);
        err(1, "Error writing to foo.log");
    }

    // Close foo.log
    if (close(log_fd) == -1) {
        err(1, "Error closing foo.log");
    }

    // Lock the account
    dprintf(fileno(stdout), "Locking the account...\n");
    // Replace the above line with the actual command to lock the account

    // Terminate user's processes
    dprintf(fileno(stdout), "Terminating user's processes...\n");
    // Replace the above line with the actual command to terminate processes

    exit(0);
}
