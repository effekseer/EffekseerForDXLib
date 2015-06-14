■　EffekseerForDXLib
現在開発中のDXライブラリでエフェクトを使用するためのラッパーです。

VS2012以降専用です。また、現在x64対応はしておりません。

■　使用方法
「プロジェクトに追加すべきファイル_VC用/」 にEffekseerが追加されたDXライブラリが保存されています。
DXライブラリと同じように「プロジェクトに追加すべきファイル_VC用/」にあるファイルをプロジェクトに追加してください。

既にDXライブラリを使用している場合、
「Effekseer_vs2012_x86.lib」
「Effekseer_vs2012_x86_d.lib」
「EffekseerRendererDX9_vs2012_x86_d.lib」
「EffekseerRendererDX9_vs2012_x86.lib」
「EffekseerForDXLib_vs2012_x86_d.lib」
「EffekseerForDXLib_vs2012_x86.lib」

「Effekseer.h」
「EffekseerRendererDX9.h」
「EffekseerForDXLib.h」
のみをコピーしてください。

2D版の使い方は、とりあえずサンプルを見てください。

■　不足している機能
アーカイブからのファイル読み込み
3D対応
x64対応
