# 截面探针示例代码

## 示例1：创建截面探针并设置部分参数（指定名称）
p=Probe.ProbeSection()
p.createProbe('Probe-1')
p.setSingleNodeGroup1('NodeGroup-1')
p.setSingleNodeGroup2('NodeGroup-2')
p.setSingleNodeGroup3('NodeGroup-3')
p.setNodeGroup('NodeGroup-4')
p.setIsave(0)
p.setFileName('section.out')
p.setDt(1.0)
p.setAlpha(0.0)
p.setShellElementGroup('ShellSet-1')
p.setIframe(0)

## 根据探针名称获取类型
AI.AI_Solution().getProbeType('Probe-1')

## 根据名称获取截面探针操作对象
p=Probe.ProbeSection.GetProbeSection('Probe-1')

## 修改探针名称
p.setName('Probe-2')

## 设置单点节点组1
p.setSingleNodeGroup1('NodeGroup-1')

## 设置单点节点组2
p.setSingleNodeGroup2('NodeGroup-2')

## 设置单点节点组3
p.setSingleNodeGroup3('NodeGroup-3')

## 设置节点组
p.setNodeGroup('NodeGroup-4')

## 设置 I_SAVE 标志
p.setIsave(1)

## 设置文件名
p.setFileName('section_new.out')

## 设置时间步长
p.setDt(0.5)

## 设置指数平均滤波系数
p.setAlpha(0.1)

## 设置壳单元组
p.setShellElementGroup('ShellSet-2')

## 设置局部坐标系中心选项
p.setIframe(1)

## 清除部分关联对象
p.setSingleNodeGroup1('None')
p.setSingleNodeGroup2('None')
p.setSingleNodeGroup3('None')
p.setNodeGroup('None')
p.setShellElementGroup('None')