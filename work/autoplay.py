import subprocess
import os


#Counts the winning
'''
player1 = 0
player2 = 0
for i in range(100):
	rslt = subprocess.run('./host', stdout = subprocess.PIPE)
	if rslt.stdout.decode('ascii').split("\n")[-2] == 'PLAYER 2 WON':
		player2 += 1
	else:
		player1 += 1

	print( "\b" * 30 + "Player 1: %d Player 2: %d" % (player1,player2))
'''

#Encodes the winning state and categorize

import json
from pathlib import Path

if(not os.path.exists('white')):
	os.mkdir('white')
if(not os.path.exists('black')):
	os.mkdir('black')

p1state = {}
p2state = {}

if(not os.path.exists('white/win.json')):
	Path('./white/win.json').touch()
if(not os.path.exists('black/win.json')):
	Path('./black/win.json').touch()

with open('white/win.json', 'r') as file:
	p2state = json.load(file)
with open('black/win.json', 'r') as file:
	p1state = json.load(file)

for i in range(100):
	rslt = subprocess.run('./host-stat', stdout = subprocess.PIPE)
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

with open('white/win.json', 'w') as file:
	json.dump(p2state ,file)
with open('black/win.json', 'w') as file:
	json.dump(p1state ,file)
