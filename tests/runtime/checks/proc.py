def get_cases():
    return [
        exit_process
    ]


def exit_process(handler, mock_app):
    result_1 = handler.check_if_alive()
    handler.send_command(':exit<cr>')
    result_2 = not handler.check_if_alive()
    return result_1 & result_2


def execute():
    return False


def start_external():
    return False


def start_shell():
    return False
