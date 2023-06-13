from win import get_maximum_window_size


def get_cases():
    return [
        decrease_window_height,
        increase_window_width,
        resize_window_height,
        snap_current_window_to_right,
    ]


def decrease_window_height(handler, mock_app):
    mock_app.focus()

    w1, h1 = mock_app.get_window_size()

    handler.send_command('<c-w>-', pause=2)
    w2, h2 = mock_app.get_window_size()

    return w1 == w2 and h1 > h2


def increase_window_width(handler, mock_app):
    mock_app.focus()

    w1, h1 = mock_app.get_window_size()

    handler.send_command('<c-w><gt>', pause=2)
    w2, h2 = mock_app.get_window_size()

    return w1 < w2 and h1 == h2


def resize_window_height(handler, mock_app):
    mock_app.focus()

    handler.send_command(':res vert 100<cr>', pause=2)
    w, h = mock_app.get_window_size()
    return h == 100


def snap_current_window_to_right(handler, mock_app):
    mock_app.focus()

    handler.send_command('<c-w>L', pause=2)
    w, h = mock_app.get_window_size()

    sw, sh = get_maximum_window_size()
    target_w = sw // 2
    target_h = sh

    return w == target_w and h == target_h
