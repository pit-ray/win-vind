import logger

from mockapp import MockApp

from . import mouse
from . import proc
from . import window


def check_test_cases(handler, app, test_cases):
    num_of_case = len(test_cases)
    for i, case in enumerate(test_cases):
        app.reset()
        handler.send_command('<f12>', pause=2)

        if case(handler, app):
            logger.info('Case {}/{}: {} is successed.'.format(
                i + 1, num_of_case, case.__name__))
        else:
            logger.error('Case {}/{}: {} is failed.'.format(
                i + 1, num_of_case, case.__name__))


def check(handler):
    test_cases = [] \
        + mouse.get_cases() \
        + window.get_cases() \
        + proc.get_cases()

    with MockApp(asynchronous=True) as app:
        check_test_cases(handler, app, test_cases)
