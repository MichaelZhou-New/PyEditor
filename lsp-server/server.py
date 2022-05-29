from typing import Iterable
import pygments
import json
from pygments.lexers import PythonLexer
from pygments.formatters import HtmlFormatter
from pygments.token import _TokenType
from pygls.lsp.methods import (TEXT_DOCUMENT_DID_CHANGE,
                               TEXT_DOCUMENT_DID_CLOSE,
                               TEXT_DOCUMENT_DID_OPEN,
                               DOCUMENT_HIGHLIGHT,
                               DOCUMENT_COLOR)
from pygls.lsp.types import (DidChangeTextDocumentParams,
                             DidCloseTextDocumentParams,
                             DidOpenTextDocumentParams,
                             DocumentHighlightParams,
                             DocumentHighlight,
                             DocumentColorParams,
                             ColorInformation,
                             Color)

from pygls.lsp.types.basic_structures import (Position,
                                              Range)

from pygls.server import LanguageServer

from styles.default import DefaultStyle
from styles.monokai import MonokaiStyle

import logging

class PythonLanguageServer(LanguageServer):
    # print("Python Language Server is running now...")

    def __init__(self):
        super().__init__()


python_server = PythonLanguageServer()
# styles = MonokaiStyle().styles
styles = DefaultStyle().styles

@python_server.feature(DOCUMENT_HIGHLIGHT)
def document_highlight(params: DocumentHighlightParams):
    fileUri = params.text_document.uri
    filePath = fileUri.split()
    pygments.highlight()
    return DocumentHighlight(
        range = Range(
            start = Position(line = 0, character = 100),
            end = Position(line = 100, character = 100)
        )
    )


def findBaseTokenType(tokenType: _TokenType, styles):
    while tokenType != None:
        # print(tokenType)
        if styles.get(tokenType) != '':
            return tokenType
        tokenType = tokenType.parent
        # print(tokenType)
        
    return None

@python_server.feature(DOCUMENT_COLOR)
def document_color(params: DocumentColorParams):
    # print('received document/documentColor request')
    # print('params: ', DocumentColorParams)
    logging.info('received document/documentColor request')
    logging.info('params: ', DocumentColorParams)
    # fileUri: str = "file://test2.py"
    # filePath: str = fileUri.split("file://", 1)[1]
    fileUri: str = params.text_document.uri
    filePath: str = fileUri
    file = open(filePath, "r")
    code: str = file.read()

    indexToLineDict: dict[int, dict[str, int]] = dict()
    lineNumber: int = 0
    lineStart: int = 0
    for index, char in enumerate(code):
        indexToLineDict[index] = { 'lineNumber': lineNumber, 'lineStart': lineStart }
        if char == '\n':
            lineNumber = lineNumber + 1
            lineStart = index + 1

    # print(indexToLineDict)

    pythonLexer: PythonLexer = PythonLexer()
    tokenTypes: Iterable = pythonLexer.get_tokens_unprocessed(code)

    colorInformationList = []

    lineNumber: int = 0
    for tokenType in list(tokenTypes):
        # print(tokenType)
        index = tokenType[0]
        tokenLen = len(tokenType[2])
        rgbValue = None
        if styles.get(tokenType[1]) != '':
            rgbValue = styles[tokenType[1]]
        else:
            baseTokenType = findBaseTokenType(tokenType[1], styles)
            if baseTokenType != None:
                rgbValue = styles[baseTokenType]

        # if rgbValue != None, tokenType mustn't be a '\n'
        if rgbValue != None:
            colorInformationList.append(ColorInformation(
                range = Range(
                    start = Position(
                        line = indexToLineDict[index]['lineNumber'], 
                        character = index - indexToLineDict[index]['lineStart']
                    ),
                    end = Position(
                        line = indexToLineDict[index + tokenLen - 1]['lineNumber'], 
                        character = (index + tokenLen) - indexToLineDict[index + tokenLen - 1]['lineStart']
                    )
                ),
                color = Color(
                    red = int(rgbValue[1:3], 16),
                    green = int(rgbValue[3:5], 16),
                    blue = int(rgbValue[5:7], 16),
                    alpha = 1.0
                )
            ))
    # print(colorInformationList)
    logging.info('result: ', colorInformationList)
    return colorInformationList

            # print('index: ', index)
            # print('lineNumber start: ', indexToLineDict[index]['lineNumber'])
            # print('lineStart start: ',indexToLineDict[index]['lineStart'])
            # print('lineNumber end: ', indexToLineDict[index + tokenLen]['lineNumber'])
            # print('lineStart end: ', indexToLineDict[index + tokenLen]['lineStart'])
            # print('tokenLen: ', tokenLen)
            # print(indexToLineDict[index]['lineNumber'])
            # print(index - indexToLineDict[index]['lineStart'])
            # print(indexToLineDict[index + tokenLen]['lineNumber'])
            # print(index - indexToLineDict[index + tokenLen]['lineStart'])
            # print(tokenType[1])
            # print(tokenType)
# ColorInformation(
#     range = Range(
#         start = Position(line = 0, character = 100),
#         end = Position(line = 100, character = 100)
#     ),
#     color = Color(
#         red = 66,
#         green = 66,
#         blue = 66,
#         alpha = 66
#     )
# )

# DocumentHighlightParams (
#     text_document = TextdocumentIdentifier(
#         uri = str
#     ),
#     position = Position(
#         line = int,
#         character = int
#     )
# )

@python_server.feature(TEXT_DOCUMENT_DID_CHANGE)
def did_change(server, params: DidChangeTextDocumentParams):
    """Text document did change notification."""


@python_server.feature(TEXT_DOCUMENT_DID_CLOSE)
def did_close(server: PythonLanguageServer, params: DidCloseTextDocumentParams):
    """Text document did close notification."""
    server.show_message('Text Document Did Close')


@python_server.feature(TEXT_DOCUMENT_DID_OPEN)
async def did_open(ls, params: DidOpenTextDocumentParams):
    """Text document did open notification."""
    ls.show_message('Text Document Did Open')

# for test
# document_color(params=None)