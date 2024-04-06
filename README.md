# わんこ大喧嘩

これは C言語 による ターミナルゲーム の土台です。

## 使い方

[ macos ]
```
% gcc wanco.c
% ./a.out
```

## キー操作

- タブキー: わんこ選択
- スペースキー: わんこ生産
- ESCキー: 終了

## ファイル別解説

### わんこ大喧嘩

- [wanco.c](wanco.c) わんこ大喧嘩メイン
- [wanco.h](wanco.h) わんこ大喧嘩固有の関数群
- [wanco_res.h](wanco_res.h) わんこ大喧嘩用リソース

### ライブラリ

- [lib/Button.h](lib/Button.h) ボタン
- [lib/ButtonManager.h](lib/ButtonManager.h) ボタン管理
- [lib/Image.h](lib/Image.h) 画像
- [lib/Sprite.h](lib/Sprite.h) スプライト
- [lib/Screen.h](lib/Screen.h) 画面
- [lib/Keyboard.h](lib/Keyboard.h) キーボード
- [lib/Mover.h](lib/Mover.h) スプライトの移動
- [lib/MoverManager.h](lib/MoverManager.h) スプライトの移動管理

## にゃんこかわくだり

```sh
gcc kawakudari.c
./a.out
```

## AtoZはやうちゲーム

```sh
gcc a2z.c
./a.out
```

## キーコードチェックツール

```sh
gcc keycheck.c
./a.out
```
