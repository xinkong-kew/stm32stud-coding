# STM32F103 工程模板

基于 **STM32F103C8**（Cortex-M3）与 **标准外设库（StdPeriph Driver v3.6.0）** 的 Keil MDK-ARM 工程模板，内置一套常用的外设驱动库，可作为新项目的起点快速开发。

## 主要特性

- 标准外设库工程框架，目录清晰，开箱即用
- 自带 `my_lib` 驱动库，封装常用外设：
  - **delay** — 毫秒 / 微秒级延时，系统时间戳
  - **usart** — 串口收发，支持 `printf` 格式化输出
  - **spi** — SPI 总线主从收发
  - **i2c** — 硬件 I2C 收发
  - **si2c** — 软件模拟 I2C 收发
  - **oled** — OLED 显示器驱动（含点 / 线 / 圆 / 矩形绘图、中英文显示、光标 / 画笔 / 画刷 / 裁剪区域）
  - **button** — 按键驱动（短按 / 连击 / 长按、回调）
- 附带 OLED 中文字库生成工具（`my_lib/font/` 下的 `ttf2c` / `bdf2c` / `otf2bdf`）

## 开发环境

| 项目 | 说明 |
| ---- | ---- |
| 芯片 | STM32F103C8（Cortex-M3，64KB Flash，20KB RAM） |
| 库 | 标准外设库 StdPeriph Driver v3.6.0 |
| IDE | Keil MDK-ARM（μVision5） |
| 启动文件 | `startup_stm32f10x_md.s`（中等容量） |

## 目录结构

```
模板/
├── user/                  # 用户代码（main.c、中断、系统配置）
├── my_lib/                # 自研驱动库
│   ├── delay.c/h          # 延时
│   ├── usart.c/h          # 串口
│   ├── spi.c/h            # SPI
│   ├── i2c.c/h            # 硬件 I2C
│   ├── si2c.c/h           # 软件 I2C
│   ├── oled.c/h           # OLED 显示
│   ├── button.c/h         # 按键
│   └── font/              # 字库及生成工具
├── std_periph_driver/     # 标准外设库（src / inc）
├── startup/               # 启动文件
├── template.uvprojx       # Keil 工程文件
└── 清理.cmd               # 清理编译产物脚本
```

## 快速开始

1. 用 Keil MDK-ARM 打开 `template.uvprojx`
2. 在 `user/main.c` 中编写业务逻辑
3. 编译并下载到 STM32F103C8 开发板

## 使用驱动库

以 OLED 为例：

```c
#include "stm32f10x.h"
#include "delay.h"
#include "si2c.h"
#include "oled.h"

int main(void)
{
    // 初始化软件 I2C（以 PB6=SCL、PB7=SDA 为例，按实际接线调整）
    // ...
    Delay_Init();
    // 初始化 OLED 并显示
    while (1)
    {
    }
}
```

具体接口请参考各模块头文件（`my_lib/*.h`）中的函数说明。

## 清理编译产物

运行根目录下的 `清理.cmd`，可删除 `DebugConfig`、`Listings`、`Objects` 以及 `.scvd`、`.uvguix.*` 等编译生成文件，使工程目录保持干净。

## 许可

驱动库源码来自 [铁头山羊](https://space.bilibili.com/)（见各文件头注释）。仅供学习交流使用。