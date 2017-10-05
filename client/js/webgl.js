/*WebGL用変数 ここから*/
var gl = null;
var programs; 		//ユーザ定義シェーダプログラム
var index;  		//インデックスデータ配列
var width;　　	//横(X)方向の描画点数
var height;	//縦(Y)方向の描画点数
var xinterval;
var yinterval;
//var vertexBuffer; 	//頂点バッファ
//var indexBuffer;	//インデックスバッファ
//var vColorBuffer;	// 頂点色バッファ
//var vertexesData; 	//頂点データ配列
//var vertexesColor;	//頂点色データ配列
//var vertexSource;	//頂点シェーダプログラム
//var fragmentSource;	//フラグメントシェーダプログラム
//var attLocation//
//var attLocColor;//
/*WebGL用変数　ここまで*/

 /**************************************************
 *function draw:描画を行う
 * 引数：
 *   vertexesColor[Float32Array]:補間済み色データ
 * 返り値：なし
 ****************************************************/
 function draw(vertexesColor){
 	var vColorBuffer = gl.createBuffer();
 	gl.bindBuffer(gl.ARRAY_BUFFER, vColorBuffer);
 	gl.bufferData(gl.ARRAY_BUFFER, vertexesColor, gl.STATIC_DRAW);
 	attLocColor = gl.getAttribLocation(programs, 'color');
 	gl.enableVertexAttribArray(attLocColor);
 	gl.vertexAttribPointer(attLocColor, 4, gl.FLOAT, false, 0, 0);

	// 描画
	//gl.drawElements(gl.TRIANGLES, index.length, gl.UNSIGNED_SHORT, 0)
	gl.drawArrays(	gl.POINTS, 0, vertexesColor.length / 4);
	//console.log(vertexesColor.length);
	//gl.flush();
}
 /*function draw(data){
 	var level = DRAW_LEVEL/2.0;
 	var num_of_data = data.length;
	//頂点データからバッファを生成し頂点色を登録

	//var vertexesColor = genVertexwsColor(width, height);
	//頂点色バッファ(4(RGBA) * 4(1つの正方形の頂点数) * num_of_data)
	//dataは座標(x,y)が(0,0),(0,1),(0,2),...,(1,0),(1,1),(1,2),...の順に並んでいる必要がある
	var vertexesColor = new Float32Array(4 * num_of_data);
	for(var i=0, c=0; i<num_of_data; i++){
		//console.log(data[i].y+","+data[i].x+","+data[i].value);
		var value = Math.abs(data[i].value);
		var R = Math.min(level, value) / level;
		var G = 0.0;
		var B = 1.0-(Math.max(0, value-level) / level); 
		var A = 1.0;
		//for(var j=0; j<4; j++){
			vertexesColor[c++] = R;
			vertexesColor[c++] = G;
			vertexesColor[c++] = B;
			vertexesColor[c++] = A;
		//}
	}

	var vColorBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vColorBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, vertexesColor, gl.STATIC_DRAW);
	attLocColor = gl.getAttribLocation(programs, 'color');
	gl.enableVertexAttribArray(attLocColor);
	gl.vertexAttribPointer(attLocColor, 4, gl.FLOAT, false, 0, 0);

	// 描画
	//gl.drawElements(gl.TRIANGLES, index.length, gl.UNSIGNED_SHORT, 0)
	gl.drawArrays(	gl.POINTS, 0, vertexesColor.length / 4);
	//console.log(vertexesColor.length);
	gl.flush();
}*/

 /**************************************************
 *function drawSetUp:描画(WebGL)の設定を行う
 * 引数：なし
 * 返り値：なし
 ****************************************************/
 function drawSetUp(){
 	/*WebGL 初期設定*/
 	accel_canvas = document.getElementById('accel_canvas');
 	accel_canvas.width = canvasWidth;
 	accel_canvas.height = canvasHeight;

 	// CanvasエレメントからWebGLコンテキストを取得する
 	gl = accel_canvas.getContext('webgl');
	// WebGLコンテキストが取得できたかどうか調べる
	if(!gl){
		alert('webgl not supported!');
		return;
	}

	drawSetUpCore();
}

 /**************************************************
 *function drawSetUpCore:描画(WebGL)の主な設定を行う
 * 引数：なし
 * 返り値：なし
 ****************************************************/
 function drawSetUpCore(){
 	xinterval =interpolation_interval;
 	yinterval = interpolation_interval;
 	width = canvasWidth/xinterval;　　	//横(X)方向の描画点数
	height = canvasHeight/yinterval;	//縦(Y)方向の描画点数
 	// Canvasエレメントをクリアする色を指定する
 	gl.clearColor(0.0, 0.0, 0.0, 1.0);

	// Canvasエレメントをクリアする
	gl.clear(gl.COLOR_BUFFER_BIT);

	// シェーダとプログラムオブジェクト
	var vertexSource = document.getElementById('vs').textContent;
	var fragmentSource = document.getElementById('fs').textContent;

	// ユーザー定義のプログラムオブジェクト生成関数
	programs = shaderProgram(vertexSource, fragmentSource);

	// 四角形を形成する頂点のデータを受け取る
	var vertexesData =  genVertexes(width, height);
	
	// 頂点データからバッファを生成
	var vertexBuffer = gl.createBuffer(); 
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexesData), gl.STATIC_DRAW);
	// プログラムオブジェクトに頂点データを登録
	var attLocation = gl.getAttribLocation(programs, 'position');
	gl.enableVertexAttribArray(attLocation);
	gl.vertexAttribPointer(attLocation, 3, gl.FLOAT, false, 0, 0);

	//インデックスバッファを生成
	//ポリゴン描画から頂点の描画に変更したため、インデックスバッファは無効化
	/*
	index = getIndexDataArray(width, height);
	var indexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Int16Array(index), gl.STATIC_DRAW);
	*/

	//PointSize(描画点のサイズ)を指定
	var vPointSizeData =[canvasWidth/xinterval];
	var vPointSizeBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vPointSizeBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vPointSizeData), gl.STATIC_DRAW);
	uniLocPointSize = gl.getUniformLocation(programs, 'pointSize');
	gl.uniform1f(uniLocPointSize, xinterval);
}

// プログラムオブジェクト生成関数
function shaderProgram(vertexSource, fragmentSource){
	// シェーダオブジェクトの生成
	var vertexShader = gl.createShader(gl.VERTEX_SHADER);
	var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);

	// シェーダにソースを割り当ててコンパイル
	gl.shaderSource(vertexShader, vertexSource);
	gl.compileShader(vertexShader);
	// シェーダのコンパイル結果をチェック
	if(!gl.getShaderParameter(vertexShader, gl.COMPILE_STATUS)){
		alert(gl.getShaderInfoLog(vertexShader));
		return null;
	}

	// シェーダにソースを割り当ててコンパイル
	gl.shaderSource(fragmentShader, fragmentSource);
	gl.compileShader(fragmentShader);
	// シェーダのコンパイル結果をチェック
	if(!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)){
		alert(gl.getShaderInfoLog(fragmentShader));
		return null;
	}

	var program = gl.createProgram();
	gl.attachShader(program, vertexShader);
	gl.attachShader(program, fragmentShader);

	// シェーダをリンク
	gl.linkProgram(program);

	// シェーダのリンク結果をチェック
	if(!gl.getProgramParameter(program, gl.LINK_STATUS)){
		alert(gl.getProgramInfoLog(program));
		return null;
	}

	// プログラムオブジェクトを選択状態にする
	gl.useProgram(program);

	// 生成したプログラムオブジェクトを戻り値として返す
	return program;
}

function genVertexes(width, height){//頂点の生成
	var vertexes = [];
	var num_of_vertexes = (width)*(height); //頂点の数
	for(var j=0; j<height; j++){
		for(var i=0; i<width; i++){
			//x,yにて「+1 / width」,「-1 / heigit」をすることによって、マスの中心の座標を算出
			var x = ((  i  / width*2)-1) + 1/width; 
			var y = 1-( j   /height*2) - 1/ height;
			var z = 0.0;
			vertexes.push(x ,y, z); 
		}
	}
	return vertexes;
}

function getIndexDataArray(width, height){//width:X方向の補間点数 height:Y方向の補間点数
	var array = [];
	var num_of_points = width*height; //合計の補間点数
	for(var i=0; i<num_of_points; i++){
		array.push(i*4, i*4+1, i*4+2);
		array.push(i*4+1, i*4+2, i*4+3);
	}
	return array;
}

/*色の算出はdraw()内で行うため、無効化*/
/*
function genVertexwsColor(width, height){//サンプル
	var vertexwsColors = [];
	var num_of_vertexes = (width+1)*(height+1); //頂点の数
	var width2 = width/canvasWidth*2;    //X軸方向の増分
 	var height2 = height/canvasHeight*2; //Y軸方向の増分
 	for(var j=0; j<height; j++){
 		for(var i=0; i<width; i++){
 			var R = Math.random() * 100*0.001*i;
 			vertexwsColors.push(R, 0, 1, 1.0);//左上
 			vertexwsColors.push(R, 0, 1, 1.0);//右上
 			vertexwsColors.push(R, 0, 1, 1.0);//左下
 			vertexwsColors.push(R, 0, 1, 1.0);//右下
 		}
 	}
 	return vertexwsColors;
 }
 */
