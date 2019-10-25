for /f "delims=" %%i in ('dir Plugex_* /b') do (
    cd %%i\Builds\VisualStudio2017
    for %%f in ( dir *.sln ) do ( msbuild %%f -p:Configuration=Release )
    copy /y /b x64\Release\VST\*.dll ..\..\..\windows_x64_binaries\VST\
    copy /y /b x64\Release\VST3\*.vst3 ..\..\..\windows_x64_binaries\VST3\
    cd ..\..\..
)