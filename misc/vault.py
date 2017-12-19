#!/usr/bin/python3


def evalpath(vault, path):
	(x, y) = path[0]
	value = vault[y][x]
	opstring = ""

	for (x, y) in path[1:]:
		if isinstance(vault[y][x], int):
			value = eval(str(value) + opstring + str(vault[y][x]))
		else:
			opstring = vault[y][x]

	return value

def pathtext(path):
	text = ""
	last = path[0]

	for pos in path[1:]:
		(lx, ly) = last
		(px, py) = pos

		if px > lx:
			text += "east "
		elif px < lx:
			text += "west "
		elif py < ly:
			text += "north "
		elif py > ly:
			text += "south "

		last = pos

	return text

def step(vault, travel, start, end, maxdepth, goal, path=[], depth=0):
	# if the path isn't initialized, do so now
	if not path:
		path = [start]
		depth = 1

	# if we've gone too deep, return
	if depth > maxdepth:
		return

	# figure out what square we're on
	(x, y) = path[-1]


	# if we're on a literal square, determine if we need to do anything
	if isinstance(vault[y][x], int):
		# if we went back into the antechamber, stop here
		if (x, y) == start and depth > 1:
			return

		# get the path weight so far
		weight = evalpath(vault, path)
	
		# negative weights are an instant failure
		if weight < 0:
			return

		# if we're in the vault door chamber
		if (x, y) == end:
			# weight == 30 is a success, anything else is a failure
			if weight == goal:
				print(weight, pathtext(path))
			else:
				return

	# traverse into all valid adjacent squares
	for target in [(x+dx, y+dy) for (dx, dy) in travel if x+dx in range(0, len(vault)) and y+dy in range(0, len(vault[0]))]:
		path.append(target)
		step(vault, travel, start, end, maxdepth, goal, path, depth + 1)
		path.pop()

vault = [
	[ '*', 8  , '-', 1   ],
	[ 4  , '*', 11 , '*' ],
	[ '+', 4  , '-', 18  ],
	[ 22 , '-', 9  , '*' ]
];
travel = [(-1, 0), (1, 0), (0, -1), (0, 1)]
start = (0, 3)
end = (3, 0)
maxdepth = 13
goal = 30

step(vault, travel, start, end, maxdepth, goal)
