@echo off
echo=
echo 	++++++++++++++++++++
echo 	+     算命大师     +
echo 	+           by:hhy +
echo 	++++++++++++++++++++
:start
echo=

::输入生日
set /p birthday=请输入出生年月日(例如20010521),按q退出:
:: /p用于接收输入
REM echo %birthday%	
:: %%用于引用变量
if /i %birthday%==q exit
:: /i表示不区分大小写

::计算年月日
set /a year=%birthday%/10000
:: set /a 表示计算表达式
set /a month=%birthday%/100%%100
set /a day=(%birthday%)%%100
REM echo %year%, %month%, %day%

::判断输入合法性
if %year% equ 0 goto error
if %month% equ 0 goto error
if %day% equ 0 goto error
if %month% gtr 12 goto error

::判断闰年
set leap=0
set /a tmp1=(%year%)%%400
set /a tmp2=(%year%)%%100
set /a tmp3=(%year%)%%4
if %tmp1% equ 0 (
	set leap=1
) else if %tmp2% neq 0 if %tmp3% equ 0 (
	set leap=1
)

::判断日期合法性
if %month% equ 1 goto checkDay31
if %month% equ 3 goto checkDay31
if %month% equ 5 goto checkDay31
if %month% equ 7 goto checkDay31
if %month% equ 8 goto checkDay31
if %month% equ 10 goto checkDay31
if %month% equ 12 goto checkDay31
if %month% equ 4 goto checkDay30
if %month% equ 6 goto checkDay30
if %month% equ 9 goto checkDay30
if %month% equ 11 goto checkDay30
if %month% equ 2 (
	if %leap% equ 0 (
		if %day% gtr 28 goto error
	) else if %day% gtr 29 goto error
)

:next
::计算生肖
set /a animal=(%year%)%%12
if %animal% equ 0 (
	echo 生肖: 猴
) else if %animal% equ 1 (
	echo 生肖: 鸡
) else if %animal% equ 2 (
	echo 生肖: 狗
) else if %animal% equ 3 (
	echo 生肖: 猪
) else if %animal% equ 4 (
	echo 生肖: 鼠
) else if %animal% equ 5 (
	echo 生肖: 牛
) else if %animal% equ 6 (
	echo 生肖: 虎
) else if %animal% equ 7 (
	echo 生肖: 兔
) else if %animal% equ 8 (
	echo 生肖: 龙
) else if %animal% equ 9 (
	echo 生肖: 蛇
) else if %animal% equ 10 (
	echo 生肖: 马
) else if %animal% equ 11 (
	echo 生肖: 羊
)
::计算星座
set /a star=(%birthday%)%%10000
REM echo %star%
if %star% leq 119 (
	echo 星座: 摩羯座
) else if %star% leq 218 (
	echo 星座: 水瓶座
) else if %star% leq 320 (
	echo 星座: 双鱼座
) else if %star% leq 419 (
	echo 星座: 白羊座
) else if %star% leq 520 (
	echo 星座: 金牛座
) else if %star% leq 621 (
	echo 星座: 双子座
) else if %star% leq 722 (
	echo 星座: 巨蟹座
) else if %star% leq 822 (
	echo 星座: 狮子座
) else if %star% leq 922 (
	echo 星座: 处女座
) else if %star% leq 1023 (
	echo 星座: 天秤座
) else if %star% leq 1122 (
	echo 星座: 天蝎座
) else if %star% leq 1221 (
	echo 星座: 射手座
) else (
	echo 星座: 摩羯座
)
goto start

::日期合法性检查
:checkDay31
if %day% gtr 31 goto error
goto next
:checkDay30
if %day% gtr 30 goto error
goto next

::错误处理
:error
echo 输入有误,请重新输入
goto start
