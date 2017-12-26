#!/usr/bin/perl
use strict;
use warnings;

sub verify2 {
	my ($seed0, $seed1, $seed2) = @_;

	my %cache;
	my @stack = ($seed0, $seed1, $seed0.','.$seed1, 0, 0);
	my $sp = 5;

	while ($sp > 1) {
		# grab the stack frame for this iteration
		$sp -= 5;
		my ($arg0, $arg1, $hash, $state, $rv) = @stack[$sp..$sp+4];

		# continuation for calls that return immediately after
		if ($state == 1) {
			$cache{$hash} = $rv;
			$stack[$sp++] = $rv;
			next;
		}

		# continuation for lower section
		if ($state == 2) {
			$arg0 = ($arg0 + 32767) % 32768;

			@stack[$sp..$sp+8] = ($arg0, $rv, $hash, 1, # self frame
				$arg0, $rv, $arg0.','.$rv, 0, 0); # new frame
			$sp += 9;
			next;
		}

		# check cache to see if this call has already been made
		my $cached = $cache{$hash};
		if ($cached) {
			$stack[$sp++] = $cached;
			next;
		}

		if ($arg0 == 0) {
			$arg0 = ($arg1 + 1) % 32768;
			$cache{$hash} = $arg0;
			$stack[$sp++] = $arg0;
			next;
		}

		if ($arg1 == 0) {
			$arg0 = ($arg0 + 32767) % 32768;
			@stack[$sp..$sp+8] = ($arg0, $arg1, $hash, 1, # push self onto stack
				$arg0, $seed2, $arg0.','.$seed2, 0, 0); # push new frame onto stack
			$sp += 9;
			next;
		}

		$arg1 = ($arg1 + 32767) % 32768;
		@stack[$sp..$sp+8] = ($arg0, $arg1, $hash, 2, # push self onto stack
			$arg0, $arg1, $arg0.','.$arg1, 0, 0); # push new frame onto stack
		$sp += 9;
	}

	return $stack[0];
}

for (my $i = 0; $i < 10; $i++) {
	printf "%d => %d\n", $i, verify2(4, 1, $i, {});
}
