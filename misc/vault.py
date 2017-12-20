#!/usr/bin/python3
from time import time
from operator import mul, sub, add

# path_weight - finds the weight on a path
# vault: vault through which to traverse
# path:  path to follow
def path_weight(vault, path):
	(x, y) = path[0]
	weight = vault[y][x]

	for i in range(2, len(path), 2):
		(x, y) = path[i]
		(ox, oy) = path[i-1]
		weight = vault[oy][ox](weight, vault[y][x])

	return weight

# path_string - converts a list of path tuples to a string of north/south/east/west directions
# path: list of (x, y) tuples
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

# step - solves the part 8 maze
# vault:    2D (y: north->south; x: east->west) array of vault rooms; numeric literal for number rooms,
#           operator.<op> for operator rooms
# travel:   legal travel directions as array of (x, y) tuples
#           (e.g. [(0, -1), (0, 1)] would only allow north/south travel)
# start:    (x, y) tuple of starting position
# end:      (x, y) tuple of end position
# maxdepth: maximum number of moves to make
# goal:     desired value at end position
def step(vault, travel, start, end, maxdepth, goal):
	paths = []
	stack = [[start]]
	visited = 0

	while stack:
		visited += 1
		path = stack.pop()
		pos = path[-1]
		(x, y) = pos

		# paths that return to the antechamber fail
		if pos == start and len(path) > 1:
			continue

		# odd path lengths should always put you in a numeric literal room, so check the status of the path
		if len(path) & 1 == 1:
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

	return (visited, paths)

vault = [
	[ mul, 8  , sub, 1   ],
	[ 4  , mul, 11 , mul ],
	[ add, 4  , sub, 18  ],
	[ 22 , sub, 9  , mul ]
];
travel = [(-1, 0), (1, 0), (0, -1), (0, 1)]
start = (0, 3)
end = (3, 0)
maxdepth = 15
goal = 30

for i in range(1, maxdepth + 1, 2):
	print('##### Scanning up to {:n} steps #####'.format(i))

	begin = time()
	(visited, paths) = step(vault, travel, start, end, i, goal)
	elapsed = time() - begin
	print('### {:n} nodes visited in {:n} seconds, {:n} results'.format(visited, elapsed, len(paths)))

	for path in paths:
		print('{:n}: {:s}'.format(len(path), path_string(path)))

