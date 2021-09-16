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

`break main in argc > 1`

在有效的 c 条件语句中几乎可以使用任何表达式

1. 相等，逻辑和不相等运算
   break 180 if string==NULL && i < 0
2. 按位和移位运算符(&, | )
   break test.c:34 if (x & y) == 1
3. 算术运算符(+ )
   break myfunc if i % (j + 3) != 0
4. 自己的函数只要他们被连接到程序里
   break 44 if test.c:myfunc if ! check_variable_sanity(i)
5. 库函数只要该库别链接到代码中
   break 44 if strlen(mystring) == 0

如果在 gdb 表达式中使用库函数，而该库不是用调试符号编译的（几乎肯定是这个情况），那么唯一能在断点条件中
使用的返回值类型为 int。换言之,如果没有调试信息，gdb 会假设函数的返回值为 int 类型,当这种假设不正确时，
函数的方绘制会被曲解。

(gdb) print cos(0.0)
$1 = 14368
(gdb) print (double) cos(0.0) // 强转也无济于事
$2 = 14336

### 使用非 int 返回函数

实际上有一种方式可以在 gdb 表达式中使用不返回 int 的函数，技巧在于应使用指向函数的恰当数据类型
定义一个便于 gdb 使用的变量

```bash
(gdb) set $p = (dobule (*) (dobule)) cos
(gdb) ptype $p
type = dobule(*)()
$s = 14368
(gdb) p $p(3.14159265)
$4 = -1
```

将正常断点设置成条件条件断点
cond 3 i == 3
删除条件只要输入
cond 3

### 断点命令列表

commmands _breakpoint-number_
...
commands
...
end

### define 定义宏 (p73)

`$arg0 ...`
参数之间不用逗号隔开，最多可以定义 10 个参数
宏可以保存在`.gdbinit`中

`show user`
显示所有宏

## 监视点

监视点区别于断点在于没有"住在"某一行源代码中，而是指示 gdb 每当某个表达式该变了值就暂停执行。
`watch i`
`watch (i | j > 12) && i > 24 && strlen(name) > 6`
可以将监视点看作"附加"在表达式上，当一个变量被修改了，特别是全局变量而不知道什么时候修改，适合用监视点。
处理线程代码时监视点作用有限。gdb 只能监视单个线程中的变量。
在 gdb 到达定义变量作用域之前，不能在变量上设置监视点（在作用域函数上设置断点，然后设置监视点）

### 表达式 (p77)

1. 便于 gdb 使用的变量
2. 程序中的任何在作用域内的变量
3. 任意种类的字符串，数值或者字符常量
4. 预处理器宏(gcc -g3 编译)
5. 条件，函数调用，类型强制转换和所用语言定义的运算符(Fortan-77 `.GT.` 表示大于)

## 检查和设置变量

display (disp)
这个命令要求 gdb 在执行中每次有暂停时就输出指定条目
call 可以调用其中的函数（比如说打印方法）避免重新编辑源代码

### 检查动态数组

```c
int *x
...
x = (int *) malloc(25*sizeof(int));
```

```bash
p x //　打印数组地址
p *x // 打印第一个元素
```

一般形式为

`*Point@number_of_elements`
gdb 还允许适当的时候使用类型强制转换，比如:
`p (int [25]) *x`

ptype 查看类或者结构的结构
info locals 命令列出当前栈帧中所有局部变量的值。
为希望自动输出局部变量的每个断点在 commands 例程中加上 info locals 命令

直接检查内存　 x 命令(examine)主要使用于汇编语言环境

### print display 高级选项

`p/x y`
十六进制显示变量，其他常用格式为 c 表示字符(character) s 表示字符串(string)，f 表示浮点(floating-point)

`dis disp 1` 临时禁用某个显示项
`enable disp 1` 重新启用条目
`undisp 1` 删除显示条目
