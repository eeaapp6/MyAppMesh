# 获取所有全局变量的字典
global_vars = globals()

# 创建一个列表，包含所有要删除的变量名
vars_to_delete = [var for var in global_vars if var not in ('__builtins__', '__name__', '__doc__', '__package__', '__loader__', '__spec__')]

# 删除所有变量
for var in vars_to_delete:
    del global_vars[var]
    
import sys
sys.path.clear()