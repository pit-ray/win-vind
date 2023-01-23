from argparse import ArgumentParser

import logger
from checks import check
from handler import ProcHandler


def do_test(exe_path):
    with ProcHandler(exe_path, env_dirname='tmp') as h:
        logger.info(
            'Target executable file: ' + h.get_absolute_source_exe())

        logger.info('Test ID: ' + h.get_test_id())

        logger.info('Start checking ...')
        check(h)


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument(
        '--target', type=str, required=True,
        help='Path of win-vind.exe to be test.')
    args = parser.parse_args()

    do_test(args.target)
