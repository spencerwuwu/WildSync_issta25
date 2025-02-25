#!/usr/bin/env python3
import sys
import json
from loguru import logger
import argparse

from fuzzsyn.gen_harness.combine_code import do_combine 

logger.remove() #remove the old handler. Else, the old one will work along with the new one you've added below'


if __name__ ==  "__main__":
    parser = argparse.ArgumentParser('do_work.py', description='')
    parser.add_argument("code_to_extract")
    parser.add_argument("original_harness")
    parser.add_argument("--input_json", dest="input_json", required=True)
    parser.add_argument("--lib_symbols", dest="lib_symbols_f")
    parser.add_argument("--debug", dest="debug", action="store_true")
    parser.add_argument("--keep-related", dest="keep_related", action="store_true")
    parser.add_argument("-q", dest="quiet", action="store_true")
    parser.add_argument("-o", dest="output_file", required=True)
    args = parser.parse_args()

    if args.debug:
        logger.add(sys.stdout, level="DEBUG") 
    elif not args.quiet:
        logger.add(sys.stdout, level="INFO") 

    if args.input_json:
        with open(args.input_json, "r") as f:
            input_data = json.loads(f.read())
    else:
        input_data = None

    lib_symbols = []
    if args.lib_symbols_f:
        with open(args.lib_symbols_f, "r") as f:
            for l in f.read().split("\n"):
                if not l:
                    continue
                lib_symbols.append(l.strip())

    do_combine(args.code_to_extract, args.original_harness,
         input_data, args.output_file, lib_symbols=lib_symbols, 
               keep_related=args.keep_related)
