# 加速度探针示例代码

## 示例1：创建加速度探针并设置部分参数（指定名称）
p=Probe.ProbeAccelerometer()
p.createProbe('Probe-1')
p.setSystemID(0)
p.setSingleNodeGroup('NodeGroup-1')
p.setCutOffFreq(1000.0)

## 根据探针名称获取类型
AI.AI_Solution().getProbeType('Probe-1')

## 根据名称获取加速度探针操作对象
p=Probe.ProbeAccelerometer.GetProbeAccelerometer('Probe-1')

## 修改探针名称
p.setName('Probe-2')

## 设置坐标系ID
p.setSystemID(1)

## 设置单点节点组
p.setSingleNodeGroup('NodeGroup-2')

## 清除单点节点组
p.setSingleNodeGroup('None')

## 设置截止频率
p.setCutOffFreq(500.0)

## 获取首节点ID
p.getNodeID()