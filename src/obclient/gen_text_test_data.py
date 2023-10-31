import random
import string

# 生成一个随机整数
random_int = random.randint(1, 1000)

# 生成一个长度为65535的随机字符串
random_string = ''.join(random.choice(string.ascii_letters) for _ in range(20))

# 创建要写入的数据行
data_line = f"insert into t values ({random_int}, '{random_string}')"

# 将数据行写入文件
with open('input.txt', 'w') as file:
    file.write(data_line)

print(f"数据已写入input.txt文件")
