import os
import time
from datetime import datetime
import subprocess
from glob import glob
from functools import lru_cache
from shutil import copyfile


class ProcHandler:
    def __init__(self, exe_path, env_dirname='tmp'):
        self.exe_path = os.path.join(
            os.path.dirname(os.path.abspath(exe_path)),
            os.path.splitext(os.path.basename(exe_path))[0] + '.exe')

        if not os.path.isfile(self.exe_path):
            raise RuntimeError('Not found ' + self.exe_path)

        self.env_dirname = env_dirname
        self.env_root = None

    def get_absolute_source_exe(self):
        return self.exe_path

    @lru_cache(maxsize=1)
    def get_test_id(self):
        return datetime.now().strftime('%Y%m%d%H%M%S')

    def setup_environment(self):
        test_id = self.get_test_id()
        self.env_root = os.path.join(
            os.getcwd(), self.env_dirname, test_id)
        os.makedirs(self.env_root, exist_ok=True)

        # Copy win-vind binary into the temporary directory.
        dest_exe_path = os.path.join(self.env_root, 'win-vind.exe')
        if not os.path.isfile(dest_exe_path):
            copyfile(self.exe_path, dest_exe_path)

        # Copy the resource files into the temporary directory.
        resource_dir = os.path.join(self.env_root, 'resources')
        os.makedirs(resource_dir, exist_ok=True)

        source_resource_query = os.path.join(
            os.path.dirname(self.exe_path), 'resources', '*')
        resource_files = glob(source_resource_query)

        for filename in resource_files:
            dest_path = os.path.join(
                resource_dir, os.path.basename(filename))

            if not os.path.isfile(dest_path):
                copyfile(filename, dest_path)

        # Set the install type as portable.
        instype_file = os.path.join(resource_dir, '.instype')
        with open(instype_file, 'w') as f:
            f.write('portable')

        config_dir = os.path.join(self.env_root, 'config')
        os.makedirs(config_dir, exist_ok=True)
        self.create_vindrc("")

    def create_vindrc(self, texts):
        vindrc_file = os.path.join(self.env_root, 'config', '.vindrc')
        with open(vindrc_file, 'w') as f:
            f.write(texts)

    def __enter__(self):
        self.setup_environment()
        self.start()
        return self

    def __exit__(self, *args):
        self.kill()

    def _launch(self, *args, pause=1.0):
        if self.env_root is None:
            raise RuntimeError('The test environment has not yet been set up')

        proc = subprocess.Popen(
            ['win-vind.exe'] + list(args), cwd=self.env_root, shell=True)
        status = proc.poll()
        if status is not None:
            raise RuntimeError('Cloud not start win-vind ({})'.format(status))

        time.sleep(pause)

    def start(self, pause=1.0):
        self._launch(pause=pause)

    def send_command(self, command, pause=1.0):
        self._launch('--command', '\"' + command + '\"')
        time.sleep(pause)

    def kill(self):
        self.send_command('<exit>')

    def check_if_alive(self):
        output = subprocess.check_output(
            'TASKLIST /fi \"imagename eq win-vind.exe\"', shell=True)
        return output.find(b'win-vind.exe') != -1
