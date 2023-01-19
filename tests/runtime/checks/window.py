from win import click


def get_cases():
    return [
        resize_window_height

    ]


def close_current_window():
    return False


def decrease_window_height():
    return False


def decrease_window_width():
    return False


def increase_window_height():
    return False


def increase_window_width():
    return False


def maximize_current_window():
    return False


def minimize_current_window():
    return False


def resize_window_height(handler, mock_app):
    cx, cy = mock_app.get_window_center()
    click(cx, cy)

    handler.send_command(':res vert 100<cr>', pause=2)
    w, h = mock_app.get_window_size()
    return h == 100


def resize_window_width():
    return False


def select_left_window():
    return False


def snap_current_window_to_left():
    return False


def switch_window():
    return False


def window_resizer():
    return False
