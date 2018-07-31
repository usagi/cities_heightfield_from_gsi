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

### 例 日本・北海道・札幌・円山

```
cities_heightfield_from_gsi --longitude 141.315714 --latitude 43.04724 --out Maruyama_Sapporo_Hokkaido_Japan.png
```

![Maruyama, Sapporo, Hokkaido, Japan](image/Maruyama_Sapporo_Hokkaido_Japan.png)

### 例 日本・岩手・久慈

```
cities_heightfield_from_gsi --longitude 141.775583 --latitude 40.190524 --out Kuji_Iwate_Japan.png
```

![Kuji, Iwate, Japan](image/Kuji_Iwate_Japan.png)

#### 例 日本・北海道・礼文

```
cities_heightfield_from_gsi --latitude 45.30302 --longitude 141.047689 --out Rebun_Hokkaido_Japan.png
```

![Rebun, Hokkaido, Japan](image/Rebun_Hokkaido_Japan.png)

#### 例 日本・北海道・弟子屈・摩周湖

```
cities_heightfield_from_gsi --latitude 43.580411 --longitude 144.533489 --out Mashuuko_Teshikaga_Hokkaido_Japan.png
```

![Mashuuko, Teshikaga, Hokkaido, Japan](image/Mashuuko_Teshikaga_Hokkaido_Japan.png)

#### 例 日本・青森・陸奥

```
citis_heightfield_from_gsi --latitude 41.292746 --longitude 141.183476 --out Mutsu_Aomori_Japan.png
```

![Mutsu, Aomori, Japan](image/Mutsu_Aomori_Japan.png)

#### 例 日本・長崎・五島列島・若松

```
citis_heightfield_from_gsi --latitude 32.89027 --longitude 129.013749 --out Wakamatsu_Goto_Nagasaki_Japan.png
```

![Wakamatsu, Goto, Nagasaki, Japan](image/Wakamatsu_Goto_Nagasaki_Japan.png)

### 例 日本・沖縄・石垣

```
citis_heightfield_from_gsi --latitude 24.340661 --longitude 124.15558 --out Ishigaki_Okinawa_Japan.png
```
![Ishigaki, Okinawa, Japan](image/Ishigaki_Okinawa_Japan.png)

## 実行可能なバイナリーのダウンロード （ Windows だけ ）

- https://github.com/usagi/cities_heightfield_from_gsi/releases

### ソースからのビルド方法

```
cargo build --release
```

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
