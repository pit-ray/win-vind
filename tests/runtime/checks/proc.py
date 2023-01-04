def get_cases():
    return [
        exit_process
    ]


def exit_process(handler):
    result_1 = handler.check_if_alive()
    handler.send_command('<to_gui_normal>:exit<cr>')
    result_2 = not handler.check_if_alive()
    handler.start()
    return result_1 & result_2
