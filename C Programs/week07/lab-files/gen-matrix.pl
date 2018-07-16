#!/usr/bin/perl -w

# hacked version of ~/classes/cs4115-1011/labs/03week/gen-matrix.pl

use Getopt::Std;

use strict;
use warnings;

sub tailof;

my (@args) = @ARGV;
my ($prog) = tailof $0;
my ($usage) = "usage:\t$prog [-h] [-r rows ] [ -c columns ] [ -m magnitude ]\n";

my %args;
getopts("c:hr:m:", \%args);

exists $args{'h'} && die "$usage\n";;

my $rows=	$args{'r'} || 50;
my $cols=	$args{'c'} || 100;
my $magn=	$args{'m'} || 100;

my (@arr);

die "Bad row ($rows) / col ($cols) ct.\n" unless (0 <= $rows && 0 <= $cols);

print "$rows $cols\n";

for (my $i = 0; $i < $rows; $i++)
{
    for (my $j = 0; $j < $cols; $j++)
    {
	my ($sign) = (rand(1) < 0.5) ? 0 : 1; # gen sign of no.: 0 ==> negative
	my ($r) = rand($magn) * ($sign * 2 - 1);
	printf "%.6f ", $r;
    }
    print "\n";
}


###### subs here

sub tailof {
    $_[0] =~ m!([^/]*)$!;
    $1;
}
