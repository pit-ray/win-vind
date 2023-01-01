import os
from glob import glob
import time
from datetime import datetime
from shutil import copyfile
import subprocess
from argparse import ArgumentParser

import logger
from checks import check
from vindrc_gen import VINDRC_GEN


def setup_test(exe_path, tmp_dirname='tmp'):
    # test_id = datetime.now().strftime('%Y%m%d%H%M%S')
    test_id = '.debug'
    tmp_dir = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        tmp_dirname, test_id)
    os.makedirs(tmp_dir, exist_ok=True)

    # Copy win-vind binary into the temporary directory.
    dest_exe_path = os.path.join(tmp_dir, 'win-vind.exe')
    if not os.path.isfile(dest_exe_path):
        copyfile(exe_path, dest_exe_path)

    # Copy the resource files into the temporary directory.
    tmp_resource_dir = os.path.join(tmp_dir, 'resources')
    os.makedirs(tmp_resource_dir, exist_ok=True)

    exe_dir = os.path.dirname(exe_path)
    resource_files = glob(os.path.join(exe_dir, 'resources', '*'))
    for filename in resource_files:
        dest_path = os.path.join(
            tmp_resource_dir, os.path.basename(filename))
        if not os.path.isfile(dest_path):
            copyfile(filename, dest_path)

    # Set the install type as portable.
    instype_file = os.path.join(tmp_resource_dir, '.instype')
    with open(instype_file, 'w') as f:
        f.write('portable')

    vindrc_file = os.path.join(tmp_dir, 'config', '.vindrc')
    with open(vindrc_file, 'w') as f:
        f.write(VINDRC_GEN)

    return tmp_dir


def do_test(exe_path, tmp_dirname='tmp'):
    exe_path = os.path.join(
        os.path.dirname(os.path.abspath(exe_path)),
        os.path.splitext(os.path.basename(exe_path))[0] + '.exe')

    if not os.path.isfile(exe_path):
        logger.error('Not found ' + exe_path)
        return

    logger.info('Target executable file: ' + exe_path)

    tmp_dir = setup_test(exe_path)

    proc = subprocess.Popen(['win-vind.exe'], cwd=tmp_dir, shell=True)
    status = proc.poll()
    if status is not None:
        logger.error('Cloud not start win-vind ({})'.format(status))
        return

    try:
        logger.info('Waiting 3 seconds...')
        time.sleep(3)

        logger.info('Start checking ...')
        check()

    finally:
        logger.info('Terminate process')
        subprocess.run(
            'win-vind.exe --func exit_config_gui',
            cwd=tmp_dir, shell=True)
        proc.kill()


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument(
        '--target', type=str, required=True,
        help='Path of win-vind.exe to be test.')
    args = parser.parse_args()

    do_test(args.target)
