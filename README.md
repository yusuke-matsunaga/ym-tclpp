# ymtclpp

## [***注意***] git でリポジトリを取得する場合の注意事項

ymtclpp は内部で ym-common という別のリポジトリを submodule として使っています．
ymtclpp をただ git clone で持ってきた場合には ym-common が空のディレクトリになっていますので

```shell
$ git submodule init
$ git submodule update
```

で ym-common の中身を持ってくるようにしてください．

## はじめに

YmTclpp は Tcl の C++ ラッパクラスライブラリです．
ビルドに関しては BUILDING ファイルを参照してください．
