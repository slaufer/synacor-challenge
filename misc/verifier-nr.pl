#!/usr/bin/perl
use strict;
use warnings;
no warnings 'recursion';

sub verify2 {
	my ($seed0, $seed1, $seed2) = @_;
	my @stack = ($seed0.','.$seed1, $seed0, $seed1, 0, 0);
	my %cache;

	my $rv;

	while (@stack >= 5) {
		my ($hash, $arg0, $arg1, $state, $rv) = splice(@stack, -5, 5);

		# continuation of if clauses
		if ($state == 1) {
			$cache{$hash} = $rv;
			push @stack, $rv;
			next;
		}

		if ($state == 2) {
			$arg1 = $rv;
			$arg0 = ($arg0 + 32767) % 32768;
			push @stack, ($hash, $arg0, $arg1, 1); # push self onto stack
			push @stack, ($arg0.','.$arg1, $arg0, $arg1, 0, 0); # push new frame onto stack
			next;
		}

		# check cache to see if this call has already been made
		my $cached = $cache{$hash};
		if ($cached) {
			push @stack, $cached;
			next;
		}

		if ($arg0 == 0) {
			$arg0 = ($arg1 + 1) % 32768;
			$cache{$hash} = $arg0;
			push @stack, $arg0; 
			next;
		}

		if ($arg1 == 0) {
			$arg0 = ($arg0 + 32767) % 32768;
			push @stack, ($hash, $arg0, $arg1, 1); # push self onto stack
			push @stack, ($arg0.','.$seed2, $arg0, $seed2, 0, 0); # push new frame onto stack
			next;
		}

		$arg1 = ($arg1 + 32767) % 32768;
		push @stack, ($hash, $arg0, $arg1, 2); # push self onto stack
		push @stack, ($arg0.','.$arg1, $arg0, $arg1, 0, 0); # push new frame onto stack
	}

	return $stack[0];
}

for (my $i = 0; $i < 10; $i++) {
	printf "%d => %d\n", $i, verify2(4, 1, $i, {});
}
