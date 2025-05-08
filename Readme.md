### 方法实现
* search(key)        查找
* insert(key, value) 插入
* erase(key)         删除

### 性能测试  
* 单线程 随机插入、查找、删除一百万次，重复10次  
insert times: 1532.88ms,   search times: 20.47ms,   erase times: 1003.77ms  
insert times: 1547.61ms,   search times: 23.15ms,   erase times: 1149.98ms  
insert times: 1441.78ms,   search times: 20.24ms,   erase times: 1074.10ms  
insert times: 1445.53ms,   search times: 23.18ms,   erase times: 1102.30ms  
insert times: 1374.85ms,   search times: 22.01ms,   erase times: 1097.79ms  
insert times: 1418.60ms,   search times: 23.92ms,   erase times: 1157.77ms  
insert times: 1372.62ms,   search times: 23.23ms,   erase times: 1166.69ms  
insert times: 1383.59ms,   search times: 22.13ms,   erase times: 1190.66ms  
insert times: 1372.67ms,   search times: 21.04ms,   erase times: 1223.31ms  
insert times: 1392.21ms,   search times: 20.32ms,   erase times: 1189.60ms  

* 多线程 随机插入、查找、删除共一百万次，每个线程均分次数，重复10次    
insert times: 1618.42ms,   search times: 29.67ms,   erase times: 1258.98ms  
insert times: 2276.39ms,   search times: 31.64ms,   erase times: 1790.36ms  
insert times: 2588.23ms,   search times: 51.29ms,   erase times: 1842.62ms  
insert times: 2198.05ms,   search times: 28.09ms,   erase times: 1675.02ms  
insert times: 2497.69ms,   search times: 30.63ms,   erase times: 1914.16ms  
insert times: 2051.39ms,   search times: 32.51ms,   erase times: 1483.85ms  
insert times: 2045.77ms,   search times: 30.93ms,   erase times: 1805.13ms  
insert times: 2782.43ms,   search times: 27.64ms,   erase times: 1820.55ms  
insert times: 2392.06ms,   search times: 30.29ms,   erase times: 1893.91ms  
insert times: 2373.60ms,   search times: 27.97ms,   erase times: 1697.62ms  