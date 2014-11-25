# Testing framework

WORKING_DIRECTORY=$(pwd -P)

T()
{
	local test_name=$1

	# Create a temporary directory for this test...
	TEST_DIRECTORY=$(mktemp -d /var/tmp/ctf-XXXXXX) || exit 1

	# CD into it to begin!
	cd $TEST_DIRECTORY

	# Default is to pass...
	TEST_RESULT=true

	# Output the test name
	TEST_NAME=$test_name
}

C()
{
	local cmd=$1;shift
	local parameters=$@

	local stdout=$(mktemp stdout.XXXXXX)
	local stderr=$(mktemp stderr.XXXXXX)

	TEST_COMMAND="$cmd $parameters"

	$($TEST_COMMAND >$stdout 2>$stderr)

	TEST_RETURN=$?
	TEST_STDOUT=$(cat $stdout)
	TEST_STDERR=$(cat $stderr)
}

R()
{
	local match=$1

	if [[ "$TEST_RETURN" != "$match" ]]
	then
		TEST_RESULT=false

		echo "Failed on \"$TEST_COMMAND\""
		echo "Return value $TEST_RETURN != $match"
	fi
}

O()
{
	local match=$1

	if [[ "$TEST_STDOUT" != "$match" ]]
	then
		TEST_RESULT=false
		
		echo "Failed on \"$TEST_COMMAND\""
		echo "Stdout $TEST_STDOUT != $match"
	fi
}

E()
{
	local match=$1

	if [[ "$TEST_STDERR" != "$match" ]]
	then
		TEST_RESULT=false

		echo "Failed on \"$TEST_COMMAND\""
		echo "Stderr $TEST_STDERR != $match"
	fi
}

F()
{
	# Jump back to the directory we should be in
	cd $WORKING_DIRECTORY

	# Destroy the temporrary directory for this test
	rm -rf $TEST_DIRECTORY
	TEST_DIRECTORY=""

	# Output the test result
	if $TEST_RESULT
	then
		exit 0
	else
		exit 1
	fi
}

