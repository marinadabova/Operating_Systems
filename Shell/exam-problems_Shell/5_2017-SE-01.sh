#Намерете имената на топ 5 файловете в текущата директория с най-много hardlinks.


find . -printf "%p %n\n" | sort -nr -k 2 | head -5
find . -type f | xargs -I{} stat -c "%h %n" {} | sort -k1nr | head -n 5 | awk '{print $2}'
