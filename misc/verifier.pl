#!/usr/bin/perl
use strict;
use warnings;
use List::MoreUtils qw( any );
use Data::Dumper;
no warnings 'recursion';
$| = 1;

sub verify {
	my ($arg0, $arg1, $seed, $cache) = @_;

	my $this_call = "$arg0+$arg1";
	if (defined $cache->{$this_call}) {
		return $cache->{$this_call};
	}
	
	if ($arg0 == 0) {
		return ($cache->{$this_call} = ($arg1 + 1) % 32768);
	}

	if ($arg1 == 0) {
		return ($cache->{$this_call} = verify(($arg0 + 32767) % 32768, $seed, $seed, $cache));
	}

	$arg1 = verify($arg0, ($arg1 + 32767) % 32768, $seed, $cache);
	return ($cache->{$this_call} = verify(($arg0 + 32767) % 32768, $arg1, $seed, $cache));
}

my $start = time;

for (my $i = 0; $i < 32768; $i++) {
	printf "[%d] %d => %d\n", time - $start, $i, verify(4, 1, $i, {});
}
