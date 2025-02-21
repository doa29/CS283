#!/usr/bin/env bats
@test "Which which ... which?" {
    run "./dsh" <<EOF
which which
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/usr/bin/whichdsh>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]
}
@test "It handles quoted spaces" {
    run "./dsh" <<EOF
echo " hello     world     "
EOF
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    expected_output=" hello     world     dsh>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]
}
