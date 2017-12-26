#!/usr/bin/perl
use strict;
use warnings;

sub verify2 {
	my ($seed0, $seed1, $seed2) = @_;

	my %cache;
	my @stack = ($seed1, $seed0, $seed0.','.$seed1, 0, 0);
	my $sp = 5;

	while ($sp > 1) {
		# grab the stack frame for this iteration
		$sp -= 3;
		my ($hash, $state, $rv) = @stack[$sp..$sp+2];

		# continuation for calls that return immediately after
		if ($state == 1) {
			$cache{$hash} = $rv;
			$stack[$sp++] = $rv;
			next;
		}

		my $arg0 = $stack[--$sp];

		# continuation for lower section
		if ($state == 2) {
			$arg0 = ($arg0 + 32767) % 32768;

			my $newhash = $arg0.','.$rv;
			@stack[$sp..$sp+6] = ($hash, 1, # self frame
				$rv, $arg0, $newhash, 0, 0); # new frame
			$sp += 7;
			next;
		}

		my $arg1 = $stack[--$sp];

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
			my $newhash = $arg0.','.$seed2;
			@stack[$sp..$sp+6] = ($hash, 1, # push self onto stack
				$seed2, $arg0, $newhash, 0, 0); # push new frame onto stack
			$sp += 7;
			next;
		}

		$arg1 = ($arg1 + 32767) % 32768;
		my $newhash = $arg0.','.$arg1;
		@stack[$sp..$sp+7] = ($arg0, $hash, 2, # push self onto stack
			$arg1, $arg0, $newhash, 0, 0); # push new frame onto stack
		$sp += 8;
	}

	return $stack[0];
}

for (my $i = 0; $i < 10; $i++) {
	printf "%d => %d\n", $i, verify2(4, 1, $i, {});
}
