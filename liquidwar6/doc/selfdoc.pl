#!/usr/bin/perl

# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
# Contact author        : ufoot@ufoot.org

use strict;

sub usage {
    print stderr "usage: ".@ARGV[0]." <path-to-binary> {cmd|default|func} <keyword>\n" 
}

sub go {
    my $path_to_binary=shift;
    my $mode=shift;
    my $keyword=shift;
    my $CMD_HEADER="Command-line option";
    my $ENV_HEADER="Environment variable";
    my $XML_HEADER="XML key";
    my $FUNC_HEADER="C function exported to Guile";
    my $keyword_env=$keyword;
    my $type_void=0;

    $ENV{"LANG"}="C";

    $keyword_env =~ s/-/_/g;
    $keyword_env = uc("LW6_$keyword_env");

    if ($keyword eq "config-file" ||
	$keyword eq "prefix" ||
	$keyword eq "mod-dir" ||
	$keyword eq "data-dir" ||
	$keyword eq "map-dir" ||
	$keyword eq "script-file") {
	# we only keep user-dir, log-file and map-path
	# indeed these values are excluded from the XML config file
	$type_void=1;
    }

    SWITCH1: {
	open(CMD,"$path_to_binary --about=$keyword |");
	$type_void=1 if <CMD> !~ /^Type/;
	close(CMD);

	if ($mode eq "item") {
	    printf("\@item\n\@code{$keyword}:\n");
	    last SWITCH1;
	}
	printf("\@subsection $keyword\n");
	if ($mode eq "func") {
	    printf("\@deffn {$FUNC_HEADER} \@code{$keyword}\n");
	    last SWITCH1;
	}
	if ($type_void) {
	    printf("\@deffn {$CMD_HEADER} \@code{--$keyword}\n");
	} else {
	    printf("\@deffn {$CMD_HEADER} \@code{--$keyword=<value>}\n");
	}
	last SWITCH1 if $mode eq "cmd";
	last SWITCH1 if $type_void;
	printf("\@deffnx {$ENV_HEADER} \@code{$keyword_env}\n");
	printf("\@deffnx {$XML_HEADER} \@code{$keyword}\n");
    }

    open(CMD,"$path_to_binary --about=$keyword |");
    while (<CMD>) {
	print;
    }
    close(CMD);

    SWITCH2: {
	if ($mode eq "item") {
	    last SWITCH2;
	}
	print "\@end deffn\n";
    }
    print "\n";
}

if (scalar(@ARGV)!=3) {
    usage;
    exit(1);
} else {
    go(@ARGV[0],@ARGV[1],@ARGV[2]);
}
