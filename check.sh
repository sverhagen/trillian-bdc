#!/bin/bash -i
# 
# Copyright 2003-2012 Totaal Software / Sander Verhagen
# 
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
# in compliance with the License. You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software distributed under the License
# is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing permissions and limitations under
# the License.

extensions="cpp def h sh txt"
exceptions=("./res/instructions.txt" "./res/version history.txt" "./NOTICE.txt" "$0")
requiredLicenseString="Apache License, Version 2.0"

RED=31
GREEN=32

getExceptionsConstruct()
{
	result=""
	for exception in "${exceptions[@]}"
	do
		result="$result | grep -v \"$exception\""
	done
	echo $result
}

getInameConstruct()
{
	result=""
	for extension in $extensions
	do
		if [ "$result" ]; then result="$result -or "; fi
		result="$result -iname \"*.$extension\""
	done
	echo $result
}

getCommmandLine()
{
	echo "find \( $(getInameConstruct) \) -exec grep --files-without-match \"$requiredLicenseString\" {} \; $(getExceptionsConstruct)"
}

printColor()
{
	color=$1
	shift
	echo -e "\e[00;$color""m$@\e[00m"
}

getColumns()
{
	echo $(stty --all | grep columns | sed --regexp-extended "s/.*columns ([0-9]+).*/\1/")
}

printRuler()
{
	
	printf "%$(getColumns)s" | sed "s/ /-/g"
}

printLine()
{
	COLUMNS=$(($(getColumns) - 1))
	echo -e "$@" | fold --spaces --width=$COLUMNS | sed "s/^/ /"
}

printRuler
printLine "this little tool checks whether license statements are available in (source) files for which this is required"
printRuler
printLine "required file extensions: $extensions"
printLine "exceptions: ${exceptions[@]}"
printLine "required license string: $requiredLicenseString"
printRuler

commandLine=$(getCommmandLine)
results=$(sh -c "$commandLine")

if [ -z "$results" ];
then
	printLine "license statements are available in all (required) files"
	printRuler
	printLine $(printColor $GREEN "SUCCESS")
	printRuler
else
	printLine "license is missing from the following (required) files:"
	IFS=$'\n'
	for result in $results
	do
		printLine "\t$result"
	done
	printRuler
	printLine $(printColor $RED "FAILURE")
	printRuler
fi