#2023-SE-02
#!/bin/bash

[[ "${#}" -eq 1 ]] || { echo "Wrong number of arguments" >&2; exit 1; }
[[ -d "${1}" ]] || { echo "${1} not a directory"  >&2; exit 2; }

TMP_HASHES=$(mktemp -d)

while read -rd $'\0' FILE; do
    HASH=$(sha1sum "${file}" | cut -d' ' -f1)
    echo -n "${file}" >> "${TMP_HASHES}/${HASH}"
    echo -ne '\0' >> "${TMP_HASHES}/${HASH}"
done < <(find "${1}" -type f -print0)

TOTAL_GROUPS=0
TOTAL_BYTES=0

while read GROUP; do
    MAIN=$(head -zn 1 "${GROUP}")
    read M_INODE SIZE < <(stat -c "%i %s" "${MAIN}")

    GROUP_CHANGED=0

    while read -rd $'\0' OTHER; do
        read O_INODE O_COUNT < <(stat -c '%i %h' "${OTHER}")

        if [[ "${O_INODE}" -ne "${M_INODE}" ]]; then
            GROUP_CHANGED=1
            rm "${OTHER}"
            ln "${MAIN}" "${OTHER}"
            if [[ "${O_COUNT}" -eq 1 ]]; then
                TOTAL_BYTES=$((TOTAL_BYTES + SIZE))
            fi
        fi
    done < <(tail -zn +2 "${GROUP}")
    TOTAL_GROUPS=$((TOTAL_GROUPS + GROUP_CHANGED))
done < <(find "${TMP_HASHES}" -type f)

echo "Deduplicated groups: ${TOTAL_GROUPS}"
echo "Bytes freed: ${TOTAL_BYTES}"

rm -r "${TMP_HASHES}"
