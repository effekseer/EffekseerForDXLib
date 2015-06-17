■　EffekseerForDXLib
現在開発中のDXライブラリでエフェクトを使用するためのラッパーです。

VS2012,2013専用です。また、現在x64、DirectX11対応はしておりません。

■　環境構築方法
「プロジェクトに追加すべきファイル_VC用/」 にEffekseerが追加されたDXライブラリが保存されています。
DXライブラリと同じように「プロジェクトに追加すべきファイル_VC用/」にあるファイルをプロジェクトに追加してください。

既にDXライブラリを使用している場合、
「Effekseer_vs2012_x86.lib」
「Effekseer_vs2012_x86_d.lib」
「EffekseerRendererDX9_vs2012_x86_d.lib」
「EffekseerRendererDX9_vs2012_x86.lib」
「EffekseerForDXLib_vs2012_x86_d.lib」
「EffekseerForDXLib_vs2012_x86.lib」
「EffekseerRendererDX9_vs2013_x86_d.lib」
「EffekseerRendererDX9_vs2013_x86.lib」
「EffekseerForDXLib_vs2013_x86_d.lib」
「EffekseerForDXLib_vs2013_x86.lib」

「Effekseer.h」
「EffekseerRendererDX9.h」
「EffekseerForDXLib.h」
のみをコピーしてください。

ただし、「Ver3.14d」(同じ名称で複数バージョンあるため、現在公式ページからダウンロードできる最新バージョン)以外のバージョンを使用している場合、
上記のlibファイルのコピーでは動作しないかもしれません。
その場合は、バージョンを一致させるか、「プロジェクトに追加すべきファイル_VC用/」にあるファイルを全てコピーしてください。

2D版の使い方は、とりあえずサンプルを見てください。

アーカイブからのファイル読み込みは現在未デバッグです。
ただ、DXライブラリの関数でファイルを読み込むようにしているため、おそらく動くはずです。

■　不足している機能

3D対応
x64対応

■　ライセンス
Effekseerを追加したDXライブラリを使用する場合は、ヘルプ等に以下の文面を加える必要があります。

・DXライブラリ
DX Library Copyright (C) 2001-2015 Takumi Yamada.

・Effekseer

The MIT License (MIT)

Copyright (c) 2011 Effekseer Project

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


DirectX Tool Kit
https://directxtk.codeplex.com/
Microsoft Public License (Ms-PL)
