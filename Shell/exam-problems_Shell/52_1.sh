#!/bin/bash

if [ "${#}" -ne 1 ]; then
        echo "One arg needed"
        exit 1
fi

if [ ! -f "${1}" ]; then
        echo "Argument should be file"
        exit 2
fi

while read DEVICE STATE; do
        if ! grep -q "${DEVICE}" /proc/acpi/wakeup ; then #sravnqvame error code-a
                echo "WARN: ${DEVICE} not in wakeup" >&2
                continue
        fi
        CURSTATE=$(cat /proc/acpi/wakeup |grep ${DEVICE}  | sed -E 's/^.*\*(.*) .*$/\1/')
        if [ "${CURSTATE}" != "${STATE}" ]; then
                echo "$DEVICE" >> "/proc/acpi/wakeup"

        fi
done< <(cat "${1}" | cut -d '#' -f 1 | grep -v "^ *$")
