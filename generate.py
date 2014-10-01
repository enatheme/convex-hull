# -*- coding: utf-8 -*-

import random, os, time
file = open("generate.out", "w")
for x in range(100):
	a = random.randint(1, 100) % 100
	b = random.randint(1, 100) % 100
	if ((a < 100) & (a > 0) & (b < 100) & (b > 0)):
		file.write(str(a) + " " + str(b) + "\n")
	else:
		x = x - 1
		print(str(a) + " " + str(b))
file.close()
