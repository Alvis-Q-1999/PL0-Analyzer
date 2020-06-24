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

## Tests
```bash
$ ./analyzer - <<END
if a > b then
123begin
    a = a + 1;
end;
END
PL/0 Analizer Copyright 2020    ShuaiCheng Zhu

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Lexical Analysis

lex:2:1: invalid symbol: 123begin
```

```bash
$ ./analyzer - <<END
if a > b then
begin
    a = a + 1;
end
END
PL/0 Analizer Copyright 2020    ShuaiCheng Zhu

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Lexical Analysis

syntax:3:7: syntax error, expected becomes but got eql.
```

## License

This project is under GPL-3.0 license.