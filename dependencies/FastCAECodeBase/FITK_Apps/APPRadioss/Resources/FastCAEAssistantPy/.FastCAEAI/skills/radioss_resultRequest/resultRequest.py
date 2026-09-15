resultRequest=ResultRequest()
resultRequest.setStartTime(0)
resultRequest.setFrequency(0.0005)
resultRequest.setHisFrequency(0.0005)
resultRequest.setNodalOutputAnim([0, 1, 2])
resultRequest.setElementalOutputAnim([0])
resultRequest.setShellOutputAnim([0, 1])

## 实例设置开始时间为9
resultRequest=ResultRequest()
resultRequest.setStartTime(9)

## 实例设置开始时间为7 频率为0.01
resultRequest=ResultRequest()
resultRequest.setStartTime(7)
resultRequest.setFrequency(0.01)

## 设置节点数据输出动画类型有 位移，速度、接触压力
resultRequest=ResultRequest()
resultRequest.setNodalOutputAnim([0, 1, 6])
