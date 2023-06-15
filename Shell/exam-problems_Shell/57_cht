#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument needed"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "First argument should be dir"
    exit 2
fi

tempfile=$(mktemp)
duplicates=$(mktemp)

# Итерираме през всички файлове в директорията и генерираме хеш-сума за всеки един
while read file; do
    hash=$(sha256sum "$file" | awk '{print $1}')
    echo "$hash $file" >> "$tempfile"
    echo "$hash" >> "$duplicates"
done < <(find "$1" -type f)

space=0
group=0

# Обработваме списъка с хеш-суми
while read line; do
    firstFile=$(echo "$line" | awk '{print $2}')
    hashS=$(echo "$line" | awk '{print $1}')
    name=$(basename "$firstFile")

    # Намираме всички дубликати на текущата хеш-сума
    dupl=$(grep "$hashS" "$tempfile")

    # Изтриваме текущия хеш от списъка с дубликати
    grep -v "$hashS" "$duplicates" > "$duplicates.tmp"
    mv "$duplicates.tmp" "$duplicates"

    # Намираме оригиналния файл за текущата група
    original_file=$(grep "^$hashS" "$duplicates" | awk '{print $2}' | head -n 1)

    # Ако името на текущия файл не съвпада с името на оригиналния файл,
    # създаваме хардлинк към оригиналния файл и пресмятаме освободеното място
    if [ "$name" != "$(basename "$original_file")" ]; then
        ln "$original_file" "$1/$name"
        saved_space=$(stat -c "%s" "$1/$name")
        space=$((space + saved_space))
    fi
done < "$tempfile"

echo "Deduplicated groups: $(wc -l < "$duplicates")"
echo "Bytes freed: $space"
