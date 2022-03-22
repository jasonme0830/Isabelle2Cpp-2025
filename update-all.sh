#!/bin/bash
for file in `find example/*.thy`
do
    echo "<CONVERT> $file"
    bin/hol2cpp $file -s --move-list --reduce-cond
    echo ""
done

bin/hol2cpp example/hol_main/List_raw.thy -s --move-list --reduce-cond
