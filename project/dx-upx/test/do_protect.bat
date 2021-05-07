echo %1 
::so-protector name
echo %2
::libs path 

cd %2


for /R %DIR% %%f in (*.so) do (
    SetLocal enableDelayedExpansion
    
    echo %%f|findstr /c:"upx-test.apk">nul 2>nul && set /a ret=1 || set /a ret=0

    if !ret! equ 1 (
        ..\%1 --android-shlib %%f
    ) else (
        ..\%1 --android-shlib %%f  --android-check-sign=0xA28BA208 --android-check-integrity --anti-ida --android-anti-dump --android-anti-debug
    )
  
    Endlocal
    
)

cd ..

echo "finish..."
