# CMPT365 Assignment2 BRR Compression


### There are 3 programs wrote by C++. For BRR encoding, the values of the filter and the range are calculated every 16 samples.
### Since my computer uses a little-endian architecture, I do the swap in each program.



Run the BRR encoding program:
	> encoder RawFilename BrrFilename
	(i.e. > encoder sample.raw brr.brr)

Run the BRR decoding program:
	> decoder BrrFilename RawFilename
	(i.e. > decoder brr.brr out.raw)

Run the analyzer program:
	> psnr RawFilename1 RawFilename2
	(i.e. > psnr sample.raw out.raw)
