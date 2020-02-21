for /f "delims=" %%i in ('dir Plugex_* /b') do (
    cd %%i
    for %%f in ( *.jucer ) do ( C:\Users\Admin\JUCE\Projucer.exe --resave %%f )
    cd ..
)