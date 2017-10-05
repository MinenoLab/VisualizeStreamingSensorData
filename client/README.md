## Client program

### 1.動作環境
* Webブラウザ（推奨：Google Chorme バージョン 50.0.2661.94 m）

### 2.設定
* [js/client.js](js/client.js)上部のパラメータを用途に合わせて設定してください
ゲートウェイとの通信関係
| 設定項目 | 内容 |
|--------|--------|
|	     GW_HOST		|			ゲートウェイのIPアドレス	|
|	     WS_PORT		|			ゲートウェイとのWebSocket通信に用いるポート番号	|
可視化関係
| 設定項目 | 内容 |
|--------|--------|
|	     IDW_PVALUE		|			逆距離加重法（IDW)による空間補間の際に用いるPの値	|
|	     MAX_NODE_NUM 		|			接続されるセンサノードの最大ID	|
|	     invaild_node　		|	無効とするセンサノードのID			|
|	     canvasWidth		|			描画するcanvasの幅	|
|	     canvasHeight		|			描画するcanvasの高さ	|
|	     nodeCoordArray		|	canvas上の各ノードの座標			|
|	     DRAW_LEVEL		|			加速度の絶対値の最大値[g](可視化において最大とする値)	|


* [client.html](client.html)内の「補間サイズ」を用途に合わせて設定してください
	* 以下の部分のoptionタグのvalueにヒートマップの各点のサイズを指定
	```client.html
    ...
    <select id="interval" onchange="setInterval(this.value)">
    <option value="3.125">144x288</option>  
    <option value="6.25">72×144</option>  
    <option value="12.5">36×72</option>  
    <option value="25.0">18x36</option>
    <option value="50.0" selected="true">9×18</option>
    ...
    ```

### 3.実行方法
* Webブラウザにて[client.html](client.html)を開き「接続」ボタンを押す
