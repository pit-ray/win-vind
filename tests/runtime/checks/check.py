import logger

from . import mouse
from . import proc


def check(handler):
    test_cases = [] \
        + mouse.get_cases() \
        + proc.get_cases()

    num_of_case = len(test_cases)
    for i, case in enumerate(test_cases):
        if case(handler):
            logger.info("Case {}/{}: {} is successed.".format(
                i + 1, num_of_case, case.__name__))
        else:
            logger.error("Case {}/{}: {} is failed.".format(
                i + 1, num_of_case, case.__name__))
