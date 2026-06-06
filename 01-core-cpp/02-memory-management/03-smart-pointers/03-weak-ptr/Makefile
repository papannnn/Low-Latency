TARGET:main

main.o:main.cpp
	g++ -c -o main.o main.cpp -I . -std=c++20

main:main.o
	g++ -o main main.cpp -std=c++20 -I .

clean:
	rm main
	rm main.o