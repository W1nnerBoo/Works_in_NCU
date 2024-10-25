import pandas as pd
import matplotlib.pyplot as plt

# 创建一个 DataFrame，使用给定的数据
data = {
    "Options": ["-O0", "-O1", "-O2", "-O3", "-O4", "-Ofast"],
    "BubbleTimes": [24.021024, 25.260977, 23.357615, 23.522275, 23.853283, 24.367005],
    "HeapTimes": [0.029030, 0.034788, 0.029188, 0.031583, 0.028366, 0.030149],
    "FibbTimes": [0.110880, 0.080461, 0.217169, 0.104078, 0.084704, 0.130947],
    "AllTimes": [24.161037, 25.376795, 23.604051, 23.658023, 23.966437, 24.528177],
    "UserCPU": [24.084300, 25.370091, 23.489478, 23.639515, 23.960297, 24.475010],
    "SystemCPU": [0.087508, 0.013252, 0.119230, 0.024442, 0.011263, 0.057426],
}

df = pd.DataFrame(data)

# 设置绘图风格
plt.style.use('ggplot')

# 绘制图形
plt.figure(figsize=(12, 8))

# 绘制各项执行时间
plt.plot(df['Options'], df['BubbleTimes'], label='Bubble Sort Time', marker='o')
plt.plot(df['Options'], df['HeapTimes'], label='Heap Sort Time', marker='x')
plt.plot(df['Options'], df['FibbTimes'], label='Fibonacci Heap Time', marker='s')
plt.plot(df['Options'], df['AllTimes'], label='All Times', marker='D')
# 添加标题和标签
plt.title('Sorting Algorithm Performance Comparison', fontsize=16)
plt.xlabel('Compiler Optimization Options', fontsize=14)
plt.ylabel('Execution Time (seconds)', fontsize=14)
plt.xticks(rotation=45)  # 旋转 x 轴标签以提高可读性
plt.legend()
plt.grid()

# 保存图像为矢量图（SVG格式）
plt.savefig('sorting_performance.svg', format='svg')

# 显示图形
plt.show()

