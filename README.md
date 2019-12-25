## 项目树

```bash
cminus
├── analyze.c   # 语义分析，扫描语法树填写符号表
├── analyze.h   # 语义分析模块接口
├── cm.l    # 词法分析器 (flex)
├── cm.y    # 语法分析器 (bison)
├── globals.h   # 公共头文件，包含外部全局变量声明
├── main.c  # 编译主程序
├── makefile # 主项目运行指令
├── parse.h # 语法分析模块接口 -> cm.y
├── scan.h # 词法分析模块接口 -> cm.l
├── symtab.c # 符号表数据结构
├── symtab.h # 符号表数据结构
├── test.sh # 测试脚本
├── util.c # 语法分析树数据结构
└── util.h # 语法分析树数据结构
├──*testcase # 测试用例
```

## 运行

### Prequisations

- gcc
- make
- flex
- bison

### 执行：

```bash
make  # generate main executable file
make test # build and run default testcase
make test file=[1,2,...,11] # build and fun testcase 1-11 (testcase/test<file>e.txt)
```

or

```bash
./test <filepath>
```

