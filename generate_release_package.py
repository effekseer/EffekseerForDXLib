# -*- coding: utf-8 -*-

import utils

targetDir = 'EffekseerForDXLib_160alpha7_322c/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime_DXLib/Compiled/'

utils.cdToScript()

utils.rmdir(targetDir)
utils.mkdir(targetDir)

utils.copytree(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy(effekseerVSDir+'include/Effekseer.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX9.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX11.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy(effekseerVSDir+'lib/VS2017/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2017_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2017/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2017_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2017/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2017_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2017/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2017_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2017/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2017_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2017/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2017_x86.lib')

utils.copy(effekseerVSDir+'lib/VS2017WIN64/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2017_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2017WIN64/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2017_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2017WIN64/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2017_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2017WIN64/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2017_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2017WIN64/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2017_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2017WIN64/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2017_x64.lib')


utils.copy(effekseerVSDir+'lib/VS2019/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2019_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2019/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2019_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2019/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2019_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2019/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2019_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2019/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2019_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2019/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2019_x86.lib')

utils.copy(effekseerVSDir+'lib/VS2019WIN64/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2019_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2019_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2019_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2019WIN64/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2019_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2019_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2019_x64.lib')




utils.copy('readme.txt',targetDir)
utils.copy('Help_En.html',targetDir)
utils.copy('Help_Ja.html',targetDir)

utils.copytreeWithExt('Sample/',targetDir+'Sample/',['.sln','.png','.filters','.vcxproj','.cpp','.efk','.efkefc'])

utils.copy('Dev/EffekseerForDXLib/EffekseerForDXLib.h',targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy('Dev/lib/EffekseerForDXLib_vs2017_x86_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2017_x86.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2017_x64_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2017_x64.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')


utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x86_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x86.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x64_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x64.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')



