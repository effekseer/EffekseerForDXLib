# -*- coding: utf-8 -*-

import utils

targetDir = 'EffekseerForDXLib_101_315a/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime101/Compiled/'

utils.cdToScript()

utils.rmdir(targetDir)
utils.mkdir(targetDir)

utils.copytree('Help/', targetDir+'Help/')

utils.copytree(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy(effekseerVSDir+'include/Effekseer.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX9.h', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy(effekseerVSDir+'lib/VS2012/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2012_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2012/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2012_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2012/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2012_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2012/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2012_x86.lib')


utils.copy(effekseerVSDir+'lib/VS2013/Debug/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2013_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2013/Debug/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2013_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2013/Release/Effekseer.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/Effekseer_vs2013_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2013/Release/EffekseerRendererDX9.lib', targetDir+'プロジェクトに追加すべきファイル_VC用/EffekseerRendererDX9_vs2013_x86.lib')


utils.copy('readme.txt',targetDir)

utils.copytreeWithExt('Sample/',targetDir+'Sample/',['.sln','.png','.filters','.vcxproj','.cpp','.efk'])

utils.copy('Dev/EffekseerForDXLib/EffekseerForDXLib.h',targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy('Dev/lib/EffekseerForDXLib_vs2012_x86_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2012_x86.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copy('Dev/lib/EffekseerForDXLib_vs2013_x86_d.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')
utils.copy('Dev/lib/EffekseerForDXLib_vs2013_x86.lib',targetDir+'プロジェクトに追加すべきファイル_VC用/')





