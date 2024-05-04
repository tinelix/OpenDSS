all:
	mkdir -p ./out
	gcc -o out/opendss opendss.cpp -lavformat lavcodec -lswscale -lz
