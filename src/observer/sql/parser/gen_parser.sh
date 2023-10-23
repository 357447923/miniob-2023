#!/bin/bash
flex --outfile /home/xiaoming/miniob-competition/src/observer/sql/parser/lex_sql.cpp --header-file=lex_sql.h /home/xiaoming/miniob-competition/src/observer/sql/parser/lex_sql.l
`which bison` -v -d -Wcounterexamples --output /home/xiaoming/miniob-competition/src/observer/sql/parser/yacc_sql.cpp /home/xiaoming/miniob-competition/src/observer/sql/parser/yacc_sql.y
