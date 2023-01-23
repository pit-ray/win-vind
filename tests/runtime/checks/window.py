from win import click, get_maximum_window_size


def get_cases():
    return [
        decrease_window_height,
        increase_window_width,
        resize_window_height,
        snap_current_window_to_right,
        window_resizer
    ]


def decrease_window_height(handler, mock_app):
    cx, cy = mock_app.get_window_center()
    click(cx, cy)

    w1, h1 = mock_app.get_window_size()

    handler.send_command('<c-w>-', pause=2)
    w2, h2 = mock_app.get_window_size()

    return w1 == w2 and h1 > h2


def increase_window_width(handler, mock_app):
    cx, cy = mock_app.get_window_center()
    click(cx, cy)

    w1, h1 = mock_app.get_window_size()

    handler.send_command('<c-w><gt>', pause=2)
    w2, h2 = mock_app.get_window_size()

    return w1 < w2 and h1 == h2


def resize_window_height(handler, mock_app):
    cx, cy = mock_app.get_window_center()
    click(cx, cy)

    handler.send_command(':res vert 100<cr>', pause=2)
    w, h = mock_app.get_window_size()
    return h == 100


def snap_current_window_to_right(handler, mock_app):
    cx, cy = mock_app.get_window_center()
    click(cx, cy)

    handler.send_command('<c-w>L', pause=2)
    w, h = mock_app.get_window_size()

    sw, sh = get_maximum_window_size()
    target_w = sw // 2
    target_h = sh

    return w == target_w and h == target_h


def window_resizer(handler, mock_app):
    cx, cy = mock_app.get_window_center()
    click(cx, cy)

    x1, y1 = mock_app.get_window_pos()
    w1, h1 = mock_app.get_window_size()

    # [winresizer]  a user-defined macro defined in vindrc_gen.py.
    # [ll]          increate width in resize mode
    # [e]           change mode to move mode
    # [jj]          Down the y coordinate of the window
    # [<esc>]       Exit windowresizer
    handler.send_command('winresizerllejj<esc>', pause=2)
    x2, y2 = mock_app.get_window_pos()
    w2, h2 = mock_app.get_window_size()

    return w1 < w2 and h1 == h2 and x1 == x2 and y1 < y2
