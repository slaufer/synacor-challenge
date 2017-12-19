#/usr/bin/python3

def answer(coins, using, goal):
	if not coins:
		ans = (using[0] + using[1] * using[2]**2 + using[3]**3 - using[4])
		if ans == goal:
			print("%d + %d * %d^2 + %d^3 - %d = %d\n" % (tuple(using) + (ans,)))

	for coin in coins:
		c_coins = coins[:]
		c_coins.remove(coin)

		c_using = using[:]
		c_using.append(coin)

		answer(c_coins, c_using, goal)

coins = [ 2, 3, 5, 7, 9 ]
using = []
answer(coins, using, 399)
