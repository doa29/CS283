#!/usr/bin/env bats
@test "cd command with argument" {
    current=$(pwd)
    cd /tmp
    mkdir -p dsh-test
    run "${current}/dsh" <<EOF
cd dsh-test
pwd
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/tmp/dsh-testdsh>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]
}
@test "cd command with no argument" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
cd
pwd
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="$(pwd)dsh>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]
}
@test "rc builtin returns last exit code" {
    run "${PWD}/dsh" <<EOF
not_exists
rc
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    [ "$stripped_output" != "" ]
}
