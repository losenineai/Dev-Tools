

echo "./protect_binary.sh zip_name output-path pic_dir report_name so_list [task1] [task2]..."
so_protector=`dirname $0`/upx.out
echo $so_protector
zip_name=$1
output_path=$2
pic_dir=$3
report_name=$4
so_list=$5

task1=$6
task2=$7
task3=$8
task4=$9
task5=$10

if [ -d lib ]
then 
    rm -r lib
fi

cp $zip_name $output_path

so_files=""
for line in `cat $so_list`
do
    echo $line
    so_files=$so_files" "$line
    unzip $output_path $line
done

echo $so_files

$so_protector --android-shlib $so_files --report-name=$report_name --pic-dir=$pic_dir --zip-name=$output_path $task1 $task2 $task3 $task4 $task5

for line in `cat $so_list`
do
    echo $line
    zip -u $output_path $line
done