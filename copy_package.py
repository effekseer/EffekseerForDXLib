# -*- coding: utf-8 -*-

import utils

targetDir = 'Dev/'
dxlibDir = 'DXLib_VC/'
effekseerDir = '../Effekseer/'
effekseerVSDir = effekseerDir + 'EffekseerRuntime_DXLib/Compiled/'

utils.cdToScript()

utils.mkdir(targetDir+'include/')
utils.mkdir(targetDir+'lib/')

utils.copytreeWithExt(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'include/', ['.h'])
utils.copytreeWithExt(dxlibDir+'プロジェクトに追加すべきファイル_VC用/', targetDir+'lib/', ['.lib'])

utils.copy(effekseerVSDir+'include/Effekseer.h', targetDir+'include/')
utils.copy(effekseerVSDir+'include/Effekseer.Modules.h', targetDir+'include/')
utils.copy(effekseerVSDir+'include/Effekseer.SIMD.h', targetDir+'include/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX9.h', targetDir+'include/')
utils.copy(effekseerVSDir+'include/EffekseerRendererDX11.h', targetDir+'include/')



utils.copy(effekseerVSDir+'lib/VS2019/Debug/Effekseer.lib', targetDir+'lib/Effekseer_vs2019_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Debug/Effekseer.lib', targetDir+'lib/Effekseer_vs2019_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2019/Debug/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2019_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2019/Debug/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2019_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2019WIN64/Debug/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2019_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Debug/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2019_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2019/Release/Effekseer.lib', targetDir+'lib/Effekseer_vs2019_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Release/Effekseer.lib', targetDir+'lib/Effekseer_vs2019_x64.lib')

utils.copy(effekseerVSDir+'lib/VS2019/Release/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2019_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2019/Release/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2019_x86.lib')

utils.copy(effekseerVSDir+'lib/VS2019WIN64/Release/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2019_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2019WIN64/Release/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2019_x64.lib')



utils.copy(effekseerVSDir+'lib/VS2022/Debug/Effekseer.lib', targetDir+'lib/Effekseer_vs2022_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Debug/Effekseer.lib', targetDir+'lib/Effekseer_vs2022_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2022/Debug/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2022_x86_d.lib')
utils.copy(effekseerVSDir+'lib/VS2022/Debug/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2022_x86_d.lib')

utils.copy(effekseerVSDir+'lib/VS2022WIN64/Debug/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2022_x64_d.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Debug/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2022_x64_d.lib')

utils.copy(effekseerVSDir+'lib/VS2022/Release/Effekseer.lib', targetDir+'lib/Effekseer_vs2022_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Release/Effekseer.lib', targetDir+'lib/Effekseer_vs2022_x64.lib')

utils.copy(effekseerVSDir+'lib/VS2022/Release/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2022_x86.lib')
utils.copy(effekseerVSDir+'lib/VS2022/Release/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2022_x86.lib')

utils.copy(effekseerVSDir+'lib/VS2022WIN64/Release/EffekseerRendererDX9.lib', targetDir+'lib/EffekseerRendererDX9_vs2022_x64.lib')
utils.copy(effekseerVSDir+'lib/VS2022WIN64/Release/EffekseerRendererDX11.lib', targetDir+'lib/EffekseerRendererDX11_vs2022_x64.lib')
