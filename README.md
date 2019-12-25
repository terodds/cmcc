项目树
```bash
cminus
├── analyze.c   # 语义分析，扫描语法树填写符号表
├── analyze.h   # 语义分析模块接口
├── bs.mk   # bison 测试指令
├── cm.l    # 词法分析器 (flex)
├── cm.y    # 语法分析器 (bison)
├── globals.h   # 公共头文件，外部全局变量声明
├── lex.mk  # flex 测试指令
├── lex.yy.c # 词法分析源程序
├── main.c  # 编译主程序
├── makefile # 主项目运行指令
├── parse.h # 语义分析模块接口
├── scan.h # 语法分析模块接口
├── symtab.c 
├── symtab.h
├── test.c
├── test.mk
├── test.sh
├── testree.c
├── tree.c
├── tree.h
├── util.c # 语法分析树源程序
└── util.h # 语法分析树接口
```