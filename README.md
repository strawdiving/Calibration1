## Calibration1

地面站的Calibration校准功能模块，定制飞控的基本工作流程参考PX4，地面站校准模块的基本工作流程参考QGrounControl。针对定制的飞控开发：

- 机型：X型四旋翼
- 飞控：PX4 FMUv2/ Pixhawk或其变型，校准模块针对定制飞控进行开发，但仍可适用于类PX4的飞控。

- 定制了MAVLink消息包，添加了校准PID调参相关的消息包类型，详见 [消息包定义](https://github.com/strawdiving/Calibration1/blob/master/libs/mavlink/MAVLink/message_definitions/common.xml) 

- 定制了待加载参数metadata的xml文件[cal_params.xml](https://github.com/strawdiving/Calibration1/blob/master/cal_params.xml)

## Chapter 1. 参数加载与存储

#### 程序结构

- **ParameterLoader：** 向飞控请求所有参数，并将参数存储在对应的Fact对象中
- **PX4FirmwarePlugin：** 创建PX4ParameterMetaData对象来加载和解析参数metadata文件，添加Fact对应的metadata。调用PX4ParameterMetaData对象的唯一接口
- **PX4ParameterMetaData：** 加载和解析参数metadata文件，添加metadata到对应的Fact对象中
- **FactMetadata：** 存储（从参数元数据xml文件中解析出来的）参数的元数据
  - 本程序中——定义在/cal_params.xml中
  - PX4版本——定义在从下载的固件中解析出来的PX4ParameterFactMetaData.xml中
  - 参数的元数据信息，包括name,type,short description,default value ,min, max等信息

​          注：可以根据需要定制xml文件，仅包含所需要的参数的元数据信息

-  **Fact：** 一个Fact对象用于存储系统的一个参数值。和Fact相关的metadata元数据存储在FactMetadada中。因为一个Fact可能有多个实例，但是每个Fact只有一个FactMetaData实例，所以FactMetaData用一个独立object存储
-  **MessagePanel：** 显示参数加载进度

#### 程序流程

1. 成功连接飞控后，创建Vehicle对象和ParameterLoader对象
2. 在ParameterLoader对象中向飞控请求所有参数 **refreshAllParameters()**
3. 飞控将参数值放在 **PARAM_VALUE（#22）** 消息包中发送给地面站，Vehicle将PARAM_VALUE消息包交给UAS进行处理，数据结构如下：

```c
typedef struct __mavlink_param_value_t
{
 float param_value; /*< Onboard parameter value*/
 uint16_t param_count; /*< Total number of onboard parameters*/
 uint16_t param_index; /*< Index of this onboard parameter*/
 char param_id[16]; /*< Onboard parameter id, terminated by NULL if the length is    less than 16 human-readable chars and WITHOUT null termination (NULL) byte if the   length is exactly 16 chars - applications have to provide 16+1 bytes storage if the ID is stored as string*/
 uint8_t param_type; /*< Onboard parameter type: see the MAV_PARAM_TYPE enum for supported data types.*/
} mavlink_param_value_t;
```

解析出的 **param_id[16]** 中存储的是参数名 **paramName** ；

**param_count** 是参数的总个数， **param_index** 是当前参数的索引，用于检测所有参数是否都已加载完毕；

将 **param_type** 和 **param_value** 用 **mavlink_param_union_t** 结构体进行存储

```c
typedef struct param_union {
	union {   // 占据4字节的union
		float param_float; //4字节
		int32_t param_int32;
		uint32_t param_uint32;
		int16_t param_int16;
		uint16_t param_uint16;
		int8_t param_int8;
		uint8_t param_uint8;
		uint8_t bytes[4];
	};
	uint8_t type;
}) mavlink_param_union_t;
```

4. UAS处理好参数以后，交给ParameterLoader **存储到对应的Fact对象** 中

5. 在参数加载完毕以后，从 **参数metadata文件** 中读取各个参数的metadata，为参数对应的Fact对象添加metadata

   注：在解析PARAM_VALUE消息包，以及存储参数到Fact对象的过程中，涉及到几种数据类型的转换：

| C++ stdlib                 | Param xml               | FactMetaData                    | Qt:QMetaType                                                 | MAVLink: MAV_PARAM_TYPE             |
| -------------------------- | ----------------------- | ------------------------------- | ------------------------------------------------------------ | ----------------------------------- |
| unsigned char              | UINT8                   | typeUint8                       | UInt                                                         | MAV_PARAM_TYPE_UINT8                |
| unsigned short int         | UINT16                  | typeUint16                      | UInt                                                         | MAV_PARAM_TYPE_UINT16               |
| unsigned int               | UINT32                  | typeUint32                      | UInt                                                         | MAV_PARAM_TYPE_UINT32               |
| char                       | INT8                    | typeInt8                        | Int                                                          | MAV_PARAM_TYPE_INT8                 |
| short int                  | INT16                   | typeInt16                       | Int                                                          | MAV_PARAM_TYPE_INT16                |
| int                        | INT32                   | typeInt32                       | Int                                                          | MAV_PARAM_TYPE_INT32                |
| float                      | FLOAT                   | typeFloat                       | Float                                                        | MAV_PARAM_TYPE_REAL32               |
| double                     | DOUBLE                  | typeDouble                      | Double                                                       | MAV_PARAM_TYPE_REAL64               |
| 用于确定每种类型的数值范围 | 参数xml文件中的type类型 | FactMetaData中的ValueType_t类型 | 对QVariant进行数据类型转换（toInt，toUInt，toFloat，toDouble）时的QMetaType类型 | MAVLink消息包中的param_type参数类型 |

6. 在所有参数加载完毕后，启动校准状态检验

##### 连接飞控，加载参数，校准状态检验流程

![时序图](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/flow.png)

## Chapter 2. 校准

### 1. 校准状态检验

#### 程序结构

-  /PX4AutopilotPlugins

  **PX4AutopilotPlugin：** 创建vehicle组件，并检验组件是否需要校准；在校准状态检验时，提供通过ParameterLoader获取parameter Fact的接口

   **VehicleComponent：** 抽象类，代表一个vehicle组件，提供了组件名称，图标，校准是否完成或校准还需哪些步骤等信息

   **ComponentController：** 基类，vehicle组件校准的控制类

-  **/SetupWidget：** 处理校准进程的主窗口，显示校准状态及可视化的校准过程，包括SummaryViewPage，MessagePanel和传感器校准的SensorsGroupPage

-  **/SetupWizard/SummaryViewPage：** 显示校准的总体状态及提示

#### 程序流程

1. 在初始参数都加载完毕以后，调用 **PX4AutopilotPlugin** 来启动校准状态检验（一般是从参数对应的Fact对象中读取关键参数的值，来判断校准是否完成）

| Vehicle Component | 校准完成的条件                                      |      |
| ----------------- | --------------------------------------------------- | ---- |
| Sensors           | CAL_MAG0_ID，CAL_GYRO0_ID，CAL_ACC0_ID参数值均不为0 |      |

2. 如果校准未完成，在 **SummaryViewPage** 上显示需要校准的部件，选择对应的部件启动校准
3. 在校准完成后，飞控中会将对应的参数值进行更新
4. 地面站重新请求参数，再次进行校准状态检验

### 2. 传感器校准

#### 校准示意图

![sensors](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/sensors.png)

#### 传感器校准程序结构

- /SetupWizard/

  **SensorsGroupPage：** 处理传感器校准的主页面，主要处理校准的text message，并根据text message来更新校准的状态

  **CalRotationWidget：** 传感器校准主页面上的可视化校准部分（包括重要校准信息的显示，及图片示意的校准状态）

  **OrientationWidget：** 单个side的图片示意的（可视化）校准状态

- /PX4AutopilotPlugin/

  **SensorsComponent：** 判断传感器是否校准完成，主要用于在开始时决定是否需要进行传感器校准

  **SensorsComponentController：** 校准的控制接口，负责调用UAS中的启动、停止校准接口，转发UAS解析出来的text message

- **UAS：** 负责校准时和飞控通讯的主要接口，向飞控发送校准相关的指令—— **command_long消息包(#76)** ，command参数设为   **MAV_CMD_PREFLIGHT_CALIBRATION，即241** ，并设置对应的校准类型参数；
  获取并解析校准所需的 **MAVLINK_MSG_ID_STATUSTEXT** 消息包，再将解析出来的text message转交给实际的传感器校准程序进行处理
  ![calibrationCommand](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/calibrationCommand.PNG)

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

##### text message示例

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

##  Chapter 3. PID调参

该部分程序主要是为飞控开发人员提供便捷的PID控制参数调参接口。

#### PID调参程序结构

- /SetupWizard/

  **ParamPage：** PID调参主页面，用以显示和设置PID参数，包含6个PIDParamSet widget，分别进行PITCH/ROLL/YAW/X/Y/Z的PID调参

  **PIDParamSet：** PID调参主页面上的单个调参widget，提供了各个PID参数的默认值、最大/小值等

- /PX4AutopilotPlugin/

  **ParamEditController：** PID调参的控制接口，负责调用UAS中写PID参数的接口，在Vehicle收到 **PID_SET_Confirm（#157）** 消息时转发Vehicle解析出来的消息包

- **UAS：** 负责向飞控发送PID参数设置相关的消息包（ **#151~#156** ）来进行PID参数的设置

- 新增的自定义的PID调参相关的消息包详见 [消息包定义](https://github.com/strawdiving/Calibration1/blob/master/libs/mavlink/MAVLink/message_definitions/common.xml) 中（id为 **#151~#157** 的8个消息包）

#### PID调参流程

PID调参时序图： [PID调参时序图](https://github.com/strawdiving/Calibration1/blob/master/ReadmeImages/pidset.png)