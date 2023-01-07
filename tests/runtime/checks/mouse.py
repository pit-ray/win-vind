from win import get_cursor_pos, set_cursor_pos, click


def get_cases():
    return [
        jump_cursor_to_active_window,
        easy_click_left,

        move_cursor_left,
        move_cursor_right,
        move_cursor_up,
        move_cursor_down,
    ]


def move_cursor_left(handler, mock_app):
    x = get_cursor_pos()[0]
    handler.send_command('<to_insert><ctrl-]>hi')
    delta = x - get_cursor_pos()[0]

    return delta > 0


def move_cursor_right(handler, mock_app):
    x = get_cursor_pos()[0]
    handler.send_command('<to_insert><ctrl-]>li')
    delta = get_cursor_pos()[0] - x
    return delta > 0


def move_cursor_up(handler, mock_app):
    y = get_cursor_pos()[1]
    handler.send_command('<to_insert><ctrl-]>ki')
    delta = y - get_cursor_pos()[1]

    return delta > 0


def move_cursor_down(handler, mock_app):
    y = get_cursor_pos()[1]
    handler.send_command('<to_insert><ctrl-]>ji')
    delta = get_cursor_pos()[1] - y

    return delta > 0


def easy_click_left(handler, mock_app):
    mock_app.frame.ready_button()

    cx, cy = mock_app.get_window_center()
    click(cx, cy)  # Select mock app

    handler.send_command('<to_gui_normal>FFGi')
    result = mock_app.frame.check_if_clicked()

    mock_app.frame.ready_button()
    return result


def jump_cursor_to_active_window(handler, mock_app):
    left, right = mock_app.get_window_pos()
    click(left + 10, right + 10)  # Select mock app

    cx, cy = mock_app.get_window_center()

    width, height = mock_app.get_window_size()
    set_cursor_pos(left + width + 20, right + height + 20)

    handler.send_command('<to_gui_normal>t')
    x, y = get_cursor_pos()

    return x == cx and y == cy
