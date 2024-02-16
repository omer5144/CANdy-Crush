#!/usr/bin/bash

trap "rm dump.log dump_0.log dump_1.log >/dev/null 2>&1" EXIT

echo "Press ctrl+C action occurred"
candump -L vcan0 > dump.log
echo ""

while true; do
    total_lines=$(wc -l < dump.log)
    if [ $total_lines -gt 1 ]; then
        half_lines=$((total_lines / 2))

        head -n $half_lines dump.log > dump_0.log
        tail -n +$((half_lines + 1)) dump.log > dump_1.log
        rm dump.log
        
        canplayer -I dump_1.log
        read -p "Did you see the action? (0 / 1):   " choice

        if [ $choice -eq 0 ]; then
            mv dump_0.log dump.log
        else
            mv dump_1.log dump.log
        fi
    else
        break
    fi
done

cat dump.log
rm dump.log