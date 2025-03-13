# 读取日志文件并累加相应操作的时间
while read -r line; do
    if [[ $line =~ (delete|write|read)\ complete\ time:([0-9]+) ]]; then
        operation=${BASH_REMATCH[1]}
        time=${BASH_REMATCH[2]}
        case $operation in
            delete) delete_time=$((delete_time + time)) ;;
            write) write_time=$((write_time + time)) ;;
            read) read_time=$((read_time + time)) ;;
        esac
    fi
done < "code_craft.log"  # 请替换为你的日志文件名

# 输出统计结果
echo "Total delete time: $delete_time"
echo "Total write time: $write_time"
echo "Total read time: $read_time"
