## マルチストリーミングセンサデータ向けリアルタイム空間補間可視化システム

### システム概要
加速度データ等の高レートに生成されるストリーミングセンサデータを、リアルタイムに空間補間して可視化可能とするシステム[1]のソフトウェア

### システム構成
* センサノード、センサゲートウェイ、クライアントの３部から構成
* 詳細は[システム紹介PDF](./system_develop.pdf)を参照


### システムソフトウェア
#### [gateway](./gateway)
* センサゲートウェイソフトウェア
* センサノードからのデータ受信、集約、クライアントへの送信を行う
	* クライアントへの送信にはcwebsocketを利用しています  
	cwebsocket is developed by [m8rge](https://github.com/m8rge/cwebsocket), Copyright (c) 2015 Andrew Putilov

#### [sensor node](./sensor_node)
* センサノードソフトウェア
* センサノード（RaspberryPi2)において加速度センサのサンプリングとゲートウェイへの送信を行う

#### [client](./client)
* クライアントソフトウェア
* ブラウザ上でセンサゲートウェイからのデータ受信と空間補間可視化をリアルタイムに行う


### 免責事項
* 本ソフトウェアにいかなる欠陥もないという無制限の保証を行うものではありません。
* 本ソフトウェアに関する不具合修正や質問についてのお問い合わせもお受けできない場合があります。


### 文献
[1] [若森和昌，丸島晃明，峰野博史：マルチストリーミングセンサデータ向けリアルタイム空間補間可視化システム, 情報処理学会論文誌コンシューマ・デバイス＆システム（CDS）, Vol.7, No.2, pp.76-86(2017.5).](https://ipsj.ixsq.nii.ac.jp/ej/index.php?active_action=repository_view_main_item_detail&page_id=13&block_id=8&item_id=181100&item_no=1)
