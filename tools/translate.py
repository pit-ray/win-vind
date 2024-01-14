from argparse import ArgumentParser

import openai


class MarkdownTranslator:
    def __init__(self, api_key: str, lang: str, temperature: float = 1.0):
        openai.api_key = api_key
        self.temperature = temperature

        self.model = 'gpt-3.5-turbo-16k-0613'

        rules = '''
- Never change the Markdown markup structure. Don't add or remove links. Do not change any URL.
- Never change the contents of code blocks even if they appear to have a bug.
- Always preserve the original line breaks. Do not add or remove blank lines.
- Never touch the permalink such as `{/*examples*/}` at the end of each heading.
- Never touch HTML-like tags such as `<Notes>`.
'''

        self.base_prompt = '''I am translating the documentation of a software.
Translate the Markdown content I'll paste later into {}.
You must strictly follow the rules below.
{}'''.format(lang, rules)

    def make_messages(self, text):
        return [
            {
                'role': 'system',
                'content': 'You are a translator for Markdown documents.',
            },
            {
                'role': 'user',
                'content': self.base_prompt,
            },
            {
                'role': 'assistant',
                'content':
                    'Okay, input the Markdown.\n' + \
                    'I will only return the translated text.',
            },
            {
                'role': 'user',
                'content': text,
            }
        ]

    def translate(self, text):
        messages = self.make_messages(text)
        response = openai.ChatCompletion.create(
            model=self.model, messages=messages, temperature=self.temperature)
        result = response['choices'][0]['message']['content']
        return result


def read_markdown(filename):
    with open(filename, 'r') as f:
        text = f.read()
        f.close()
    return text


def translate_to_japanese(api_key, markdown_filename):
    translator = MarkdownTranslator(api_key, 'Japanese')

    text = read_markdown(markdown_filename)
    result = translator.translate(text)

    print(result)


def print_prompts(markdown_filename):
    translator = MarkdownTranslator('', 'Japanese')

    text = read_markdown(markdown_filename)
    messages = translator.make_messages(text)
    for msg in messages:
        print(msg['content'])
        print('\n')


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--api_key', type=str, help='API Key of ChatGPT')
    args = parser.parse_args()

    translate_to_japanese(args.api_key, 'docs/index.md')
    # print_prompts('docs/index.md')
