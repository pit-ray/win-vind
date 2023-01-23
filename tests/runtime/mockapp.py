import time
import threading
import tkinter as tk


DEFAULT_TEXT = '''MIT License
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.'''


class MockFrame(tk.Frame):
    def __init__(self, master=None, width=480, height=360):
        super().__init__(master)
        self.pack()

        self.width = width
        self.height = height
        self.master.title('win-vind Runtime Test')
        self.master.overrideredirect(True)
        self.init_geometry()

        self.text = tk.Text(
            self.master, width=40, height=20, wrap='none')

        self.text.insert(tk.INSERT, DEFAULT_TEXT)

        self.button = tk.Button(
            self.master, text='Click',
            width=40, height=5, command=self._clicked)

        self.text.pack()
        self.button.pack()

        self.button_state = False

        self.set_caret_position(0, 0)

    def init_geometry(self):
        self.master.geometry(
            '{}x{}+0+0'.format(self.width, self.height))

    def set_caret_position(self, row, col):
        self.text.mark_set(
            tk.INSERT, '{}.{}'.format(row + 1, col))

    def get_caret_position(self):
        idx = self.text.index(tk.INSERT)
        row, col = idx.split('.')
        return (int(row) - 1, int(col))

    def get_line_text(self, row=None):
        _row, _col = self.get_caret_position()

        if row is not None:
            _row = row

        start_idx = '{}.{}'.format(_row + 1, 0)
        text = self.text.get(start_idx, 'end-1c').split('\n')[0]
        return text

    def get_last_row(self):
        idx = self.text.index('end-1c linestart')
        row, col = idx.split('.')
        return int(row) - 1

    def ready_button(self):
        self.button_state = False

    def check_if_clicked(self):
        return self.button_state

    def _clicked(self):
        self.button_state = True


class MockApp:
    def __init__(self, asynchronous=False):
        super().__init__()

        self.frame = None
        self.thread = None

        self.asynchronous = asynchronous

    def _launch(self):
        if self.frame is None:
            self.root = tk.Tk()
            self.root.protocol('WM_DELETE_WINDOW', self.on_quit)
            self.root.attributes('-topmost', True)

            self.frame = MockFrame(master=self.root)
            self.frame.mainloop()

    def launch(self):
        if not self.asynchronous:
            return self._launch()

        self.thread = threading.Thread(target=self._launch)
        self.thread.setDaemon(True)
        self.thread.start()

        while self.frame is None:
            time.sleep(0.5)

    def close(self):
        if self.frame is not None:
            self.frame.destroy()
            self.frame = None

    def on_quit(self):
        self.root.destroy()
        self.frame = None
        self.root = None

    def __enter__(self):
        self.launch()
        return self

    def __exit__(self, *args):
        self.close()

    def reset(self):
        self.frame.init_geometry()
        self.frame.ready_button()

    def get_title(self):
        return self.root.title()

    def get_window_pos(self):
        return (self.root.winfo_x(), self.root.winfo_y())

    def get_window_center(self):
        x, y = self.get_window_pos()
        w, h = self.get_window_size()

        return (x + w // 2, y + h // 2)

    def get_window_size(self):
        return (self.root.winfo_width(), self.root.winfo_height())

    def get_maximum_size(self):
        return self.root.maxsize()

    def get_screen_size(self):
        return (self.root.winfo_screenwidth(), self.root.winfo_screenheight())


if __name__ == '__main__':
    with MockApp(asynchronous=True) as app:
        print(app.get_title())
        while True:
            app.frame.get_line_text()
            time.sleep(5)
