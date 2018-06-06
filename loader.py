import sys


if sys.maxsize == 2147483647:  # 32 bit python install
    from x86.cpymcts import Board, search

else:  # 64 bit
    from x64.cpymcts import Board, search
