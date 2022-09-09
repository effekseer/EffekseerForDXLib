
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2019.sln /p:configuration=Debug /p:platform=win32
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2019.sln /p:configuration=Release /p:platform=win32
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2019.sln /p:configuration=Debug /p:platform=x64
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2019.sln /p:configuration=Release /p:platform=x64

"C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2022.sln /p:configuration=Debug /p:platform=win32
"C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2022.sln /p:configuration=Release /p:platform=win32
"C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2022.sln /p:configuration=Debug /p:platform=x64
"C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" Dev\EffekseerForDXLib_vs2022.sln /p:configuration=Release /p:platform=x64

python generate_release_package.py

pause