#!/usr/bin/perl
use strict;
use warnings;

my @x;
push @x, (1, 3, 5, 7, 9);
@x[1..3] = (6, 6, 6);
print join(' ', @x) . "\n";

my %hash;
my $x;
my $y;
my $z = 28;

for (my $j = 0; $j < 5; $j++) {
	for (my $i = 0; $i < 100000; $i++) {
		$hash{$i} = 37;
	}

	for (my $i = 0; $i < 100000; $i++) {
		$y = $hash{$i};
		$x = 14;
		$x += 14;
		$x += $z;
		$x += $y;
	}

	for (my $i = 0; $i < 100000; $i++) {
		$hash{$i} ^= $hash{$i};
	}
}
