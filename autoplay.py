import subprocess
import os

#Just keep playing
'''
for i in range(10000):
	rslt = subprocess.run(['./bin/newhost', './bin/newrandomplayer', './bin/newrandomplayer'])
'''
#Counts the winning
'''
player1 = 0
player2 = 0
for i in range(100):
	rslt = subprocess.run('./bin/host', stdout = subprocess.PIPE)
	if rslt.stdout.decode('ascii').split("\n")[-2] == 'PLAYER 2 WON':
		player2 += 1
	else:
		player1 += 1

	print( "\b" * 30 + "Player 1: %d Player 2: %d" % (player1,player2))
'''

#Encodes the winning state and categorize

import json
from pathlib import Path


p1state = {}
p2state = {}

if(not os.path.exists('./data/white-win.json')):
	Path('./data/white-win.json').touch()
if(not os.path.exists('./datablack/win.json')):
	Path('./data/black-win.json').touch()

with open('./data/white-win.json', 'r') as file:
	p2state = json.load(file)
with open('./data/black-win.json', 'r') as file:
	p1state = json.load(file)

for i in range(100):
	rslt = subprocess.run('./bin/host-stat', stdout = subprocess.PIPE)
	if(rslt.stdout.decode('ascii').split("\n")[-2] == 'PLAYER 2 WON'):
		encoded = "".join(rslt.stdout.decode('ascii').split('\n')[0: -3])
		if(encoded in p2state):
			print("p2old")
			p2state[encoded] += 1
		else:
			print("p2new")
			p2state[encoded] = 1
	else:
		encoded = "".join(rslt.stdout.decode('ascii').split('\n')[0: -3])
		if(encoded in p1state):
			print("p1old")
			p1state[encoded] += 1
		else:
			print("p1new")
			p1state[encoded] = 1

with open('./data/white-win.json', 'w') as file:
	json.dump(p2state ,file)
with open('./data/black-win.json', 'w') as file:
	json.dump(p1state ,file)

