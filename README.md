# EffekseerForDXLib

DXライブラリ向けEffekseerのラッパーです。

## 開発版コンパイル方法

### 環境構築

VisaulStudio2019, 2022をインストールします。

pythonの3系列をインストールします。

Cmake(3.15以上)をインストールします。

### ファイルの準備

EffekseerとEffekseerForDXLibをgithubからcloneし同じディレクトリに配置します。

```
(Root)/Effekseer/
(Root)/EffekseerForDXLib/
```

となります。

DXライブラリの最新版をダウンロードして解凍し、EffekseerForDXLibの直下に配置します。

```
(Root)/Effekseer/
(Root)/EffekseerForDXLib/DxLib_VC/
```

となります。

### コンパイル

Effekseerをコンパイルします。

```
(Root)/Effekseer/release_dxlib.bat
```

を実行します。

```
(Root)/EffekseerForDXLib/copy_package.py
```

を実行します。


```
(Root)/EffekseerForDXLib/Dev/EffekseerForDXLib_vs2022.sln
```

で、ラッパーの開発ができるようになります。

## ユーザー向けパッケージの作成

上記の開発環境を整えた後、

```
(Root)/EffekseerForDXLib/release.bat
```

を実行します。