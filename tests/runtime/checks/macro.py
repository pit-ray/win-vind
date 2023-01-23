def get_cases():
    return [
        insert_static_text
    ]


def insert_static_text(handler, mock_app):
    mock_app.focus()

    mock_app.frame.set_caret_position(0, 0)

    # It calls macros defined in .vindrc.
    handler.send_command('<f7><f7>', pause=3)

    text = mock_app.frame.get_line_text(0)
    return text == 'Hello World!'
