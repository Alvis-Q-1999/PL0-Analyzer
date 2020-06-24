# PL0-Analyzer

一个简单的 PL/0 词法/语法分析器

## build

### gcc
```bash
gcc -I. ./*.c -o analyzer
```

## Usage
Read from file:
```bash
./analyzer inputfile [-o outputfile]
```

Read from stdin:
```bash
./analyzer -
```

## License

This project is under GPL-3.0 license.