TARGET:main

main.o:main.cpp
	g++ -c -o main.o main.cpp -I . -std=c++23

main:main.o
	g++ -o main main.o -std=c++23

clean:
	rm main.o
	rm main