/*********PARAMETER****************************************************/
const GW_HOST       = '192.168.11.100'; //GWのホスト名
//const GW_HOST     = 'localhost';
const WS_PORT       = 8000; //GWのWebSocketポート
const IDW_PVALUE    = 2.0;  //IDWのP値
const MAX_NODE_NUM  = 17;   //最大ノードID
const invaild_node  = [];   //無効とするノードIDの配列
const canvasWidth   = 900;  //描写するキャンバスの幅
const canvasHeight  = 450;  //描写するキャンバスの幅
const nodeCoordArray= [    //ノードの座標
{x:0.0 ,                y:0.0 ,                 z:0.0 }, //00
{x:0.0 ,                y:canvasHeight/2 -1 ,   z:0.0 }, //01
{x:0.0 ,                y:canvasHeight - 1 ,    z:0.0 }, //02
{x:canvasWidth/5 - 1,   y:0.0,                  z:0.0 }, //03
{x:canvasWidth/5 - 1,   y:canvasHeight/2 - 1 ,  z:0.0 }, //04
{x:canvasWidth/5 - 1,   y:canvasHeight - 1 ,    z:0.0 }, //05
{x:2*canvasWidth/5 - 1, y:0.0 ,                 z:0.0 }, //06
{x:2*canvasWidth/5 - 1, y:canvasHeight/2 -1 ,   z:0.0 }, //07
{x:2*canvasWidth/5 - 1, y:canvasHeight - 1 ,    z:0.0 }, //08
{x:3*canvasWidth/5 - 1, y:0.0 ,                 z:0.0 }, //09
{x:3*canvasWidth/5 - 1, y:canvasHeight/2 -1 ,   z:0.0 }, //10
{x:3*canvasWidth/5 - 1, y:canvasHeight - 1 ,    z:0.0 }, //11
{x:4*canvasWidth/5 - 1, y:0.0 ,                 z:0.0 }, //12
{x:4*canvasWidth/5 - 1, y:canvasHeight/2 -1 ,   z:0.0 }, //13
{x:4*canvasWidth/5 - 1, y:canvasHeight - 1 ,    z:0.0 }, //14
{x:canvasWidth - 1 ,    y:0.0 ,                 z:0.0 }, //15
{x:canvasWidth - 1 ,    y:canvasHeight/2 -1 ,   z:0.0 }, //16
{x:canvasWidth - 1 ,    y:canvasHeight - 1 ,    z:0.0 }  //17
];
/**********************************************************************/
var DRAW_LEVEL = 2.0;       //加速度の絶対値の最大値[g](可視化において最大とする値)
var inter_algorithm = "idw";//idw:逆距離加重法 or nn：最近傍法
var interpolation_interval = 100;//デフォルト補間間隔
var axis = "synthesized";	//描画する加速度の軸 "synthesized" or "x" or "y" or "z"
//var data_point_array = new Array(MAX_NODE_NUM);
var recvDataBuffer = new Array(); //受信データ格納バッファ
var runningWorkers = false; //Workerが処理中であるかどうかのフラグ

/*Worker関係変数*/
var worker = null;
var result_count = 0;//Workerからのメッセージカウント用変数(全てのWorkerからデータが得られたかを判断するために使用)
var worker_num = 1;  //使用Worker数(並列数)

/*WebGL関係変数*/
var vertexesColor; //WebGLの頂点色バッファ

/*フレームレート関係*/
var st,et,d,frame_count = 0,max = 100,fps = 0;

/*補間時間計測用*/
var inter_time_s = 0;
var inter_time_f = 0;

//データクラス
var DataPoint = function(x,y,z,value){
  this.x = x; 		//x座標
  this.y = y; 		//y座標
  this.z = z; 		//z座標
  this.value = value;	//センサ値
}

/***************************/
/*  初期設定関数　ここから     */
/***************************/
function initWorker(){
  /*worker配列を再構築*/
  worker     = null;
  worker     = [];
  var level  = DRAW_LEVEL/2.0;//可視化の最大値/2
  var height = canvasHeight/interpolation_interval;
  var width  = canvasWidth/interpolation_interval;
  var point_per_worker = ( height * width )/worker_num;

  for(var i=0;i<worker_num; i++){
    var xs = Math.floor(i*point_per_worker % width);
    var xf = Math.floor(((i+1)*point_per_worker-1) % width);
    var ys = Math.floor((i*point_per_worker)/width);
    var yf = Math.floor(((i+1)*point_per_worker-1)/width);

    worker[i] = new Worker("js/worker.js");
    worker[i].postMessage( {type:0, worker_id:i, P:IDW_PVALUE} );//worker_idを通知
    worker[i].postMessage( {type:1, interval:interpolation_interval, 
      size_x:width, size_y:height, size_z:1, 
        xs:xs, ys:ys, zs:0, xf:xf, yf:yf, zf:0} );//補間範囲指定
    worker[i].postMessage({type:2, draw_level:DRAW_LEVEL});

    worker[i].onmessage=function(event){
      var msg = event.data;
      var worker_id = msg.worker_id;//postmessage()を実行したWorkerのIDを取得
      var colors = msg.colors;//

      var xs = Math.floor(worker_id*point_per_worker % width);
      var ys = Math.floor((worker_id*point_per_worker)/width);
      var startPointIndex = (width*ys + xs)*4;//点(xs,ys)は左上から何番目の点か
      for(var i=0;i<colors.length;i++){
        vertexesColor[startPointIndex+i] = colors[i];
      }
      result_count++;
      if(result_count >= worker_num){
        inter_time_f = Date.now();
        
        result_count = 0;
        var draw_time_s = Date.now();
        draw(vertexesColor);
        var draw_time_f = Date.now();

        /*フレームレート計測処理 FROM*/
        
        frame_count ++;
        if(frame_count === 1){
          st = Date.now();
        }
        if(frame_count === max){
          et = Date.now();
          d = et - st;
          fps = frame_count / d * 1000;
          //ログに出力
          //console.log("Fps:" + fps);
          frame_count = 0;
        }
        /*フレームレート計測処理 END*/

        runningWorkers = false;//補間処理と描画処理が完了したのでfalseに
        
        if(recvDataBuffer.length > 0){
          //バッファにデータが存在する場合は取り出してWorkerに処理を依頼

          runningWorkers = true;//再び処理中に
          var data = recvDataBuffer.shift();//バッファからデータ取り出し
          //Workerに処理依頼
          inter_time_s = Date.now();
          for(var i=0; i<worker_num;i++){
            worker[i].postMessage({type:3, data:data});
          }
        }
      }
    }
  }
}

function initDrawMaxValue(){
  setDrawMaxValue(DRAW_LEVEL);
}

function initWorkerNum(){
  var value = document.getElementById("worker_num").value;
  setWorkerNum(value);
}

function initInterpolationInterval(){
  var value = document.getElementById("interval").value;
  setInterval(value);
}
/***************************/
/*  初期設定関数　ここまで     */
/***************************/
/******************************
DOM関連 ここから
*******************************/
function onLoad(){//HTMLがロードされると呼び出される
  initInterpolationInterval();//可視化における補間間隔の初期化
  initDrawMaxValue();//可視化における最大値の初期化
  initWorkerNum(); //Worker数の初期化
  drawSetUp();

  console.log("INFO:READY");
}

function setInterval(value){//補間間隔のプルダウンメニューが変更されると呼ばれる
  interpolation_interval = Number(value);

  vertexesColor = null;
  vertexesColor = new Float32Array(4* (canvasWidth/interpolation_interval) *(canvasHeight/interpolation_interval) );
  
  drawSetUp();
  initWorker();
}

function setAxis(value){//可視化する軸(x,y,z)のプルダウンメニューが変更されると呼ばれる
  axis = value;
  console.log("INFO:set axis:"+axis);
}

function setDrawMaxValue(value){//描画における最大値の設定rangeが変更されると呼ばれる
  //0以下が来たら０．１にする
  if(value <= 0){
    value = 0.1;
  }
  document.getElementById("draw_max_value_range").value = value;
  document.getElementById("draw_max_value").value=value;
  DRAW_LEVEL = value;

  //Workerに変更を通知
  if(worker != null){
   for(var i=0; i<worker_num;i++){
     worker[i].postMessage({type:2, draw_level:DRAW_LEVEL});
   }
 }

 console.log("INFO:set draw max value "+DRAW_LEVEL);
}

function setWorkerNum(value){
  worker_num = value;
  initWorker();

  console.log("INFO:change worker num:"+worker_num);
}
/******************************
DOM関連 ここまで
*******************************/

/******************************
WebSocket関連 ここから
*******************************/
function connectWS(){//「接続」ボタンが押されると呼び出される
  frame_count = 0;//フレームレートカウント変数初期化
  ws = new WebSocket('ws://'+GW_HOST+':'+WS_PORT+'/send');

  ws.onopen = onOpen;
  ws.onclose = onClose;
  ws.onmessage = onMessage;
}  

function disConnectWS(){//「切断」ボタンが押されると呼び出される
  ws.close(1000,"this_is_client_close_message");//ウェブソケット正常終了
}

function onOpen(event) {
  document.getElementById("connectWS_button").disabled = true;    //接続ボタンを無効化
  document.getElementById("disConnectWS_button").disabled = false;//切断ボタンを有効化
  document.getElementById("interval").disabled = true;		        //描画方法選択を無効化
  document.getElementById("worker_num").disabled = true;		      //描画方法選択を無効化

  data_point_array = new Array();
  for(var i=0; i<MAX_NODE_NUM; i++){//データ格納配列初期化
    if(invaild_node.indexOf(i, 0) >= 0){//無効化ノードに含まれている
      dp = new DataPoint(-1, -1, -1, 0);//左上
      continue;
    }else{
      dp = new DataPoint(nodeCoordArray[i].x, nodeCoordArray[i].y, 
        nodeCoordArray[i].z, 0);
    }
    data_point_array.push(dp);
  }

  console.log("INFO:WebSocket OPEN");
}

function onClose(event) {
  ws = null;
  document.getElementById("connectWS_button").disabled = false;//接続ボタンを有効化
  document.getElementById("disConnectWS_button").disabled = true;//切断ボタンを無効化
  document.getElementById("interval").disabled = false;		//描画方法選択を無効化
  document.getElementById("worker_num").disabled = false;		//描画方法選択を無効化

  console.log("INFO:WebSocket CLOSE");
}


function onMessage(event){

  time_total_s = Date.now();
  var data = JSON.parse(event.data);
  var data_array = data["data"];

  for(var i = 0, len = data_array.length; i<len; i++){
    if(invaild_node.indexOf(data_array[i].id, 0) >= 0){
      continue;
    }

    var dp;
    var value = 0;
    if(axis == "synthesized"){//3軸合成値
      value = parseFloat(Math.sqrt(Math.pow(data_array[i].x,2) + Math.pow(data_array[i].y,2) + Math.pow(data_array[i].z,2) ));
    }else if(axis == "x"){
      value = Math.abs(parseFloat(data_array[i].x));
    }else if(axis == "y"){
      value = Math.abs(parseFloat(data_array[i].y));
    }else if(axis == "z"){
      value = Math.abs(parseFloat(data_array[i].z));
    }
    data_point_array[data_array[i].id].value = value;
  }
  
  if(!runningWorkers && recvDataBuffer.length <= 0){
    //Workerが処理中でない かつ バッファにデータが存在しない場合はWorkerに補間処理を依頼
    runningWorkers = true; //処理中に変更
    inter_time_s = Date.now();
    for(var i=0; i<worker_num; i++){
      worker[i].postMessage({type:3, data:data_point_array});
    }
  }else{
    //そうでない場合はバッファにデータを格納
    recvDataBuffer.push(data_point_array.slice(0));//配列をコピーしてバッファに追加
    for(var i=0; i<data_point_array.length; i++){
      recvDataBuffer[recvDataBuffer.length-1][i] = $.extend(true, {}, data_point_array[i]);
    }
  }
}
/******************************
WebSocket関連 ここまで
*******************************/
