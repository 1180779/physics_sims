length($0) > 72 {
    printf "CommitMsg: line %d exceeds 72 characters (%d)\n", NR, length($0) > "/dev/stderr"
    invalid = 1
}
NR == 2 && $0 != "" {
    print "CommitMsg: separate the title and description with an empty line" > "/dev/stderr"
    invalid = 1
}
END {
    if (NR == 0) {
        print "CommitMsg: message is empty" > "/dev/stderr"
        invalid = 1
    }
    exit invalid
}