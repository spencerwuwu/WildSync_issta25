#!/usr/bin/env python3
import re
import os
import sys
import subprocess


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 hopper_sanitze.py <crash_item> <queue_item> ")
        sys.exit(1)
    results_dir = sys.argv[1]
    crash_item = sys.argv[2]

    if not os.path.exists(results_dir):
        print(f"Cannot find {results_dir}")
        sys.exit(1)
    if not os.path.exists(crash_item):
        print(f"Cannot find {crash_item}")
        sys.exit(1)


    try:
        verify_cmd = "%s/bin/hopper-sanitizer %s" \
            % (results_dir, crash_item)
        
        output = subprocess.check_output(verify_cmd, \
            stderr=subprocess.STDOUT, shell=True)
    except Exception as e:
        output = e.output

    not_verified = bool(re.search(r'\[hopper_sanitizer\] violate', \
        str(output), re.MULTILINE))

    if not_verified:
        print(f"hopper_sanitize: {crash_item} not verified")
        sys.exit(1)
