for /f "delims=" %%i in ('dir Plugex_* /b') do (
    cd %%i\Builds\VisualStudio2017
    for %%f in ( dir *.sln ) do ( msbuild %%f -p:Configuration=Release )
    cd ..\..\..
)