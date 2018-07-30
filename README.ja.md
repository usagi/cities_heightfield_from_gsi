# Cities Heightfield from GSI

[en](README.md) / ja

## このアプリは何か

- [CITIES : skylines](http://store.steampowered.com/app/255710) 向けの標高データを生成するアプリです。
- 機能
    - GSI の標高タイル群を元に生成します。 ( GSI: 国土地理院; [ja](http://www.gsi.go.jp/) [en](http://www.gsi.go.jp/ENGLISH/) )

## 使い方

`cui` 実行:

```
使い方 (1) 対話実行モード: cui
使い方 (2) 直接実行モード: cui [options]

Options:
        --longitude 141.315714
                        目標の経度を設定
        --latitude 43.04724
                        目標の緯度を設定
    -o, --out out.png   出力ファイルのパスを設定
    -h, --help          このヘルプを表示
    -v, --version       このアプリのバージョン情報を表示
    -s, --silent        このアプリを沈黙動作にします
```

例; (1) の使い方

```
cui
```

実行したら、対話的にパラメーターを入力します。

例; (2) の使い方

```
cui --longitude 141.315714 --latitude 43.04724 --out Maruyama_Sapporo_Hokkaido_Japan.png
```

`gui` 実行:

（ 残念ながら、こちらはまだ開発中です。今のところは、 `cui` を使って下さい。）

## 出力例

### 例 日本北海道札幌円山; 43.04724, 141.315714

```
cui --longitude 141.315714 --latitude 43.04724 --out Maruyama_Sapporo_Hokkaido_Japan.png
```

![Maruyama, Sapporo, Hokkaido, Japan](image/Maruyama_Sapporo_Hokkaido_Japan.png)

### 例 日本岩手久慈; 

```
cui --longitude 141.315714 --latitude 43.04724 --out Maruyama_Sapporo_Hokkaido_Japan.png
```

![Kuji, Iwate, Japan](image/Kuji_Iwate_Japan.png)

### ビルド済みのバイナリーのダウンロード （ Windows だけ ）

- https://github.com/usagi/cities_heightfield_from_gsi/releases

## ライセンス

### このアプリ

- [MIT](LICENCE.md)

### 生成されたデータ

- [GSI Licencing Agreements](http://www.gsi.go.jp/LAW/2930-index.html) (ja)
    - [Copyright Notices](http://www.gsi.go.jp/LAW/2930-meizi.html) (ja)

## 作者

伊藤 兎<usagi@WonderRabbitProject.net>
北海道札幌産。

## 参考

- http://www.skylineswiki.com/Map_Editor
- https://steamcommunity.com/app/255710/discussions/0/611702631244016398/?l=japanese
- https://www.reddit.com/r/CitiesSkylinesModding/comments/31l4xu/importing_model_with_maya_2014/
- http://www.linz.govt.nz/data/geodetic-system/datums-projections-heights/geodetic-datums/world-geodetic-system-1984-wgs84
- http://wiki.openstreetmap.org/wiki/Zoom_levels
