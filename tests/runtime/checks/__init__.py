from .mouse import move_cursor_left
from .proc import exit_process

import logger


def check(handler):
    test_cases = [
        move_cursor_left,
        exit_process
    ]

    num_of_case = len(test_cases)
    for i, case in enumerate(test_cases):
        if case(handler):
            logger.info("Case {}/{}: {} is successed.".format(
                i + 1, num_of_case, case.__name__))
        else:
            logger.error("Case {}/{}: {} is failed.".format(
                i + 1, num_of_case, case.__name__))
