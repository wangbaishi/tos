import os

byte1=int('01010101',2)	#0x55
byte2=int('10101010',2)	#0xaa
fp=open("bootsec",'wb')
fp.seek(510)
fp.write('%c' % byte1);
fp.write('%c' % byte2);
fp.close();
