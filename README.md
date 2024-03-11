
### 文件说明

#### 文件夹

`src/sketch` 存放sketch算法代码
`src/util` 存放数据集读取、哈希模板等代码

#### 文件

`src/sketch/newsketch/param.cpp(.h)` 算法基本参数，见文件内注释

`runtest/runtest.cpp` 批量使用命令行命令运行实验

`src/util/read_dataset.cpp(.h)` 读取数据集，可以在文件中指定特定数据集

`src/chk_cache.cpp` 通过运行 `run_newsketch` 得到的缓存日志，计算缓存效率

`src/chk_l*u.cpp` 测试lru/lfu缓存效率

`run_correct.cpp` 运行unordered_map算法，得到正确的burst列表并输出到 `tmp/correct.txt`

`read_correct.cpp(.h)` 方便其他算法读取正确的burst列表

`run_newsketch.cpp` 运行新sketch算法，输出缓存日志，将求得的burst与 `tmp/correct.txt` 进行比较，计算准确率

`run_strawman.cpp` 运行对比算法

`run_***.cpp` 运行其他对比算法，将求得的burst与 `tmp/correct.txt` 进行比较，计算准确率


`run_temp.cpp` 运行水温测试算法，其中算法在该cpp文件中实现

`run_timewidth_test.cpp` 运行包之间时间跨度检测算法

`run_jitter_test.cpp` 运行jitter检测算法，其中算法在该cpp文件中实现

命令行及参数说明位于文件中
