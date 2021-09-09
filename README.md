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

## 删除断点

delete _breakping_list_ 基于标识符删除
delete 2 4
delete 删除所有断点 除非设置 set confirm off

clear 删除下一个指令的断点

clear _function_ , clear _filename:function_ , clear _line_number_, clear _filename:line_number_ 与 break 对应

## 禁用断点

disable _breakpoint_list_
disable 1 3 对应 enable
enable once _breakpoint_list_ 引起 gdb 暂停后禁用
与 tbreak 类似（禁用而不是暂停）

## 断点的属性 (info breakpoints)

行号， 条件， 启用禁用状态

标识符 num : 断点的唯一标识符
类型 type: 指出字段是断点，监视点，还是捕获点
部署 disp: (keep) 保持 => b， （del)删除 => tb（dis)禁用 => enable once
启用状态 enb
地址（address): 内存中设置断点的位置，它主要提供汇编语言程序员或者试图调试没有用扩充的符号表编译的可执行文件
位置（what）源代码的特定行，对于监视点显示变量名

还可以看到断点执行次数

## 恢复执行

next [n] 下一步 下 n 步
step [n] 步进 步进 n 次

continue c [n] 继续直到运行结束或下一个断点/ 忽略 n 个断点

finish fin 指示 gdb 运行直到恰好在当前栈帧刚好运行完成之后
在一个递归调用中，finish 只会将你带到递归上一层，这是因为每一次都会被看作一次函数调用。因为每次调用都有各自的栈帧,在递归层次较高退出递归更适合 continue 或 until。

until 执行程序，直到到达当前循环体外下一行代码
until 实际执行的是：执行程序直到它到达内存地址比当前内存地址更高的机器指令，而不是直到到达源代码中一个更大的行号。这可以解释 until 运行循环后再次运行有时候会调到上一行。
如果在循环末尾，使用 until 会回到循环顶部，再次执行就会离开当前循环

until 也可以使用代码中的位置参数

## 条件断点

break _break-args_ if (condition)
