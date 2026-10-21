# STM32 学习工程合集

基于 **STM32F103** + **标准外设库（StdPeriph Lib）** 的 Keil MDK 学习例程，按章节组织，每个目录都是一个可独立编译的 Keil 工程（`Project.uvprojx`）。

## 目录结构

| 目录 | 内容 |
| --- | --- |
| `2-1 STM32工程模板` | 空白工程模板（Start / Library / User 三层结构） |
| `3-1 LED闪烁` | GPIO 输出：点亮、闪烁 LED |
| `3-2 LED流水灯` | GPIO 输出：流水灯 |
| `3-3 蜂鸣器` | GPIO 输出：蜂鸣器发声 |
| `3-4 按键控制LED` | GPIO 输入：按键消抖、按键控制 LED |
| `3-5 光敏传感器控制蜂鸣器` | ADC + GPIO：光敏电阻采样，阈值控制蜂鸣器 |
| `4-1 OLED显示屏` | 软件 I2C 驱动 SSD1306 OLED，显示字符/数字/图形 |
| `5-1 对射式红外传感器计次` | 外部中断：红外对射计数 |
| `5-2 旋转编码器计次` | 外部中断：旋转编码器计数与方向判断 |

每个工程内部结构：

```
Project.uvprojx   Keil 工程文件
Start/            startup_stm32f10x_*.s、system_stm32f10x.c、stm32f10x.h
Library/          STM32F10x 标准外设库
User/             main.c、stm32f10x_it.c、stm32f10x_conf.h
Objects/          编译产物（已 .gitignore，首次编译后生成）
Listings/         编译列表文件（已 .gitignore）
```

## 使用方法

1. 安装 **Keil MDK 5** 及 `Keil.STM32F1xx_DFP` 器件支持包。
2. 用 Keil 打开任一工程目录下的 `Project.uvprojx`。
3. 编译（F7）并下载（F8）。生成的 `.hex` / `.axf` 位于 `Objects/`。

## 说明

- 编译产物（`Objects/`、`Listings/`、`*.o`、`*.axf`、`*.hex` 等）已通过 `.gitignore` 排除，克隆后重新编译即可生成。
- `5-2 旋转编码器计次/4-1 OLED显示屏/` 是早期复制进来的 OLED 工程副本，保留作为参考。
