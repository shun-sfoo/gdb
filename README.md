# gdb

the art of debugging with gdb

## 常用命令

bt
info
finish
print
ptype 查看数据结构
continue
clear [line/fn] 清除断点
disable 禁用断点
step 进入函数
whatis 查看类型
list [函数名] ,[行数]
display 查看
layout src 查看源代码
rr 回放流程操作(单线程)

### 条件断点

b (insert) if `num_y==1`

b insert
condition if `num_y==1`

### 加载配置

gdb -command=z x 从可执行文件 x 中运行 gdb，首先要加载 z 中文件中的命令

## 持久化

save breakpoints [filename]
source [filename]

## 配置

`vim $HOME/.gdbinti`

```gdbinit
source ~/chromium/src/tools/gdb/gdbinit // 调试chromium 需要用到
set print object on // 查看运行时的类(多态)
set print pretty on // 格式化输出
set history save yes
set history size 10000
set history remove-duplicates unlimited
```
