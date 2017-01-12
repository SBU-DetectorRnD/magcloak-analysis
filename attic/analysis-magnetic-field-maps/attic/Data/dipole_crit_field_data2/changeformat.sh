rm *.txt2
rm *.txt3
for file in `ls *.txt`; 
do 
    echo "-> $file"; 
    file2=$(echo $file | sed 's/.txt/.txt2/g'); 
    file3=$(echo $file | sed 's/.txt/.txt3/g');
    echo "t/F:I/F:x/F:y/F:z/F:B/F:B_I0_T0/F:B_I0" >> $file2; 
    awk '{print $1, $2, 0, 0, 0, $3, 0.003, -0.104 }'  OFS="\t" $file >> $file2; 
    tr -d '\015' < $file2 > $file3
    mv $file3 $file
    rm $file2 
done
