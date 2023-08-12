
#!/bin/bash

# Check for the correct number of arguments
if [ $# -lt 3 ]; then
    echo "Usage: $0 <time> <command> [args...]"
    exit 1
fi

# Extract the time argument
time_limit=$1

# Extract the command and its arguments
command_to_run="${@:2}"

# Initialize counters
total_runtime=0
num_runs=0

# Run the command repeatedly until the time limit is reached
start_time=$(date +%s.%N)
while true; do
    current_time=$(date +%s.%N)
    elapsed_time=$(echo "$current_time - $start_time" | bc -l)

    # Check if the elapsed time exceeds the time limit
    if [ $(echo "$elapsed_time >= $time_limit" | bc -l) -eq 1 ]; then
        break
    fi

    # Run the command and measure its runtime
    start_command=$(date +%s.%N)
    $command_to_run
    end_command=$(date +%s.%N)

    runtime=$(echo "$end_command - $start_command" | bc -l)

    # Accumulate runtimes
    total_runtime=$(echo "$total_runtime + $runtime" | bc -l)
    num_runs=$((num_runs + 1))
done

# Calculate average runtime
average_runtime=$(echo "$total_runtime / $num_runs" | bc -l)

# Display results
$echo "Ran the command '$command_to_run' $num_runs times for  $(printf "%.2f" $total_runtime) seconds."
echo "Average runtime: $(printf "%.2f" $average_runtime) seconds."


#Зад. 1
#Задачата ви е да напишете скрипт benchmark.sh, който измерва средното време за изпълнение на
#дадена команда. Първият аргумент на скрипта е число (време за провеждане на експеримента, в
#секунди), а останалите аргументи на скрипта са измерваната команда и нейните аргументи.
#Скриптът трябва да изпълнява подадената команда многократно, докато изтече подаденото време.
#Когато това се случи, скриптът трябва да изчака последното извикване на командата да приключи и
#да изведе съобщение, описващо броя направени извиквания, общото и средното време за изпълнение.
#$ ./benchmark.sh 60 convert image.jpg result.png
#Ran the command ’convert image.jpg result.png’ 8 times for 63 seconds.
#Average runtime: 7.88 seconds.
#$ ./benchmark.sh 10 sleep 1.5
#Ran the command ’sleep 1.5’ 7 times for 10.56 seconds.
#Average runtime: 1.51 seconds.
#Забележки:
#• Времената се извеждат в секунди, с точност два знака след запетайката.
#• Приемете, че времето на изпълнение на частите от скрипта извън подадената команда е пре-
#набрежимо малко.
