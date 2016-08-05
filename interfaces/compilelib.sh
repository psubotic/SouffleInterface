g++ -fPIC -g -j 8 -rdynamic -D__EMBEDDED_SOUFFLE__ -Wall -std=c++11 -c -I./include -I../ ./$1.cpp -o $1.a
g++ -rdynamic -shared -o lib$1.so $1.a

