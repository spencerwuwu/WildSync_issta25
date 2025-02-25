#!/usr/bin/env python3
import re
import os
import sys
import subprocess

def fix_hopper_translation(queue_item, tran_output):
    matches = re.findall(r'error: must use \'([\w]*)\' tag to refer to type \'([\w]*)\'', \
        tran_output, re.MULTILINE)

    if len(matches) == 0: return

    subprocess.call(f"cp {queue_item} {queue_item}.orig", shell=True)  

    with open("%s" % (queue_item)) as f:
        prog_text = f.read()
        for (x_type,x_name) in set(matches):
            prog_text = prog_text.replace("%s " % (x_name), "%s %s " % (x_type, x_name))

    with open("%s" % (queue_item), "w") as f:
        f.write(prog_text)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 hopper_help.py <queue_item> <tran_output_log>")
        sys.exit(1)
    with open(sys.argv[2]) as f:
        print("handling" + sys.argv[1])
        fix_hopper_translation(sys.argv[1], f.read())


