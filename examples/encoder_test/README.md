# esp32 encoder test

  使用 ESP-IDF 开发框架，编写编码器驱动测试示例。



## Encder GPIO define

- ENCODER_PIN_A 34
- ENCODER_PIN_B 35



## 编译运行

1. 配置好 ESP-IDF 运行环境；
2. 进入到 encoder_test 目录中；
3. 执行 `idf.py build` 构建项目；
4. 执行 `idf.py -p com flash` 进行烧写；
5. 执行 `idf.py -p com monitor` 监控串口输出。





