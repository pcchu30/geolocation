# geolocation
kalman filter

## Basic 任務
在 main() 中執行 Unit Test，testPositionTable(); 其中包含所有 Basic 任務。  
請查看 "PositionTable.cpp" and "PositionTable.h"。

## Advance 任務

### 任務一 (用 Kalman Filter 濾除雜訊與調整位置)
每一個時間單位，抓9個點，每個點各含 x 和 y 座標，所以共有18個 measurements，類似 sensor fusion  
route_user_1 result
![alt text](https://github.com/pcchu30/static/blob/master/images/geolocation/advanced%20_mission_I/advanced_I_user_1.png?raw=true)
route_user_1 result 和 原始數據
![alt text](https://github.com/pcchu30/static/blob/master/images/geolocation/advanced%20_mission_I/advanced_I_user_1_zoom_in.png?raw=true)
route_user_2 result
![alt text](https://github.com/pcchu30/static/blob/master/images/geolocation/advanced%20_mission_I/advanced_I_user_2.png?raw=true)
route_user_2 result 和 原始數據
![alt text](https://github.com/pcchu30/static/blob/master/images/geolocation/advanced%20_mission_I/advanced_I_user_2_zoom_in.png?raw=true)

### 任務二 (用資料庫數據調整位置)
用 for loop，每次抓資料庫數據的一個點，然後從原始數據中找離最近的一個點(用distance函數)，存起來後連成新的linestring。  
route_user_1 result 紅色線是調整過後的結果，藍色線是資料庫數據。
![alt text](https://github.com/pcchu30/static/blob/master/images/geolocation/advanced_mission_II/advance_II_user_1.png?raw=true)
route_user_2 result 紫色線是調整過後的結果，藍色線是資料庫數據。
![alt text](https://github.com/pcchu30/static/blob/master/images/geolocation/advanced_mission_II/advance_II_user_2.png?raw=true)


