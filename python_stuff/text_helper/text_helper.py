import markdown
import html

def convert2html(text: str) -> str:
    paragraphy = '<p>'
    font = '<font face="arial" size="4">'
    result = markdown.markdown(
        html.escape(text), extensions=['tables', 'codehilite']
    )
    result = result.replace('<p>', '')
    result = result.replace('</p>', '')
    result = result.rstrip('\n')
    result = f'<p>{result}</p>'
    result = result.replace('\n\n', '\n')
    result = result.replace('\n', f'</p>{paragraphy}')
    result = result.replace('<p>', f'<p>{font}')
    result = result.replace('</p>', '</font></p>\n')
    result = result.replace('&apos;', "'")
    result = result.replace('&quot;', "\"")
    while (' ' * 2) in result:
        result = result.replace(' ' * 2,' &nbsp;')
    return result
