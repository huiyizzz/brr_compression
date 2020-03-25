all: encoder decoder psnr
encoder: encoder.cpp
	g++ -o encoder encoder.cpp
decoder:decoder.cpp
	g++ -o decoder decoder.cpp
psnr:psnr.cpp
	g++ -o psnr psnr.cpp
clean:
	rm -f psnr psnr

