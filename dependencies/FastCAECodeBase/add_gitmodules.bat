@echo off
setlocal enabledelayedexpansion
chcp 65001
:: 遍历 modulesPath.txt 的每一行
:: 设置路径
set "A2=FITK_Component/"

for /f "usebackq delims=" %%i in ("modulesPath.txt") do (
    :: 获取 URL 最后一段
    set "line=%%i"
    for %%j in ("!line!") do (
        set "last_part=%%~nj"
    )
    :: 提取第一个点号之前的部分
    for /f "tokens=1 delims=." %%k in ("!last_part!") do (
        set "a1=%%k"
        
        set "a3=!A2!!a1!"
    echo 正在添加子模块: %%i 到目录:!a3!
    git submodule add %%i !a3!
    
    )
)

pause
