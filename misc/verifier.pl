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

sub to_hms {
	
	my $secs = shift;
	return sprintf("%02d:%02d:%02d", int($secs / 3600), int(($secs / 60) % 60), $secs % 60);
}

my $first = 0;
my $count = 32768;

my $start = time;
for (my $i = $first; $i < $first + $count; $i++) {
	my $done = ($i - $first + 1);
	my $undone = $count - $done;
	my $elapsed = time - $start;
	my $remain = $elapsed / $done * $undone;

	printf "%d => %d (%s elapsed, ETA %s)\n", $i, verify(4, 1, $i, {}), to_hms($elapsed), to_hms($remain);
}
