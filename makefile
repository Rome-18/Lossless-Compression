lossless: main.o Huffman.o shannon.o bitio.o 
	g++ -o debug/debug main.o Huffman.o shannon.o bitio.o

main.o: main.cpp Huffman.h shannon.h bitio.h errhand.h
	g++ -c main.cpp

shannon.o: shannon.cpp Huffman.h bitio.h errhand.h
	g++ -c shannon.cpp

Huffman.o: Huffman.cpp Huffman.h bitio.h errhand.h 
	g++ -c Huffman.cpp 

bitio.o: bitio.cpp bitio.h errhand.h
	g++ -c bitio.cpp


