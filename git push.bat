call git.exe init

call git.exe remote add origin https://github.com/sergeyhaki/avr.git

call git.exe add . 

call git.exe commit -m "from bat"

call git.exe push -u origin master

pause
