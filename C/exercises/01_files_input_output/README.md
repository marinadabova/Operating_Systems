## C files, input, output

All problems can be found [here](https://github.com/avelin/fmi-os/tree/master/sample_code/06.c.file.input.output)

[Ex1](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task0) - Копирайте съдържанието на файл1 във файл2

[Ex2](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task1) - Реализирайте команда head без опции (т.е. винаги да извежда на стандартния изход само първите 10 реда от съдържанието на файл подаден като първи параматър)

[Ex3](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task2) - Реализирайте команда wc, с един аргумент подаден като входен параметър

[Ex4](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task3) - Реализирайте команда swap, разменяща съдържанието на два файла, подадени като входни параметри.

[Ex5](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task4) - Реализирайте команда cat, работеща с произволен брой подадени входни параметри.

[Ex6](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task5) - Реализирайте команда cp, работеща с произволен брой подадени входни параметри.

[Ex7](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task6) - Koпирайте файл /etc/passwd в текущата ви работна директория и променете разделителят на копирания файл от ":", на "?"

[Ex8](https://github.com/marinadabova/Operating_Systems/tree/main/C/exercises/01_files_input_output/task7) - Напишете програма, която приема точно 2 аргумента. Първият може да бъде само --min, --max или --print (вижте man 3 strcmp). Вторият аргумент е двоичен файл, в който има записани цели неотрицателни двубайтови числа (uint16_t - вижте man stdint.h). 


Ако първият аргумент е:

--min - програмата отпечатва кое е най-малкото число в двоичния файл.

--max - програмата отпечатва кое е най-голямото число в двоичния файл.

--print - програмата отпечатва на нов ред всяко число.


Използвайте двоичния файл binary/dump или генерирайте сами такъв (правилно подравнен).


