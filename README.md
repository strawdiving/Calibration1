### Chapter1. 传感器校准

#### text message示例

本程序的传感器校准是参考PX4的校准过程实现的，PX4传感器校准的text message实例如下所示：

```c++
磁力计校准失败
"[cal] calibration started: 2 mag"
"[cal] Skipped scale calibration, sensor 0"
"[cal] pending: back front left right up down"
"[cal] hold vehicle still on a pending side"
"[cal] detected rest position, hold still..."
"[cal] up orientation detected"
"[cal] Rotate vehicle around the detected orientat"
"[cal] Continue rotation for up 7 s"
"Failed: This calibration requires rotation."
"[cal] calibration failed: mag"
// 磁力计校准，需要转动飞机，7s内用户没有转动飞机，校准失败

陀螺仪校准
"[cal] calibration started: 2 gyro"
"[cal] progress <5>"
"[cal] progress <10>"
"[cal] progress <15>"
"[cal] progress <20>"
"[cal] progress <25>"
"[cal] progress <35>"
"[cal] progress <40>"
"[cal] progress <45>"
"[cal] progress <50>"
"[cal] progress <55>"
"[cal] progress <60>"
"[cal] progress <65>"
"[cal] progress <70>"
"[cal] progress <75>"
"[cal] progress <80>"
"[cal] progress <85>"
"[cal] progress <90>"
"[cal] progress <95>"
"[cal] progress <100>"
"[cal] calibration done: gyro"

加速度计校准
"[cal] calibration started: 2 accel"
"[cal] pending: back front left right up down"
"[cal] hold vehicle still on a pending side"
"[cal] detected rest position, hold still..."
"[cal] down orientation detected"
"[cal] Hold still, measuring down side"
"[cal] down side result: [ 1.5294 -0.2218 -11.85"
"[cal] progress <17>"
"[cal] down side done, rotate to a different side"

"[cal] pending: back front left right up"
"[cal] hold vehicle still on a pending side"
"[cal] detected rest position, hold still..."
"[cal] down side already completed"

"[cal] pending: back front left right up"
"[cal] hold vehicle still on a pending side"
"[cal] detected rest position, hold still..."
"[cal] down side already completed"

"[cal] pending: back front left right up"
"[cal] hold vehicle still on a pending side"
"[cal] detected rest position, hold still..."
"[cal] down side already completed"

"[cal] pending: back front left right up"
"[cal] hold vehicle still on a pending side"
"[cal] detected rest position, hold still..."
"[cal] down side already completed"

"[cal] pending: back front left right up"
"[cal] hold vehicle still on a pending side"
"[cal] detected rest position, hold still..."
"[cal] down side already completed"
"[cal] calibration failed: timeout: no motion"
// 一直是一个方向，用户没有转动飞机，报错
```

#### 校准示意图

![sensors](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/sensors.png)

#### 校准方法

注： **传感器的校准策略一般由飞控程序开发人员设计实现，如有需要，还会重新定义通讯协议（重新定义MAVLink消息包，定制额外的MAVLink消息包），开发地面站时以此为参考，需要同步MAVLink库**  

- Gyro（陀螺仪）的校准只需检测一个面，水平放置足够长时间
- Compass（磁力计/罗盘）的校准需要检测6个面，6个面依次持续旋转（不低于7s），以采集足够的数据点
- Accel（加速计）的校准需要检测6个面，6个面依次平放足够长时间

#### 校准提示

- 校准未开始时，图片显示红色边框，文字显示”Incompleted“

- 在某一面的校准过程中（_ calInProgress为true），图片显示黄色边框

  如果是磁力计校准，图片显示旋转示意图，文字显示“Rotate”

  如果是加速计校准，图片显示普通方向示意图，文字显示“Hold Still"

- 某一面校准成功（_calDone）时，图片显示绿色边框，文字显示”Completed“

#### 校准 text message详解

1. 所有校准的text message **都以“[cal] "开始** ，如果收到的text message不以“[cal] "开始，就不是有效的校准消息
2.  **以“[cal] calibration started:”开始** ，后面跟着firmwareversion和传感器部件名（即校准type，如“gyro","accel","mag"）
3.  **“[cal] xxx orientation detected"** ，检测到某个面，“xxx”为”up", "down", "left", "right", "front", "back"6个面之一
4.  **“[cal] xxx side done, rotate to a different side”** ，某个面校准完成，“xxx”为6个面之一
5.  **“[cal] xxx side already completed”** ，某个面校准完成
6.  **“[cal] calibration done: xxx”** ，校准完成，xxx为部件名
7.  **“[cal] calibration cancelled”** ，校准取消
8.  **“[cal] calibration failed”** ，校准失败，有些错误会附加错误消息，如果用户没有转动飞机，一直是一个方向，则会报错"[cal] calibration failed: timeout: no motion"
9. gyro陀螺仪校准过程中，会发送 **"[cal] progress <45>"**  消息，显示校准进度

飞控开发人员可以根据需要更改校准的text message，地面站开发人员据此进行调整。

#### 校准流程

校准时序图： [Sensors Calibration](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/sensorCalibration.png)

##### 传感器校准程序结构

- /SetupWizard/

   **SensorsGroupPage：** 处理传感器校准的主页面，主要处理校准的text message，并根据text message来更新校准的状态

   **CalRotationWidget：** 传感器校准主页面上的可视化校准部分（包括重要校准信息的显示，及图片示意的校准状态）

   **OrientationWidget：** 单个side的图片示意的（可视化）校准状态

- /PX4AutopilotPlugin/

   **SensorsComponent：** 判断传感器是否校准完成，主要用于在开始时决定是否需要进行传感器校准

   **SensorsComponentController：** 校准的控制接口，负责调用UAS中的启动、停止校准接口，转发UAS解析出来的text message

-  **UAS：** 负责校准时和飞控通讯的主要接口，向飞控发送校准相关的指令—— **command_long消息包(#76)** ，command参数设为   **MAV_CMD_PREFLIGHT_CALIBRATION，即241** ，并设置对应的校准类型参数；
   获取并解析校准所需的 **MAVLINK_MSG_ID_STATUSTEXT** 消息包，再将解析出来的text message转交给实际的传感器校准程序进行处理
   ![calibrationCommand](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/calibrationCommand.PNG)

### Chapter2 .  PID调参

该部分程序主要是为飞控开发人员提供便捷的PID控制参数调参接口。

#### PID调参流程

PID调参时序图： [PID调参时序图](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/pidset.png)

#### PID调参程序结构

- /SetupWizard/

  **ParamPage：** PID调参主页面，用以显示和设置PID参数，包含6个PIDParamSet widget，分别进行PITCH/ROLL/YAW/X/Y/Z的PID调参

  **PIDParamSet：** PID调参主页面上的单个调参widget，提供了各个PID参数的默认值、最大/小值等

- /PX4AutopilotPlugin/

  **ParamEditController：** PID调参的控制接口，负责调用UAS中写PID参数的接口，在Vehicle收到 **PID_SET_Confirm（#157）** 消息时转发Vehicle解析出来的消息包

- **UAS：** 负责向飞控发送PID参数设置相关的消息包（ **#151~#156** ）来进行PID参数的设置

- 新增的自定义的PID调参相关的消息包详见 [消息包定义](https://github.com/strawdiving/Calibration1/blob/master/libs/mavlink/MAVLink/message_definitions/common.xml) 中（id为 **#151~#157** 的8个消息包）
