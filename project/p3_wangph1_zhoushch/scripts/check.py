#!/usr/bin/env python3

import sys

import argparse
import numpy as np


# Intermediate class to parse arguments
class InputParser(argparse.ArgumentParser):
    def __init__(self):
        super(InputParser, self).__init__(
            description="Testing script for LBM",
            fromfile_prefix_chars='@',
            formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        )

        # % tolerance
        self.add_argument("--tolerance",
                          nargs=1,
                          default=[0.0001],
                          type=float,
                          help="""Percentage tolerance to match against reference results""",
                          action='store')
        # Reference results
        self.add_argument("--reference",
                          nargs=1,
                          required=True,
                          help="""reference final_state results file""",
                          action='store')

        # Calculated results
        self.add_argument("--result",
                          nargs=1,
                          required=True,
                          help="""calculated final_state results file""",
                          action='store')


parser = InputParser()
parsed_args = parser.parse_args()


def load_dat_files(state_filename):
        with open(state_filename, "r") as final_state_file:
            final_state = np.loadtxt(final_state_file, usecols=[0, 1, 2])
            return final_state


# Open reference and input files
state_ref = load_dat_files(parsed_args.reference[0])
state_sim = load_dat_files(parsed_args.result[0])

# Make sure the coordinates are in the right order
if np.any(state_ref[:, 0:2] != state_sim[:, 0:2]):
    print("Final state files coordinates were not the same")
    exit(1)


def get_diff_values(ref_vals, sim_vals):
    # Get the differences between the original and reference results
    diff = np.abs(ref_vals - sim_vals)

    max_diff_step = np.argmax(diff)

    diffs = {
        "max_diff_step": max_diff_step,
        "max_diff": diff[max_diff_step],
        "sim_val": sim_vals[max_diff_step],
        "ref_val": ref_vals[max_diff_step],
        "total": np.sum(diff),
    }

    return diffs


def print_diffs(format_strings, format_dict):
    for s in format_strings:
        print(s.format(**format_dict))


final_state_diffs = get_diff_values(state_ref[:, 2], state_sim[:, 2])

final_state_strings = [
    "Total difference in final_state : {total:.12E}",
    "Biggest difference (at coord ({ii:d},{jj:d})) :",
    "       | {sim_val:.12E} - {ref_val:.12E} | = {max_diff:.12E}",
]

# We want the location of the biggest difference
max_diff_loc = int(final_state_diffs["max_diff_step"])
final_state_diffs["ii"] = int(state_sim[max_diff_loc, 0])
final_state_diffs["jj"] = int(state_sim[max_diff_loc, 1])

print_diffs(final_state_strings, final_state_diffs)

print()

# Find out if either of them failed
final_state_failed = (not np.isfinite(final_state_diffs["max_diff"])) or (
    np.abs(final_state_diffs["max_diff"]) > parsed_args.tolerance[0])

if final_state_failed:
    print("Final state check failed.")
    exit(1)
else:
    print("Final state check passed!")
    exit(0)
