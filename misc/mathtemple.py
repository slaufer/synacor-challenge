#/usr/bin/python

def answer(coins, using):
	if not coins:
		ans = (using[0] + using[1] * using[2]**2 + using[3]**3 - using[4])
		if ans == 399:
			print(using)

	for coin in coins:
		c_coins = coins[:]
		c_coins.remove(coin)

		c_using = using[:]
		c_using.append(coin)

		answer(c_coins, c_using)

coins = [ 2, 3, 5, 7, 9 ]
using = []
answer(coins, using);
