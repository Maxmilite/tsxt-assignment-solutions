# RaspberryPi-Vehicle

此项目为「雅俗共赏中老年车」解决方案。项目允许在不连接电脑的情况下仅使用手机 APP 和遥控器便可开启 RGB 灯光、播放动人单曲等，开机即用。

此项目包含无线遥控、黑线循迹、超声波避障、音乐播放等功能的实现，于树莓派 4B 下开发。树莓派中代码均使用 C 语言实现，手机 APP 使用 Java 语言开发，手机 APP 与树莓派通信使用 Socket 完成。

此工程需要在用户名为 `maxmilite` 的树莓派 4B 中运行。如果需要适配不同用户，请自行将所有文件中的 `/home/maxmilite` 替换为 `/home/{{ new_username }}` 并测试稳定性。 **所有程序需要放置在 `/home/maxmilite/app/` 目录下方可运行**。

**演示视频地址**：<https://www.bilibili.com/video/BV1CL41187M4/>

### 硬件清单

下表为小车使用硬件型号清单及接口连接信息。特别的，对 GPIO 接口，以下编号均为 BCM 格式。

|        硬件名称及型号        |                         接口连接信息                         |
| :--------------------------: | :----------------------------------------------------------: |
|         L298N 驱动板         | `ENA: GPIO 16; ENB: GPIO 17; IN1 ~ IN4: GPIO 18, 19, 20, 21` |
| TCRT5000L 五路循迹红外传感器 |             `OUT1 ~ OUT5: GPIO 4, 5, 6, 12, 13`              |
|     HC-SR04 超声波传感器     |                `TRIG: GPIO 22; ECHO: GPIO 23`                |
|        OV5647 摄像头         |                            `CSI`                             |
|             音响             |                          3.5mm 接口                          |

### 目录结构说明

`rv-raspi-code/app`：运行在树莓派中的代码目录。

`rv-android-app`：安卓 APP 工程目录。

以下内容均为 `rv-raspi-code/app` 中内容。

`main.c`：主控程序代码，负责代理控制所有的功能、向 L298N 发送指令、与 Socket 服务端通信等。

`socket.c`：Socket 服务端程序代码，负责网络通信：将 APP 发送的指令转发给主控程序，查询状态并回传等。

`line_tracing.c`：黑线循迹程序代码。

`ultra_sonic.c`：超声波避障程序代码。

`logger.h`：Logger 头文件。

`music_player.h` 音乐播放头文件。

`music/`：音乐存放目录。

`main.py`：[WIP] 颜色识别代码。

 `compile.sh`：脚本，编译所有代码。

`init.sh`：初始化脚本，可以直接在树莓派中执行以完成几乎所有功能的配置。

### 配置方法

本项目在截至 2023 年 5 月 10 日的树莓派 OS Lite 版下配置。

多数情况下，直接在全新树莓派系统下运行 `init.sh` 即可完成配置。以下列出详细的配置方法：

1. 安装 `neofetch`，`vim`，`motion`，`madplay`，`pip`。

2. （可选）配置 `~/.vimrc`。

3. 更新 `/etc/sysctl.conf`，增加如下代码：

   ```
   net.ipv4.tcp_syncookies = 1
   net.ipv4.tcp_tw_reuse = 1
   net.ipv4.tcp_tw_recycle = 1
   net.ipv4.tcp_fin_timeout = 30
   ```

   后使用 `sudo /sbin/sysctl -p` 更新配置。

4. 建立 `pipe, query_pipe, python_pipe` 管道文件。

5. 安装 `WiringPi`。

6. 开启 `Legacy Camera` 支持。

7. 修改 `/etc/motion/motion.conf` 配置文件，开启 `daemon`，配置合适的分辨率及帧率，关闭控制及串流的本机限制。

8. 执行 `compile.sh` 编译所有程序。

9. 将 `motion`，`~/app/socket &`，`~/app/main &` 加入 `rc.local` 开机自启。特别的 `socket`，`main` 需要开启后台运行。

10. 重启树莓派。

### 使用方法

需要提前将树莓派与安卓手机配置于同一局域网下。组装完成后，树莓派上电开机，接下来使用手机 APP 控制即可。操控演示视频在上方已经提供。具体控制流程如下：

1. 在初始 **Login** 界面输入树莓派网络地址登录。
2. 在 **Manual Controller** 界面，监看实时图传，使用上下左右方向键进行控制，使用 Speed 滑钮进行调速。
3. 在 **Features** 界面，使用 Speed 滑钮进行调速，使用 Line Tracing 开关控制黑线循迹，使用 Ultra Sonic 开关控制超声波。
4. 在 **Music** 界面，选择不同的选项播放不同歌曲。

### 主控交互

主控开机自启动，无法从 `stdin` 中接受指令，而是从 `/home/maxmilite/app/pipe` 管道文件中接受指令，向 `/home/maxmilite/app/query_pipe` 中写入状态结果。

指令以多个参数的形式给出，参数两两之间由一个空格隔开。具体分指令及逻辑如下表：

#### `execute` 分指令

| 指令 |      解释 / 接受参数      |     样例     |
| :--: | :------------: | :----------: |
| `f`  |  令小车前进。  | `execute f`  |
| `lf` |  令小车左转。  | `execute lf` |
| `rf` |  令小车右转。  | `execute rf` |
| `r`  |  令小车后退。  | `execute r`  |
| `lr` | 令小车左后退。 | `execute lr` |
| `rr` | 令小车右后退。 | `execute rr` |
| `st` |  令小车停止。  | `execute st` |
| `speed` |  调整小车速度。接受两个整数参数。第一个参数取值范围 [0, 100]，代表速度；第二个参数取值范围 [-1, 1]，代表调整速度的一侧/两侧，其中 -1 为两侧，0 为左侧，1 为右侧。  | `execute speed 50 -1` |
| `stop_speed` |  调整小车停止速度。接受一个整数参数。第一个参数取值范围 [0,  1]，代表是否有速度。该选项用于超声波等需要小车停止的功能。  | `execute stop_speed 0` |

#### `query` 分指令

| 指令 |      解释 / 接受参数      |     样例     |
| :--: | :------------: | :----------: |
| `speed`  |  查询速度，返回两个整数，分别代表左右侧百分比速度。  | `query speed`  |
| `line_tracing`  |  查询是否开启黑线循迹，返回 0/1。  | `query line_tracing`  |
| `ultra_sonic`  |  查询是否开启超声波，返回 0/1。  | `query ultra_sonic`  |
| `color_identification`  |  查询是否开启颜色识别，返回 0/1。  | `query color_identification`  |

#### `switch` 分指令

| 指令 |      解释 / 接受参数      |     样例     |
| :--: | :------------: | :----------: |
| `line_tracing`  |  开关黑线循迹，接受一个整数参数 0/1，代表是否开启。  | `switch line_tracing`  |
| `ultra_sonic`  |  开关是否开启超声波，接受一个整数参数 0/1，代表是否开启。   | `switch ultra_sonic`  |
| `color_identification`  |  开关是否开启颜色识别，接受一个整数参数 0/1，代表是否开启。   | `switch color_identification`  |
| `music`  |  播放音乐，接受一个整数参数，代表音乐序号。   | `switch music 1`  |

### Socket  通信端口

Socket 默认通信端口为 8887，可以在 `socket.c` 中修改。
