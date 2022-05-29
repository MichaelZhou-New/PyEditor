'''
Author: dylan
Date: 2022-05-03 14:35:39
LastEditTime: 2022-05-06 14:39:54
LastEditors: dylan
Description: 
FilePath: \lsp-server\entry.py
'''
import argparse
import logging

from server import python_server

logging.basicConfig(filename="pygls.log", level=logging.DEBUG, filemode="w")

def add_arguments(parser):
    parser.description = "python language server"

    parser.add_argument(
        "--tcp", action="store_true",
        help="Use TCP server"
    )
    parser.add_argument(
        "--ws", action="store_true",
        help="Use WebSocket server"
    )
    parser.add_argument(
        "--host", default="127.0.0.1",
        help="Bind to this address"
    )
    parser.add_argument(
        "--port", type=int, default=2087,
        help="Bind to this port"
    )


def main():
    parser = argparse.ArgumentParser()
    add_arguments(parser)
    args = parser.parse_args()

    # print("Python Language Server is running now...")
    logging.info("Python Language Server is running now...")

    # print('host: ', args.host)
    # print('port: ', args.port)
    if args.tcp:
        # print('tcp mode')
        logging.info('tcp mode')
        python_server.start_tcp(args.host, args.port)
    elif args.ws:
        # print('websocket mode')
        logging.info('websocket mode')
        python_server.start_ws(args.host, args.port)
    else:
        # print('io mode')
        logging.info('io mode')
        python_server.start_io()


if __name__ == '__main__':
    main()