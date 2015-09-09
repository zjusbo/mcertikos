#!/usr/bin/perl

open(BB, $ARGV[1]) || die "open $ARGV[1]: $!";

binmode BB;
my $buf;
read(BB, $buf, 65536);
$n = length($buf);

if($n > $ARGV[0]){
	print STDERR "boot block too large: $n bytes (max $ARGV[0])\n";
	exit 1;
}

print STDERR "boot block is $n bytes (max $ARGV[0])\n";
