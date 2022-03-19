#!/bin/bash
for file in `find example/*.thy`
do
    echo "<CONVERT> $file"
    bin/hol2cpp $file -s --enable-list-move
    echo ""
done

bin/hol2cpp example/hol_main/List_raw.thy -s --enable-list-move
