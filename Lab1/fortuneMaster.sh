# 错误输出函数
error()
{
	echo "输入有误,请重新输入"
}
# 函数定义需放在使用前
echo ""
echo "    ++++++++++++++++++++"
echo "    +     算命大师     +"
echo "    +           by:hhy +"
echo "    ++++++++++++++++++++"

# 生肖数组
animals=("猴" "鸡" "狗" "猪" "鼠" "牛" "虎" "兔" "龙" "蛇" "马" "羊")
# 空格为分隔符

# 主函数
main()
{
	echo ""
	# 输入生日
	read -p "请输入出生年月日(例如20010521),按q退出:" birthday
	# 按q退出
	if (($birthday == "q"));then 
		exit 0
	fi
	year=$[birthday/10000]	# 注意[后不能有空格
	month=$[birthday/100%100]
	day=$[birthday%100]
	#echo $year,$month,$day
	# 判断输入合法性
	if ((year == 0)) || ((month == 0)) || ((day == 0)) || ((month > 12));then
		error
		return
	fi
	# 判断闰年
	leap=0
	tmp1=$[year%400]
	tmp2=$[year%100]
	tmp3=$[year%4]
	if ((tmp1==0)) || ((tmp2!=0)) && ((tmp3==0)) ;then
		leap=1
	fi
	# 判断日期合法性
	flag=1
	for i in {1,3,5,7,8,10,12}; do
		if ((month==i));then
			if ((day>31));then
				error
				return
			fi
			flag=0
			break
		fi
	done
	if ((flag));then
		if ((month!=2));then
			if((day>30)); then
				error
				return
			fi
		elif((leap==0));then
			if((day>28));then
				error
				return
			fi
		elif ((day>29));then
			error
			return
		fi
	fi
	# 输出生肖
	animal=$[year%12]
	echo "生肖: ${animals[animal]}"
	# 输出星座
	star=$[birthday%10000]
	if ((star<=119));then
		echo "星座: 摩羯座"
	elif ((star<=218));then
		echo "星座: 水瓶座"
	elif ((star<=320));then
		echo "星座: 双鱼座"
	elif ((star<=419));then
		echo "星座: 白羊座"
	elif ((star<=520));then
		echo "星座: 金牛座"
	elif ((star<=621));then
		echo "星座: 双子座"
	elif ((star<=722));then
		echo "星座: 巨蟹座"
	elif ((star<=822));then
		echo "星座: 狮子座"
	elif ((star<=922));then
		echo "星座: 处女座"
	elif ((star<=1023));then
		echo "星座: 天秤座"
	elif ((star<=1122));then
		echo "星座: 天蝎座"
	elif ((star<=1221));then
		echo "星座: 射手座"
	else
		echo "星座: 摩羯座"
	fi
}

while true
do
	main
done
		


