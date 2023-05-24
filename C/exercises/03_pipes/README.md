## Pipes

All problems can be found [here](https://github.com/avelin/fmi-os/tree/master/sample_code/08.c.pipes)


[task1](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/03_pipes/task1) -Да се напише програма на C, която приема аргумент - име на файл. Програмата да:

-записва във файла 'fo'

-създава child процес, който записва 'bar\n'

-parent-а, след като изчака child процеса, записва 'o\n'


[task2](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/03_pipes/task2)- Напишете програма на C, която демонстрира комуникация през pipe между parent и child процеси. Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, който да го отпечатва на стандартния изход.
    
[task3](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/03_pipes/task3) - Напишете програма на C, която демонстрира употребата на dup/dup2 между parent и child процеси. Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, където той да може да се чете от stdin. Child процесът да изпълняваwc -c.

[task4](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/03_pipes/task4) - Напишете програма на C с dup2, реализираща: cat /etc/passwd | grep 5 | sort
