#!/usr/bin/python3

# THIS DOESN'T WORK

stack = []

def lab0(r0, r1, r7, stack):
	# lab0
	if r0 == 0:
		r0 = r1 + 1
		return (r0, r1, r7, stack)

	# lab1
	if r1 == 0:
		r0 -= 1
		r1 = r7
		(r0, r1, r7, stack) = lab0(r0, r1, r7, stack)
		return (r0, r1, r7, stack)

	#lab2
	stack.append(r0)
	r1 -= 1
	(r0, r1, r7, stack) = lab0(r0, r1, r7, stack)
	r1 = r0
	r0 = stack.pop()
	r0 -= 1
	(r0, r1, r7, stack) = lab0(r0, r1, r7, stack)
	return (r0, r1, r7, stack)
	
print(lab0(4, 1, 31337, []))
