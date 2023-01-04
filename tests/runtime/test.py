import time
from argparse import ArgumentParser

import logger
from checks import check
from vindrc_gen import VINDRC_GEN
from handler import ProcHandler


def do_test(exe_path):
    with ProcHandler(exe_path, env_dirname='tmp') as h:
        logger.info(
            'Target executable file: ' + h.get_absolute_source_exe())

        h.create_vindrc(VINDRC_GEN)

        logger.info('Start checking ...')
        check(h)


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument(
        '--target', type=str, required=True,
        help='Path of win-vind.exe to be test.')
    args = parser.parse_args()

    do_test(args.target)
