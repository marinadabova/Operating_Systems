## Processes

All problems can be found [here](https://github.com/avelin/fmi-os/tree/master/sample_code/07.c.processes)


[task1](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/02_processes/task1) - Да се напише програма на C, която изпълнява команда date.

[task2](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/02_processes/task2)- Да се напише програма на C, която изпълнява команда ls с точно един аргумент.
    
[task3](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/02_processes/task3) - Да се напише програма на C, която изпълнява команда sleep (за 60 секунди).

[task4](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/02_processes/task4) - Да се напише програма на C, която създава процес дете и демонстрира принцина на конкурентност при процесите.

[task5](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/02_processes/task5) - Да се напише програма на C, която е аналогична на горния пример, но принуждава бащата да изчака сина си да завърши.

[task-for-loop](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/02_processes/task-for-loop) - Прегледайте прикачената програма на C във fork-loop/main.c и отговорете на следните въпроси:

        -В какъв ред ще се отпечатат символните низове на всеки от процесите? Пробвайте да изпълните програмата няколко пъти.

        -Какво ще се изпише на стандартния изход, ако само разкоментираме exit(0); в детинските процеси?

        -В какъв ред ще се отпечатат низовете, ако само разкоментираме wait(NULL);? Редът един и същ ли ще е винаги? Защо?

        -Какво ще се отпечата на стандартния изход, ако разкоментираме и exit(0);, и wait(NULL);?

        -Колко реда ще се отпечатат, ако N = 3 при обстоятелствата от предния въпрос?

        -Колко реда ще се отпечатат, ако N = 3, но exit(0); е закоментиран? При тези обстоятелства, как нараства броят на отпечатаните редове при растене на N? (Съвет: НЕ пробвайте да изпълнявате програмата с "голямо" N)

        -Ще има ли memory leak, ако разменим реда на free(indentation); и разкоментирания exit(0);?

        -Ще има ли memory leak, ако махнем всичките 2 реда free(indentation); и оставим само един free(indentation); на последния ред във for цикъла? Ако да, при какви ситуации?
