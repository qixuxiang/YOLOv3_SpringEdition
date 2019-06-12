/usr/bin/g++-5 test.cpp -std=c++11 `pkg-config --cflags --libs opencv` -ldl  -Wl,-rpath,. -o test
