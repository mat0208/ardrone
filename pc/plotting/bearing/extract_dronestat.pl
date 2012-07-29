#!/usr/bin/perl

use warnings;
use strict;

#RAW seq=11135 a= 2056, 2016, 2572 g=   48,    8,    9 unk=  144,54732 h=  892 
#mag=   31,  -71,   29
#11135 a=-0.020, 0.021, 9.830 m/s² g=-0.04,+0.06,+0.04deg/s h=  0m dt=1000ms
#20174 a= 0.057, 0.021, 9.753 m/s² g=+0.02,-0.01,-0.02deg/s h=  0m dt= 5ms
 

my ($mx,$my,$mz);
my $pl;
  my $state=0;

while(<STDIN>) {
  chomp;
  
  my $l=$_;
  
  
  my $numRe="(-?\\\+?[0-9.]+)";
  
  if ($l =~ /RAW\s*seq=$numRe\s*a=\s$numRe,\s*$numRe,\s*$numRe/) {
    my $rx=$2;
    my $ry=$3;
    my $rz=$4;
    print join(",",$rx,$ry,$rz)."\n";
    $state=1;
  } elsif ($l =~ /mag=\s*$numRe\s*,\s*$numRe,\s*$numRe/) {
    $mx=$1;
    $my=$2;
    $mz=$3;
    $state=2;
  } elsif ($l =~ /[0-9]* a=\s*$numRe,\s*$numRe,\s*$numRe\s*m\/s.*g=$numRe,$numRe,$numRe/) {
  
    my ($ax,$ay,$az,$gx,$gy,$gz)=($1,$2,$3,$4,$5,$6);
    if ($state !=2) {
      warn "State strange: $pl\n";
    }
    $state=3;
#    print join ( ",", $ax,$ay,$az,$gx,$gy,$gz,$mx,$my,$mz )."\n";
  } elsif ($l !~ /^$/) {
    warn "Unparsable line: $l\n";
  }
  $pl=$l;
}