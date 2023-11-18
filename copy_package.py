# -*- coding: utf-8 -*-

import utils

targetDir = 'Dev/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime_DXLib/'

utils.cdToScript()

utils.mkdir(targetDir+'include/')
utils.mkdir(targetDir+'lib/')

utils.copytreeWithExt(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'include/', ['.h'])
utils.copytreeWithExt(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'lib/', ['.lib'])

utils.copytreeWithExt(effekseerVSDir+'vs2022_x64/install/include/', targetDir+'include/', ['.h'])

vs_types = ['vs2019', 'vs2022']
cpu_types = ['x86', 'x64']
suffix_types = ['_d', '']
libs = ['Effekseer', 'EffekseerRendererDX9', 'EffekseerRendererDX11', 'EffekseerRendererCommon']

for lib in libs:
    for vs_type in vs_types:
        for cpu_type in cpu_types:
            for suffix_type in suffix_types:
                utils.copy(effekseerVSDir+f'{vs_type}_{cpu_type}/install/lib/{lib}{suffix_type}.lib', targetDir+f'lib/{lib}_{vs_type}_{cpu_type}{suffix_type}.lib')
