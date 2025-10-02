# -*- coding: utf-8 -*-

import utils

targetDir = 'EffekseerForDXLib_1.8.0.0_324f/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime_DXLib/'

utils.cdToScript()

utils.rmdir(targetDir)
utils.mkdir(targetDir)

utils.copytree(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'プロジェクトに追加すべきファイル_VC用/')

utils.copytreeWithExt(effekseerVSDir+'vs2022_x64/install/include/', targetDir+'プロジェクトに追加すべきファイル_VC用/', ['.h'])

vs_types = ['vs2019', 'vs2022']
cpu_types = ['x86', 'x64']
suffix_types = ['_d', '']
libs = ['Effekseer', 'EffekseerRendererDX9', 'EffekseerRendererDX11', 'EffekseerRendererCommon']

for lib in libs:
    for vs_type in vs_types:
        for cpu_type in cpu_types:
            for suffix_type in suffix_types:
                utils.copy(effekseerVSDir+f'{vs_type}_{cpu_type}/install/lib/{lib}{suffix_type}.lib', targetDir+f'プロジェクトに追加すべきファイル_VC用/{lib}_{vs_type}_{cpu_type}{suffix_type}.lib')


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

