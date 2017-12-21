#!/usr/bin/perl
use strict;
use warnings;
no warnings 'recursion';

sub verify {
	my ($arg0, $arg1, $seed, $cache) = @_;

	my $this_call = $arg0 . ',' . $arg1;

	my $cached = $cache->{$this_call};
	if ($cached) {
		return $cached;
	}
	
	if (!$arg0) {
		$arg0 = ($arg1 + 1) % 32768;
		$cache->{$this_call} = $arg0;
		return $arg0;
	}

	if (!$arg1) {
		$arg0 = ($arg0 + 32767) % 32768;
		$arg0 = verify($arg0, $seed, $seed, $cache);
		$cache->{$this_call} = $arg0;
		return $arg0;
	}

	$arg1 = ($arg1 + 32767) % 32768;
	$arg1 = verify($arg0, $arg1, $seed, $cache);
	$arg0 = ($arg0 + 32767) % 32768;
	$arg0 = verify($arg0, $arg1, $seed, $cache);
	$cache->{$this_call} = $arg0;
	return $arg0;
}

for (my $i = 25000; $i < 32768; $i++) {
	printf "%d => %d\n", $i, verify(4, 1, $i, {});
}
