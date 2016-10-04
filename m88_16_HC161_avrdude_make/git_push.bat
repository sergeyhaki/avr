call git.exe init

call git.exe remote add origin https://github.com/sergeyhaki/autolisp.git

call git.exe add . 

call git.exe rm --cached git_push.bat

call git.exe commit -m "%Date:~6,4% %Date:~3,2% %Date:~0,2%"

call git.exe push -u origin master

pause
