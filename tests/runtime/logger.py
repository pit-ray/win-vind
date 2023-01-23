from ctypes import windll, wintypes, byref
from logging import \
    getLogger, \
    INFO, \
    Formatter, \
    StreamHandler, \
    basicConfig

logger = getLogger(__name__)


class ColorHandler(StreamHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        INVALID_HANDLE_VALUE = -1
        STD_OUTPUT_HANDLE = -11
        ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004

        hOut = windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)
        if hOut != INVALID_HANDLE_VALUE:
            dwMode = wintypes.DWORD()
            if windll.kernel32.GetConsoleMode(hOut, byref(dwMode)) != 0:
                dwMode.value |= ENABLE_VIRTUAL_TERMINAL_PROCESSING
                windll.kernel32.SetConsoleMode(hOut, dwMode)

    def emit(self, record):
        record.levelname = {
            'INFO': '\x1b[0;32m  OK  \x1b[0m',
            'WARNING': '\x1b[0;33mWARNING\x1b[0m',
            'ERROR': '\x1b[0;31mFAILED\x1b[0m',
        }.get(record.levelname, '')
        super().emit(record)


handler = ColorHandler()
handler.setFormatter(Formatter('[%(levelname)s] %(message)s'))
basicConfig(level=INFO, handlers=[handler])


def info(message):
    logger.info(message)


def warning(message):
    logger.warning(message)


def error(message):
    logger.error(message)
