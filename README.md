# STM32F103 学习项目

基于 **STM32F103C8T6**（Cortex-M3）与 **标准外设库（StdPeriph Driver v3.6.0）** 的嵌入式学习项目集合，使用 Keil MDK-ARM（μVision5）开发。

## 项目列表

| 编号 | 项目名称 | 说明 |
|------|----------|------|
| 2-1 | STM32工程模板 | 基础工程模板，包含标准外设库和自研驱动库，可作为新项目起点 |
| 2-2 | LED闪烁 | GPIO 输出控制 LED 闪烁，入门级实验 |
| 2-3 | 按钮实验 | GPIO 输入检测按键状态，含短按/连击/长按识别 |
| 4-1 | I2C模块 | STM32 硬件 I2C 外设的使用 |
| 4-2 | I2C软接口 | 软件模拟 I2C 时序，驱动 OLED 显示器 |

## 开发环境

| 项目 | 说明 |
|------|------|
| 芯片 | STM32F103C8T6（Cortex-M3，64KB Flash，20KB RAM） |
| 库 | 标准外设库 StdPeriph Driver v3.6.0 |
| IDE | Keil MDK-ARM（μVision5） |
| 调试器 | ST-Link / J-Link |

## 自研驱动库（my_lib）

项目模板中内置了一套常用外设驱动库：

| 模块 | 文件 | 功能 |
|------|------|------|
| delay | `delay.c/h` | 毫秒/微秒级延时、系统时间戳 |
| usart | `usart.c/h` | 串口收发，支持 `printf` 格式化输出 |
| spi | `spi.c/h` | SPI 总线主从收发 |
| i2c | `i2c.c/h` | 硬件 I2C 收发 |
| si2c | `si2c.c/h` | 软件模拟 I2C 收发 |
| oled | `oled.c/h` | OLED 显示器驱动（点/线/圆/矩形绘图、中英文显示） |
| button | `button.c/h` | 按键驱动（短按/连击/长按、回调） |

附带 OLED 中文字库生成工具（`my_lib/font/` 下的 `ttf2c` / `bdf2c` / `otf2bdf`）。

## 快速开始

1. 用 Keil MDK-ARM 打开对应项目目录下的 `.uvprojx` 工程文件
2. 编译（F7）并下载（F8）到开发板
3. 观察运行效果

## 清理编译产物

各项目目录下均包含 `清理.cmd` 脚本，运行后可删除 `DebugConfig`、`Listings`、`Objects` 等编译生成文件。

## 许可

驱动库源码来自 [铁头山羊](https://space.bilibili.com/)，仅供学习交流使用。