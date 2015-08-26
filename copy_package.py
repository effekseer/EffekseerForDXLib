# -*- coding: utf-8 -*-

import utils

targetDir = 'Dev/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime101/Compiled/'

utils.cdToScript()

utils.mkdir(targetDir+'include/')
utils.mkdir(targetDir+'lib/')

utils.copytreeWithExt(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'include/', ['.h'])
utils.copytreeWithExt(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'lib/', ['.lib'])

utils.copy(effekseerVSDir+'include/Effekseer.h', targetDir+'include/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX9.h', targetDir+'include/')

utils.copy(effekseerVSDir+'lib/VS2012/Debug/Effekseer.lib', targetDir+'lib/Effekseer_vs2012_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2012/Debug/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2012_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2013/Debug/Effekseer.lib', targetDir+'lib/Effekseer_vs2013_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2013/Debug/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2013_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2012/Release/Effekseer.lib', targetDir+'lib/Effekseer_vs2012_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2012/Release/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2012_x86.lib')

utils.copy(effekseerVSDir+'lib/VS2013/Release/Effekseer.lib', targetDir+'lib/Effekseer_vs2013_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2013/Release/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2013_x86.lib')




