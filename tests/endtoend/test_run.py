# -*- coding: utf-8 -*-
"""
Test standalone program execution.
"""

import subprocess
import os

#===========================================================================

OMIPHONE_PROG = os.environ["OMIPHONE_PROG"]

#===========================================================================

def run_prog(input_value):
    proc = subprocess.Popen([OMIPHONE_PROG], stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE)
    try:
        (sout, serr) = proc.communicate(input_value.encode("utf-8"), 1)
        exit_code = proc.wait()
        assert exit_code == 0
        return sout.decode("utf-8")
    except subprocess.TimeoutExpired:
        proc.kill()
        return None

#===========================================================================

def test_program_execution():
    assert run_prog("") == "no input\n"
    assert (run_prog("2 10 6 9 30 6 6 4") ==
            "Interpretation 1: 2106930664 [phone number: VALID]\n" +
            "Interpretation 2: 210693664 [phone number: INVALID]\n")
