#!/usr/bin/python3
from time import time
import operator

def path_weight(vault, path):
	(x, y) = path[0]
	weight = vault[y][x]

	for i in range(2, len(path), 2):
		(x, y) = path[i]
		(ox, oy) = path[i-1]
		weight = vault[oy][ox](weight, vault[y][x])

	return weight

def path_string(path):
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

def step(vault, travel, start, end, maxdepth, goal):
	paths = []
	stack = [[start]]
	count = 0

	while stack:
		count += 1
		path = stack.pop()
		pos = path[-1]
		(x, y) = pos

		# paths that return to the antechamber fail
		if pos == start and len(path) > 1:
			continue

		# if this is a numeric literal room, check the status of the path
		if isinstance(vault[y][x], int):
			# get path weight
			weight = path_weight(vault, path)

			# paths with negative weights fail
			if weight < 0:
				continue

			# if we're in the orb room, the path is over one way or another
			if pos == end:
				# if this path matches our criteria, save it
				if weight == goal:
					paths.append(path)

				continue

		# if this is the deepest we can go, move on
		if len(path) >= maxdepth:
			continue

		# if this path hasn't been cut short yet, queue its children
		stack += [path + [(x+dx, y+dy)] for (dx, dy) in travel if x+dx in range(0, len(vault)) and y+dy in range(0, len(vault[0]))]

	return paths

vault = [
	[ operator.mul, 8  , operator.sub, 1   ],
	[ 4  , operator.mul, 11 , operator.mul ],
	[ operator.add, 4  , operator.sub, 18  ],
	[ 22 , operator.sub, 9  , operator.mul ]
];
travel = [(-1, 0), (1, 0), (0, -1), (0, 1)]
start = (0, 3)
end = (3, 0)
maxdepth = 15
goal = 30

for i in range(1, maxdepth + 1, 2):
	print('*** Looking for solutions with up to {:n} steps, approx. {:n} nodes to visit'.format(i, sum([j**4 for j in range(1, i+1, 2)])))

	begin_time = time()
	paths = step(vault, travel, start, end, i, goal)
	elapsed_time = time() - begin_time
	print('*** Finished in {:n} seconds'.format(elapsed_time))

	for path in paths:
		print('{:n}: {:s}'.format(len(path), path_string(path)))

