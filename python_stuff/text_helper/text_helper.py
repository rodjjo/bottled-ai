import os
import re
import markdown
import html

TREATMENTS = [
    'Mr. ', 
    'Mrs. ', 
    'Ms. ',
    '0. ',
    '1. ',
    '2. ',
    '3. ',
    '4. ',
    '5. ',
    '6. ',
    '7. ',
    '8. ',
    '9. ',
]

def indent(text: str) -> str:
    if text.startswith(' '):
        text = f'    {text}'
    while (' ' * 2) in text:
        text = text.replace(' ' * 2,' &nbsp;')
    return text


def remove_code_blocks(text: str) -> str:
    regex = r"(<code>)([^$]+)(</code>)"
    result = re.sub(regex, r'<b>\2</b>' , text, flags=re.MULTILINE | re.IGNORECASE)
    return result


def code_table(match) -> str:
    code_type = 'text'
    code = match.group(1)
    m2 = re.match(r'```(.*)\n([^$]+)```', match.group(0))
    if m2:
        code_type = m2.group(1)
        code = m2.group(2)
    result = f'<table border=1 width=95%><tr><td align=center><font face="courier" size="4"><b>{code_type}</b></font></td></tr>'
    result = f'{result}<tr><td><code><font face="courier" size="4">{code}</font><code></td></tr>'
    result = result.replace('<br>', '\n')
    return result


def create_code(text: str) -> str:
    text = re.sub(r'```([^$]+)```', code_table, text)
    return text


def create_paragrapys(text: str) -> str:
    result = text.split('\n\n')
    for index, f in enumerate(result):
        if '<p>' not in f and f.strip() != '':
            f = f'<p>{indent(f)}</p>'
        result[index] = f
    return ''.join(result)

def convert2html(text: str) -> str:
    try:
        result = text.strip()
        result = result.replace('&', '&amp;')
        result = result.replace('<', '&lt;')
        result = result.replace('>', '&gt;')
        
        # break the line after the end of the sentence
        result = create_code(result)

        result = markdown.markdown(result, output_format='html', extensions=['tables'])
        result = result.replace('<table>', '<table border=1 width=95%>')
        result = re.sub(r'(.*>)\n', r'\1', result)

        # create code block
        result = remove_code_blocks(result)
        
        lines = []
        for l in result.split('\n\n'):
            l = l.replace('\n', '\n\n')
            lines.append(l)
        result = '\n\n'.join(lines)


        # break the lines
        # result = re.sub(r'([^\n]+)([\n])([^\n]+)', r'\1\n\n\2', result)

        # create paragraphys
        result = create_paragrapys(result)
        result = result.replace('<li>', '<li><font face="arial" size="4">')
        result = result.replace('</li>', '</font></li>')
        result = result.replace('<p>', '<p><font face="arial" size="4">')
        result = result.replace('</p>', '</font></p>')
    except Exception as e :
        return text + 'n' + str(e)

    return result

