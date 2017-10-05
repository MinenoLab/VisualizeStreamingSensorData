var DRAW_LEVEL = 50.00;//加速度の絶対値の最大値[m/s^2](可視化において最大とする値)
var size_x = 18; //可視化点数(X軸)
var size_y = 9;  //可視化点数(Y軸)
var size_z = 1;   //可視化点数(Z軸)    
var interval = 100;//補間間隔
var PVALUE = 2.0;

var xs,ys,zs;//補間開始点
var xf,yf,zf;//補間終了点
var worker_id = 0; //Worker識別ID

onmessage = function(event){
	var worker_starttime = Date.now();
	var msg = event.data;

	switch(msg.type){
		case 0://初期設定(WorkerIDの設定)
		worker_id = msg.worker_id;
		PVALUE = msg.P
		break;

		case 1://補間間隔と補間範囲の設定
		interval = msg.interval;
		size_x = msg.size_x;
		size_y = msg.size_y;
		size_z = msg.size_z;
		xs = msg.xs;
		xf = msg.xf;
		ys = msg.ys;
		yf = msg.yf;
		zs = msg.zs;
		zf = msg.zf;
		break;

		case 2:
		DRAW_LEVEL = msg.draw_level;
		break;
		
		case 3:
		var datas = msg.data;
		var colors = IDW(datas,  xs, ys, zs, xf, yf, zf, interval, PVALUE, DRAW_LEVEL);
		var post_data = {
			worker_id : worker_id,
			colors : colors
			,workertime: (Date.now()-worker_starttime)
		};
		postMessage(post_data, [post_data.colors.buffer]);
		break;
	}
}

/**************************************************
 *function DoIDW:逆距離加重法（IDW)を行う
 * 引数：
 *   datas[Array(DataPoint)]:入力データ（センサデータ）の配列
 *   *s[float]：補間開始点
 *   *f[float]：補間終了点
 *   interval[float]：補間間隔
 *   PVALUE[float]：P値
 * 返り値：
 *   result[Float32Array]：補間結果の色データ配列
 ****************************************************/
 function IDW(datas, xs, ys, zs, xf, yf, zf, interval, PVALUE, level){
	level /= 2.0;//可視化の最大値/2
	var draw_point_num = (zf - zs)*size_x*size_y + (yf - ys)*size_x + (xf - xs) + 1; //可視化点数
	var colors = new Float32Array( 4 * draw_point_num );//描画色(R,G,B,A)を格納する配列
	var x = xs; //x座標(可視化点数における座標)
	var y = ys; //y座標(可視化点数における座標)
	var z = zs; //z座標(可視化点数における座標)
	var x_coordinates = xs*interval; //x座標(実座標:ピクセル単位)
	var y_coordinates = ys*interval; //y座標(実座標:ピクセル単位)
	var z_coordinates = zs*interval; //z座標(実座標:ピクセル単位)

	for(var p_count=0, c=0; p_count < draw_point_num; p_count++){
		var dist;
		var InterValue = 0.0;
		var wValue = 0.0;
		var interrupt = false;

		//座標が一致する計測点が存在するか探索しつつ、補間値算出
		for(var i = 0, len = datas.length; i<len; i++){
			x_coordinates = x*interval;
			y_coordinates = y*interval;
			z_coordinates = z*interval;

			//座標が負のデータは無視
			if( datas[i].x < 0 || datas[i].y < 0 || datas[i].z < 0 ){
				continue;
			} 

			//座標が一致する計測点が存在
			if( datas[i].x == x_coordinates && datas[i].y == y_coordinates && datas[i].z == z_coordinates ){
				InterValue = datas[i].value;
				interrupt = true;	
				break;
			}

			//距離(dist)算出
			dist = Math.sqrt((datas[i].x - x_coordinates) * (datas[i].x - x_coordinates) + 
				(datas[i].y - y_coordinates) * (datas[i].y - y_coordinates) + 
				(datas[i].z - z_coordinates) * (datas[i].z - z_coordinates) );

			InterValue += datas[i].value / Math.pow(dist,PVALUE);
			wValue += 1.0 / Math.pow(dist,PVALUE); 
		} 

		if(wValue == 0 || interrupt){
 			//計測点データを用いる
 		}else{
 			//補間値を算出
 			InterValue /= wValue; 					
 		}

 		//描画色(R,G,B,A)を算出
 		var value = Math.abs(InterValue);
 		var R = Math.min(level, value) / level;
 		var G = 0.0;
 		var B = 1.0-(Math.max(0, value-level) / level); 
 		var A = 1.0;
 		colors[c++] = R;
 		colors[c++] = G;
 		colors[c++] = B;
 		colors[c++] = A;	
 		/*次の点ヘ*/
 		x = (x+1)%size_x;
 		if(x == 0){
 			y = (y+1)%size_x;
 			if(y == 0){
 				z = (z+1)%size_z;
 			}
 		}
 	}
 	return colors;
 }
