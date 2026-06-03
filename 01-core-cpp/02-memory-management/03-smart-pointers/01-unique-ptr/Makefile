TARGET:main

main.o:main.cpp
	g++ -c -o main.o main.cpp -I .

main:main.o
	g++ -o main main.o

clean:
	rm main.o
	rm main