# -*- coding: utf-8 -*-

import utils

targetDir = 'EffekseerForDXLib_141_319f/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime141/Compiled/'

utils.cdToScript()

utils.call('call compile.bat')

utils.rmdir(targetDir)
utils.mkdir(targetDir)

utils.copytree('docs/', targetDir+'Help/')

utils.copytree(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy(effekseerVSDir+'include/Effekseer.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX9.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX11.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy(effekseerVSDir+'lib/VS2015/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2015_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2015_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2015_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2015/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2015_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2015_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2015_x86.lib')

utils.copy(effekseerVSDir+'lib/VS2015/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2015_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2015_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Debug/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2015_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2015/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2015_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2015_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2015/Release/EffekseerRendererDX11.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX11_vs2015_x64.lib')

utils.copy('readme.txt',targetDir)

utils.copytreeWithExt('Sample/',targetDir+'Sample/',['.sln','.png','.filters','.vcxproj','.cpp','.efk'])

utils.copy('Dev/EffekseerForDXLib/EffekseerForDXLib.h',targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy('Dev/lib/EffekseerForDXLib_vs2015_x86_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2015_x86.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')




