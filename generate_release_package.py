# -*- coding: utf-8 -*-

import utils

targetDir = 'EffekseerForDXLib_170_323d/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime_DXLib/Compiled/'

utils.cdToScript()

utils.rmdir(targetDir)
utils.mkdir(targetDir)

utils.copytree(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy(effekseerVSDir+'include/Effekseer.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/Effekseer.Modules.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/Effekseer.SIMD.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX9.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX11.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')



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



utils.copy(effekseerVSDir+'lib/VS2022/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2022_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2022/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2022_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2022/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2022_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2022/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2022_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2022/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2022_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2022/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2022_x86.lib')

utils.copy(effekseerVSDir+'lib/VS2022WIN64/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2022_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2022_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2022_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2022WIN64/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2022_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2022_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2022_x64.lib')



utils.copy('readme.txt',targetDir)
utils.copy('Help_En.html',targetDir)
utils.copy('Help_Ja.html',targetDir)

utils.copytreeWithExt('Sample/',targetDir+'Sample/',['.sln','.png','.filters','.vcxproj','.cpp','.efk','.efkefc'])

utils.copy('Dev/EffekseerForDXLib/EffekseerForDXLib.h',targetDir+'プロジェクトに追加すべきファイル_VC用/')


utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x86_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x86.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x64_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2019_x64.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy('Dev/lib/EffekseerForDXLib_vs2022_x86_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2022_x86.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2022_x64_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2022_x64.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')

